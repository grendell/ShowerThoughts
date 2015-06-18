#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

const char * const KERNEL_SOURCE = "check_number.cl";
const char * const KERNEL_NAME = "checkNumber";

// These are more or less arbitrary.
const uint64_t MAX_BUFFER_SIZE = 1ULL << 20;
const size_t WORK_GROUP_SIZE = 1ULL << 24;
const uint64_t MAX_OFFSET = 1ULL << 31;

const int ARGS_MATCHES = 0;
const int ARGS_NUM_MATCHES = 1;
const int ARGS_NUM_MATCHES_FOUND = 2;
const int ARGS_OFFSET = 3;
const int ARGS_DEVICE_INDEX = 4;

const int INVALID_KERNEL_FILE = 1;

typedef struct clProperties clProperties;
struct clProperties {
    cl_platform_id platformId;
    cl_uint numDeviceIds;
    cl_device_id * deviceIds;
    cl_context context;
    cl_kernel * kernels;
    cl_command_queue * queues;

    cl_mem * matches;
    cl_uint numMatches;
    cl_mem * numMatchesFound;
};

typedef struct threadProperties threadProperties;
struct threadProperties {
    clProperties * clProps;
    cl_uint deviceIndex;
    pthread_mutex_t * offsetMutex;
    cl_ulong * offset;
};

typedef struct monitorProperties monitorProperties;
struct monitorProperties {
    cl_uint numDeviceIds;
    cl_command_queue * queues;
    cl_mem * numMatchesFound;
    cl_ulong * offset;
    int * shouldMonitor;
};

void cleanup(clProperties * props) {
    if (props->deviceIds != NULL) {
        free(props->deviceIds);
    }

    if (props->kernels != NULL) {
        for (int i = 0; i < props->numDeviceIds; i++) {
            clReleaseKernel(props->kernels[i]);
        }
        free(props->kernels);
    }

    if (props->queues != NULL) {
        for (int i = 0; i < props->numDeviceIds; i++) {
            clReleaseCommandQueue(props->queues[i]);
        }
        free(props->queues);
    }

    if (props->matches != NULL) {
        for (int i = 0; i < props->numDeviceIds; i++) {
            clReleaseMemObject(props->matches[i]);
        }
        free(props->matches);
    }

    if (props->numMatchesFound != NULL) {
        for (int i = 0; i < props->numDeviceIds; i++) {
            clReleaseMemObject(props->numMatchesFound[i]);
        }
        free(props->numMatchesFound);
    }

    clReleaseContext(props->context);
}

cl_int init(clProperties * props) {
    memset(props, 0, sizeof(clProperties));

    cl_uint platformIdCount;
    clGetPlatformIDs(0, NULL, &platformIdCount);

    if (platformIdCount != 1) {
        printf("warning: found %d platforms, defaulting to first.\n", platformIdCount);
    }
    clGetPlatformIDs(1, &props->platformId, NULL);

    clGetDeviceIDs(props->platformId, CL_DEVICE_TYPE_ALL, 0, NULL, &props->numDeviceIds);
    props->deviceIds = (cl_device_id *)malloc(props->numDeviceIds * sizeof(cl_device_id));
    clGetDeviceIDs(props->platformId, CL_DEVICE_TYPE_ALL, props->numDeviceIds, props->deviceIds, NULL);

    const cl_context_properties contextProperties[] = {
        CL_CONTEXT_PLATFORM,
        (cl_context_properties)props->platformId,
        0
    };

    cl_int status = CL_SUCCESS;
    props->context = clCreateContext(contextProperties, props->numDeviceIds, props->deviceIds, NULL, NULL, &status);

    if (status != CL_SUCCESS) {
        return status;
    }

    struct stat st;
    stat(KERNEL_SOURCE, &st);
    if (st.st_size <= 0) {
        fprintf(stderr, "unable to find kernel source.\n");
        return INVALID_KERNEL_FILE;
    }

    char * source = (char *)malloc((st.st_size + 1) * sizeof(char));
    source[st.st_size] = '\0';
    FILE * file = fopen(KERNEL_SOURCE, "r");
    if (file == NULL) {
        fprintf(stderr, "unable to read kernel source.\n");
        free(source);
        return INVALID_KERNEL_FILE;
    }

    fread(source, st.st_size, 1, file);
    size_t length = st.st_size;
    fclose(file);

    do {
        props->kernels = (cl_kernel *)calloc(props->numDeviceIds, sizeof(cl_kernel));
        for (int i = 0; i < props->numDeviceIds; i++) {
            cl_program program = clCreateProgramWithSource(props->context, 1, (const char **)&source, &length, &status);
            if (status != CL_SUCCESS) {
                break;
            }

            status = clBuildProgram(program, 1, props->deviceIds + i, NULL, NULL, NULL);
            if (status != CL_SUCCESS) {
                size_t logSize;
                clGetProgramBuildInfo(program, props->deviceIds[i], CL_PROGRAM_BUILD_LOG, 0, NULL, &logSize);
                char * log = (char *)malloc((logSize + 1) * sizeof(char));
                log[logSize] = '\0';
                clGetProgramBuildInfo(program, props->deviceIds[i], CL_PROGRAM_BUILD_LOG, logSize, log, NULL);

                printf("%s\n", log);
                free(log);
                break;
            }

            props->kernels[i] = clCreateKernel(program, KERNEL_NAME, &status);
            if (status != CL_SUCCESS) {
                break;
            }

            clReleaseProgram(program);
        }

        free(source);
        if (status != CL_SUCCESS) {
            break;
        }

        cl_ulong maxMemAllocSize;
        cl_ulong minMaxMemAllocSize = MAX_BUFFER_SIZE * sizeof(cl_ulong);
        for (int i = 0; i < props->numDeviceIds; i++) {
            clGetDeviceInfo(props->deviceIds[i], CL_DEVICE_MAX_MEM_ALLOC_SIZE, sizeof(cl_ulong), &maxMemAllocSize, NULL);
            if (minMaxMemAllocSize > maxMemAllocSize) {
                minMaxMemAllocSize = maxMemAllocSize;
            }
        }
        props->numMatches = minMaxMemAllocSize / sizeof(cl_ulong);

        if (props->numMatches < MAX_BUFFER_SIZE) {
            printf("warning: only %d matches will be found.\n", props->numMatches);
        }

        props->matches = (cl_mem *)calloc(props->numDeviceIds, sizeof(cl_mem));
        for (int i = 0; i < props->numDeviceIds; i++) {
            props->matches[i] = clCreateBuffer(props->context, CL_MEM_WRITE_ONLY, sizeof(cl_ulong) * props->numMatches, NULL, &status);
            if (status != CL_SUCCESS) {
                break;
            }
        }
        if (status != CL_SUCCESS) {
            break;
        }

        cl_uint zero = 0;
        props->numMatchesFound = (cl_mem *)calloc(props->numDeviceIds, sizeof(cl_mem));
        for (int i = 0; i < props->numDeviceIds; i++) {
            props->numMatchesFound[i] = clCreateBuffer(props->context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(cl_uint), &zero, &status);
            if (status != CL_SUCCESS) {
                break;
            }
        }
        if (status != CL_SUCCESS) {
            break;
        }

        for (int i = 0; i < props->numDeviceIds; i++) {
            clSetKernelArg(props->kernels[i], ARGS_MATCHES, sizeof(cl_mem), props->matches + i);
            clSetKernelArg(props->kernels[i], ARGS_NUM_MATCHES, sizeof(cl_uint), &props->numMatches);
            clSetKernelArg(props->kernels[i], ARGS_NUM_MATCHES_FOUND, sizeof(cl_mem), props->numMatchesFound + i);
        }

        props->queues = (cl_command_queue *)calloc(props->numDeviceIds, sizeof(cl_command_queue));
        for (int i = 0; i < props->numDeviceIds; i++) {
            props->queues[i] = clCreateCommandQueue(props->context, props->deviceIds[i], 0, &status);
            if (status != CL_SUCCESS) {
                break;
            }
        }
        if (status != CL_SUCCESS) {
            break;
        }

        return status;
    } while (0);

    cleanup(props);
    return status;
}

void * hostThread(void * threadProps) {
    threadProperties * props = (threadProperties *)threadProps;
    cl_ulong offset;
    clSetKernelArg(props->clProps->kernels[props->deviceIndex], ARGS_DEVICE_INDEX, sizeof(cl_uint), &props->deviceIndex);

    while (1) {
        pthread_mutex_lock(props->offsetMutex);
        offset = *(props->offset);
        *(props->offset) = offset + WORK_GROUP_SIZE;
        pthread_mutex_unlock(props->offsetMutex);

        if (offset >= MAX_OFFSET) {
            break;
        }

        clSetKernelArg(props->clProps->kernels[props->deviceIndex], ARGS_OFFSET, sizeof(cl_ulong), &offset);

        cl_event event;
        cl_int status = clEnqueueNDRangeKernel(props->clProps->queues[props->deviceIndex],
                                               props->clProps->kernels[props->deviceIndex],
                                               1, NULL, &WORK_GROUP_SIZE, NULL, 0, NULL, &event);
        if (status != CL_SUCCESS) {
            fprintf(stderr, "error enqueuing kernel on device %d: %d\n", props->deviceIndex, status);
            pthread_exit(NULL);
        }

        clWaitForEvents(1, &event);
        clReleaseEvent(event);
    }
    return NULL;
}

void * monitorThread(void * monitorProps) {
    monitorProperties * props = (monitorProperties *)monitorProps;
    cl_uint numMatchesFound;
    cl_uint totalNumMatchesFound;

    while (*(props->shouldMonitor)) {
        totalNumMatchesFound = 0;

        for (int i = 0; i < props->numDeviceIds; i++) {
            clEnqueueReadBuffer(props->queues[i], props->numMatchesFound[i], CL_TRUE, 0, sizeof(cl_uint), &numMatchesFound, 0, NULL, NULL);
            totalNumMatchesFound += numMatchesFound;
        }
        printf("offset: %llu, matches found: %u\n", *(props->offset), totalNumMatchesFound);
        sleep(1);
    }

    return NULL;
}

int main() {
    clProperties props;
    cl_int status = init(&props);
    if (status != CL_SUCCESS) {
        fprintf(stderr, "error during init: %d\n", status);
        exit(1);
    }

    cl_ulong offset = 1;
    int shouldMonitor = 1;

    monitorProperties monitorProps;
    monitorProps.numDeviceIds = props.numDeviceIds;
    monitorProps.queues = props.queues;
    monitorProps.numMatchesFound = props.numMatchesFound;
    monitorProps.offset = &offset;
    monitorProps.shouldMonitor = &shouldMonitor;

    pthread_t monitor;
    pthread_create(&monitor, NULL, monitorThread, &monitorProps);

    pthread_mutex_t offsetMutex;
    pthread_mutex_init(&offsetMutex, NULL);

    threadProperties * threadProps = (threadProperties *)malloc(props.numDeviceIds * sizeof(threadProperties));
    for (int i = 0; i < props.numDeviceIds; i++) {
        threadProps[i].clProps = &props;
        threadProps[i].deviceIndex = i;
        threadProps[i].offsetMutex = &offsetMutex;
        threadProps[i].offset = &offset;
    }

    pthread_t * threads = (pthread_t *)malloc(props.numDeviceIds * sizeof(pthread_t));
    for (int i = 0; i < props.numDeviceIds; i++) {
        pthread_create(threads + i, NULL, hostThread, threadProps + i);
    }

    for (int i = 0; i < props.numDeviceIds; i++) {
        pthread_join(threads[i], NULL);
    }

    shouldMonitor = 0;
    pthread_join(monitor, NULL);

    cl_ulong * matches = (cl_ulong *)malloc(props.numMatches * sizeof(cl_ulong));
    cl_uint numMatchesFound;
    cl_uint totalNumMatchesFound = 0;

    for (int i = 0; i < props.numDeviceIds; i++) {
        clEnqueueReadBuffer(props.queues[i], props.numMatchesFound[i], CL_TRUE, 0, sizeof(cl_uint), &numMatchesFound, 0, NULL, NULL);
        if (numMatchesFound > 0) {
            clEnqueueReadBuffer(props.queues[i], props.matches[i], CL_TRUE, 0, numMatchesFound * sizeof(cl_ulong), matches + totalNumMatchesFound, 0, NULL, NULL);
            totalNumMatchesFound += numMatchesFound;
        }
    }

    for (int i = 0; i < totalNumMatchesFound; i++) {
        int deviceIndex = (matches[i] >> 60) & 0xf;
        uint64_t match = matches[i] & 0x0fffffffffffffffLL;

        printf("%d found %llu\n", deviceIndex, match);
    }

    free(matches);
    free(threadProps);
    free(threads);
    pthread_mutex_destroy(&offsetMutex);
    cleanup(&props);
    return 0;
}

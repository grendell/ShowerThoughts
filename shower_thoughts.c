#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// These are more or less arbitrary.
const uint64_t MAX_BUFFER_SIZE = 1ULL << 20;
const size_t WORK_GROUP_SIZE = 1ULL << 24;
const uint64_t MAX_OFFSET = 100000000;

typedef struct monitorProperties monitorProperties;
struct monitorProperties {
    uint32_t * numMatchesFound;
    uint64_t * offset;
    int * shouldMonitor;
};

void * monitorThread(void * monitorProps) {
    monitorProperties * props = (monitorProperties *)monitorProps;

    while (*(props->shouldMonitor)) {
        printf("offset: %llu, matches found: %u\n", *(props->offset), *(props->numMatchesFound));
        sleep(1);
    }

    return NULL;
}

int ipow(int base, int power) {
    int ret = base;
    for (int i = 1; i < power; ++i) {
        ret = ret * base;
    }

    return ret;
}

void checkNumber(uint64_t * matches, uint32_t numMatches, uint32_t * numFoundMatches, uint64_t offset) {
    uint64_t poweredSum = 0;
    for (int power = 1; poweredSum < offset && power < 20; ++power) {
        uint64_t scratch = offset;
        poweredSum = 0;

        while (scratch > 0) {
            poweredSum += ipow(scratch % 10, power);
            scratch /= 10;
        }
        poweredSum += ipow(scratch, power);

        if (offset == poweredSum) {
            uint32_t index = (*numFoundMatches)++;

            if (index < numMatches) {
                matches[index] = offset;
            }

            break;
        }
    }
}

int main() {
    uint64_t * matches = (uint64_t *)malloc(MAX_BUFFER_SIZE * sizeof(uint64_t));
    uint32_t numMatchesFound = 0;
    uint64_t offset = 1;
    int shouldMonitor = 1;

    monitorProperties monitorProps;
    monitorProps.numMatchesFound = &numMatchesFound;
    monitorProps.offset = &offset;
    monitorProps.shouldMonitor = &shouldMonitor;

    pthread_t monitor;
    pthread_create(&monitor, NULL, monitorThread, &monitorProps);

    for ( ; offset < MAX_OFFSET; offset++) {
        checkNumber(matches, MAX_BUFFER_SIZE, &numMatchesFound, offset);
    }

    shouldMonitor = 0;
    pthread_join(monitor, NULL);

    for (int i = 0; i < numMatchesFound; i++) {
        printf("%llu\n", matches[i]);
    }

    free(matches);
    return 0;
}
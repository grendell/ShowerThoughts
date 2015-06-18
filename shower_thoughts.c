#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// These are more or less arbitrary.
const uint64_t MAX_BUFFER_SIZE = 1ULL << 20;
const size_t WORK_GROUP_SIZE = 1ULL << 24;
const uint64_t MAX_OFFSET = 1ULL << 31;

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

uint64_t parseOnesValue(uint32_t ones) {
    switch(ones) {
        case 9: {
            return 'n' + 'i' + 'n' + 'e'
                 - 'a' - 'a' - 'a' - 'a'
                 +  1  +  1  +  1  +  1;
        }
        case 8: {
            return 'e' + 'i' + 'g' + 'h' + 't'
                 - 'a' - 'a' - 'a' - 'a' - 'a'
                 +  1  +  1  +  1  +  1  +  1;
        }
        case 7: {
            return 's' + 'e' + 'v' + 'e' + 'n'
                 - 'a' - 'a' - 'a' - 'a' - 'a'
                 +  1  +  1  +  1  +  1  +  1;
        }
        case 6: {
            return 's' + 'i' + 'x'
                 - 'a' - 'a' - 'a'
                 +  1  +  1  +  1;
        }
        case 5: {
            return 'f' + 'i' + 'v' + 'e'
                 - 'a' - 'a' - 'a' - 'a'
                 +  1  +  1  +  1  +  1;
        }
        case 4: {
            return 'f' + 'o' + 'u' + 'r'
                 - 'a' - 'a' - 'a' - 'a'
                 +  1  +  1  +  1  +  1;
        }
        case 3: {
            return 't' + 'h' + 'r' + 'e' + 'e'
                 - 'a' - 'a' - 'a' - 'a' - 'a'
                 +  1  +  1  +  1  +  1  +  1;
        }
        case 2: {
            return 't' + 'w' + 'o'
                 - 'a' - 'a' - 'a'
                 +  1  +  1  +  1;
        }
        case 1: {
            return 'o' + 'n' + 'e'
                 - 'a' - 'a' - 'a'
                 +  1  +  1  +  1;
        }
    }

    return 0;
}

uint64_t parseTensValue(uint32_t tensGroup) {
    uint64_t sum = 0;
    uint32_t tens = tensGroup / 10;
    uint32_t ones = tensGroup % 10;

    if (tens != 1) {
        sum += parseOnesValue(ones);
    }

    switch (tens) {
        case 9: {
            sum += 'n' + 'i' + 'n' + 'e' + 't' + 'y'
                 - 'a' - 'a' - 'a' - 'a' - 'a' - 'a'
                 +  1  +  1  +  1  +  1  +  1  +  1;
            break;
        }
        case 8: {
            sum += 'e' + 'i' + 'g' + 'h' + 't' + 'y'
                 - 'a' - 'a' - 'a' - 'a' - 'a' - 'a'
                 +  1  +  1  +  1  +  1  +  1  +  1;
            break;
        }
        case 7: {
            sum += 's' + 'e' + 'v' + 'e' + 'n' + 't' + 'y'
                 - 'a' - 'a' - 'a' - 'a' - 'a' - 'a' - 'a'
                 +  1  +  1  +  1  +  1  +  1  +  1  +  1;
            break;
        }
        case 6: {
            sum += 's' + 'i' + 'x' + 't' + 'y'
                 - 'a' - 'a' - 'a' - 'a' - 'a'
                 +  1  +  1  +  1  +  1  +  1;
            break;
        }
        case 5: {
            sum += 'f' + 'i' + 'f' + 't' + 'y'
                 - 'a' - 'a' - 'a' - 'a' - 'a'
                 +  1  +  1  +  1  +  1  +  1;
            break;
        }
        case 4: {
            sum += 'f' + 'o' + 'r' + 't' + 'y'
                 - 'a' - 'a' - 'a' - 'a' - 'a'
                 +  1  +  1  +  1  +  1  +  1;
            break;
        }
        case 3: {
            sum += 't' + 'h' + 'i' + 'r' + 't' + 'y'
                 - 'a' - 'a' - 'a' - 'a' - 'a' - 'a'
                 +  1  +  1  +  1  +  1  +  1  +  1;
            break;
        }
        case 2: {
            sum += 't' + 'w' + 'e' + 'n' + 't' + 'y'
                 - 'a' - 'a' - 'a' - 'a' - 'a' - 'a'
                 +  1  +  1  +  1  +  1  +  1  +  1;
            break;
        }
        case 1: {
            switch(ones) {
                case 9: {
                    sum += 'n' + 'i' + 'n' + 'e' + 't' + 'e' + 'e' + 'n'
                         - 'a' - 'a' - 'a' - 'a' - 'a' - 'a' - 'a' - 'a'
                         +  1  +  1  +  1  +  1  +  1  +  1  +  1  +  1;
                    break;
                }
                case 8: {
                    sum += 'e' + 'i' + 'g' + 'h' + 't' + 'e' + 'e' + 'n'
                         - 'a' - 'a' - 'a' - 'a' - 'a' - 'a' - 'a' - 'a'
                         +  1  +  1  +  1  +  1  +  1  +  1  +  1  +  1;
                    break;
                }
                case 7: {
                    sum += 's' + 'e' + 'v' + 'e' + 'n' + 't' + 'e' + 'e' + 'n'
                         - 'a' - 'a' - 'a' - 'a' - 'a' - 'a' - 'a' - 'a' - 'a'
                         +  1  +  1  +  1  +  1  +  1  +  1  +  1  +  1  +  1;
                    break;
                }
                case 6: {
                    sum += 's' + 'i' + 'x' + 't' + 'e' + 'e' + 'n'
                         - 'a' - 'a' - 'a' - 'a' - 'a' - 'a' - 'a'
                         +  1  +  1  +  1  +  1  +  1  +  1  +  1;
                    break;
                }
                case 5: {
                    sum += 'f' + 'i' + 'f' + 't' + 'e' + 'e' + 'n'
                         - 'a' - 'a' - 'a' - 'a' - 'a' - 'a' - 'a'
                         +  1  +  1  +  1  +  1  +  1  +  1  +  1;
                    break;
                }
                case 4: {
                    sum += 'f' + 'o' + 'u' + 'r' + 't' + 'e' + 'e' + 'n'
                         - 'a' - 'a' - 'a' - 'a' - 'a' - 'a' - 'a' - 'a'
                         +  1  +  1  +  1  +  1  +  1  +  1  +  1  +  1;
                    break;
                }
                case 3: {
                    sum += 't' + 'h' + 'i' + 'r' + 't' + 'e' + 'e' + 'n'
                         - 'a' - 'a' - 'a' - 'a' - 'a' - 'a' - 'a' - 'a'
                         +  1  +  1  +  1  +  1  +  1  +  1  +  1  +  1;
                    break;
                }
                case 2: {
                    sum += 't' + 'w' + 'e' + 'l' + 'v' + 'e'
                         - 'a' - 'a' - 'a' - 'a' - 'a' - 'a'
                         +  1  +  1  +  1  +  1  +  1  +  1;
                    break;
                }
                case 1: {
                    sum += 'e' + 'l' + 'e' + 'v' + 'e' + 'n'
                         - 'a' - 'a' - 'a' - 'a' - 'a' - 'a'
                         +  1  +  1  +  1  +  1  +  1  +  1;
                    break;
                }
                case 0: {
                    sum += 't' + 'e' + 'n'
                         - 'a' - 'a' - 'a'
                         +  1  +  1  +  1;
                    break;
                }
            }
            break;
        }
    }

    return sum;
}

uint64_t getHundredValue() {
    return 'h' + 'u' + 'n' + 'd' + 'r' + 'e' + 'd'
         - 'a' - 'a' - 'a' - 'a' - 'a' - 'a' - 'a'
         +  1  +  1  +  1  +  1  +  1  +  1  +  1;
}

uint64_t getThousandValue() {
    return 't' + 'h' + 'o' + 'u' + 's' + 'a' + 'n' + 'd'
         - 'a' - 'a' - 'a' - 'a' - 'a' - 'a' - 'a' - 'a'
         +  1  +  1  +  1  +  1  +  1  +  1  +  1  +  1;
}

uint64_t getMillionValue() {
    return 'm' + 'i' + 'l' + 'l' + 'i' + 'o' + 'n'
         - 'a' - 'a' - 'a' - 'a' - 'a' - 'a' - 'a'
         +  1  +  1  +  1  +  1  +  1  +  1  +  1;
}

uint64_t getBillionValue() {
    return 'b' + 'i' + 'l' + 'l' + 'i' + 'o' + 'n'
         - 'a' - 'a' - 'a' - 'a' - 'a' - 'a' - 'a'
         +  1  +  1  +  1  +  1  +  1  +  1  +  1;
}

uint64_t getTrillionValue() {
    return 't' + 'r' + 'i' + 'l' + 'l' + 'i' + 'o' + 'n'
         - 'a' - 'a' - 'a' - 'a' - 'a' - 'a' - 'a' - 'a'
         +  1  +  1  +  1  +  1  +  1  +  1  +  1  +  1;
}

uint64_t parseDigitGroup(uint32_t digitGroup) {
    uint64_t sum = 0;
    uint32_t hundreds = digitGroup / 100;

    if (hundreds > 0) {
        sum += parseOnesValue(hundreds);
        sum += getHundredValue();
    }
    
    uint32_t tensGroup = digitGroup % 100;
    if (tensGroup > 0) {
        sum += parseTensValue(tensGroup);
    }

    return sum;
}

void checkNumber(uint64_t * matches, uint32_t numMatches, uint32_t * numFoundMatches, uint64_t offset) {
    uint64_t scratch = offset;

    uint64_t sum = 0;
    int i = 0;
    while(scratch > 0) {
        uint32_t digitGroup = scratch % 1000;
        sum += parseDigitGroup(digitGroup);
        scratch /= 1000;

        switch (i++) {
            case 0: {
                uint32_t tensValue = digitGroup % 100;
                if (tensValue > 0 && offset >= 100) {
                    sum += 'a' + 'n' + 'd'
                         - 'a' - 'a' - 'a'
                         +  1  +  1  +  1;
                }
                break;
            }
            case 1: {
                sum += getThousandValue();
                break;
            }
            case 2: {
                sum += getMillionValue();
                break;
            }
            case 3: {
                sum += getBillionValue();
                break;
            }
            case 4: {
                sum += getTrillionValue();
                break;
            }
        }
    }

    if (offset == sum) {
        uint32_t index = (*numFoundMatches)++;

        if (index < numMatches) {
            matches[index] = offset;
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

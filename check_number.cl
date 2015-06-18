ulong parseOnesValue(uint ones) {
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

ulong parseTensValue(uint tensGroup) {
    ulong sum = 0;
    uint tens = tensGroup / 10;
    uint ones = tensGroup % 10;

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

ulong getHundredValue() {
    return 'h' + 'u' + 'n' + 'd' + 'r' + 'e' + 'd'
         - 'a' - 'a' - 'a' - 'a' - 'a' - 'a' - 'a'
         +  1  +  1  +  1  +  1  +  1  +  1  +  1;
}

ulong getThousandValue() {
    return 't' + 'h' + 'o' + 'u' + 's' + 'a' + 'n' + 'd'
         - 'a' - 'a' - 'a' - 'a' - 'a' - 'a' - 'a' - 'a'
         +  1  +  1  +  1  +  1  +  1  +  1  +  1  +  1;
}

ulong getMillionValue() {
    return 'm' + 'i' + 'l' + 'l' + 'i' + 'o' + 'n'
         - 'a' - 'a' - 'a' - 'a' - 'a' - 'a' - 'a'
         +  1  +  1  +  1  +  1  +  1  +  1  +  1;
}

ulong getBillionValue() {
    return 'b' + 'i' + 'l' + 'l' + 'i' + 'o' + 'n'
         - 'a' - 'a' - 'a' - 'a' - 'a' - 'a' - 'a'
         +  1  +  1  +  1  +  1  +  1  +  1  +  1;
}

ulong getTrillionValue() {
    return 't' + 'r' + 'i' + 'l' + 'l' + 'i' + 'o' + 'n'
         - 'a' - 'a' - 'a' - 'a' - 'a' - 'a' - 'a' - 'a'
         +  1  +  1  +  1  +  1  +  1  +  1  +  1  +  1;
}

ulong parseDigitGroup(uint digitGroup) {
    ulong sum = 0;
    uint hundreds = digitGroup / 100;

    if (hundreds > 0) {
        sum += parseOnesValue(hundreds);
        sum += getHundredValue();
    }
    
    uint tensGroup = digitGroup % 100;
    if (tensGroup > 0) {
        sum += parseTensValue(tensGroup);
    }

    return sum;
}

__kernel void checkNumber(__global ulong * matches, uint numMatches, __global uint * numFoundMatches, ulong offset, uint deviceIndex) {
    ulong input = offset + get_global_id(0);
    ulong scratch = input;

    ulong sum = 0;
    int i = 0;
    while(scratch > 0) {
        uint digitGroup = scratch % 1000;
        sum += parseDigitGroup(digitGroup);
        scratch /= 1000;

        switch (i++) {
            case 0: {
                uint tensValue = digitGroup % 100;
                if (tensValue > 0 && input >= 100) {
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

    if (input == sum) {
        uint index = atomic_inc(numFoundMatches);

        if (index < numMatches) {
            matches[index] = ((ulong)deviceIndex << 60) | input;
        }
    }
}

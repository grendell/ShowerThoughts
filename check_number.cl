int ipow(int base, int power) {
    int ret = base;
    for (int i = 1; i < power; ++i) {
        ret = ret * base;
    }

    return ret;
}

__kernel void checkNumber(__global ulong * matches, uint numMatches, __global uint * numFoundMatches, ulong offset, uint deviceIndex) {
    ulong input = offset + get_global_id(0);

    ulong poweredSum = 0;
    for (int power = 1; poweredSum < offset && power < 20; ++power) {
        ulong scratch = input;
        poweredSum = 0;

        while (scratch > 0) {
            poweredSum += ipow(scratch % 10, power);
            scratch /= 10;
        }

        if (input == poweredSum) {
            uint index = atomic_inc(numFoundMatches);

            if (index < numMatches) {
                matches[index] = ((ulong)deviceIndex << 60) | input;
            }

            break;
        }
    }
}

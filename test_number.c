#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char ** argv) {
    if (argc != 2) {
        fprintf(stderr, "usage: %s <number as words>\n", argv[0]);
        exit(1);
    }

    int sum = 0;
    for (int i = 0; i < strlen(argv[1]); i++) {
        sum += argv[1][i] - 'a' + 1;
    }

    printf("%d\n", sum);
    return 0;
}

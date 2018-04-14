#include <stdio.h>
#include "scanner.h"
#include "cubes.h"

char proto_pattern[CUBES_NUM];
extern const char permutations[PATTERNS_NUM / 2][3];

int main() {
    int sequenceLen;
    unsigned char pat3[PAT3_LEN] = {1, 0, 3};
    Step *sequence = getSequence(&sequenceLen, 1, pat3);
    for (int i = 0; i < sequenceLen; i++)
        printf("%i/%i ", sequence[i].step, sequence[i].push);
    return 0;
}
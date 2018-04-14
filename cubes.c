//
// Created by MI on 04/11/2018.
//

#include "cubes.h"
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>

const char inversions[CUBES_NUM] = {NULL_CUBE, 3, 4, 1, 2};
const char permutations[PATTERNS_NUM / 2][PER_LEN] = {
        {0, 1, 4},
        {0, 4, 1},
        {1, 0, 4},
        {1, 4, 0},
        {4, 0, 1},
        {4, 1, 0}};

char proto_pattern[CUBES_NUM];

char diff(char cube, char cube1) {
    char num = cube1 - cube;
    if (num == 3)
        return -1;
    else if (num == -3)
        return 1;
    else return num;
}

int cmp(Weight weight, Weight weight1) {
    if (weight.steps != weight1.steps)
        return weight.steps - weight1.steps;
    else return abs(weight.shift) - abs(weight1.shift);
}

void get_freeCubes(char *cube, char *cube1, const unsigned char *pat3) {
    unsigned int set = 0b11111u ^(0b1u << pat3[0]) ^(0b1u << pat3[1]) ^(0b1u << pat3[2]);
    unsigned int tmp = set & (~set + 1);
    set ^= tmp;
    *cube = (char) (((tmp & 0b10u) != 0) * 1u |
                    ((tmp & 0b100u) != 0) * 2u |
                    ((tmp & 0b1000u) != 0) * 3u |
                    ((tmp & 0b10000u) != 0) * 4u);
    tmp = set & (~set + 1);
    *cube1 = (char) (((tmp & 0b10u) != 0) * 1u |
                     ((tmp & 0b100u) != 0) * 2u |
                     ((tmp & 0b1000u) != 0) * 3u |
                     ((tmp & 0b10000u) != 0) * 4u);
}

void generate_pattern(char pattern[CUBES_NUM], const char permutation[3]) {
    int k = 0;
    for (int i = 0; i < PER_LEN; i++) {
        int shift = permutation[i];
        for (int j = 0; j < 1 + 2 * (shift % 2); j++, k++)
            pattern[k] = proto_pattern[shift + j];
    }
}

void normalize_pattern(char pattern[CUBES_NUM]) {
    char rows[CUBES_NUM] = {pattern[0] % (char) 2, 1, 0, 1, 0};
    char shift = NULL_CUBE;
    for (int i = 0; i < CUBES_NUM - 1; i++) {
        char cube = pattern[i];
        char cube1 = pattern[i + 1];
        char inv_cube = inversions[cube];
        if (i < 2 && rows[cube] != rows[cube1] && inv_cube != pattern[0])
            rows[inv_cube] = !rows[inv_cube];
        if (rows[cube] == shift)
            rows[cube] += 2;
        if (i < 2 && inversions[cube] == cube1)
            shift = rows[cube];
    }

    char cubes[EDGE_CUBES_NUM] = {NULL_CUBE, NULL_CUBE, NULL_CUBE, NULL_CUBE};
    for (int i = 0; i < CUBES_NUM; i++) {
        char row = rows[pattern[i]];
        if (cubes[row] == NULL_CUBE)
            cubes[row] = pattern[i];
        pattern[i] = cubes[row];
    }
}

Weight get_patWeight(char initCube, char *pattern) {
    char step = diff(initCube, pattern[0]);
    Weight weight = {.steps = (char) abs(step), .shift = step};
    for (int j = 0; j < CUBES_NUM - 1; j++) {
        step = diff(pattern[j], pattern[j + 1]);
        weight.steps += abs(step);
        weight.shift += step;
    }
    return weight;
}

Step *getSequence(int *sequenceLen, char initCube, const unsigned char pat3[PAT3_LEN]) {
    proto_pattern[1] = pat3[0];
    proto_pattern[2] = pat3[1];
    proto_pattern[3] = pat3[2];
    get_freeCubes(proto_pattern, proto_pattern + 4, pat3);

    char patterns[PATTERNS_NUM][CUBES_NUM];
    char *pattern = patterns[0];
    Weight minWeight = {.steps = SCHAR_MAX, .shift = SCHAR_MAX};
    for (int i = 0; i < PATTERNS_NUM; i++) {
        char *pat = patterns[i];
        if (i == 6) {
            char tmp = proto_pattern[1];
            proto_pattern[1] = proto_pattern[3];
            proto_pattern[3] = tmp;
        }
        generate_pattern(pat, permutations[i % 6]);
        if (pat[0]) normalize_pattern(pat);

        Weight weight = pat[0] ? get_patWeight(initCube, pat) :
                        (Weight) {.steps = SCHAR_MAX, .shift = SCHAR_MAX};
        if (cmp(weight, minWeight) < 0) {
            minWeight = weight;
            pattern = pat;
        }
    }

    int j = 0;
    Step *sequence = malloc(CUBES_NUM * sizeof(int));
    sequence[j++].step = diff(initCube, pattern[0]);
    for (int i = 0; i < CUBES_NUM - 1; i++) {
        char step = diff(pattern[i], pattern[i + 1]);
        if (step) sequence[j++].step = step;
    }
    j = 0;
    char nums[2] = {0, 0};
    for (int i = 0; i < CUBES_NUM; i++) {
        int row = pattern[i] % 2;
        nums[row]++;
        if (i == CUBES_NUM - 1 || pattern[i] != pattern[i + 1])
            sequence[j++].push = nums[row];
        if (nums[row] == 2)
            nums[!row]++;
    }
    *sequenceLen = j;
    return sequence;
}
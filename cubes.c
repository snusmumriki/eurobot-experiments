//
// Created by MI on 04/11/2018.
//

#include "cubes.h"
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>

const char inv[CUBES_NUM] = {5, 3, 4, 1, 2};
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

void set_freeCubes(char *cube, char *cube1, const unsigned char pat3[PAT3_LEN]) {
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
    int index = 0;
    for (int i = 0; i < PER_LEN; i++) {
        int shift = permutation[i];
        for (int j = 0; j < 1 + 2 * (shift % 2); j++, index++)
            pattern[index] = proto_pattern[shift + j];
    }
}

void normalize_pattern(char pattern[CUBES_NUM]) {
    if (pattern[0]) {
        char rows[CUBES_NUM] = {pattern[0] % (char) 2, 1, 0, 1, 0};
        char shift = 5;
        for (int i = 0; i < CUBES_NUM - 1; i++) {
            char cube = pattern[i];
            char cube1 = pattern[i + 1];
            char invCube = inv[cube];
            if (i < 2 && rows[cube] != rows[cube1] && invCube != pattern[0])
                rows[invCube] = !rows[invCube];

            if (rows[cube] == shift)
                rows[cube] += 2;
            if (i < 2 && inv[cube] == cube1)
                shift = rows[cube];
        }

        char cubes[4] = {5, 5, 5, 5};
        for (int i = 0; i < CUBES_NUM; i++) {
            char row = rows[pattern[i]];
            if (cubes[row] == 5)
                cubes[row] = pattern[i];
            pattern[i] = cubes[row];
        }
    }
}

int optPatIndex(char init_cube, char patterns[PATTERNS_NUM][CUBES_NUM]) {
    Weight minWeight = {.steps = SCHAR_MAX, .shift = SCHAR_MAX};
    int minI = 0;
    for (int i = 0; i < PATTERNS_NUM; i++) {
        char *pattern = patterns[i];
        Weight weight;
        if (pattern[0]) {
            char step = diff(init_cube, pattern[0]);
            weight.steps = (char) abs(step);
            weight.shift = step;
            for (int j = 0; j < CUBES_NUM - 1; j++) {
                step = diff(pattern[j], pattern[j + 1]);
                weight.steps += abs(step);
                weight.shift += step;
            }
        } else {
            weight.steps = SCHAR_MAX;
            weight.shift = SCHAR_MAX;
        }
        if (cmp(weight, minWeight) < 0) {
            minWeight = weight;
            minI = i;
        }
    }

    return minI;
}

Step *getSequence(int *sequenceLen, char initCube, const unsigned char pat3[PAT3_LEN]) {
    proto_pattern[1] = pat3[0];
    proto_pattern[2] = pat3[1];
    proto_pattern[3] = pat3[2];
    set_freeCubes(proto_pattern, proto_pattern + 4, pat3);
    char patterns[PATTERNS_NUM][CUBES_NUM];
    for (int i = 0; i < PATTERNS_NUM; i++) {
        if (i == 6) {
            char tmp = proto_pattern[1];
            proto_pattern[1] = proto_pattern[3];
            proto_pattern[3] = tmp;
        }
        generate_pattern(patterns[i], permutations[i % 6]);

        for (int j = 0; j < CUBES_NUM; j++)
            printf("%i ", patterns[i][j]);
        printf("    ");

        normalize_pattern(patterns[i]);

        for (int j = 0; j < CUBES_NUM; j++)
            printf("%i ", patterns[i][j]);
        printf("\n");
    }

    int j = 0;
    Step *sequence = malloc(CUBES_NUM * sizeof(int));

    char *pattern = patterns[optPatIndex(initCube, patterns)];
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

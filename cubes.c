//
// Created by MI on 04/11/2018.
//

#include "cubes.h"
#include <limits.h>
#include <stdlib.h>

#define CUBES_NUM 5
#define PATTERNS_NUM 12
#define PAT3_LEN 3
#define ROW_LEN (char) 3

typedef struct {
    char rotation;
    char push;
} Step;

typedef struct {
    char steps;
    char shift;
} Weight;

const char inv[CUBES_NUM] = {0, 3, 4, 1, 2};
const char permutations[PATTERNS_NUM / 2][3] = {
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
    *cube = (char) (((tmp & 0b1u) != 0) * 1u |
                    ((tmp & 0b10u) != 0) * 2u |
                    ((tmp & 0b1000u) != 0) * 3u |
                    ((tmp & 0b10000u) != 0) * 4u);
    tmp = set & (~set + 1);
    *cube1 = (char) (((tmp & 0b1u) != 0) * 1u |
                     ((tmp & 0b10u) != 0) * 2u |
                     ((tmp & 0b1000u) != 0) * 3u |
                     ((tmp & 0b10000u) != 0) * 4u);
}

void generate_pattern(char pattern[CUBES_NUM], const char permutation[3]) {
    int index = 0;
    for (int i = 0; i < PAT3_LEN; i++) {
        int shift = permutation[i];
        for (int j = 0; j < 1 + 2 * (shift % 2); j++, index++)
            pattern[index] = proto_pattern[shift + j];
    }
}

void normalize_pattern(char pattern[CUBES_NUM]) {
    static char rows[CUBES_NUM] = {-1, 1, 0, 1, 0};
    char cubes[4] = {-1, -1, -1, -1};
    rows[0] = (char) (pattern[0] % 2);
    for (int i = 0; i < 2; i++)
        if (rows[pattern[i]] != rows[pattern[i + 1]]) {
            char inv_cube = inv[pattern[i]];
            rows[inv_cube] = !rows[inv_cube];
        }
    char shift = 0;
    for (int i = 0; i < CUBES_NUM - 1; i++) {
        rows[pattern[i]] += shift;
        if (inv[pattern[i]] == pattern[i + 1])
            shift = 2;
        if (shift == 2 && rows[pattern[i]] != rows[pattern[i + 1]])
            shift = 0;
    }
    for (int i = 0; i < CUBES_NUM - 1; i++) {
        char row = rows[pattern[i]];
        if (cubes[row] == -1)
            cubes[row] = pattern[i];
        if (row == rows[pattern[i + 1]])
            pattern[i + 1] = cubes[row];
    }
}

int optPatternIndex(char init_cube, char patterns[PATTERNS_NUM][CUBES_NUM]) {
    Weight minWeight = {.steps = SCHAR_MAX, .shift = SCHAR_MAX};
    int minI = 0;
    for (int i = 0; i < PATTERNS_NUM; i++) {
        char *pattern = patterns[i];
        char step = diff(init_cube, pattern[0]);
        Weight weight = {.steps = (char) abs(step), .shift = step};
        for (int j = 0; j < CUBES_NUM - 1; j++) {
            step = diff(pattern[j], pattern[j + 1]);
            weight.steps += abs(step);
            weight.steps += step;
        }
        if (cmp(weight, minWeight) < 0) {
            minWeight = weight;
            minI = i;
        }
    }
    return minI;
}

Step *get_equence(int *sequenceLen, char initCube, const unsigned char pat3[PAT3_LEN]) {
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
        normalize_pattern(patterns[i]);
    }

    char steps[CUBES_NUM];
    char *pattern = patterns[optPatternIndex(initCube, patterns)];
    steps[0] = diff(initCube, pattern[0]);
    int seqLen = 1;
    for (int i = 0; i < CUBES_NUM - 1; i++)
        seqLen += (steps[i + 1] = diff(pattern[i], pattern[i + 1])) != 0;

    char nums[2] = {ROW_LEN, ROW_LEN};
    char pushes[CUBES_NUM];
    for (int i = 0; i < CUBES_NUM; i++) {
        int row = pattern[i] % 2;
        pushes[i] = ROW_LEN - nums[row]--;
        if (nums[row] == 1)
            nums[!row]--;
    }

    int j = 0;
    Step *sequence = malloc(seqLen * sizeof(Step));
    for (int i = 0; i < CUBES_NUM; i++)
        if (i != 0 && steps[i] != 0)
            sequence[j++] = {.rotation = steps[i], .push = pushes[i]};

    *sequenceLen = seqLen;
    return sequence;
}

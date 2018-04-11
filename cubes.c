//
// Created by MI on 04/11/2018.
//

#include "cubes.h"
#include <limits.h>
#include <stdlib.h>

#define CUBES_NUM 5
#define PATS_NUM 12
#define PAT_LEN 3

typedef struct {
    char rotation;
    char push;
} Step;

typedef struct {
    char steps;
    char shift;
} Weight;

const char inv[CUBES_NUM] = {0, 3, 4, 1, 2};
const char per[PATS_NUM / 2][PAT_LEN] = {
        {0, 1, 4},
        {0, 4, 1},
        {1, 0, 4},
        {1, 4, 0},
        {4, 0, 1},
        {4, 1, 0}};

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

void sequence_set(Step sequence[CUBES_NUM], char init_cube, const char cubes[PAT_LEN]) {
    unsigned int set = 0b11111u ^(0b1u << cubes[0]) ^(0b1u << cubes[1]) ^(0b1u << cubes[2]);
    unsigned int tmp = set & (~set + 1);
    set ^= tmp;
    char proto_pattern[CUBES_NUM];
    proto_pattern[0] = (char) (((tmp & 0b1u) != 0) * 1u |
                               ((tmp & 0b10u) != 0) * 2u |
                               ((tmp & 0b1000u) != 0) * 3u |
                               ((tmp & 0b10000u) != 0) * 4u);
    proto_pattern[1] = cubes[0];
    proto_pattern[2] = cubes[1];
    proto_pattern[3] = cubes[2];
    tmp = set & (~set + 1);
    proto_pattern[4] = (char) (((tmp & 0b1u) != 0) * 1u |
                               ((tmp & 0b10u) != 0) * 2u |
                               ((tmp & 0b1000u) != 0) * 3u |
                               ((tmp & 0b10000u) != 0) * 4u);

    char patterns[PATS_NUM][CUBES_NUM];
    for (int i = 0; i < PATS_NUM; i++) {
        if (i == 6) {
            char tmp = proto_pattern[1];
            proto_pattern[1] = proto_pattern[3];
            proto_pattern[3] = tmp;
        }
        int index = 0;
        for (int j = 0; j < PAT_LEN; j++) {
            int shift = per[i % 6][j];
            for (int k = 0; k < 1 + 2 * (shift & 1); k++, index++)
                patterns[i][index] = proto_pattern[shift + k];
        }
    }

    char rows[CUBES_NUM] = {2, 1, 0, 1, 0};
    for (int i = 0; i < PATS_NUM; i++) {
        char *pattern = patterns[i];
        rows[0] = (char) (pattern[0] % 2);
        for (int j = 0; j < 2; j++)
            if (rows[pattern[j]] != rows[pattern[j + 1]]) {
                char inv_cube = inv[pattern[j]];
                rows[inv_cube] = !rows[inv_cube];
            }
        char shift = 0;
        for (int j = 0; j < CUBES_NUM - 1; j++) {
            rows[pattern[j]] ^= shift;
            if (inv[pattern[j]] == pattern[j + 1])
                shift = 1;
        }
        for (int j = 0; j < CUBES_NUM - 1; j++)
            if (rows[pattern[j]] == rows[pattern[j + 1]])
                pattern[j + 1] = pattern[j];
    }

    Weight minWeight = {.steps = SCHAR_MAX, .shift = SCHAR_MAX};
    int minI = 0;
    for (int i = 0; i < PATS_NUM; i++) {
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

    char steps[CUBES_NUM];
    char *pattern = patterns[minI];
    steps[0] = diff(init_cube, pattern[0]);
    for (int j = 0; j < CUBES_NUM - 1; j++)
        steps[j + 1] = diff(pattern[j], pattern[j + 1]);

    int nums[2] = {3, 3};
    for(int i = 0; i < PAT_LEN; i++);
}

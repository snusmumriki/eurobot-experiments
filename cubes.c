//
// Created by MI on 04/11/2018.
//

#include "cubes.h"

#define CUBES_NUM 5
#define PER_NUM 6
#define PAT_LEN 3

typedef struct {
    char rotation;
    char push;
} Step;

typedef struct {
    char steps;
    char shift;
} Weight;

const unsigned char inv[CUBES_NUM] = {0, 3, 4, 1, 2};
const unsigned char per[PER_NUM][PAT_LEN] = {
        {0, 1, 4},
        {0, 4, 1},
        {1, 0, 4},
        {1, 4, 0},
        {4, 0, 1},
        {4, 1, 0}};

Weight weights[PER_NUM];


char diff(char cube, char cube1) {
    char num = cube1 - cube;
    if (num == 3)
        return -1;
    else if (num == -3)
        return 1;
    else return num;
}

void sequence_set(Step sequence[CUBES_NUM], char init_cube, const unsigned char pattern[PAT_LEN]) {
    unsigned int set = 0b11111u ^(0b1u << pattern[0]) ^(0b1u << pattern[1]) ^(0b1u << pattern[2]);
    unsigned int tmp = set & (~set + 1);
    set ^= tmp;
    unsigned char pre_pattern[CUBES_NUM];
    pre_pattern[0] = (unsigned char) (((tmp & 0b1u) != 0) * 1u |
                                      ((tmp & 0b10u) != 0) * 2u |
                                      ((tmp & 0b1000u) != 0) * 3u |
                                      ((tmp & 0b10000u) != 0) * 4u);
    pre_pattern[1] = pattern[0];
    pre_pattern[2] = pattern[1];
    pre_pattern[3] = pattern[2];
    tmp = set & (~set + 1);
    pre_pattern[4] = (unsigned char) (((tmp & 0b1u) != 0) * 1u |
                                      ((tmp & 0b10u) != 0) * 2u |
                                      ((tmp & 0b1000u) != 0) * 3u |
                                      ((tmp & 0b10000u) != 0) * 4u);
    unsigned char patterns[PER_NUM][CUBES_NUM];
    for (int i = 0; i < PER_NUM; i++) {
        int index = 0;
        for (int j = 0; j < PAT_LEN; j++) {
            int shift = per[i][j];
            for (int k = 0; k < 1 + 2 * (shift & 1); k++, index++)
                patterns[i][index] = pre_pattern[shift + k];
        }
    }

    char rows[CUBES_NUM] = {2, 1, 0, 1, 0};
    for (int i = 0; i < PER_NUM; i++) {

    }

    char groups[CUBES_NUM];
    int groups_size = 0;
}

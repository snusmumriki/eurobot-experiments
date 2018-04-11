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

const char inv[CUBES_NUM] = {-1, 3, 4, 1, 2};
const char per[PER_NUM][PAT_LEN] = {
        {0, 1, 2},
        {0, 2, 1},
        {1, 0, 2},
        {1, 2, 0},
        {2, 0, 1},
        {2, 1, 0}};

char diff(char cube, char cube1) {
    char num = cube1 - cube;
    if (num == 3)
        return -1;
    else if (num == -3)
        return 1;
    else return num;
}

void sequence_set(Step sequence[CUBES_NUM], char init_cube, const unsigned int pattern[PAT_LEN]) {
    char rows[CUBES_NUM] = {-1, 1, 0, 1, 0};
    unsigned int set = 0b11111u ^(0b1u << pattern[0]) ^(0b1u << pattern[1]) ^(0b1u << pattern[2]);
    char pre_pattern[CUBES_NUM][PAT_LEN] = {{},
                                            {},
                                            {}};
    char patterns[PER_NUM][CUBES_NUM] = {

    };
    for (int i = 0; i < PER_NUM; i++)
        for (int j = 0; j < PAT_LEN; j++) {
            for (int k = 0; k < 1 + 2 * (per[i][j] % 2); k++)
        }


}

//
// Created by MI on 04/11/2018.
//

#ifndef UNTITLED2_CUBES_H
#define UNTITLED2_CUBES_H

#define CUBES_NUM 5
#define EDGE_CUBES_NUM 4
#define NULL_CUBE 5
#define PATTERNS_NUM 12
#define PAT3_LEN 3
#define PER_LEN 3

typedef struct {
    char step;
    char push;
} Step;

typedef struct {
    char steps;
    char shift;
} Weight;

char diff(char cube, char cube1);

int cmp(Weight weight, Weight weight1);

void get_freeCubes(char *cube, char *cube1, const unsigned char *pat3);

void generate_pattern(char pattern[CUBES_NUM], const char permutation[3]);

void normalize_pattern(char pattern[CUBES_NUM]);

Weight get_patWeight(char init_cube, char *pattern);

Step *getSequence(int *sequenceLen, char initCube, const unsigned char pat3[PAT3_LEN]);

#endif //UNTITLED2_CUBES_H

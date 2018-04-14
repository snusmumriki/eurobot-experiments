//
// Created by MI on 04/14/2018.
//

#include <malloc.h>
#include <math.h>
#include <float.h>
#include "manipulator.h"

const RGB colors_rgb[CUBES_NUM] = {
        {247, 181, 0},  //yellow
        {97, 153, 59},  //green
        {14, 14, 16},   //black
        {0, 124, 176},  //blue
        {208, 93, 40}   //orange
};

void set_colors(char colors[CUBES_NUM], const RGB cubes_rgb[EDGE_CUBES_NUM]) {
    for (int i = 0; i < EDGE_CUBES_NUM; i++) {
        float minLen = FLT_MAX;
        int minJ = 0;
        for (int j = 0; j < CUBES_NUM; j++) {
            RGB rgb = colors_rgb[j];
            RGB rgb1 = cubes_rgb[i];
            float rd = (float) (rgb1.r - rgb.r);
            float gd = (float) (rgb1.g - rgb.g);
            float bd = (float) (rgb1.b - rgb.b);
            float len = sqrtf(rd * rd + gd * gd + bd * bd);
            if (len < minLen) {
                minLen = len;
                minJ = j;
            }
        }
        colors[i] = (char) minJ;
    }
}

char get_freeColor(const unsigned char cubes[CUBES_NUM]) {
    unsigned int set = 0b11111u ^(0b1u << cubes[0]) ^(0b1u << cubes[1]) ^(0b1u << cubes[2]);
    return (char) (((set & 0b10u) != 0) * 1u |
                   ((set & 0b100u) != 0) * 2u |
                   ((set & 0b1000u) != 0) * 3u |
                   ((set & 0b10000u) != 0) * 4u);
}

//платформа поворачивается влево!
char *get_pat3(const RGB cubes_rgb[EDGE_CUBES_NUM], const char color_pat3[PAT3_LEN]) {
    char colors[CUBES_NUM];
    set_colors(colors, cubes_rgb);
    unsigned char cubes[CUBES_NUM];
    cubes[colors[0]] = 2;
    cubes[colors[1]] = 4;
    cubes[colors[2]] = 1;
    cubes[colors[3]] = 3;
    cubes[get_freeColor(cubes)] = 0;
    char *pat3 = malloc(PER_LEN * sizeof(char));
    pat3[0] = cubes[color_pat3[0]];
    pat3[1] = cubes[color_pat3[1]];
    pat3[2] = cubes[color_pat3[3]];
}

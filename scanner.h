//
// Created by MI on 04/08/2018.
//

#ifndef UNTITLED2_SCANNER_H
#define UNTITLED2_SCANNER_H

#define DIFF 0.5f
#define SAMPLES_NUM 9
#define STEPS_MAX 16
#define STEPS (STEPS_MAX - SAMPLES_NUM / 2)

typedef struct {
    float x;
    float y;
} Point;

void get_target(Point *point, int *angle, const int samples[SAMPLES_NUM]);

#endif //UNTITLED2_SCANNER_H

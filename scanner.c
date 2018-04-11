//
// Created by MI on 04/08/2018.
//

#include <math.h>
#include <malloc.h>
#include <limits.h>
#include <stdio.h>
#include "scanner.h"

void target_set(int *point_x, int *point_y, int *angle, int samples[SAMPLES_NUM]) {
    Point points[SAMPLES_NUM];
    for (int i = 0; i < SAMPLES_NUM; i++) {
        float ang = (float) (STEPS - i) / (float) STEPS_MAX * (float) M_PI;
        points[i].x = cosf(ang) * (float) samples[i];
        points[i].y = sinf(ang) * (float) samples[i];
    }

    float slopes[SAMPLES_NUM - 1];
    for (int i = 0; i < SAMPLES_NUM - 1; i++)
        slopes[i] = (points[i + 1].y - points[i].y) / (points[i + 1].x - points[i].x);

    char diffs[SAMPLES_NUM];
    for (int i = 1; i < SAMPLES_NUM - 1; i++)
        diffs[i] = fabsf(slopes[i] - slopes[i - 1]) > DIFF;

    diffs[0] = !(diffs[1] && !diffs[2]);
    diffs[SAMPLES_NUM - 1] = diffs[SAMPLES_NUM - 2] && !diffs[SAMPLES_NUM - 3];
    for (int i = 1; i < SAMPLES_NUM - 1; i++)
        if (diffs[i] && diffs[i + 1])
            diffs[i] = 0;

    int basicPoints[SAMPLES_NUM / 2];
    int j = 0;
    for (int i = 0; i < SAMPLES_NUM; i++)
        if (diffs[i]) {
            basicPoints[j] = i;
            j++;
        }
    basicPoints[j] = SAMPLES_NUM;

    int basicPoints_size = j;
    int start, stop;
    float minX = 10000.f;
    int minJ = 0;
    for (j = 0; j < basicPoints_size; j++) {
        float xSum = 0.f;
        start = basicPoints[j];
        stop = basicPoints[j + 1];
        for (int i = start; i < stop; i++)
            xSum += points[i].x;
        float x = xSum / (float) (stop - start);
        if (x < minX) {
            minX = x;
            minJ = j;
        }
    }

    *point_x = (int) roundf(minX);
    start = basicPoints[minJ];
    stop = basicPoints[minJ + 1];
    float ySum = 0.f;
    for (int i = start; i < stop; i++)
        ySum += points[i].y;
    *point_y = (int) roundf(ySum / (float) (stop - start));

    stop--;
    if (angle != NULL) {
        float slopesSum = 0.f;
        for (int i = start; i < stop; i++)
            slopesSum += slopes[i];
        *angle = (int) roundf(atanf(slopesSum / (float) (stop - start)) / (float) M_PI * 180.f);
    }
}

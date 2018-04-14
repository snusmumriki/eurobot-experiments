//
// Created by MI on 04/08/2018.
//

#include <math.h>
#include <malloc.h>
#include <limits.h>
#include <stdio.h>
#include "scanner.h"

void get_target(Point *point, int *angle, const int samples[SAMPLES_NUM]) {
    Point row[SAMPLES_NUM];
    for (int i = 0; i < SAMPLES_NUM; i++) {
        float ang = (float) (STEPS - i) / (float) STEPS_MAX * (float) M_PI;
        row[i].x = cosf(ang) * (float) samples[i];
        row[i].y = sinf(ang) * (float) samples[i];
    }

    float slopes[SAMPLES_NUM - 1];
    for (int i = 0; i < SAMPLES_NUM - 1; i++)
        slopes[i] = (row[i + 1].y - row[i].y) / (row[i + 1].x - row[i].x);

    char diffs[SAMPLES_NUM];
    for (int i = 1; i < SAMPLES_NUM - 1; i++)
        diffs[i] = fabsf(slopes[i] - slopes[i - 1]) > DIFF;

    diffs[0] = !(diffs[1] && !diffs[2]);
    diffs[SAMPLES_NUM - 1] = diffs[SAMPLES_NUM - 2] && !diffs[SAMPLES_NUM - 3];
    for (int i = 1; i < SAMPLES_NUM - 1; i++)
        if (diffs[i] && diffs[i + 1])
            diffs[i] = 0;

    int points[SAMPLES_NUM / 2];
    int pointsNum = 0;
    for (int i = 0; i < SAMPLES_NUM; i++)
        if (diffs[i]) {
            points[pointsNum] = i;
            pointsNum++;
        }
    if (diffs[SAMPLES_NUM - 1] == 0)
        points[pointsNum++] = SAMPLES_NUM;

    int start, stop;
    float minX = 10000.f;
    int minJ = 0;
    for (int j = 0; j < pointsNum - 1; j++) {
        float xSum = 0.f;
        start = points[j];
        stop = points[j + 1];
        for (int i = start; i < stop; i++)
            xSum += row[i].x;
        float x = xSum / (float) (stop - start);
        if (x < minX) {
            minX = x;
            minJ = j;
        }
    }

    point->x = (int) roundf(minX);
    start = points[minJ];
    stop = points[minJ + 1];
    float ySum = 0.f;
    for (int i = start; i < stop; i++)
        ySum += row[i].y;
    point->y = (int) roundf(ySum / (float) (stop - start));

    stop--;
    if (angle != NULL) {
        float slopesSum = 0.f;
        for (int i = start; i < stop; i++)
            slopesSum += slopes[i];
        *angle = (int) roundf(atanf(slopesSum / (float) (stop - start)) / (float) M_PI * 180.f);
    }
}

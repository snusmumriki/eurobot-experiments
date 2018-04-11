#include <stdio.h>
#include "scanner.h"

int main() {
    int x, y;
    int angle;
    int samples[SAMPLES_NUM] = {57, 49, 45, 41, 40, 41, 45, 49, 57};
    target_set(&x, &y, &angle, samples);
    printf("%i %i %i", x, y, angle);
    return 0;
}
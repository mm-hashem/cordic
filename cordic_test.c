#include <stdint.h>
#include "cordic.h"
#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <time.h>

#define FRCT_BITS     28                       // Number of fraction digits
#define FIX2FLT_SCALE (1.0 / (1 << FRCT_BITS)) // Scale used in fixed-point to real conversion
#define FIX2FLT(x)    ((x) * FIX2FLT_SCALE)    // Convert Fixed-point values to real values

int32_t rint32(void) {
    uint32_t x = ((uint32_t)rand() << 16) ^ (uint32_t)rand();
    return (int32_t)x;
}

void test(int32_t angle) {
    int32_t arr[2];

    double cos_exp = cos(FIX2FLT(angle));
    double sin_exp = sin(FIX2FLT(angle));

    printf("Angle: %lf degrees\n", FIX2FLT(angle)*180.0/M_PI);
    cordic(angle, arr);
    printf("CORDIC:\t\t");
    printf("Cosine: %lf Sine: %lf\n", FIX2FLT(arr[0]), FIX2FLT(arr[1]));
    printf("Expected:\t");
    printf("Cosine: %lf Sine: %lf\n\n", cos_exp, sin_exp);
}

int main() {
    //int i;
    //int32_t test_angle = -_2PI;
    //for (i = 0; i < 17; i++) {
    //    test(test_angle);
    //    test_angle += _PI_4;
    //}
    //srand(time(NULL));
    //printf("\n##### RANDOM TESTS #####\n");
    //for (i = 0; i < 20; i++) test((int32_t)(((int32_t)rand() << 17) ^ rand()));
    //////////////////
    //int32_t angle = 0x0c90fdaa;
    //test(angle);
    //printf("ASM Cosine: %lf\n", FIX2FLT(0x0b5043af));
    //printf("ASM Sine:   %lf",   FIX2FLT(0x0b505ab6));
    printf("%x", -_PI_2);
    return 0;
}
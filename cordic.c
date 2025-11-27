#include <stdint.h>
#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <time.h>
#include <stdlib.h>

// Fixed-point format: Q3.28, 1 sign bit, 3 integer bits, 28 fraction bits
// Hex values are generated using MATLAB with Fixed-Point Designer Toolbox
#define _PI           0x3243f6a8
#define _PI_2         (_PI >> 1)
#define _PI_4         (_PI >> 2)
#define _2PI          (_PI << 1)
#define _3PI_2        (3*_PI_2)
#define N             16                       // Number of LUT values
#define K             0x09b74eda               // The cosines multiplication constant
#define FRCT_BITS     28                       // Number of fraction digits
#define FIX2FLT_SCALE (1.0 / (1 << FRCT_BITS))
#define FIX2FLT(x)    ((x) * FIX2FLT_SCALE)

static const int32_t atan_lut[] = {0x0c90fdaa, 0x076b19c1, 0x03eb6ebf, 0x01fd5ba9, 0x00ffaadd, 0x007ff556, 0x003ffeaa, 0x001fffd5, 0x000ffffa, 0x0007ffff, 0x0003ffff, 0x0001ffff, 0x0000ffff, 0x00007fff, 0x00003fff, 0x00001fff};

/*
    Input angle in Q3.28 format,
    angle range: [-2*PI, 2*PI]
    Output arr[0] = cos(angle) in Q3.28 format
           arr[1] = sin(angle) in Q3.28 format
*/

void cordic(int32_t angle, int32_t* arr) {
    uint32_t i;
    int32_t dir = 1;
    int32_t sign_cos = 1;
    int32_t x_cos_old = K;
    int32_t x_cos = K;
    int32_t y_sin = 0;

    // Normalizing angle to range [-PI/2, 2PI]
    if (angle < -_PI_2) angle += _2PI;

    // Normalizing angle to CORDIC range [-PI/2, PI/2]
    if (angle > _3PI_2) angle -= _2PI;
    else if (angle > _PI_2) {
        angle = _PI - angle;
        sign_cos = -1;
    }
    
    for (i = 0; i < N; i++) {
        /*
        dir = angle < 0 ? -1 : 0;
        x_cos  = x_cos - (((y_sin     >> i) ^ dir) - dir);
        y_sin  = y_sin + (((x_cos_old >> i) ^ dir) - dir);
        angle -=          ((atan_lut[i]     ^ dir) - dir);
        */
        dir = angle < 0 ? -1 : 1;

        x_cos  = x_cos - dir * (y_sin     >> i);
        y_sin  = y_sin + dir * (x_cos_old >> i);
        angle -=         dir * atan_lut[i];

        x_cos_old = x_cos;
    }

    arr[0] = sign_cos*x_cos;
    arr[1] = y_sin;
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
    int i;
    int32_t test_angle = -_2PI;
    for (i = 0; i < 17; i++) {
        test(test_angle);
        test_angle += _PI_4;
    }
    srand(time(NULL));
    printf("\n##### RANDOM TESTS #####\n");
    for (i = 0; i < 20; i++) test((int32_t)(((int32_t)rand() << 17) ^ rand()));
    return 0;
}
// math_accelerate.h
// Accelerated math functions using Apple Accelerate Framework

#ifndef MATH_ACCELERATE_H
#define MATH_ACCELERATE_H

#include <Accelerate/Accelerate.h>
#include <stddef.h>

// Vector operations using vDSP
static inline void vector_multiply(const float *a, const float *b, float *result, size_t count) {
    vDSP_vmul(a, 1, b, 1, result, 1, count);
}

static inline void vector_add(const float *a, const float *b, float *result, size_t count) {
    vDSP_vadd(a, 1, b, 1, result, 1, count);
}

static inline void vector_subtract(const float *a, const float *b, float *result, size_t count) {
    vDSP_vsub(b, 1, a, 1, result, 1, count);  // Note: vDSP_vsub computes a - b
}

static inline void vector_square(const float *input, float *result, size_t count) {
    vDSP_vsq(input, 1, result, 1, count);
}

static inline void vector_sqrt(const float *input, float *result, size_t count) {
    vvsqrtf(result, input, (int *)&count);
}

// Double precision versions
static inline void vector_multiply_d(const double *a, const double *b, double *result, size_t count) {
    vDSP_vmulD(a, 1, b, 1, result, 1, count);
}

static inline void vector_add_d(const double *a, const double *b, double *result, size_t count) {
    vDSP_vaddD(a, 1, b, 1, result, 1, count);
}

static inline void vector_square_d(const double *input, double *result, size_t count) {
    vDSP_vsqD(input, 1, result, 1, count);
}

static inline void vector_sqrt_d(const double *input, double *result, size_t count) {
    vvsqrt(result, input, (int *)&count);
}

// Fast trigonometric functions
static inline void vector_sin(const float *input, float *result, size_t count) {
    vvsinf(result, input, (int *)&count);
}

static inline void vector_cos(const float *input, float *result, size_t count) {
    vvcosf(result, input, (int *)&count);
}

static inline void vector_sincos(const float *input, float *sin_result, float *cos_result, size_t count) {
    vvsincosf(sin_result, cos_result, input, (int *)&count);
}

// Double precision trig
static inline void vector_sin_d(const double *input, double *result, size_t count) {
    vvsin(result, input, (int *)&count);
}

static inline void vector_cos_d(const double *input, double *result, size_t count) {
    vvcos(result, input, (int *)&count);
}

// Distance calculation for arrays of points
static inline void calculate_distances_2d(const float *x1, const float *y1,
                                         const float *x2, const float *y2,
                                         float *distances, size_t count) {
    float *dx = (float *)malloc(count * sizeof(float));
    float *dy = (float *)malloc(count * sizeof(float));
    float *dx_sq = (float *)malloc(count * sizeof(float));
    float *dy_sq = (float *)malloc(count * sizeof(float));
    
    // Calculate differences
    vDSP_vsub(x1, 1, x2, 1, dx, 1, count);
    vDSP_vsub(y1, 1, y2, 1, dy, 1, count);
    
    // Square the differences
    vDSP_vsq(dx, 1, dx_sq, 1, count);
    vDSP_vsq(dy, 1, dy_sq, 1, count);
    
    // Sum squares
    vDSP_vadd(dx_sq, 1, dy_sq, 1, distances, 1, count);
    
    // Square root
    vvsqrtf(distances, distances, (int *)&count);
    
    free(dx);
    free(dy);
    free(dx_sq);
    free(dy_sq);
}

// Matrix operations using BLAS
static inline void matrix_multiply(const float *a, const float *b, float *c,
                                  int m, int n, int k) {
    // C = A * B
    // A is m x k, B is k x n, C is m x n
    cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
                m, n, k, 1.0f, a, k, b, n, 0.0f, c, n);
}

// Enable/disable Accelerate framework
#define USE_ACCELERATE 1

#endif // MATH_ACCELERATE_H
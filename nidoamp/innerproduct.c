/** calculating the inner product
 *
 * we do this a lot, lets make sure it's fast
 *
 */

#ifdef __SSE3__
//#include <xmmintrin.h>
#include <pmmintrin.h>
#endif                          //__SSE3__
#include <stdio.h>
#include <sys/time.h>
#include <math.h>

#include "cpuid.h"

#define SSE_MASK_RESULT_FIRST 0xF1

#define MEASURE_SIZE 1024
#define USEC_IN_SEC 0.000001

// add one __m128 (val) together
// _m_hadd_ps(_m_hadd_ps(val, val), irrelevant_val)

#ifdef __SSE3__
/** Calculates the next step in the output using convolution using
 * sse4.1
 *
 * @param input the input buffer (needs to be of at least length
 *              FOURIER_SIZE)
 * @param kernel the (time domain) convolution kernel
 *
 * @return the next output of the convolution
 */
float inner_product_sse3(float *input, float *kernel)
{
    float output = 0;
    __m128 inputvector;
    __m128 kernelvector;
    __m128 ssetemp = _mm_setzero_ps();
    __m128 sseunit = _mm_set_ps1(1.0f);
    int i;
    for (i = 0; i < FOURIER_SIZE - 3; i += 4) {
        inputvector = _mm_loadu_ps(input + i);
        kernelvector = _mm_loadu_ps(kernel + i);
        ssetemp = _mm_add_ss(ssetemp, _mm_mul_ps(inputvector, kernelvector)
            );
    }
    _mm_hadd_ps(ssetemp, sseunit);
    output = _mm_cvtss_f32(ssetemp);
    // do the parts not done in sse
    for (; i < FOURIER_SIZE; i++) {
        output += input[i] * kernel[i];
    }
    return output;
}
#endif //__SSE3__

/** Calculates the next step in the output using convolution
 *
 * @param input the input buffer (needs to be of at least length
 *              FOURIER_SIZE)
 * @param kernel the (time domain) convolution kernel
 *
 * @return the next output of the convolution
 */
float inner_product(float *input, float *kernel)
{

    int i;
    float output = 0;
    for (i = 0; i < FOURIER_SIZE; i++) {
        output += input[i] * kernel[i];
    }
    return output;
}

float measure_function(float (*function_name) (float *, float *))
{
    struct timeval start;
    struct timeval end;
    float result;
    float input[FOURIER_SIZE + MEASURE_SIZE];
    float kernel[FOURIER_SIZE];
    int i;
    float check = 0.0;

    for (i = 0; i < FOURIER_SIZE; i++) {
        input[i] = i % 2 - 0.5;
        kernel[i] = i % 2 - 0.5;
    }
    for (i = FOURIER_SIZE; i < FOURIER_SIZE + MEASURE_SIZE; i++) {
        input[i] = i % 2 - 0.5;
    }

    gettimeofday(&start, NULL);
    for (i = 0; i < MEASURE_SIZE; i++) {
        check += function_name(input + i, kernel);
    }
    gettimeofday(&end, NULL);
    result =
        end.tv_sec - start.tv_sec + (end.tv_usec -
                                     start.tv_usec) * USEC_IN_SEC;
#ifdef DEBUG
    printf("measured %f\n", result);
#endif
    return result;
}

/** return the fastest inner product function for this system
 */
void set_inner_product(float (**function_name) (float *, float *))
{
    float fastest = INFINITY;
    float measure;

    measure = measure_function(inner_product);
    if (measure < fastest) {
        *function_name = inner_product;
        fastest = measure;
    }
#ifdef __SSE3__
	if (has_sse3()){
	    measure = measure_function(inner_product_sse3);
	    if (measure < fastest) {
	        *function_name = inner_product_sse3;
	        fastest = measure;
	    } else {
#ifdef DEBUG
	        printf("measure is broken, sse3 is faster on dev system\n");
#endif
	        *function_name = inner_product_sse3;
	    }
	}
#endif                          //__SSE3__
}

// vim: ts=4 sw=4 textwidth=72

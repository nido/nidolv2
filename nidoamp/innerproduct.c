/** calculating the inner product
 *
 * we do this a lot, lets make sure it's fast
 *
 */

#ifndef __SSE4_1__
#error Fuck you, need sse41
#endif
#ifdef __SSE4_1__
#include <xmmintrin.h>
#include <smmintrin.h>
#endif                          //__SSE4_1__
#include <stdio.h>

#include "cpuid.h"
#define SSE_MASK_RESULT_FIRST 0xF1


#ifdef __SSE4_1__
/** Calculates the next step in the output using convolution using
 * sse4.1
 *
 * @param input the input buffer (needs to be of at least length
 *              FOURIER_SIZE)
 * @param kernel the (time domain) convolution kernel
 *
 * @return the next output of the convolution
 */
float inner_product_sse41(float *input, float *kernel)
{
    float output = 0;
    __m128 inputvector;
    __m128 kernelvector;
    __m128 ssetemp;
    int i;
    for (i = 0; i < FOURIER_SIZE / 4; i++) {
        inputvector = _mm_load_ps(input + i * 4);
        kernelvector = _mm_load_ps(kernel + i * 4);
        ssetemp =
            _mm_dp_ps(inputvector, kernelvector, SSE_MASK_RESULT_FIRST);
        output += _mm_cvtss_f32(ssetemp);
    }
    // do the parts not done in sse
    for (i *= 4; i < FOURIER_SIZE; i++) {
        output += input[i] * kernel[i];
    }
    return output;
}
#endif                          //__SSE4_1__

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

/** return the fastest inner product function for this system
 */
void set_inner_product(float (**function_name) (float *, float *))
{
    // TODO: measure performance and choose the best
    *function_name = inner_product;
#ifdef __SSE4_1__
    if (has_sse41()) {
        *function_name = inner_product_sse41;
    }
#endif                          //__SSE4_1__
}

// vim: ts=4 sw=4 textwidth=72

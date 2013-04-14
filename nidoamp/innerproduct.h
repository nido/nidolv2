/** calculating the inner product
 *
 * we do this a lot, lets make sure it's fast
 *
 */

#ifdef __SSE4_1__
float inner_product_sse41(float *input, float* kernel);
#endif //__SSE4_1__
float inner_product(float *input, float *kernel);
void set_inner_product(float (**function_name)(float*, float*));

// vim: ts=4 sw=4 textwidth=72

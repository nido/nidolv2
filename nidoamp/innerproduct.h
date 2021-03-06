/** calculating the inner product
 *
 * we do this a lot, lets make sure it's fast
 *
 */

#ifdef __SSE3__
float inner_product_sse3(float *input, float *kernel);
#endif                          //__SSE3__
float inner_product(float *input, float *kernel);
void set_inner_product(float (**function_name) (float *, float *));

// vim: ts=4 sw=4 textwidth=72

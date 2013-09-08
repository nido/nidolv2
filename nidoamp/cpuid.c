/** CPU Identification code
 * used for the recognition of SSE and alike for using optimised inner products.
 * initial version from http://blog.paphus.com/blog/2012/07/24/runtime-cpu-feature-checking/
 */
#include <stdbool.h>
#include <stdio.h>

#define FLAG_SSE   2^25
#define FLAG_SSE3  2^0
#define FLAG_SSE42 2^20

/** Canonical example cpuid function from wikipedia
 *
 * @param info feature of cpuid you wish to adress
 * @param eax output value of eax register
 * @param ebx output value of ebx register
 * @param ecx output value of ecx register
 * @param edx output value of edx register
 */
void cpuid(unsigned info, unsigned *eax, unsigned *ebx, unsigned *ecx,
           unsigned *edx)
{
    *eax = info;
    __asm volatile
     ("mov %%ebx, %%edi;"       /* 32bit PIC: don't clobber ebx */
      "cpuid;"
      "mov %%ebx, %%esi;"
      "mov %%edi, %%ebx;":"+a" (*eax), "=S"(*ebx), "=c"(*ecx), "=d"(*edx)
      ::"%edi");
}

/** Checks whether sse is supported.
 *
 * @return whether the runtime environment supports sse
 */
bool has_sse(void)
{
	unsigned int scratch, edx;
	cpuid(1, &scratch, &scratch, &scratch, &edx);
    return (FLAG_SSE & edx);
}

/** Checks whether sse3 is supported.
 *
 * @return whether the runtime environment supports sse3
 */
bool has_sse3(void)
{
	unsigned int scratch, ecx;
	cpuid(1, &scratch, &scratch, &ecx, &scratch);
    return (FLAG_SSE3 & ecx);
}

// vim: ts=4 sw=4 textwidth=72

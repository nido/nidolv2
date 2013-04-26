/** CPU Identification code
 * used for the recognition of SSE and alike for using optimised inner products.
 * initial version from http://blog.paphus.com/blog/2012/07/24/runtime-cpu-feature-checking/
 */
#include <stdbool.h>
#include <stdio.h>

#define FLAG_SSE42 0x0080000

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
      ::"edi");
}

/** Returns whether the cpu runnigng this supports a feature
 *
 * @param flag the flag for the feature you wish to query
 *
 * @return whether the feature is supported in the runtime environment
 */
bool has_feature(int flag)
{
    bool result;
    unsigned int eax, ebx, ecx, edx;
    cpuid(1, &eax, &ebx, &ecx, &edx);
    result = ((ecx & flag) != 0);
#ifdef VERBOSE_DEBUG
    if (result) {
        printf("%x - succes\n", ecx);
    } else {
        printf("%x - failure\n", ecx);
    }
#endif
    return result;
}

/** Checks whether sse3 is supported.
 *
 * @return whether the runtime environment supports sse3
 */
bool has_sse3(void)
{
    return (has_feature(FLAG_SSE42));
}

// vim: ts=4 sw=4 textwidth=72

/** CPU Identification code
 * used for the recognition of SSE and alike for using optimised inner products.
 * initial version from http://blog.paphus.com/blog/2012/07/24/runtime-cpu-feature-checking/
 */
#include <stdbool.h>
#include <stdio.h>

#define FLAG_SSE42 0x0080000

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

bool has_sse41(void)
{
    bool result;
    unsigned int eax, ebx, ecx, edx;
    cpuid(1, &eax, &ebx, &ecx, &edx);
    result = ((ecx & FLAG_SSE42) != 0);
#ifdef VERBOSE_DEBUG
    if (result) {
        printf("%x - succes\n", ecx);
    } else {
        printf("%x - failure\n", ecx);
    }
#endif
    return result;
}

// vim: ts=4 sw=4 textwidth=72

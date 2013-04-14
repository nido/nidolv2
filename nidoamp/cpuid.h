#include <stdbool.h>

void cpuid(unsigned info, unsigned *eax, unsigned *ebx, unsigned *ecx,
           unsigned *edx);
bool has_sse41(void);

// vim: ts=4 sw=4 textwidth=72

#include <lv2.h>

#ifndef S_SPLINT_S
#include <complex.h>
#endif
#include <fftw3.h>
#include "ringbuffer.h"


const LV2_Descriptor descriptor;

LV2_Handle instantiate(const LV2_Descriptor * descriptor, double rate,
                       const char *bundle_path,
                       /*@null@ */ const LV2_Feature * const *features);
void connect_port(LV2_Handle instance, uint32_t port, void *data);
void activate(LV2_Handle instance);
void run(LV2_Handle instance, uint32_t n_samples);
void deactivate(LV2_Handle instance);
void cleanup( /*@only@ */ LV2_Handle instance);
LV2_SYMBOL_EXPORT const LV2_Descriptor *lv2_descriptor(uint32_t index);
// vim: ts=4 sw=4 textwidth=72

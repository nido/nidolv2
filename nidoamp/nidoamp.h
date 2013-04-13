#include <lv2.h>

#ifndef S_SPLINT_S
#include <complex.h>
#endif
#include <fftw3.h>

/** Datastructure holding the buffers and links to input and output ports.
 */
typedef struct {
    /** the high pass control fader */
    float *hipass;
    /** the low pass control fader */
    float *lopass;
    /** input buffer */
    float *input;
    /** output buffer */
    float *output;
    /** output port giving the latency */
    float *latency;
    /** the gate control fader */
    float *gate;
    /** Buffer for the complex component to the fourier transform */
    fftwf_complex *complex_buffer;
    /** Buffer from and to which fourier transforms are done */
    float *real_buffer;
    /** input buffer as sent by the host */
    float *in_buffer;
    /** output buffer as sent by the host */
    float *out_buffer;
    /** The location in the internal buffer */
    int buffer_index;
    /** The plan to do forward DFT's */
    fftwf_plan forward;
    /** the plan to do backward DFT's */
    fftwf_plan backward;
} Amp;

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
//

/*
  LV2 Amp Example Plugin
  Copyright 2006-2011 Steve Harris <steve@plugin.org.uk>,
                      David Robillard <d@drobilla.net>

  Permission to use, copy, modify, and/or distribute this software for any
  purpose with or without fee is hereby granted, provided that the above
  copyright notice and this permission notice appear in all copies.

  THIS SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
  WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
  MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
  ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
  ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
  OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#include <math.h>
#include <stdlib.h>
#include <string.h>

#include <stdbool.h>
#include <complex.h>
#include <fftw3.h>

//#include "lv2/lv2plug.in/ns/lv2core/lv2.h"
#include <lv2.h>

#include "nidoamp.peg"


#ifndef FOURIER_SIZE
#define FOURIER_SIZE 512
#endif

#ifndef COMPLEX_SIZE
#define COMPLEX_SIZE (FOURIER_SIZE / 2 + 1)
#endif

typedef struct {
	float* hipass;
	float* input;
	float* output;
	fftw_complex* complex_buffer;
	double* real_buffer;
	float* inbuffer;
	float* outbuffer;
	int buffer_index;
	fftw_plan forward;
	fftw_plan backward;
} Amp;

LV2_Handle
instantiate(const LV2_Descriptor * descriptor,
	    double rate,
	    const char *bundle_path, const LV2_Feature * const *features)
{
	Amp* amp = malloc(sizeof(Amp));
	amp->complex_buffer = fftw_malloc(sizeof(fftw_complex) * COMPLEX_SIZE);
	amp->real_buffer = fftw_malloc(sizeof(double) * (FOURIER_SIZE));
	// todo: malloc when latency is implemented.
	amp->inbuffer = calloc(sizeof(float), FOURIER_SIZE);
	amp->outbuffer = calloc(sizeof(float), FOURIER_SIZE);
	amp->buffer_index = 0;
	amp->forward = fftw_plan_dft_r2c_1d(FOURIER_SIZE, amp->real_buffer, amp->complex_buffer, FFTW_ESTIMATE);
	amp->backward =  fftw_plan_dft_c2r_1d(FOURIER_SIZE, amp->complex_buffer, amp->real_buffer, FFTW_ESTIMATE);
	// making this zero (float zero's are zero too)
	amp->real_buffer = memset(amp->real_buffer, 0, sizeof(double) * FOURIER_SIZE);

    return (LV2_Handle) amp;
}

static void connect_port(LV2_Handle instance, uint32_t port, void *data)
{
	Amp* amp = (Amp*)instance;

	switch ((enum nidoamp_port_enum)port) {
	case nidoamp_hipass:
		amp->hipass = (float*)data;
		break;
	case nidoamp_in:
		amp->input = (float*)data;
		break;
	case nidoamp_out:
		amp->output = (float*)data;
		break;
	}
}

void activate(LV2_Handle instance)
{
	Amp* amp = (Amp*)instance;
}

static void fftprocess(Amp * amp)
{
	int iterator;
	double* real_buffer = amp->real_buffer;
	fftw_complex* complex_buffer = amp->complex_buffer;
	int hipass = (int) *(amp->hipass);

	for (iterator = 0; iterator < FOURIER_SIZE; iterator++){
		real_buffer[iterator] = amp->inbuffer[iterator];
	}
	fftw_execute(amp->forward);
	for (iterator = 0; iterator < COMPLEX_SIZE; iterator++){
		if (iterator < hipass) {
			complex_buffer[iterator] *= 0.0;
		}
	}
	fftw_execute(amp->backward);
	for (iterator = 0; iterator < FOURIER_SIZE; iterator++){
		amp->outbuffer[iterator] = (float)(real_buffer[iterator] / FOURIER_SIZE);
	}
}

void
run(LV2_Handle instance, uint32_t n_samples)
{
        Amp* amp = (Amp*)instance;
        const float* const input  = amp->input;
        float* const       output = amp->output;
        float*      inbuffer = amp->inbuffer;
        float*      outbuffer = amp->outbuffer;
        uint32_t readindex = 0;
        uint32_t readcount;

        do {
                uint32_t iterator;
                uint32_t bufferlength = (uint32_t)(FOURIER_SIZE - amp->buffer_index);
                readcount = n_samples;
                if (amp->buffer_index + readcount > bufferlength) {
                        readcount = bufferlength;
                }

                for(iterator=0; iterator < readcount; iterator++){
                        inbuffer[amp->buffer_index + iterator] = input[readindex + iterator];
			output[readindex + iterator] = outbuffer[amp->buffer_index + iterator];
                }

                if (amp->buffer_index + readcount == FOURIER_SIZE){
                        fftprocess(amp);
                }
	readindex += readcount;
	amp->buffer_index = (amp->buffer_index + readcount) % FOURIER_SIZE;
    } while (readindex < n_samples);
}

static void deactivate(LV2_Handle instance)
{
}

static void cleanup(LV2_Handle instance)
{
    free(instance);
}

const void *extension_data(const char *uri)
{
    return NULL;
}

static const LV2_Descriptor descriptor = {
    nidoamp_uri,
    instantiate,
    connect_port,
    activate,
    run,
    deactivate,
    cleanup,
    extension_data
};

LV2_SYMBOL_EXPORT const LV2_Descriptor *lv2_descriptor(uint32_t index)
{
    switch (index) {
    case 0:
	return &descriptor;
    default:
	return NULL;
    }
}

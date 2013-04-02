/*
*/

#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <strings.h>
#include <stdbool.h>
#include <lv2.h>

#include "nidoamp.h"
#include "nidoamp.peg"

#ifndef FOURIER_SIZE
#define FOURIER_SIZE 512
#endif

#ifndef COMPLEX_SIZE
#define COMPLEX_SIZE (FOURIER_SIZE / 2 + 1)
#endif

LV2_Handle instantiate( /*@unused@ */ const LV2_Descriptor * descriptor,
		       /*@unused@ */ double rate,
		       /*@unused@ */ const char *bundle_path,
		       /*@unused@ */ const LV2_Feature * const *features)
{
    Amp *amp = malloc(sizeof(Amp));
    assert(amp != NULL);
    amp->complex_buffer = fftwf_malloc(sizeof(fftwf_complex) * COMPLEX_SIZE);
    assert(amp->complex_buffer != NULL);
    amp->real_buffer = fftwf_malloc(sizeof(float) * (FOURIER_SIZE));
    assert(amp->real_buffer != NULL);
    // todo: malloc when latency is implemented.
    amp->in_buffer = fftwf_malloc(sizeof(float) * FOURIER_SIZE);
    assert(amp->in_buffer != NULL);
    amp->out_buffer = fftwf_malloc(sizeof(float) * FOURIER_SIZE);
    assert(amp->out_buffer != NULL);
    amp->buffer_index = 0;
    amp->forward =
	fftwf_plan_dft_r2c_1d(FOURIER_SIZE, amp->real_buffer,
			     amp->complex_buffer, FFTW_ESTIMATE);
    assert(amp->forward != NULL);
    amp->backward =
	fftwf_plan_dft_c2r_1d(FOURIER_SIZE, amp->complex_buffer,
			     amp->real_buffer, FFTW_ESTIMATE);
    assert(amp->backward != NULL);

    return (LV2_Handle) amp;
}

void connect_port(LV2_Handle instance, uint32_t port, void *data)
{
    Amp *amp = (Amp *) instance;

    switch ((enum nidoamp_port_enum) port) {
    case nidoamp_hipass:
	amp->hipass = (float *) data;
	break;
    case nidoamp_lopass:
	amp->lopass = (float *) data;
	break;
    case nidoamp_in:
	amp->input = (float *) data;
	break;
    case nidoamp_out:
	amp->output = (float *) data;
	break;
    case nidoamp_n_ports:
	printf("%s severely broken\n", nidoamp_uri);
	exit(EXIT_FAILURE);
	break;
    }
}

void activate(LV2_Handle instance)
{
    Amp *amp = (Amp *) instance;
    // making this zero (float zero's are zero too)
    bzero(amp->real_buffer, sizeof(float) * FOURIER_SIZE);
    bzero(amp->real_buffer, sizeof(float) * FOURIER_SIZE);
}

static void fftprocess(Amp * amp)
{
    int iterator;
    float*real_buffer = amp->real_buffer;
    fftwf_complex *complex_buffer = amp->complex_buffer;
    int hipass = (int) *(amp->hipass);
    int lopass = (int) *(amp->hipass);
    float in;
    float out;
    float start;
    float stop;
    float slope;
    float centre;

    for (iterator = 0; iterator < FOURIER_SIZE; iterator++) {
	real_buffer[iterator] = amp->in_buffer[iterator];
    }
    in = amp->in_buffer[0];
    out = amp->in_buffer[FOURIER_SIZE - 1];
    start = amp->in_buffer[0];
    stop = amp->in_buffer[FOURIER_SIZE - 1];
//      printf("in: %f", amp->in_buffer[FOURIER_SIZE - 1]);
    fftwf_execute(amp->forward);

    for (iterator = 0; iterator < COMPLEX_SIZE; iterator++) {
	if ((iterator < hipass) || (iterator > lopass)) {
	    complex_buffer[iterator] *= 0.0;
	}
    }

    fftwf_execute(amp->backward);

    for (iterator = 0; iterator < FOURIER_SIZE; iterator++) {
	amp->out_buffer[iterator] =
	    (float) ((real_buffer[iterator]) / FOURIER_SIZE);
    }
    start -= amp->out_buffer[0];
    stop -= amp->out_buffer[FOURIER_SIZE - 1];
    slope = (start - stop) / 2;
    centre = start - slope;
    // naive smoothing
    for (iterator = 0; iterator < FOURIER_SIZE; iterator++) {
	amp->out_buffer[iterator] +=
	    centre + slope * cosf(iterator * (M_PI) / (FOURIER_SIZE - 1));
    }
    in -= amp->out_buffer[0];
    out -= amp->out_buffer[FOURIER_SIZE - 1];

//      printf(", out: %f\n", amp->out_buffer[FOURIER_SIZE - 1]);
//      printf("%f, %f, %f, %f\n", in, out, centre, slope);

}

void run(LV2_Handle instance, uint32_t n_samples)
{
    Amp *amp = (Amp *) instance;
    const float *const input = amp->input;
    float *const output = amp->output;
    uint32_t io_index = 0;
    float *in_buffer = amp->in_buffer;
    float *out_buffer = amp->out_buffer;
    uint32_t readcount;

    do {
	uint32_t iterator;
	uint32_t bufferlength =
	    (uint32_t) (FOURIER_SIZE - amp->buffer_index);
	readcount = n_samples;
	if (amp->buffer_index + readcount > bufferlength) {
	    readcount = bufferlength;
	}

	for (iterator = 0; iterator < readcount; iterator++) {
	    in_buffer[amp->buffer_index + iterator] =
		input[io_index + iterator];
	    output[io_index + iterator] =
		out_buffer[amp->buffer_index + iterator];
	}

	if (amp->buffer_index + readcount == FOURIER_SIZE) {
	    fftprocess(amp);
	}
	io_index += readcount;
	amp->buffer_index =
	    (int) (amp->buffer_index + readcount) % FOURIER_SIZE;
    } while (io_index < n_samples);
}

void deactivate( /*@unused@ */ LV2_Handle instance)
{
}

void cleanup(LV2_Handle instance)
{
    Amp *amp = (Amp *) instance;
    fftwf_free(amp->complex_buffer);
    fftwf_free(amp->real_buffer);
    fftwf_destroy_plan(amp->forward);
    fftwf_destroy_plan(amp->backward);
    fftwf_free(amp->in_buffer);
    fftwf_free(amp->out_buffer);
    free(instance);
    fftwf_cleanup();
}

/*@null@*/ static const void *extension_data( /*@unused@ */ const char
					     *uri)
{
    return NULL;
}

const LV2_Descriptor descriptor = {
    nidoamp_uri,
    instantiate,
    connect_port,
    activate,
    run,
    deactivate,
    cleanup,
    extension_data
};

/*@null@*/ LV2_SYMBOL_EXPORT const LV2_Descriptor *lv2_descriptor(uint32_t index)
{
    switch (index) {
    case 0:
	return &descriptor;
    default:
	return NULL;
    }
}

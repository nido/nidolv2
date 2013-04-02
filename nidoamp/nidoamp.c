/*
*/

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#ifndef S_SPLINT_S
#include <complex.h>
#endif

#include <fftw3.h>
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
	fftwf_complex* complex_buffer;
	float* real_buffer;
	float* inbuffer;
	float* outbuffer;
	int buffer_index;
	fftwf_plan forward;
	fftwf_plan backward;
} Amp;

static LV2_Handle
instantiate(const LV2_Descriptor * descriptor,
	    double rate,
	    const char *bundle_path, const LV2_Feature * const *features)
{
	Amp* amp = malloc(sizeof(Amp));
	amp->complex_buffer = fftwf_malloc(sizeof(fftwf_complex) * COMPLEX_SIZE);
	amp->real_buffer = fftwf_malloc(sizeof(float) * (FOURIER_SIZE));
	// todo: malloc when latency is implemented.
	amp->inbuffer = calloc(sizeof(float), FOURIER_SIZE);
	amp->outbuffer = calloc(sizeof(float), FOURIER_SIZE);
	amp->buffer_index = 0;
	amp->forward = fftwf_plan_dft_r2c_1d(FOURIER_SIZE, amp->real_buffer, amp->complex_buffer, FFTW_ESTIMATE);
	amp->backward =  fftwf_plan_dft_c2r_1d(FOURIER_SIZE, amp->complex_buffer, amp->real_buffer, FFTW_ESTIMATE);
	// making this zero (float zero's are zero too)
	amp->outbuffer = memset(amp->real_buffer, 0, sizeof(float) * FOURIER_SIZE);

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
	case nidoamp_n_ports:
		return;
	}
}

static void activate(LV2_Handle instance)
{
}

static void fftprocess(Amp * amp)
{
	int iterator;
	float* real_buffer = amp->real_buffer;
	fftwf_complex* complex_buffer = amp->complex_buffer;
	int hipass = (int) *(amp->hipass);
	float start;
	float stop;
	float slope;
	float centre;

	for (iterator = 0; iterator < FOURIER_SIZE; iterator++){
		real_buffer[iterator] = amp->inbuffer[iterator];
	}


	fftwf_execute(amp->forward);

	for (iterator = 0; iterator < COMPLEX_SIZE; iterator++){
		if (iterator < hipass) {
			complex_buffer[iterator] *= 0.0;
		}
	}

	fftwf_execute(amp->backward);


	for (iterator = 0; iterator < FOURIER_SIZE; iterator++){
		amp->outbuffer[iterator] = real_buffer[iterator] / FOURIER_SIZE;
	}
	// naive smoothing
	start = amp->inbuffer[0] - amp->outbuffer[0];
	stop = amp->inbuffer[FOURIER_SIZE - 1] - amp->outbuffer[FOURIER_SIZE - 1];
	slope = (stop - start) / 2;
	centre = start - slope;
	
	printf("%f, %f, %f, %f\n", start, stop, centre, slope);

//	for (iterator = 0; iterator < FOURIER_SIZE; iterator++){
//		amp->outbuffer[iterator] += centre + slope * cosf(iterator * (M_PI / 2) / FOURIER_SIZE);
//	}
}

static void
run(LV2_Handle instance, uint32_t n_samples)
{
        Amp* amp = (Amp*)instance;
        const float* const input  = amp->input;
        float* const       output = amp->output;
        float*      inbuffer = amp->inbuffer;
        float*      outbuffer = amp->outbuffer;
        uint32_t io_index = 0;
        uint32_t readcount;

        do {
                uint32_t iterator;
                uint32_t bufferlength = (uint32_t)(FOURIER_SIZE - amp->buffer_index);
                readcount = n_samples;
                if (amp->buffer_index + readcount > bufferlength) {
                        readcount = bufferlength;
                }

                for(iterator=0; iterator < readcount; iterator++){
                        inbuffer[amp->buffer_index + iterator] = input[io_index + iterator];
			output[io_index + iterator] = outbuffer[amp->buffer_index + iterator];
                }

                if (amp->buffer_index + readcount == FOURIER_SIZE){
                        fftprocess(amp);
                }
	io_index += readcount;
	amp->buffer_index = (amp->buffer_index + readcount) % FOURIER_SIZE;
    } while (io_index < n_samples);
}

static void deactivate(LV2_Handle instance)
{
}

static void cleanup(LV2_Handle instance)
{
    free(instance);
}

static const void *extension_data(const char *uri)
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

/*@null@*/ LV2_SYMBOL_EXPORT const LV2_Descriptor *lv2_descriptor(uint32_t index)
{
    switch (index) {
    case 0:
	return &descriptor;
    default:
	return NULL;
    }
}

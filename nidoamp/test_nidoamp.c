#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <lv2.h>
#include <math.h>
#include "nidoamp.h"

#define MAX_ERROR 0.000001


/** Datastructure holding the buffers and links to input and output ports.
 * TODO: THIS IS COPYPASTA, NEEDS TO BE MADE AN LV2HANDLE
 */
typedef struct {
    /** the high pass control fader */
    float *hipass;
    /** the low pass control fader */
    float *lopass;
    /** output port giving the latency */
    float *latency;
    /** the gate control fader */
    float *gate;
    /** input buffer as sent by the host */
    float *input;
    /** output buffer as sent by the host */
    float *output;

    /** input buffer, local storage */
    struct ringbuffer *in_buffer;
    /** output bufferm local storage */
    struct ringbuffer *out_buffer;
    /** Buffer for the complex component to the fourier transform */
    fftwf_complex *complex_buffer;
    /** Buffer for the complex kernel */
    fftwf_complex *kernel_buffer;
    /** Buffer for the convolution kernel */
    float *convolution_buffer;
    /** Buffer for the previous convolution kernel */
    float *previous_buffer;
    /** Buffer from and to which fourier transforms are done */
    float *fourier_buffer;
    /** buffer for convolution */
    float (*convolve_func) (float *, float *);
    /** The location in the internal buffer */
    int buffer_index;
    /** The plan to do forward DFT's */
    fftwf_plan forward;
    /** the plan to do backward DFT's */
    fftwf_plan backward;
} Amp;

int main(void);
int smoketest(void);
int latencytest(void);
Amp *get_amp(void);

#define DATASIZE 104857

/** Test program to run like a host to see if it actually operates and have full debug capabilities; basically just do a smoke test */
int main(void)
{
    if (smoketest() == EXIT_FAILURE) {
        return (EXIT_FAILURE);
    }
    if (latencytest() == EXIT_FAILURE) {
        return (EXIT_FAILURE);
    }
    if (avgkerneltest() == EXIT_FAILURE) {
        return (EXIT_FAILURE);
    }
    return EXIT_SUCCESS;
}

Amp *get_amp(void)
{
    Amp *amp;
    const LV2_Descriptor *descriptor = lv2_descriptor(0);
    amp = (Amp *) instantiate(descriptor, 96000.0, "", NULL);
    return amp;
}

int latencytest(void)
{
    Amp *amp = get_amp();
    float zero = 0.0;
    float latency = 0.0;
    float *inbuffer;
    float *outbuffer;
    int i;

    inbuffer = calloc(DATASIZE, sizeof(float));
    outbuffer = calloc(DATASIZE, sizeof(float));
    inbuffer[0] = 1.0;

    connect_port(amp, 4, &latency);
    connect_port(amp, 0, &zero);
    connect_port(amp, 3, &zero);
    connect_port(amp, 5, &zero);
    connect_port(amp, 1, inbuffer);
    connect_port(amp, 2, outbuffer);

    activate(amp);
    run(amp, DATASIZE);

    for (i = 0; i < DATASIZE; i++) {
        if ((outbuffer[i] > 0.9) && (i != (int) latency)) {
            printf("Measured latency %i, expected %f\n", i, latency);
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}

/** */
int avgkerneltest(void)
{
	float left[FOURIER_SIZE];
	float right[FOURIER_SIZE];
	float out[FOURIER_SIZE];
	float outsse[FOURIER_SIZE];
	int i;
	for (i=0; i<FOURIER_SIZE; i++){
		left[i] = (float) rand() / RAND_MAX;
		right[i] = (float) rand() / RAND_MAX;
	}
	Amp *amp = get_amp();

	amp->previous_buffer = left;
	amp->fourier_buffer = right;

	average_kernels_sse(outsse, amp);
	average_kernels(out, amp);

	for (i=0; i<FOURIER_SIZE; i++){
		if(fabs(out[i] - outsse[i]) > MAX_ERROR) {
			printf("%g\n", fabs(out[i] - outsse[i]));
			return EXIT_FAILURE;
		}
	}
}

/** Smoketest, basically connects the whole system, lets it run for a bit and
 * disassembles it again. At the end no memory should be allocated and
 * no function should fail.
 */
int smoketest(void)
{
    float *buffer;
    float hipass;
    LV2_Handle handle;
    int i;
    int j;
    float latency;

    if (lv2_descriptor(0) == NULL) {
        printf("%s\n", "lv2 descriptor 0 is wrong");
        return EXIT_FAILURE;
    }
    if (lv2_descriptor(1) != NULL) {
        printf("%s\n", "lv2 descriptor 1 is wrong");
        return EXIT_FAILURE;
    }

    buffer = malloc(sizeof(float) * DATASIZE);
    assert(buffer != NULL);

    j = 0;
    hipass = 0.2;
    latency = 0.0;

    handle = instantiate(&descriptor, 96000.0, "Irrelevant", NULL);
    if (handle == NULL) {
        printf("%s\n", "Cannot instantiate");
    }
    for (i = 0; i < DATASIZE; i++) {
        buffer[i] = (float) rand() / RAND_MAX - 1;
    }
    connect_port(handle, 4, &latency);
    connect_port(handle, 0, &hipass);
    connect_port(handle, 3, &hipass);
    connect_port(handle, 5, &hipass);
    connect_port(handle, 1, buffer);
    connect_port(handle, 2, buffer);

    activate(handle);
    for (i = 0; i < DATASIZE; i += j) {
        j = rand() % DATASIZE;
        if (i + j > DATASIZE) {
            j = DATASIZE - i;
        }
        run(handle, j);
    }

    deactivate(handle);
    cleanup(handle);
    free(buffer);
    fftwf_cleanup();
    return (EXIT_SUCCESS);
}

/** This file implements the main parts of the nidolv2 fourier based
 * spectral filter, as specified by LV2 on http://lv2plug.in/
 */

#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <strings.h>
#include <stdbool.h>
#include <lv2.h>

#include "nidoamp.h"
#include "nidoamp.peg"

#include "innerproduct.h"

#ifndef FOURIER_SIZE
/** The number of samples to take a fourier transformation of. */
#define FOURIER_SIZE 512
#endif

#ifndef COMPLEX_SIZE
/** The size of the fourier transform output (and input for the inverse). */
#define COMPLEX_SIZE (FOURIER_SIZE / 2 + 1)
#endif

#ifdef __OPENMP__
#include <omp.h>
#endif

#define BUFFER_SIZE (FOURIER_SIZE * 10)

/** Instantiate the plugin.
 *
 * This function initialises the plugin. It includes allocating memory
 * and fftw plans and saving them to a datastructure for later
 * reference.
 *
 * @param descriptor argument of unknown significance
 * @param rate the amount of samples per second thius plusin operates at
 * @param bundle_path argument of unknownn significance
 * @param features argument of unknown significance
 *
 * @return an LV2_Handle representation of the datastructure
 */
LV2_Handle instantiate( /*@unused@ */ const LV2_Descriptor *
                       descriptor, /*@unused@ */ double rate,
                       /*@unused@ */ const char *bundle_path,
                       /*@unused@ */
                       const LV2_Feature * const *features)
{
    Amp *amp = malloc(sizeof(Amp));

    assert(amp != NULL);
    amp->complex_buffer =
        fftwf_malloc(sizeof(fftwf_complex) * COMPLEX_SIZE);
    assert(amp->complex_buffer != NULL);
    amp->kernel_buffer =
        fftwf_malloc(sizeof(fftwf_complex) * COMPLEX_SIZE);
    assert(amp->kernel_buffer != NULL);
    amp->fourier_buffer = fftwf_malloc(sizeof(float) * FOURIER_SIZE);
    assert(amp->fourier_buffer != NULL);
    amp->in_buffer = init_buffer(BUFFER_SIZE, FOURIER_SIZE);
    assert(amp->in_buffer != NULL);
    amp->previous_buffer = malloc(sizeof(float) * FOURIER_SIZE);
    assert(amp->previous_buffer != NULL);
    amp->out_buffer = init_buffer(BUFFER_SIZE, FOURIER_SIZE);
    assert(amp->out_buffer != NULL);
    amp->buffer_index = 0;
    set_inner_product(&(amp->convolve_func));
    amp->forward =
        fftwf_plan_dft_r2c_1d(FOURIER_SIZE, amp->fourier_buffer,
                              amp->complex_buffer, FFTW_ESTIMATE);
    assert(amp->forward != NULL);
    amp->backward =
        fftwf_plan_dft_c2r_1d(FOURIER_SIZE, amp->kernel_buffer,
                              amp->fourier_buffer, FFTW_ESTIMATE);
    assert(amp->backward != NULL);
#ifdef __OPENMP__
    omp_set_num_threads(omp_get_num_procs());
#endif
    return (LV2_Handle) amp;
}

/** Connects a port to the plugin for processing.
 * Plugins have multiple input- and outputports. In order for these to
 * be used correctly the connect_port function allows to connect these
 * ports to host-defined data.
 *
 * @param instance A Handler for the LV2 Plugin Instance.
 * @param port The port number to connect
 * @param data The location of/for the data for the port.port
 */
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
    case nidoamp_latency:
        amp->latency = (float *) data;
        break;
    case nidoamp_gate:
        amp->gate = (float *) data;
        break;
    case nidoamp_n_ports:
        printf("%s severely broken\n", nidoamp_uri);
        exit(EXIT_FAILURE);
        break;
    }

}

/** Activates the plugin.
 * Given the relative statelessness of this plugin, all this does is
 * toclean the buffers.
 *
 * @param instance A Handler for the LV2 Plugin Instance.
 */
void activate(LV2_Handle instance)
{

    Amp *amp = (Amp *) instance;
    // making this zero (float zero's are zero too)
    bzero(amp->fourier_buffer, sizeof(float) * FOURIER_SIZE);
    bzero(amp->fourier_buffer, sizeof(float) * FOURIER_SIZE);

}

/** Computes the frequency kernel based on the input buffer
 *
 * @param buffer the input buffer
 * @param buffer for kernel the kernel to compute
 */
void compute_kernel(Amp * amp)
{
    int i;
    fftwf_complex *buffer = amp->complex_buffer;
    fftwf_complex *kernel = amp->kernel_buffer;
    int hipass = (int) *(amp->hipass);
    int lopass = COMPLEX_SIZE - (int) *(amp->lopass);
    float normalisation_factor = 0.0;
    float gate = pow(*(amp->gate), 10) * FOURIER_SIZE;

    fftwf_execute(amp->forward);
    for (i = 0; i < COMPLEX_SIZE; i++) {

        float measure = fabs(crealf(buffer[i]));
        if ((i < hipass)
            || (i > lopass)
            || (measure < gate)
            ) {
            kernel[i] = 0.0;
        } else {
            kernel[i] = 1.0;
        }
        //  normalisation_factor += kernel[i];
    }

    fftwf_execute(amp->backward);

    // make sure the kernel window size is normalised
    normalisation_factor = 1.0 / FOURIER_SIZE;
    for (i = 0; i < FOURIER_SIZE; i++) {
        amp->fourier_buffer[i] *= normalisation_factor;
    }
    //printf("normalisation factor %f\n", normalisation_factor);
}

void average_kernels(float *kernel, Amp * amp)
{
    int i;
    for (i = 0; i < FOURIER_SIZE; i++) {
        kernel[i] =
            (amp->previous_buffer[i] * (i + 1) +
             amp->fourier_buffer[i] * (FOURIER_SIZE - i -
                                       1)) / (FOURIER_SIZE);
#ifdef DEBUGf
        if (!
            ((kernel[i] >= amp->previous_buffer[i]
              || kernel[i] >= amp->fourier_buffer[i])
             && (kernel[i] <= amp->previous_buffer[i]
                 || kernel[i] <= amp->fourier_buffer[i]))) {
            printf("%f, %f, %f\n", kernel[i], amp->previous_buffer[i],
                   amp->fourier_buffer[i]);
        }
#endif
    }
}

/** processes the actual fourier transformation
 * does a fourier transformation. This part of the program is rather
 * expensive to compute, so make sure you call this a minimum number of
 * times.
 *
 * @param amp A handler for the LV2 plugin instance.
 */
void fftprocess(Amp * amp)
{

    int i;
    float *fourier_buffer = amp->fourier_buffer;
    float output[FOURIER_SIZE];

    bcopy(amp->fourier_buffer, amp->previous_buffer,
          sizeof(float) * FOURIER_SIZE);
    peek_buffer(fourier_buffer, amp->in_buffer, FOURIER_SIZE);
    compute_kernel(amp);
#ifdef __OPENMP__
#pragma omp parallel for
#endif
    for (i = 0; i < FOURIER_SIZE; i++) {
        float inbuf[FOURIER_SIZE];
        float kernel[FOURIER_SIZE];
        //float* inbufp = (float*)inbuf; // pointer type

        prefetch_buffer(inbuf, amp->in_buffer, FOURIER_SIZE, i);

        average_kernels(kernel, amp);
        //bcopy(kernel, amp->fourier_buffer, sizeof(float) * FOURIER_SIZE);

        output[i] = (amp->convolve_func) (inbuf, kernel);
    }

    write_buffer(amp->out_buffer, output, FOURIER_SIZE);
    read_buffer(NULL, amp->in_buffer, FOURIER_SIZE);
}

/** Run the plugin to obtain n_samples of output.
 *
 * @param instance A Handler for the LV2 Plugin Instance.
 * @param n_samples number of samples to operate on.
 */
void run(LV2_Handle instance, uint32_t n_samples)
{
    Amp *amp = (Amp *) instance;
    const float *const input = amp->input;
    float *const output = amp->output;
    uint32_t io_index = 0;
    uint32_t readcount;

    if (amp->latency != NULL) {
        *(amp->latency) = (float) FOURIER_SIZE;
    }
    do {

        uint32_t bufferlength =
            (uint32_t) (FOURIER_SIZE - (amp->buffer_index % FOURIER_SIZE));
        assert(bufferlength <= FOURIER_SIZE);
        readcount = n_samples;

        // make sure to stop at readbuffer lengths
        if (io_index + readcount > n_samples) {
            readcount = n_samples - io_index;
        }
        // make sure to stop at FOURIER_SIZE lengths
        if ((amp->buffer_index % FOURIER_SIZE) + readcount > bufferlength) {
            readcount = bufferlength;
        }

        assert(readcount <= FOURIER_SIZE);
        assert(readcount + (amp->buffer_index % FOURIER_SIZE) <=
               FOURIER_SIZE);
        // read input and write output
        write_buffer(amp->in_buffer, input + io_index, readcount);
/*        for (i = 0; i < readcount; i++) {
            in_buffer[(amp->buffer_index + i) % BUFFER_SIZE] = input[io_index + i];
            output[io_index + i] = amp->out_buffer[amp->buffer_index + i];
        }*/

        read_buffer(output + io_index, amp->out_buffer, readcount);

        if ((amp->buffer_index + readcount) % FOURIER_SIZE == 0) {
            fftprocess(amp);
        }

        amp->buffer_index =
            (int) (amp->buffer_index + readcount) % BUFFER_SIZE;
        io_index += readcount;
    } while (io_index < n_samples);
}

/** Deactivate the plugin (technically, in this case, do nothing)
 *
 * @param instance A Handler for the LV2 Plugin Instance.
 */
void deactivate( /*@unused@ */ LV2_Handle instance)
{
}

void cleanup(LV2_Handle instance)
{
    Amp *amp = (Amp *) instance;
    fftwf_free(amp->complex_buffer);
    fftwf_free(amp->fourier_buffer);
    fftwf_free(amp->kernel_buffer);
    free_buffer(amp->in_buffer);
    free_buffer(amp->out_buffer);
    free(amp->previous_buffer);
    fftwf_destroy_plan(amp->forward);
    fftwf_destroy_plan(amp->backward);
    free(amp);

}

/** Function with an interesting name that is required but not used
 *
 * @param uri a parameter that probably points to something lv2 related
 */
/*@null@*/ static const void *extension_data( /*@unused@ */ const char
                                             *uri)
{
    return NULL;
}

/** A descriptor of this plugin with links to all its (externally
 * usable) functions. 
 */
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

/** Function outputting the plugin descriptor for a host to use
 *
 * @param index Something that should be 0 in this implementation (dont
 * know real functionality of this)
 */
/*@null@*/ LV2_SYMBOL_EXPORT const LV2_Descriptor *lv2_descriptor(uint32_t
                                                                  index)
{
    switch (index) {
    case 0:
        return &descriptor;
    default:
        return NULL;
    }
}

// vim: ts=4 sw=4 textwidth=72

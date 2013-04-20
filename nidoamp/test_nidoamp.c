#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <lv2.h>
#include "nidoamp.h"

#define DATASIZE 1048576

/** Test program to run like a host to see if it actually operates and have full debug capabilities */
int main(void)
{
    float *buffer;
    float hipass;
    LV2_Handle handle;
    int i;
    int j;
    float latency;

    buffer = malloc(sizeof(float) * DATASIZE);
    assert(buffer != NULL);
    j = 0;
    hipass = 2.0;

    handle = instantiate(&descriptor, 96000.0, "Irrelevant", NULL);
    connect_port(handle, 4, &latency);
    for (i = 0; i < DATASIZE; i++) {
        buffer[i] = (float) rand() / RAND_MAX - 1;
    }
    connect_port(handle, 0, &hipass);
    connect_port(handle, 3, &hipass);
    connect_port(handle, 5, &hipass);

    connect_port(handle, 1, buffer);
    connect_port(handle, 2, buffer);
    connect_port(handle, 4, buffer);
    activate(handle);
    for (i = 0; i < DATASIZE; i += j) {
        j = rand() % DATASIZE;
        if (i + j > DATASIZE) {
            j = DATASIZE - i;
        }
        run(handle, j);
    }

    assert(lv2_descriptor(0) != NULL);
    assert(lv2_descriptor(1) == NULL);
    deactivate(handle);
    cleanup(handle);
    free(buffer);
    fftwf_cleanup();
    return (0);
}

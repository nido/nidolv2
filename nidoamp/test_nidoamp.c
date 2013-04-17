#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <lv2.h>
#include "nidoamp.h"

#define DATASIZE 10485760

/** Test program to run like a host to see if it actually operates and have full debug capabilities */
int main(void)
{
    float *buffer;
    float hipass;
    float output;
    LV2_Handle handle;
    int i;
    int j;
    float latency;

    j = 0;
<<<<<<< HEAD
    hipass = 1.0;

    handle = instantiate(&descriptor, 96000.0, "Irrelevant", NULL);
    connect_port(handle, 4, &latency);
    buffer = malloc(sizeof(float) * (DATASIZE + (int)latency));
=======
    hipass = 0.0;
    buffer = malloc(sizeof(float) * DATASIZE);
    if (buffer == NULL) {
        printf("%s\n", "Not enough memory");
        exit(EXIT_FAILURE);
    }
>>>>>>> d2e290c6e0d5a2a715cf47bdd3ccff7bc13be353
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
    deactivate(handle);
    cleanup(handle);
    free(buffer);
    fftwf_cleanup();
    return (0);
}

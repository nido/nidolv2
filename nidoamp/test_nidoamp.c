#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <lv2.h>
#include "nidoamp.h"

#define DATASIZE 1048576

int main(void)
{
    float *buffer;
    float hipass;
    LV2_Handle handle;
    int i;
    int j;
    float latency;

    j = 0;
    hipass = 1.0;

    handle = instantiate(&descriptor, 96000.0, "Irrelevant", NULL);
    connect_port(handle, 4, &latency);
    buffer = malloc(sizeof(float) * (DATASIZE + (int)latency));
    for (i = 0; i < DATASIZE; i++) {
	buffer[i] = (float) rand() / RAND_MAX * 2 - 1;
    }
    connect_port(handle, 0, &hipass);
    connect_port(handle, 1, buffer);
    connect_port(handle, 2, buffer);

    if (buffer == NULL) {
	printf("%s\n", "Not enough memory");
	exit(EXIT_FAILURE);
    }

    run(handle, (int)latency);
    connect_port(handle, 2, buffer);
    activate(handle);
    for (i = 0; i < DATASIZE; i += j) {
	j = rand();
	if (i + j > DATASIZE) {
	    j = DATASIZE - i;
	}
	run(handle, j);
    }
    deactivate(handle);
    cleanup(handle);
    free(buffer);
    return (0);
}

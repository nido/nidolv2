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

    hipass = 1.0;
    buffer = malloc(sizeof(float) * DATASIZE);
    if (buffer == NULL) {
	printf("%s\n", "Not enough memory");
	exit(EXIT_FAILURE);
    }
    for (i = 0; i < DATASIZE; i++) {
	buffer[i] = (float) rand() / RAND_MAX * 2 - 1;
    }

    handle = instantiate(&descriptor, 96000.0, "Irrelevant", NULL);
    connect_port(handle, 0, &hipass);
    connect_port(handle, 1, buffer);
    connect_port(handle, 2, buffer);
    activate(handle);
    run(handle, DATASIZE);
    deactivate(handle);
    cleanup(handle);
    free(buffer);
    return (0);
}

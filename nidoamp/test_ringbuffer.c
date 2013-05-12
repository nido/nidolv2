#include <stdlib.h>
#include <assert.h>

#include "ringbuffer.h"

/** smoketest for the ringbuffer
 */
int main(void)
{
    float *buffer;

    struct ringbuffer *yay;
    int i;

    buffer = malloc(sizeof(float) * 1024);
    assert(init_buffer(10, 20) == NULL);
    yay = init_buffer(1024, 0);

    for (i = 0; i < 512; i++) {
        write_buffer(yay, buffer, 1020);
        read_buffer(buffer, yay, 1020);
    }
    free_buffer(yay);
    yay = init_buffer(1024, -1);

    for (i = 0; i < 512; i++) {
        read_buffer(buffer, yay, 1020);
        write_buffer(yay, buffer, 1020);
    }

    free_buffer(yay);
    free(buffer);
    return (0);
}

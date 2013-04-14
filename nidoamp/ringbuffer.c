#include <assert.h>
#include <stdlib.h>
#include "ringbuffer.h"
/** Initialise a ringbuffer
 *
 * @param size the size of the buffer
 * @param latency the amount of room between the read and write pointer
 *
 * @return the ringbuffer
 */
struct ringbuffer* init_buffer(size, latency)
{
	int i;
	struct ringbuffer* buffer;
	if(size > latency){
		return (struct ringbuffer*)NULL;
	}
	buffer = malloc(sizeof(struct ringbuffer));
	if(buffer == NULL){
		return (struct ringbuffer*)NULL;
	}
	buffer->read_index = 0;
	buffer->write_index = 0;
	buffer->size = size;
	buffer->buffer = malloc(sizeof(float) * size);
	for (i=0; i < latency; i++){
		buffer->buffer[i] = 0.0;
	}
	return buffer;
}

/** frees the ringbuffer
 *
 * @param buffer the buffer to free
 *
 */
void free_buffer(struct ringbuffer* buffer)
{
	free(buffer->buffer);
	free(buffer);
}

/** write size data to the ringbuffer from input
 *
 * @param ringbuffer the buffer to save to
 * @param input the input to read from
 * @param size the amount of items to read
 */
void write_buffer(struct ringbuffer* buffer, float* input, int size)
{
	int i;
	for (i=0; i<size; i++) {
		assert(((buffer->write_index + i - 1) % buffer->size) != buffer->read_index);
		buffer->buffer[(buffer->write_index + i) % buffer->size] = input[i];
	}
	buffer->write_index = (buffer->write_index + size) % buffer->size;
}

void read_buffer(float* output, struct ringbuffer* buffer, int size)
{
	int i;
	for (i=0; i<size; i++) {
		assert(((buffer->read_index + i) % buffer->size) != buffer->write_index);
		output[i] = buffer->buffer[(buffer->read_index + i) % buffer->size];
	}
	buffer->read_index = (buffer->read_index + size) % buffer->size;
}

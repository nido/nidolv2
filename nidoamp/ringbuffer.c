#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "ringbuffer.h"

#undef DEBUG

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
	if(size <= latency) {
#ifdef DEBUG
	printf("size (%i) needs to be bigger then latency (%i)\n", size, latency);
#endif
		return (struct ringbuffer*)NULL;
	}
	if (latency < 0) {
		latency = size + latency;
	}
	buffer = malloc(sizeof(struct ringbuffer));
	if(buffer == NULL){
#ifdef DEBUG
	printf("cannot allocate struct\n");
#endif
		return (struct ringbuffer*)NULL;
	}
	buffer->read_index = 0;
	buffer->size = size;
	buffer->buffer = malloc(sizeof(float) * size);
	assert(buffer->buffer != NULL);
	for (i=0; i < latency; i++){
		buffer->buffer[i] = 0.0;
	}
	buffer->write_index = latency;
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
void write_buffer(struct ringbuffer* buffer, const float* input, const int size)
{
	int i;
#ifdef DEBUG
	printf("Writing to %lx, readidx %i, writeidx %i, writesize %i\n", (unsigned long int) buffer, buffer->read_index, buffer->write_index, size);
#endif
	for (i=0; i<size; i++) {
		assert(((buffer->write_index + i + 1) % buffer->size) != buffer->read_index);
		buffer->buffer[(buffer->write_index + i) % buffer->size] = input[i];
	}
	buffer->write_index = (buffer->write_index + size) % buffer->size;
}

void read_buffer(float* output, struct ringbuffer* buffer, const int size)
{
#ifdef DEBUG
	printf("Reading at %lx, readidx %i, writeidx %i, readsize %i\n", (unsigned long int) buffer, buffer->read_index, buffer->write_index, size);
#endif
	peek_buffer(output, buffer, size);
	buffer->read_index = (buffer->read_index + size) % buffer->size;
}

void peek_buffer(float* output, const struct ringbuffer* buffer, const int size)
{
	int i;

	i=0;
	while (i < size){
		int index;
		int maxread;
		maxread = buffer->size - buffer->read_index;
		if (maxread > size - i) {
			maxread = size - i;
		}
		index = (buffer->read_index + i) % buffer->size;
		memcpy(output + i, buffer->buffer + index, maxread * sizeof(float));
		i+= maxread;
	}
	assert(i==size);
#ifdef DEBUG
	printf("Peeking at %lx, readidx %i, writeidx %i, peeksize %i\n", (unsigned long int) buffer, buffer->read_index, buffer->write_index, size);

	for (i=0; i<size; i++) {
		assert(((buffer->read_index + i) % buffer->size) != buffer->write_index);
		output[i] = buffer->buffer[(buffer->read_index + i) % buffer->size];
	}
#endif
}

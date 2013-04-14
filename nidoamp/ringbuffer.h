#ifndef H_RINGBUFFER
#define H_RINGBUFFER

struct ringbuffer {
	int read_index;
	int write_index;
	int size;
	float* buffer;
};

struct ringbuffer* init_buffer(const int size, const int latency);
void free_buffer(struct ringbuffer*);
void write_buffer(struct ringbuffer*, const float* input, const int size);
void peek_buffer(float* output, const struct ringbuffer*, const int size);
void read_buffer(float* output, struct ringbuffer*, const int size);

#endif

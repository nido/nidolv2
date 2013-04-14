struct ringbuffer {
	int read_index;
	int write_index;
	int size;
	float* buffer;
};

struct ringbuffer* init_buffer(int size, int latency);
void free_buffer(struct ringbuffer*);
void write_buffer(struct ringbuffer*, float* input, int size);
void read_buffer(float* output, struct ringbuffer*, int size);

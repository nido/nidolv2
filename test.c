#include <stdlib.h>

#include "nidoamp/ringbuffer.h"

void main(void){
float* buffer;

struct ringbuffer* yay;
int i;

buffer = malloc(sizeof(float) * 1024);
yay = init_buffer(1024, -2);


for (i=0; i<512; i++){
	read_buffer(buffer, yay, 1023);
	write_buffer(yay, buffer, 1023);
}
}

#include <stdlib.h>

#include "ringbuffer.h"

void main(void){
float* buffer;

struct ringbuffer* yay;
int i;

buffer = malloc(sizeof(float) * 1024);
yay = init_buffer(1024, 0);

for (i=0; i<512; i++){
	write_buffer(yay, buffer, 1020);
	read_buffer(buffer, yay, 1020);
}

yay = init_buffer(1024, -1);

for (i=0; i<512; i++){
	read_buffer(buffer, yay, 1020);
	write_buffer(yay, buffer, 1020);
}

}

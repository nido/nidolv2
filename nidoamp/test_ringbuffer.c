#include <stdlib.h>
#include <string.h>

#include "ringbuffer.h"

int main(void){
float* buffer;

struct ringbuffer* yay;
int i;

buffer = malloc(sizeof(float) * 1024);
yay = init_buffer(1024, 0);

for (i=0; i<512; i++){
	write_buffer(yay, buffer, 1020);
	read_buffer(buffer, yay, 1020);
}
printf("write/read test succes\n");

yay = init_buffer(1024, -1);

for (i=0; i<512; i++){
	read_buffer(buffer, yay, 1020);
	write_buffer(yay, buffer, 1020);
}
printf("read/write test succes\n");
return(0);

}

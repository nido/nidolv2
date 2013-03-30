#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <lv2.h>
//#include <complex.h>
#include <fftw3.h>

LV2_Handle instantiate(const LV2_Descriptor*     descriptor,
            double                    rate,
            const char*               bundle_path,
            const LV2_Feature* const* features);

struct bla {
	float* gain;
	float* input;
	float* output;
	fftw_complex complex_buffer[512/2+1];
	double real_buffer[512];
	float buffer[1024];
	int buffer_index;
	fftw_plan forward;
	fftw_plan backward;
};

int main(int argc, char* argv[]){
	uint32_t length = 1048576;
	float* stream;
	float* stream2;
	struct bla temp;
	LV2_Handle* stub;
	stream = calloc(length, sizeof(float));
	stream2 = calloc(length, sizeof(float));

	stub = (LV2_Handle)&temp;
	printf("%s\n", "instantiate");
	*stub = instantiate(NULL, 96000.0, "bla", NULL);
	printf("%i\n", stub);
	temp.input = stream;
	temp.output = stream2;
	printf("%s\n", "activate");
	activate(*stub);
	printf("%s\n", "assign");
	printf("%s\n", "run");
	run(*stub, length);
}

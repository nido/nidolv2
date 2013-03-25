#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <fftw3.h>

#define USECINSEC 1000000

int main(int argc, char* argv[])
{
	struct timeval *t, *u;
	fftw_complex *in, *out;
        fftw_plan p, q;
	int N = 512;
	int i;
	int j;
	double result;
	long elapsed;
	t = malloc(sizeof(t));
	u = malloc(sizeof(u));
	in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
        out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);

	gettimeofday(t, NULL);
        p = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
        q = fftw_plan_dft_1d(N, out, in, FFTW_BACKWARD, FFTW_ESTIMATE);
	gettimeofday(u, NULL);

	elapsed = (u->tv_sec - t->tv_sec)*USECINSEC + u->tv_usec - t->tv_usec;
	printf("Time planning: %d\n", elapsed);

	for(i=0; i < N; i++) {
		if (i%4==0){
			in[i] = 0;
		}
		if (i%4==1){
			in[i] = 1;
		}
		if (i%4==2){
			in[i] = 0;
		}
		if (i%4==3){
			in[i] = -1;
		}
	 	in[i]= ((i % 25) - 12.5) / 12.5;
	}

	gettimeofday(t, NULL);
	printf("%s\n", "precalc");
	for (i=0; i < USECINSEC; i++){

		fftw_execute(p);
		fftw_execute(q);
		for (j=0; j<N; j++){
			in[j] = in[j] / 512.0;
		}
	}
	gettimeofday(u, NULL);

	elapsed = (u->tv_sec - t->tv_sec)*USECINSEC + u->tv_usec - t->tv_usec;

	result = ((double)USECINSEC / (double)elapsed) * 512;
	printf("Time executing: %d, max number of plugins: %f\n", elapsed, result);
	for(i=0; i <N; i++) {
		printf("[ %f : %f ]",
			creal(in[i]) / (512),
			cimag(in[i]) / (512)
		);
	}
	printf("%s","\n");

        //fftw_destroy_plan(p);
        //fftw_free(in); fftw_free(out);
}

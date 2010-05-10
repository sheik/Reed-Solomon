/* Implementation of Reed-Solomon */

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>

#define DEBUG 0

unsigned int W;
unsigned int NW;

unsigned int prim_poly_4 = 023;
unsigned int prim_poly_8 = 0435;
unsigned int prim_poly_16 = 0210013;
unsigned short *gflog, *gfilog;
unsigned short *F;

/* Multiplies two numbers in GF(2^W) */
int gf_mult(int a, int b) {
	int sum_log;
	if(a == 0 || b == 0) return 0;
	sum_log = gflog[a] + gflog[b];
	if(sum_log >= NW - 1) sum_log -= NW - 1;
	return gfilog[sum_log];
}

/* Divides two numbers in GF(2^W) */
int gf_div(int a, int b) {
	int diff_log;

	if(a == 0) return 0;
	if(b == 0) return -1;
	diff_log = gflog[a] - gflog[b];
	if(diff_log < 0) diff_log += NW - 1;
	return gfilog[diff_log];
}


/* Sets up logarithm and inverse logarithm
   tables for GF(2^4), GF(2^8), and GF(2^16)
   w is the wordsize i.e. GF(2^w)
   */
int setup_tables(int w) {
	unsigned int b, log, x_to_w, prim_poly;

	switch(w) {
		case 4:  prim_poly = prim_poly_4;  break;
		case 8:	 prim_poly = prim_poly_8;  break;
		case 16: prim_poly = prim_poly_16; break;
		default: return -1;
	}

	x_to_w = 1 << w;
	gflog  = (unsigned short *) malloc(sizeof(unsigned short) * x_to_w);
	gfilog = (unsigned short *) malloc(sizeof(unsigned short) * x_to_w);

	b = 1;
	for(log = 0; log < x_to_w - 1; log++) {
		gflog[b] = (unsigned short) log;
		gfilog[log] = (unsigned short) b;
		if(DEBUG) {
			printf("gflog[%d] = %d\n", b, gflog[b]);
			printf("gfilog[%d] = %d\n",log,gfilog[log]);
		}
		b = b << 1;
		if (b & x_to_w) b = b ^ prim_poly;
	}
	return 0;
}

/* Initialize the algorithm */
int init(int n, int m) {
	int i,j;
	W = 4;
	while( n + m > pow(2,W) ) W*=2;
	if(DEBUG) printf("Initializing using W: %d\n", W);
	NW = 1 << W;
	F = (unsigned short *) malloc(sizeof(unsigned short) * n * m);
	for(i = 0; i < n; i++) {
		for(j = 0; j < m; j++) {
			F[m * i + j] = (unsigned short) pow(j+1,i);
			if(DEBUG)
				printf("F[%d][%d] = %d\n",i,j,(unsigned short) pow(j+1,i));
		}
	}
	setup_tables(W);
	return 0;
}

int main(int argc, char *argv[]) {
	if(init(3,3) == -1) {
		fprintf(stderr, "Error initializing\n");
		return -1;
	}
	printf("%d\n", gf_mult(3, 7));      // should be 9
	printf("%d\n", gf_mult(13, 10));    // should be 11
	printf("%d\n", gf_div(13, 10));     // should be 3 
	printf("%d\n", gf_div(3, 7));       // should be 10
	return 0;
}

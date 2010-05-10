/* Implementation of Reed-Solomon */

#include <stdio.h>
#include <malloc.h>

#define W 8
#define NW (1 << W)

unsigned int prim_poly_4 = 023;
unsigned int prim_poly_8 = 0435;
unsigned int prim_poly_16 = 0210013;
unsigned short *gflog, *gfilog;


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
		b = b << 1;
		if (b & x_to_w) b = b ^ prim_poly;
	}
	return 0;
}

int main(int argc, char *argv[]) {

	return 0;
}

/* service_gaussblur_gpu.c */

#include "service_gaussblur.h"

#include <math.h>

int32_t run_service_gaussblur_gpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns, int32_t size_filter) {
	int32_t i, j, k, l, pos_k, pos_l, pos;
	
	double *filter = malloc(size_filter*size_filter*sizeof(double));
	if (NULL == filter) {
		fprintf(stderr, "!!! ERROR: Cannot allocate memory.\n");
		return -1;
	}
	
	/*
	 * size_filter/3.0 seems to be a good choice for sigma,
	 * since Nvidia also uses this equation for their implementations.
	 */
	double sigma			= size_filter/3.0;
	double sigma2_2			= sigma*sigma*2;
	double factor			= 1.0/(M_PI*sigma2_2);
	int32_t size_filter12	= size_filter/2;
	int32_t size_filter2k	= 0;
	for (k=-size_filter12; k<size_filter12+1; ++k) {
		pos_k			= (k+size_filter12)*size_filter;
		size_filter2k	= (2*k*size_filter);
		for (l=-size_filter12; l<size_filter12+1; ++l) {
			pos_l	= l+size_filter12;
			pos		= pos_k + pos_l;
			if (k <= 0) {
				filter[pos] = factor * exp(-((k*k+l*l)/sigma2_2));
			} else {
				filter[pos] = filter[pos-size_filter2k];
			}
		}
	}
	
	#pragma acc kernels copyin(pixpack_source[0:columns*rows], filter[0:size_filter*size_filter]) copyout(pixpack_target[0:columns*rows])
	{
		#pragma acc loop independent
		for (i=size_filter12; i<rows-size_filter12; ++i) {
			#pragma acc loop independent private(pos, k, l, pos_k, pos_l)
			for (j=size_filter12; j<columns-size_filter12; ++j) {
				pos			= i*columns+j;
				double temp_red		= 0.0;
				double temp_green	= 0.0;
				double temp_blue	= 0.0;
				for (k=-size_filter12; k<size_filter12+1; ++k) {
					pos_k	= (k+size_filter12)*size_filter;
					for (l=-size_filter12; l<size_filter12+1; ++l) {
						pos_l		= l+size_filter12;
						temp_red 	+= filter[pos_k + pos_l] * pixpack_source[pos+k*columns+l].red;
						temp_green 	+= filter[pos_k + pos_l] * pixpack_source[pos+k*columns+l].green;
						temp_blue 	+= filter[pos_k + pos_l] * pixpack_source[pos+k*columns+l].blue;
					}
				}
				
				pixpack_target[pos].red		= (uint16_t)temp_red;
				pixpack_target[pos].green	= (uint16_t)temp_green;
				pixpack_target[pos].blue	= (uint16_t)temp_blue;
				pixpack_target[pos].opacity	= 0;
			}
		}
	}
	
	return 0;
}

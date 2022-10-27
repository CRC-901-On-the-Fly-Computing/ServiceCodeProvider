/* service_grayhistoequal_gpu.c */

#include "service_grayhistoequal.h"

int32_t run_service_grayhistoequal_gpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns) {
	uint32_t histogram[QuantumRange+1] = {0};
	
	Quantum max = 0, min = QuantumRange;
	
	int32_t i, j, pos;
	#pragma acc enter data create(pixpack_target[0:columns*rows])
	#pragma acc kernels copyin(pixpack_source[0:columns*rows]) present(pixpack_target[0:columns*rows]) copy(histogram[0:QuantumRange], min, max)
	{
		#pragma acc loop independent reduction(min:min) reduction(max:max)
		for (i=0; i<rows; ++i) {
			#pragma acc loop independent private(pos)
			for (j=0; j<columns; ++j) {
				pos = i*columns+j;
				pixpack_target[pos].red = (pixpack_source[pos].red + pixpack_source[pos].green + pixpack_source[pos].blue)/3;
				max = pixpack_target[pos].red > max ? pixpack_target[pos].red : max;
				min = pixpack_target[pos].red < min ? pixpack_target[pos].red : min;
				#pragma acc atomic
				histogram[pixpack_target[pos].red]++;
			}
		}
	}
	
	// 100/quantil %
	int32_t quantil = 5;
	int32_t temp = columns * rows / quantil;
	pos = min;
	while (temp > 0) {
		temp -= histogram[pos++];
	}
	min = --pos;
	
	temp = columns * rows / quantil;
	pos = max;
	while (temp > 0) {
		temp -= histogram[pos--];
	}
	max = ++pos;
	
	#pragma acc kernels present(pixpack_target[0:columns*rows])
	{
		if (max > min) {
			#pragma acc loop independent
			for (i=0; i<rows; ++i) {
				#pragma acc loop independent private(pos)
				for (j=0; j<columns; ++j) {
					pos = i*columns+j;
					pixpack_target[pos].red		= pixpack_target[pos].red < min ? min : (pixpack_target[pos].red > max ? max : pixpack_target[pos].red);
					
					/*
					 * Do not change the following code lines!
					 * I had problems with too small integer types.
					 */
					uint64_t tmp;
					tmp  = QuantumRange;
					tmp *= (pixpack_target[pos].red - min);
					tmp /= (max - min);
					pixpack_target[pos].red		= (uint16_t)tmp;
					pixpack_target[pos].green	= pixpack_target[pos].red;
					pixpack_target[pos].blue	= pixpack_target[pos].red;
					pixpack_target[pos].opacity	= 0;
				}
			}
		} else {
			#pragma acc loop independent
			for (i=0; i<rows; ++i) {
				#pragma acc loop independent private(pos)
				for (j=0; j<columns; ++j) {
					pos = i*columns+j;
					pixpack_target[pos].green	= pixpack_target[pos].red;
					pixpack_target[pos].blue	= pixpack_target[pos].red;
					pixpack_target[pos].opacity	= 0;
				}
			}
		}
	}
	#pragma acc exit data copyout(pixpack_target[0:columns*rows])
	return 0;
}

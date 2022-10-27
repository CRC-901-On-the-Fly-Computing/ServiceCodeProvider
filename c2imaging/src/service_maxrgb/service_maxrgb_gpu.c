/* service_maxrgb_gpu.c */

#include "service_maxrgb.h"

int32_t run_service_maxrgb_gpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns, int32_t mode) {
	int32_t i, j, pos;

	/**
	  * Modes:
	  * 1: maxRGB
	  * 2: minRGB
	  * 3: max increase
	  * 4: min erase
	**/

	uint32_t cc;
	uint16_t min, max;
	uint16_t val;

	switch (mode) {
		case 1:
		case 2:
		case 4:
			val = 0;
			break;
		case 3:
		default:
			val = QuantumRange - 1;
			break;
	}


#pragma acc kernels copyin(pixpack_source[0:columns*rows]) copyout(pixpack_target[0:columns*rows])
	{
		switch (mode) {
			case 1:
#pragma acc loop independent private(min, max, cc)
				for (i = 0; i < rows; ++i) {
#pragma acc loop independent private(pos)
					for (j = 0; j < columns; ++j) {
						pos = i * columns + j;

						//check if color is grey
						if ((pixpack_source[pos].red == pixpack_source[pos].green) && (pixpack_source[pos].red == pixpack_source[pos].blue)) {
							pixpack_target[pos].red	  = pixpack_source[pos].red;
							pixpack_target[pos].green = pixpack_source[pos].green;
							pixpack_target[pos].blue  = pixpack_source[pos].blue;
							continue;
						}

						max	= pixpack_source[pos].red;
						cc	= 1;
						if (max < pixpack_source[pos].green) {
							max = pixpack_source[pos].green;
							cc	= 2;
						}
						if (max < pixpack_source[pos].blue) {
							max = pixpack_source[pos].blue;
							cc	= 3;
						}

						pixpack_target[pos].red		= (cc == 1)	? pixpack_source[pos].red	: val;
						pixpack_target[pos].green	= (cc == 2)	? pixpack_source[pos].green	: val;
						pixpack_target[pos].blue	= (cc == 3)	? pixpack_source[pos].blue	: val;
					}
				}
				break;
			case 2:
#pragma acc loop independent private(min, max, cc)
				for (i = 0; i < rows; ++i) {
#pragma acc loop independent private(pos)
					for (j = 0; j < columns; ++j) {
						pos = i * columns + j;

						//check if color is grey
						if ((pixpack_source[pos].red == pixpack_source[pos].green) && (pixpack_source[pos].red == pixpack_source[pos].blue)) {
							pixpack_target[pos].red	  = pixpack_source[pos].red;
							pixpack_target[pos].green = pixpack_source[pos].green;
							pixpack_target[pos].blue  = pixpack_source[pos].blue;
							continue;
						}

						min	= pixpack_source[pos].red;
						cc	= 1;
						if (min > pixpack_source[pos].green) {
							min = pixpack_source[pos].green;
							cc	=2;
						}
						if (min > pixpack_source[pos].blue) {
							min = pixpack_source[pos].blue;
							cc	= 3;
						}

						pixpack_target[pos].red		= (cc == 1)	? pixpack_source[pos].red	: val;
						pixpack_target[pos].green	= (cc == 2)	? pixpack_source[pos].green	: val;
						pixpack_target[pos].blue	= (cc == 3)	? pixpack_source[pos].blue	: val;
					}
				}
				break;
			case 3:
#pragma acc loop independent private(min, max, cc)
				for (i = 0; i < rows; ++i) {
#pragma acc loop independent private(pos)
					for (j = 0; j < columns; ++j) {
						pos = i * columns + j;

						//check if color is grey
						if ((pixpack_source[pos].red == pixpack_source[pos].green) && (pixpack_source[pos].red == pixpack_source[pos].blue)) {
							pixpack_target[pos].red	  = pixpack_source[pos].red;
							pixpack_target[pos].green = pixpack_source[pos].green;
							pixpack_target[pos].blue  = pixpack_source[pos].blue;
							continue;
						}

						max	= pixpack_source[pos].red;
						cc	= 1;
						if (max < pixpack_source[pos].green) {
							max = pixpack_source[pos].green;
							cc	= 2;
						}
						if (max < pixpack_source[pos].blue) {
							max = pixpack_source[pos].blue;
							cc	= 3;
						}

						pixpack_target[pos].red		= (cc == 1)	? val : pixpack_source[pos].red;
						pixpack_target[pos].green	= (cc == 2)	? val : pixpack_source[pos].green;
						pixpack_target[pos].blue	= (cc == 3)	? val : pixpack_source[pos].blue;
					}
				}
				break;
			case 4:
			default:
#pragma acc loop independent private(min, max, cc)
				for (i = 0; i < rows; ++i) {
#pragma acc loop independent private(pos)
					for (j = 0; j < columns; ++j) {
						pos = i * columns + j;

						//check if color is grey
						if ((pixpack_source[pos].red == pixpack_source[pos].green) && (pixpack_source[pos].red == pixpack_source[pos].blue)) {
							pixpack_target[pos].red	  = pixpack_source[pos].red;
							pixpack_target[pos].green = pixpack_source[pos].green;
							pixpack_target[pos].blue  = pixpack_source[pos].blue;
							continue;
						}

						min	= pixpack_source[pos].red;
						cc	= 1;
						if (min > pixpack_source[pos].green) {
							min = pixpack_source[pos].green;
							cc	= 2;
						}
						if (min > pixpack_source[pos].blue) {
							min = pixpack_source[pos].blue;
							cc	=3;
						}

						pixpack_target[pos].red		= (cc == 1)	? val : pixpack_source[pos].red;
						pixpack_target[pos].green	= (cc == 2)	? val : pixpack_source[pos].green;
						pixpack_target[pos].blue	= (cc == 3)	? val : pixpack_source[pos].blue;
					}
				}
				break;
		}

	}

	return 0;
}

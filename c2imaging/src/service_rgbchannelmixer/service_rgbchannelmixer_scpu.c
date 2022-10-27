/* service_rgbchannelmixer_scpu.c */

#include "service_rgbchannelmixer.h"

int32_t run_service_rgbchannelmixer_scpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns, int32_t channel, float r_percent, float g_percent, float b_percent) {

	int32_t i, j, pos;
	float newVal, temp;

	for (i = 0; i < rows; ++i) {
		for (j = 0; j < columns; ++j) {
			pos = i * columns + j;


			//Calculate the new RGB-Value and set borders to 65536(16-Bit colorspace) and 0 to maintain the correct RGB-color space values
			temp = (pixpack_source[pos].red * r_percent + pixpack_source[pos].green * g_percent + pixpack_source[pos].blue * b_percent);

			if (temp > 65536) {
				newVal = 65536;
			} else {
				if (temp < 0) {
					newVal = 0;
				} else {
					newVal = temp;
				}
			}

			switch (channel) {

					//red-channel
				case 1:
					pixpack_target[pos].red	= newVal;
					pixpack_target[pos].green	= pixpack_source[pos].green;
					pixpack_target[pos].blue	= pixpack_source[pos].blue;
					pixpack_target[pos].opacity	= 0;
					break;

					//green-channel
				case 2:
					pixpack_target[pos].red	= pixpack_source[pos].red;
					pixpack_target[pos].green	= newVal;
					pixpack_target[pos].blue	= pixpack_source[pos].blue;
					pixpack_target[pos].opacity	= 0;
					break;

					//blue-channel
				case 3:
					pixpack_target[pos].red	= pixpack_source[pos].red;
					pixpack_target[pos].green	= pixpack_source[pos].green;
					pixpack_target[pos].blue	= newVal;
					pixpack_target[pos].opacity	= 0;
					break;

				default:
					pixpack_target[pos].red	= pixpack_source[pos].red;
					pixpack_target[pos].green	= pixpack_source[pos].green;
					pixpack_target[pos].blue	= pixpack_source[pos].blue;
					pixpack_target[pos].opacity	= 0;
					break;
			}
		}
	}

	return 0;
}

/* service_rotate_scpu.c */

#include "service_rotate.h"

#include <math.h>

int32_t run_service_rotate_scpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns, float degree) {
	const int32_t mu = rows >> 1;
	const int32_t mv = columns >> 1;
	int32_t u, v;
	
	// don't forget to include <math.h> for sin() and cos() support
	double teta = degree * M_PI / 180;
	double sin_teta = sin(teta);
	double cos_teta = cos(teta);
	int32_t i, j, pos_ij, pos_uv;
	
	for (i=0; i<rows; ++i) {
		for (j=0; j<columns; ++j) {
			u = i * cos_teta - j * sin_teta + mu - mu * cos_teta + mv * sin_teta;
			v = i * sin_teta + j * cos_teta + mv - mu * sin_teta - mv * cos_teta;
			pos_ij = i*columns+j;
			if (u<0 || v<0 || u>=rows || v>=columns) {
				pixpack_target[pos_ij].red		= QuantumRange;
				pixpack_target[pos_ij].green	= QuantumRange;
				pixpack_target[pos_ij].blue		= QuantumRange;
			} else {
				pos_uv = u*columns+v;
				pixpack_target[pos_ij].red		= pixpack_source[pos_uv].red;
				pixpack_target[pos_ij].green	= pixpack_source[pos_uv].green;
				pixpack_target[pos_ij].blue		= pixpack_source[pos_uv].blue;
			}
			pixpack_target[pos_ij].opacity	= 0;
		}
	}
	
	return 0;
}

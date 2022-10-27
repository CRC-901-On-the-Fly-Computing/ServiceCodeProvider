/* service_deform_fpga.c */

#include "service_deform.h"

int32_t run_service_deform_fpga(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns, float radius, float angle, int32_t positionx, int32_t positiony, int32_t mode) {
	int32_t picSize = columns * rows ;
	int32_t i, j, pos_target, pos_uv;
	float rad = (radius * ((float)columns) / 2) / 100;
	int midx = rows * positiony / 100;
	int midy = columns * positionx / 100;

	if (picSize % 16 != 0) picSize = picSize + (16 - picSize % 16);
	uint32_t *temp_pos = malloc(picSize * sizeof(uint32_t));

	switch (mode) {
		case 0 :
			run_service_deform_max_k1(rows, columns, picSize, midx, midy, rad, angle, temp_pos);
			break;
		case 1 :
			run_service_deform_max_k2(rows, columns, picSize, midx, midy, rad, angle, temp_pos);
			break;
		default  :
			fprintf(stderr, "Mode parameter hast to be 0 or 1");
			return -1;
			break;
	}

	if (mode == 0 || mode == 1) {
		for (i = 0; i < rows; i++) {
			for (j = 0; j < columns; j++) {
				pos_target = i * columns + j;
				pos_uv = *(temp_pos + pos_target);
				if (pos_uv > (rows * columns - 1) || pos_uv < 0)
					pos_uv = pos_target;
				pixpack_target[pos_target].opacity	= 0;
				pixpack_target[pos_target].red = pixpack_source[pos_uv].red;
				pixpack_target[pos_target].blue = pixpack_source[pos_uv].blue;
				pixpack_target[pos_target].green = pixpack_source[pos_uv].green;
			}
		}
	}
	free(temp_pos);
	return 0;
}

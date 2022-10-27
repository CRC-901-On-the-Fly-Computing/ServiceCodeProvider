/* service_radialblur_fpga.c */

#include "service_radialblur.h"


int32_t run_service_radialblur_fpga(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns) {
	uint32_t burst_size_in_bytes	= 384;
	uint32_t burst_size				= 384/sizeof(PixelPacket);
	uint32_t pic_size				= columns * rows;
	uint32_t aligned_pic_size		= ((columns * rows)/burst_size + 1)*burst_size;
	
	PixelPacket *pixpack_temp		= malloc(aligned_pic_size * sizeof(PixelPacket));
	if (NULL == pixpack_temp) {
		fprintf(stderr, "Couldn't allocate memory. (file: %s, line: %i)\n", __FILE__, __LINE__);
		return -1;
	}
	
	if (rows * columns > 128000) {
		fprintf(stderr, "ERROR: Image is too large for the FPGA Version!\n");
		return -1;
	}
	
	memcpy(pixpack_temp, pixpack_source, pic_size * sizeof(PixelPacket));
	
	void * engine = NULL;
	engine = run_service_radialblur_max_k1((uint16_t *)pixpack_temp, rows, columns, aligned_pic_size);
	
	engine = run_service_radialblur_max_k2_1(engine, rows, columns, aligned_pic_size);
	
	engine = run_service_radialblur_max_k2_2(engine, rows, columns, aligned_pic_size);
	
	engine = run_service_radialblur_max_k2_1(engine, rows, columns, aligned_pic_size);

	engine = run_service_radialblur_max_k2_2(engine, rows, columns, aligned_pic_size);
	
	run_service_radialblur_max_k3(engine, (uint16_t *)pixpack_target, rows, columns, aligned_pic_size);

	free(pixpack_temp);
	
	return 0;
}

/* service_blend_fpga.c */

#include "service_blend.h"

int32_t run_service_blend_fpga(PixelPacket *pixpack_target, PixelPacket *pixpack_source1, PixelPacket *pixpack_source2, int32_t rows1, int32_t columns1, int32_t rows2, int32_t columns2, float dirX, float dirY, float x, float y, int32_t mode) {

	if(mode < 0 || mode > 19){
		fprintf(stderr, "mode must be between 0 and 19.");
		return -1;			
	}
	
	run_service_blend_max((uint16_t *) pixpack_target, (uint16_t *) pixpack_source1, (uint16_t *) pixpack_source2, rows1, columns1, rows2, columns2, dirX, dirY, x, y, (uint8_t) mode);
	
    return 0;

}

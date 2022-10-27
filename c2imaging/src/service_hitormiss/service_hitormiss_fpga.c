/* service_hitormiss_fpga.c */

#include "service_hitormiss.h"

int32_t run_service_hitormiss_fpga(PixelPacket *pixpack_target, PixelPacket *pixpack_source0, PixelPacket *pixpack_source1, PixelPacket *pixpack_source2, int32_t rows0, int32_t columns0,  int32_t rows1, int32_t columns1,  int32_t rows2, int32_t columns2, int32_t threshold) {

    int i;
    int pos;
    int i_offset;
    int anchor;
    int offsetsize1, offsetsize2;
    int offsetindex;
    /** Calculate Offset Array from first Picture - Hit **/
    int32_t *offset1 = malloc(rows0 * columns0 * sizeof(int32_t));
	if (NULL == offset1) {
		fprintf(stderr, "Couldn't allocate memory. The Algorithm will Exit! (file: %s, line: %i) \n", __FILE__, __LINE__);
		return -1;
	}
    i_offset = 0; anchor = 0;
    
    for(i = 0; i < rows0 * columns0; i++) {
        if(pixpack_source0[i].red == 0 && pixpack_source0[i].green == 0 && pixpack_source0[i].blue == 0) {
            offset1[i_offset] = i - anchor;
            i_offset++;
        }
        if(pixpack_source0[i].red != 0 && pixpack_source0[i].green == 0 && pixpack_source0[i].blue == 0) {
            anchor = i;
        }
    }
    offsetsize1 = i_offset;
    for(i = 0; i < offsetsize1; i++) {
        offset1[i] = offset1[i] - anchor;
    }

    /** Calculate Offset Array from second Picture - Miss **/

    i_offset = 0;
    anchor = 0;
    int32_t *offset2 = malloc(rows1 * columns1 * sizeof(int32_t));
    for(i = 0; i < rows1 * columns1; i++) {
        if(pixpack_source1[i].red == 0 && pixpack_source1[i].green == 0 && pixpack_source1[i].blue == 0) {
            offset2[i_offset] = i - anchor ;
            i_offset++;
        }
        if(pixpack_source1[i].red != 0 && pixpack_source1[i].green == 0 && pixpack_source1[i].blue == 0) {
            anchor = i;
        }
    }
    offsetsize2 = i_offset;
    for(i = 0; i < offsetsize2; i++) {
        offset2[i] = offset2[i] - anchor;
    }

    if(offsetsize1 > 100 || offsetsize2 > 100) {
        fprintf(stderr, "Error: too big structure element for hit or miss, maximum size is 100 pixels. %d, %d", offsetsize1, offsetsize2);
        free(offset1);
        free(offset2);
        return -1;
    }

	run_service_hitormiss_max((uint16_t *) pixpack_target, (uint16_t *) pixpack_source2, rows2, columns2, threshold, offset1, offset2, offsetsize1, offsetsize2);

    free(offset1);
    free(offset2);

    return 0;

}

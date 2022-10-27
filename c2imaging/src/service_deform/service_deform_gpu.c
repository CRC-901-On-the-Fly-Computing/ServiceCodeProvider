/* service_deform_gpu.c */
/*
radius: Radius als Prozentzahl der Breite des Bildes.
angle: Wert der Verzerrung
positionx: prozentuale Angabe des Mittelpunktes in der Höhe.
positiony: prozentuale Angabe des Mittelpunktes in der Breite.
mode: Typ der Abbildungsberechung. ('0': Twirl Verzerrung, '1' Wölbungs Verzerrung)
*/

#include "service_deform.h"

int32_t run_service_deform_gpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns, float radius, float angle, int32_t positionx, int32_t positiony, int32_t mode) {

	int32_t x, y, pos_xy, pos_uv;
	int32_t u, v;
	float k, distance, rad = radius / 100 * (float)columns / 2;
	int dx, dy, distance2;
	int midy = columns * positionx / 100;
	int midx = rows * positiony / 100;

	if (mode != 0 && mode != 1) {
		fprintf(stderr, "Mode parameter hast to be 0 or 1");
		return -1;
	}

#pragma acc kernels copyin(pixpack_source[0:columns*rows]) copyout(pixpack_target[0:columns*rows])
	{

		// Mode Fallunterscheidung
		if (mode == 0) {
			// Mode: Twirl - Radiale Verzerrung
#pragma acc loop independent
			for (x = 0; x < rows; ++x) {
#pragma acc loop independent private(dy, dx, distance2, distance, pos_uv, pos_xy, u, v, k)
				for (y = 0; y < columns; ++y) {
					dy = y - midy;
					dx = x - midx;
					distance2 = dx * dx + dy * dy;
					distance = sqrt(distance2);
					if (distance > rad) {
						// Wenn außerhalbt des Radius => Keine Berechnung e.n. BP
						pos_uv = x * columns + y;
					} else {
						// Berechnung der neuen Bildpunkte
						k = (float)atan2f(dy, dx) + angle / 20 * (float)(rad - distance) / rad;
						u = (float)midx + distance * (float)cosf(k);
						v = (float)midy + distance * (float)sinf(k);
						pos_uv = u * columns + v;
					}

					pos_xy = x * columns + y;
					if (pos_uv > (rows * columns - 1) || pos_uv < 0)
						pos_uv = pos_xy;

					// Speicher der berechneten Abbildungswerte
					pixpack_target[pos_xy].red = pixpack_source[pos_uv].red;
					pixpack_target[pos_xy].green = pixpack_source[pos_uv].green;
					pixpack_target[pos_xy].blue = pixpack_source[pos_uv].blue;

					pixpack_target[pos_xy].opacity = 0;
				}
			}

		} else {
			if (mode == 1) {
				// Mode: Wölbung // Aenderung dx dy
#pragma acc loop independent
				for (x = 0; x < rows; ++x) {
#pragma acc loop independent private(dy, dx, distance2, distance, pos_uv, pos_xy, u, v, k)
					for (y = 0; y < columns; ++y) {
						dy = y - midy;
						dx = x - midx;
						distance2 = dx * dx + dy * dy;
						distance = sqrt(distance2);
						if (distance > rad) {
							// Wenn außerhalbt des Radius => Keine Berechnung e.n. BP
							pos_uv = x * columns + y;
						} else {
							// Berechnung der neuen Bildpunkte
							k = (float)atan2f(dy, dx) ;
							distance = pow(distance / rad, (angle / 50)) * distance;
							u = (float)midx + distance * (float)cosf(k);
							v = (float)midy + distance * (float)sinf(k);
							pos_uv = u * columns + v;
						}

						pos_xy = x * columns + y;
						if (pos_uv > (rows * columns - 1) || pos_uv < 0)
							pos_uv = pos_xy;
						
						// Speicher der berechneten Abbildungswerte
						pixpack_target[pos_xy].red = pixpack_source[pos_uv].red;
						pixpack_target[pos_xy].green = pixpack_source[pos_uv].green;
						pixpack_target[pos_xy].blue = pixpack_source[pos_uv].blue;

						pixpack_target[pos_xy].opacity = 0;
					}
				}
			}
		}
	}
	return 0;
}

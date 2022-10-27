/* service_reflect_cpu.c*/

#include "service_reflect.h"

int32_t run_service_reflect_cpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns, float angle) {

		//recalculate angle in [rad]
	float angleNew = angle;
	double angleRad = angleNew * M_PI / 180.0;

	//other variable names + conversion to double
	//double xLength, yLength;
	double xLength = (double)columns;
	double yLength = (double)rows;

	//sinus- and cosinus values for our angle
	double sin_angle = sin(angleRad);
	double cos_angle = cos(angleRad);

	//offset calculation for shifting picture to the middle (simulation of rotation around the middle of the picture, not an edge)
	double xOffset = xLength/2 - yLength/2 * sin_angle - xLength/2 * cos_angle;
	double yOffset = yLength/2 -yLength/2*cos_angle + xLength/2 *sin_angle;

	int32_t x, y, newX, newY, oldPosition, newPosition, finalY, finalX;
	#pragma omp parallel
	{
		#pragma omp for private(y, newX, newY, oldPosition, newPosition, finalY, finalX)
		//iterate over pixels
		for (y = 0; y < rows; ++y) {
			for (x = 0; x < columns; ++x) {

				//reflection
				int32_t newX = x;
				int32_t newY = rows - y - 1;

				//calculating pixel position
				int32_t oldPosition = y * columns + x;

				//rotation + shift
				int32_t finalY = (int32_t) round((newY * cos_angle - newX * sin_angle) + yOffset);
				int32_t finalX = (int32_t) round((newY * sin_angle + newX * cos_angle) + xOffset);

				//calculation of new position
				int32_t newPosition = finalY * columns + finalX;

				//checking if pixel matches in picture
				if (finalX >= 0 && finalX < columns && finalY >= 0 && finalY < rows) {
					//copying pixels
					pixpack_target[oldPosition].red			= pixpack_source[newPosition].red;
					pixpack_target[oldPosition].green		= pixpack_source[newPosition].green;
					pixpack_target[oldPosition].blue		= pixpack_source[newPosition].blue;
				} else {
					//if pixel does not match in picture, there will be a black pixel instead
					pixpack_target[oldPosition].red			= 0;
					pixpack_target[oldPosition].green		= 0;
					pixpack_target[oldPosition].blue		= 0;
				}
				pixpack_target[oldPosition].opacity = 0;
			}
		}
	}
	return 0;
}

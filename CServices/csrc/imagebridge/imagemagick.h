/* imagemagick.h */

#ifndef __IMAGEMAGICK_H__
#define __IMAGEMAGICK_H__

#include <magick/MagickCore.h>

typedef struct __im {
	Image			*image;
	ImageInfo		*image_info;
	ExceptionInfo	*image_exception;
	size_t			 rows;
	size_t			 columns;
} IMAGE;

#endif /* __IMAGEMAGICK_H__ */

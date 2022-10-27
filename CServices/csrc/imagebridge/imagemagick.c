/* imagemagick.c */

#include "include/C2Data_C2ImageMagickCaller.h"

#include "imagemagick.h"

#include <string.h>
#include <stdlib.h>
#include <stdint.h>

JNIEXPORT void JNICALL Java_C2Data_C2ImageMagickCaller_im_1env_1up(JNIEnv *env, jobject obj) {
	MagickCoreGenesis(NULL, MagickFalse);
}

JNIEXPORT void JNICALL Java_C2Data_C2ImageMagickCaller_im_1env_1down(JNIEnv *env, jobject obj) {
	MagickCoreTerminus();
}

JNIEXPORT jlong JNICALL Java_C2Data_C2ImageMagickCaller_open_1image(JNIEnv *env, jobject obj, jstring jstr_filename) {
	IMAGE *im = malloc(sizeof(IMAGE));
	if (NULL == im) {
		fprintf(stderr, "Couldn't allocate memory. (file: %s, line: %i)\n", __FILE__, __LINE__);
		(*env)->ThrowNew(env, (*env)->FindClass(env, "java.lang.OutOfMemoryError"), "Couldn't allocate memory.");
		return 0;
	}
	
	im->image_exception	= AcquireExceptionInfo();
	im->image_info		= CloneImageInfo(NULL);
	im->image			= NULL;
	im->rows			= 0;
	im->columns			= 0;

	const char *filename		= (*env)->GetStringUTFChars(env, jstr_filename, 0);
	const jsize filename_len	= (*env)->GetStringUTFLength(env, jstr_filename);
	
	strncpy(im->image_info->filename, filename, filename_len+1);

	(*env)->ReleaseStringUTFChars(env, jstr_filename, filename);
	
	return (jlong)im;
}

JNIEXPORT void JNICALL Java_C2Data_C2ImageMagickCaller_close_1image(JNIEnv *env, jobject obj, jlong addr) {
	IMAGE *im = (IMAGE *)addr;
	
	im->image_info		= DestroyImageInfo(im->image_info);
	im->image_exception	= DestroyExceptionInfo(im->image_exception);
	im->image			= DestroyImage(im->image);
	
	free(im);
}

JNIEXPORT void JNICALL Java_C2Data_C2ImageMagickCaller_create_1image(JNIEnv *env, jobject obj, jlong addr, jint rows, jint columns) {
	IMAGE *im = (IMAGE *)addr;
	
	MagickPixelPacket background;
	memset(&background, 0, sizeof(MagickPixelPacket));
	
	background.storage_class	= DirectClass;
	background.depth			= 16;
	background.colorspace		= RGBColorspace;
	
	im->image = NewMagickImage(im->image_info, columns, rows, &background);
}

JNIEXPORT jshortArray JNICALL Java_C2Data_C2ImageMagickCaller_read_1image(JNIEnv *env, jobject obj, jlong addr) {
	IMAGE *im = (IMAGE *)addr;
	
	im->image = ReadImage(im->image_info, im->image_exception);
	if (im->image_exception->severity != UndefinedException) {
		CatchException(im->image_exception);
	}
	if (NULL == im->image) {
		fprintf(stderr, "Couldn't load image. (file: %s, line: %i)\n", __FILE__, __LINE__);
		(*env)->ThrowNew(env, (*env)->FindClass(env, "java.io.IOException"), "Couldn't read image file.");
		return NULL;
	}
	
	PixelPacket *pixpack	= GetAuthenticPixels(im->image, 0, 0, im->image->columns, im->image->rows, im->image_exception);
	size_t pixpack_len		= im->image->columns * im->image->rows * 4;
	
	jshortArray result;
	result = (*env)->NewShortArray(env, pixpack_len);
	if (result == NULL) {
		fprintf(stderr, "Couldn't allocate memory. (file: %s, line: %i)\n", __FILE__, __LINE__);
		(*env)->ThrowNew(env, (*env)->FindClass(env, "java.lang.OutOfMemoryError"), "Couldn't allocate memory.");
		return NULL;
	}
	
	(*env)->SetShortArrayRegion(env, result, 0, (jsize)pixpack_len, (const jshort *)pixpack);
	
	im->rows	= im->image->rows;
	im->columns	= im->image->columns;
	
	SyncAuthenticPixels(im->image, im->image_exception);
	
	return result;
}

JNIEXPORT void JNICALL Java_C2Data_C2ImageMagickCaller_write_1image(JNIEnv *env, jobject obj, jlong addr, jshortArray jarr_pixpack) {
	IMAGE *im = (IMAGE *)addr;
	
	PixelPacket *pixpack	= GetAuthenticPixels(im->image, 0, 0, im->image->columns, im->image->rows, im->image_exception);
	size_t pixpack_len		= im->image->columns * im->image->rows * 4;
	
	(*env)->GetShortArrayRegion(env, jarr_pixpack, 0, (jsize)pixpack_len, (jshort *)pixpack);
	
	SyncAuthenticPixels(im->image, im->image_exception);
	
	WriteImage(im->image_info, im->image);
}

JNIEXPORT jint JNICALL Java_C2Data_C2ImageMagickCaller_get_1image_1rows(JNIEnv *env, jobject obj, jlong addr) {
	IMAGE *im = (IMAGE *)addr;
	
	return im->image->rows;
}

JNIEXPORT jint JNICALL Java_C2Data_C2ImageMagickCaller_get_1image_1columns(JNIEnv *env, jobject obj, jlong addr) {
	IMAGE *im = (IMAGE *)addr;
	
	return im->image->columns;
}

JNIEXPORT void JNICALL Java_C2Data_C2ImageMagickCaller_set_1new_1path(JNIEnv *env, jobject obj, jlong addr, jstring jstr_filename) {
	IMAGE *im = (IMAGE *)addr;
	
	const char* filename		= (*env)->GetStringUTFChars(env, jstr_filename, 0);
	const jsize filename_len	= (*env)->GetStringUTFLength(env, jstr_filename);
	
	strncpy(im->image->filename, filename, filename_len+1);
	
	(*env)->ReleaseStringUTFChars(env, jstr_filename, filename);
}

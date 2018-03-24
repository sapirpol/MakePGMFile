#ifndef __PGM_FILES
#define __PGM_FILES

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include "pgms.h"



//creates a new grayImage.
grayImage* createGrayImage(unsigned short rows, unsigned short cols, unsigned char **pixels);

//loads a pgm file to the memory.
grayImage *readPGM(char *fname);

//saves the image in a compressed format according to max_gray_level.
void saveCompressed(char* file_name, grayImage *image, unsigned char max_gray_level);

//returns the new gray level according to max_gray_level.
unsigned char calcNewGrayLevel(unsigned char gray_level, unsigned char max_gray_level);

//returns the minimum number of bits needed to represent the value.
unsigned char binaryRepLength(unsigned char value);

//opens a compressed file and saves it in a pgm format.
void convertCompressedImageToPgm(char *compressed_file_name, char *pgm_file_name);

//returns the approximate old gray level according to max_gray_level.
unsigned char calcOldGrayLevel(unsigned char gray_level, unsigned char max_gray_level);

//saves a grayImage as a pgm
void savePgm(grayImage *image, char* filename);

//free a gray image.
void freeGrayImage(grayImage *img);

#endif 
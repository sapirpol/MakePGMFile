#include "pgmfiles.h"


//creates a new grayImage.
grayImage* createGrayImage(unsigned short rows, unsigned short cols, unsigned char **pixels)
{
	grayImage *img;
	img = (grayImage*)malloc(sizeof(grayImage));
	checkMA(img);

	img->rows = rows;
	img->cols = cols;
	img->pixels = pixels;

	return img;
}

//loads a pgm file to the memory.
grayImage *readPGM(char *fname)
{
	FILE *fp;
	unsigned int i, j;
	unsigned char **resPixels;
	unsigned int maxGrayVal;
	unsigned int rows, cols;
	grayImage *new_image;
	

	fp = fopen(fname, "r");
	checkFopen(fp);

	//extracting rows and cols.
	fscanf(fp, "P2 %u %u %u", &rows, &cols, &maxGrayVal);

	//creating pixel matrix.
	resPixels = (unsigned char**)malloc(rows * sizeof(unsigned char*));
	checkMA(resPixels);

	for (i = 0; i < rows; i++)
	{
		resPixels[i] = (char*)malloc(cols * sizeof(unsigned char));
		checkMA(resPixels[i]);

		for (j = 0; j < cols; j++)
		{
			fscanf(fp, "%hhu", &resPixels[i][j]);
		}
	}
	fclose(fp);

	new_image = createGrayImage(rows, cols, resPixels);

	return new_image;
}

//saves the image in a compressed format according to max_gray_level.
void saveCompressed(char* file_name, grayImage *image, unsigned char max_gray_level)
{
	FILE *fp;
	int bitsToWrite, unusedBits, row, col;
	BYTE mask, writer=0;
	unsigned char gray_lev, max_gray_size;

	fp = fopen(file_name, "wb");
	checkFopen(fp);

	//writing the rows cols and max gray level into the files beggining.
	fwrite(&image->rows, sizeof(unsigned short), 1, fp);
	fwrite(&image->cols, sizeof(unsigned short), 1, fp);
	fwrite(&max_gray_level, sizeof(unsigned char), 1, fp);

	
	max_gray_size = binaryRepLength(max_gray_level);
	unusedBits = CHAR_BIT;

	for (row = 0; row < image->rows; row++)
	{
		for (col = 0; col < image->cols; col++)
		{
			gray_lev = calcNewGrayLevel(image->pixels[row][col], max_gray_level);
			bitsToWrite = max_gray_size;

			while (bitsToWrite > 0)
			{
				if (unusedBits > bitsToWrite) //there is excess space in writer.
				{
					gray_lev = gray_lev << (unusedBits - bitsToWrite);
					writer = writer | gray_lev;
					unusedBits -= bitsToWrite;
					bitsToWrite = 0;
				}
				else if (unusedBits == bitsToWrite) //there is exactly enough space in writer.
				{
					writer = writer | gray_lev;
					//writing the data into the file.
					fwrite(&writer, sizeof(BYTE), 1, fp);
					bitsToWrite = writer = 0;
					unusedBits = CHAR_BIT;
				}
				else if (unusedBits < bitsToWrite) //not enough space in writer.
				{
					//inserting what can be inserted into writer.
					gray_lev = gray_lev >> (bitsToWrite - unusedBits);
					mask = createMask(unusedBits -1, 0);
					writer = writer |(mask & gray_lev);
					
					//recalculting to insert remaining data in the next iteration.
					gray_lev = calcNewGrayLevel(image->pixels[row][col], max_gray_level);
					bitsToWrite -= unusedBits;

					//writing the data into the file.
					fwrite(&writer, sizeof(BYTE), 1, fp);
					unusedBits = CHAR_BIT;
					writer = 0;
				}
			}
		}
	}

	//additional space in case the last values of the picture were zeros.
	fwrite(&writer, sizeof(BYTE), 1, fp);
	
	fclose(fp);

}

//returns the new gray level according to max_gray_level.
unsigned char calcNewGrayLevel(unsigned char gray_level, unsigned char max_gray_level)
{
	double ratio = (double)max_gray_level / (double)MAX_GRAY_LV;
	double newGrayLevel = ratio*(double)gray_level;
	unsigned char res = (unsigned char)newGrayLevel;
	return res;
}

//returns the minimum number of bits needed to represent the value.
unsigned char binaryRepLength(unsigned char value)
{
	unsigned char size = 0;
	unsigned char tempVal = value;

	if (value == 0)
		return 1;

	while (value > 0)
	{
		value /= 2;
		size++;
	}
	
	return size;

}

//opens a compressed file and saves it in a pgm format.
void convertCompressedImageToPgm(char *compressed_file_name, char* pgm_file_name)
{
	FILE *compFp, *pgmFp;
	//bitsUnread represents the number of bits in reader that wern't read yet.
	//bitsToRead represents the number of bits missing to complete the current gray_lev.
	int bitsToRead, bitsUnread, row, col;
	//reader holds data from the compressed file.
	BYTE mask1, mask2, reader = 0;
	unsigned char gray_lev, max_gray_size, max_gray_lev;
	unsigned short rows, cols;
	char lfOrSpace;

	//opening the compressed and the new file.
	compFp = fopen(compressed_file_name, "rb");
	checkFopen(compFp);

	pgmFp = fopen(pgm_file_name, "w");
	checkFopen(pgmFp);

	fread(&rows, sizeof(unsigned short), 1, compFp);
	fread(&cols, sizeof(unsigned short), 1, compFp);
	fread(&max_gray_lev, sizeof(unsigned char), 1, compFp);		//X

	//writing the first lines in the new pgm file.
	fprintf(pgmFp, "P2\n%hu %hu\n%hhu\n", rows, cols, max_gray_lev);

	max_gray_size = binaryRepLength(max_gray_lev);
	fread(&reader, sizeof(BYTE), 1, compFp);
	bitsUnread = CHAR_BIT;

	for (row = 0; row < rows; row++)
	{
		for (col = 0; col < cols; col++)
		{
			//each iteration creates a new gray_lev and writes it into the new pgm file.
			gray_lev = 0;
			bitsToRead = max_gray_size;

			if (bitsUnread > bitsToRead) //there is excess data in reader.
			{
				gray_lev = reader;
				gray_lev = gray_lev >> (bitsUnread - bitsToRead);
				mask1 = createMask(max_gray_size - 1, 0);
				gray_lev = gray_lev & mask1;

				//writing the data into the file.
				lfOrSpace = col != cols - 1 ? ' ' : '\n';
				gray_lev = calcOldGrayLevel(gray_lev, max_gray_lev);
				fprintf(pgmFp, "%hhu%c", gray_lev, lfOrSpace); 

				bitsUnread -= bitsToRead;
				bitsToRead = 0;
			}
			else if (bitsUnread == bitsToRead) //there is exactly enough data in reader.
			{
				gray_lev = reader;
				mask1 = createMask(max_gray_lev - 1, 0);
				gray_lev = gray_lev & mask1;

				//writing the data into the file.
				lfOrSpace = col != cols - 1 ? ' ' : '\n';
				gray_lev = calcOldGrayLevel(gray_lev, max_gray_lev);
				fprintf(pgmFp, "%hhu%c", gray_lev, lfOrSpace);

				//writing the data into the file.
				fread(&reader, sizeof(BYTE), 1, compFp);
				bitsToRead = 0;
				bitsUnread = CHAR_BIT;
			}
			else if (bitsUnread < bitsToRead) //not enough data in reader for the current gray_lev.
			{
				//reading what can be read into gray_lev.
				mask1 = createMask(bitsUnread -1, 0);
				gray_lev = mask1 & reader;

				//moving the bits in gray_lev and updating variables.
				bitsToRead -= bitsUnread;
				gray_lev = gray_lev << bitsToRead;

				//reading the next block from the compressed file.
				fread(&reader, sizeof(BYTE), 1, compFp);
				bitsUnread = CHAR_BIT;
				mask1 = createMask(LEFT_BIT, LEFT_BIT - bitsToRead +1);

				//reading the remaining bits from reader into gray_lev.
				mask2 = reader & mask1;
				mask2 = mask2 >> (CHAR_BIT - bitsToRead);
				mask1 = createMask(bitsToRead - 1, 0);
				gray_lev = gray_lev | (mask1 & mask2);
				bitsUnread -= bitsToRead;
				bitsToRead = 0;

				//writing the data into the file.
				lfOrSpace = col != cols - 1 ? ' ' : '\n';
				gray_lev = calcOldGrayLevel(gray_lev, max_gray_lev);
				fprintf(pgmFp, "%hhu%c", gray_lev, lfOrSpace);
			}
		}
	}

	// closing the files.
	fclose(compFp);
	fclose(pgmFp);
}

//returns the approximate old gray level according to max_gray_level.
unsigned char calcOldGrayLevel(unsigned char gray_level, unsigned char max_gray_level)
{
	double inverseRatio = (double)MAX_GRAY_LV / (double)max_gray_level;
	double oldGrayLevel = inverseRatio*(double)gray_level;
	unsigned char res = (unsigned char)oldGrayLevel;
	return res;
}

//saves a grayImage as a pgm.
void savePgm(grayImage *image, char* filename)
{
	FILE *fp;
	int i, j;
	fp = fopen(filename, "w");
	checkFopen(fp);

	fprintf(fp, "P2\n%hhu %hhu\n255\n", image->rows, image->cols);

	for (i = 0; i < image->rows; i++)
	{
		fprintf(fp, "%hhu" ,image->pixels[i][0]);

		for (j = 1; j < image->cols; j++)
		{
			fprintf(fp, " %hhu", image->pixels[i][j]);


		}
		fprintf(fp, "\n");

	}

	fclose(fp);

}

//free a gray image.
void freeGrayImage(grayImage *img)
{
	int i;
	for (i = 0; i < img->rows; i++)
		free(img->pixels[i]);

	free(img->pixels);

	free(img);
}
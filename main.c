/*
final project 16/06/2017
Sapir Pol  315826206
Lior Eliav 308057769
*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>

#include "pgms.h"
#include "segment.h"
#include "pgmfiles.h"

void main()
{
	BOOL exit = FALSE, getInput = TRUE;
	grayImage *image = NULL, *coloredImage = NULL;
	imgPosList *segments = NULL;
	int segmentsSize = 0;
	short choice;
	char filename1[40];
	char filename2[40];
	unsigned char threshold, max_gray_level;

	printMenu();

	while (exit == FALSE)
	{

		if (getInput == TRUE)
		{
			printf("\nType your choice: ");
			scanf("%hu", &choice);
		}

		switch (choice)
		{
		case 1: //	option one - reading a pgm file.
		{
			printf("Enter a file name: ");
			scanf("%s", filename1);

			if (image != NULL)
				freeGrayImage(image);

			image = readPGM(filename1);
			printf("Image loaded.\n\n");

			getInput = TRUE;
			break;
		}
		case 2: // option two - segmenting the image.
		{
			if (image != NULL) 
			{
				printf("Enter the segmenting threshold: ");
				scanf("%hhu", &threshold);

				if (segments != NULL)
					freeImgPosList(segments);

				segmentsSize = findAllSegments(image, threshold, &segments);
				printf("Segments created.\n\n");

				getInput = TRUE;
			}
			else
			{
				printf("There is no image loaded, Please load an image first.\n");
				choice = 1;
				getInput = FALSE;
			}
			break;
		}
		case 3: // opiton three - coloring the segments.
		{
			if (segments != NULL)
			{
				coloredImage = colorSegments(segments, segmentsSize);
				printf("Colored image created.\n\n");

				getInput = TRUE;
			}
			else
			{
				printf("Segments are needed to be created first.\n");
				choice = 2;
				getInput = FALSE;
			}
			break;
		}
		case 4: // option four - compressing the colored image.
		{
			if (coloredImage != NULL)
			{
				printf("Enter a new compressed file name: ");
				scanf("%s", filename1);
				printf("Enter your new max gray level: ");
				scanf("%hhu", &max_gray_level);
				saveCompressed(filename1, coloredImage, max_gray_level);
				printf("Compressed image file created.\n\n");

				getInput = TRUE;
			}
			else
			{
				printf("a colored image needs to be created first.\n");
				choice = 3;
				getInput = FALSE;
			}
			break;
		}
		case 5: // option five - compressing the original image.
		{
			if (image != NULL)
			{
				printf("Enter a new compressed file name: ");
				scanf("%s", filename1);
				printf("Enter your new max gray level: ");
				scanf("%hhu", &max_gray_level);
				saveCompressed(filename1, image, max_gray_level);
				printf("Compressed image file created.\n\n");

				getInput = TRUE;
			}
			else
			{
				printf("There is no image loaded, Please load an image first.\n");
				choice = 1;
				getInput = FALSE;
			}
			
			break;
		}
		case 6: // option six - decompressing an image.
		{
			printf("Enter a compressed file name: ");
			scanf("%s", filename1); 
			printf("Enter a new pgm file name: ");
			scanf("%s", filename2);
			convertCompressedImageToPgm(filename1, filename2);
			printf("PGM file created.\n\n");

			break;
		}
		case 7: // EXIT.
		{
			if (image != NULL)
				freeGrayImage(image);

			if (coloredImage != NULL)
				freeGrayImage(coloredImage);

			if (segments != NULL)
				freeImgPosList(segments);

			exit = TRUE;
			break;
		}
		default:
		{
			printf("Invalid input. Please enter a number between 1 and 7\n");
			getInput = TRUE;
			break;
		}

		}//end of switch.

	}//end of while.

}



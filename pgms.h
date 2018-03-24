#ifndef __PGMS

#define __PGMS

#include <stdio.h>
#include <stdlib.h>

#define _CRT_SECURE_NO_WARNINGS
#define TRUE 1
#define FALSE 0
#define MAX_POS_CODE 8
#define MIN_POS_CODE 1
#define MAX_GRAY_LV 255
#define LEFT_BIT 7
#define CHAR_BIT 8

typedef struct _grayImage {
	unsigned short rows, cols;
	unsigned char **pixels;
} grayImage;

typedef unsigned short imgPos[2];

typedef struct _treeNode {
	imgPos position;
	struct _treeNodeListCell *next_possible_positions;
} treeNode;


typedef struct _treeNodeListCell {
	treeNode *node;
	struct _treeNodeListCell *next;
}treeNodeListCell;


typedef struct _segment {
	treeNode *root;
}Segment;


typedef struct _imgPosCell {
	imgPos position;
	struct _imgPosCell *next, *prev;
}imgPosCell;


typedef struct _imPosList {
	imgPosCell *head, *tail;
}imgPosList;

typedef int BOOL;

typedef unsigned char BYTE;


//ASSISTINGS FUNCTIONS.

void printSegLDR(treeNode *root, int lev, grayImage *img);

void printList(imgPosList list);



//checks memory allocation.
void checkMA(void *ptr);

//checks file opening.
void checkFopen(FILE *fptr);

//creates a BYTE sized mask with the bits leftSetBit to rightSetBit set.
BYTE createMask(int leftSetBit, int rightSetBit);

//prints the menu.
void printMenu();



#endif

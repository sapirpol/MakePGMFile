#ifndef __SEGMENT
#define __SEGMENT


#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include "pgms.h"


//creates a treeNode.
treeNode* createTreeNode(imgPos position, treeNodeListCell *next_possible_positions);

//creates a treeNodeListCell.
treeNodeListCell* createTreeNodeListCell(treeNode *node, treeNodeListCell *next);

//creates a new imgPosCell.
imgPosCell* createImgPosCell(imgPos pos, imgPosCell *next, imgPosCell *prev);

//creates a new segment.
Segment* createSegment(treeNode* root);

//creates a reset bit sized flag array.
BYTE* createBitArr(int rows, int cols);

//free a treeNode
void freeTreeNode(treeNode *tr);

//free an imgPosList.
void freeImgPosList(imgPosList *list);

//free a treeNodeListCell
void freeTreeNodeListCell(treeNodeListCell *listCell);

//free a bit sized flags array.
void freeBitArray(BYTE *flagArr);

//free a Segment.
void freeSegment(Segment seg);

//checks if the position is flagged in the flag array.
int isFlagged(imgPos position, unsigned short maxCol, BYTE* flagArr);

//flags a position in the flag array.
void flagPosition(imgPos position, unsigned short maxCol, BYTE* flagArr);

//returns TRUE if the position exists in the grayImage.
int isValidPosition(imgPos position, grayImage* img);

//returns a position from those that surround start according to posCode.
void generateImgPos(imgPos start, int posCode, imgPos newPos);

//finds all segments in the img and returns them as an array of imgPosList.
int findAllSegments(grayImage *img, unsigned char threshold, imgPosList **segments);

//finding segment from start position
Segment findSingleSegment(grayImage *img, imgPos start, unsigned char threshold);

//finds a single segment in the img.
Segment findSegment(grayImage *img, imgPos start, unsigned char threshold, BYTE *flagArr);

//assists findSingleSegment. 
treeNodeListCell* findNextPositions(grayImage *img, imgPos start, unsigned char startValue, unsigned char threshold, BYTE *flagArr);

//converts a segment to an imgPosList.
imgPosList convertSegmentToImgPosList(Segment seg);

//creates imgPosCells for every treeNode in the tree and connects them to each other.
void chainNodes(treeNodeListCell *tNode, imgPosCell **nodePtrPtr);

//builds and returns a grayImage from an imgPosList 
grayImage *colorSegments(imgPosList *segments, unsigned int size);

//updates the rows and cols to the max value
void findMaxRowsAndCols(imgPosList *segments, unsigned short *rows, unsigned short *cols, unsigned int size);

//changes the pixels of each position in the segment
void changePixels(imgPosList *segments, int i, unsigned int size, grayImage *img);


#endif

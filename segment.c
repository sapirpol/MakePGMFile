#include "segment.h"


//creates a treeNode.
treeNode* createTreeNode(imgPos position, treeNodeListCell *next_possible_positions)
{
	treeNode *newTreeNode;

	newTreeNode = (treeNode*)malloc(sizeof(treeNode));
	checkMA(newTreeNode);

	newTreeNode->position[0] = position[0];
	newTreeNode->position[1] = position[1];

	newTreeNode->next_possible_positions = next_possible_positions;

	return newTreeNode;
}

//creates a treeNodeListCell.
treeNodeListCell* createTreeNodeListCell(treeNode *node, treeNodeListCell *next)
{
	treeNodeListCell *res;

	res = (treeNodeListCell*)malloc(sizeof(treeNodeListCell));
	checkMA(res);

	res->node = node;
	res->next = next;

	return res;
}

//creates a new imgPosCell.
imgPosCell* createImgPosCell(imgPos pos, imgPosCell *next, imgPosCell *prev)
{
	imgPosCell *cell;
	cell = (imgPosCell*)malloc(sizeof(imgPosCell));
	checkMA(cell);

	cell->position[0] = pos[0];
	cell->position[1] = pos[1];
	cell->next = next;
	cell->prev = prev;

	return cell;

}

//creates a new segment.
Segment* createSegment(treeNode* root)
{
	Segment *seg = (Segment*)malloc(sizeof(Segment));
	checkMA(seg);

	seg->root = root;
	return seg;
}

//creates a reset bit sized flag array.
BYTE* createBitArr(int rows, int cols)
{
	BYTE* flagMatrix;

	flagMatrix = (BYTE*)calloc(1 + rows*cols/CHAR_BIT, sizeof(BYTE));
	checkMA(flagMatrix);
	
	return flagMatrix;
	
}

//free a treeNode
void freeTreeNode(treeNode *tr)
{
	freeTreeNodeListCell(tr->next_possible_positions);
	free(tr);
}

//free an imgPosList.
void freeImgPosList(imgPosList *list)
{
	imgPosCell *node, *next;
	node = list->head;

	while (node != NULL)
	{
		next = node->next;
		free(node);
		node = next;
	}

	free(list);

}

//free a treeNodeListCell
void freeTreeNodeListCell(treeNodeListCell *listCell)
{
	if (listCell == NULL)
		return;
	else
	{
		freeTreeNodeListCell(listCell->next);
		freeTreeNode(listCell->node);
		free(listCell);
	}
}

//free a bit sized flags array.
void freeBitArray(BYTE *flagArr)
{
	free(flagArr);
}

//free a Segment.
void freeSegment(Segment seg)
{
	freeTreeNode(seg.root);
}

//checks if the position is flagged in the flag array.
int isFlagged(imgPos position, unsigned short maxCol, BYTE* flagArr)
{
	BYTE mask, res;
	int byte = (maxCol*position[0] + position[1]) / CHAR_BIT;
	int bit = (maxCol*position[0] + position[1]) % CHAR_BIT;
	mask = createMask(bit, bit);

	res = flagArr[byte] & mask;

	if (res != 0)
		return TRUE;
	else
		return FALSE;

}

//flags a position in the flag array.
void flagPosition(imgPos position, unsigned short maxCol, BYTE* flagArr)
{
	BYTE mask;
	int byte = (maxCol*position[0] + position[1]) / CHAR_BIT;
	int bit = (maxCol*position[0] + position[1]) % CHAR_BIT;
	mask = createMask(bit, bit);
	flagArr[byte] = flagArr[byte] | mask;

}

//returns TRUE if the position exists in the grayImage.
int isValidPosition(imgPos position, grayImage* img)
{
	if (position[0] < 0 || position[1] < 0)
		return FALSE;

	if (position[0] >= img->rows || position[1] >= img->cols)
		return FALSE;

	return TRUE;
}

//returns a position from those that surround start according to posCode.
void generateImgPos(imgPos start, int posCode, imgPos newPos)
{
	/*	according to:

	_1|2|3_
	_4|#|5_
	6|7|8
	*/
	switch (posCode)
	{
	case 1:
	{
		newPos[0] = start[0] - 1;
		newPos[1] = start[1] - 1;
		break;
	}
	case 2:
	{
		newPos[0] = start[0] - 1;
		newPos[1] = start[1];
		break;
	}
	case 3:
	{
		newPos[0] = start[0] - 1;
		newPos[1] = start[1] + 1;
		break;
	}
	case 4:
	{
		newPos[0] = start[0];
		newPos[1] = start[1] - 1;
		break;
	}
	case 5:
	{
		newPos[0] = start[0];
		newPos[1] = start[1] + 1;
		break;
	}
	case 6:
	{
		newPos[0] = start[0] + 1;
		newPos[1] = start[1] - 1;
		break;
	}
	case 7:
	{
		newPos[0] = start[0] + 1;
		newPos[1] = start[1];
		break;
	}
	case 8:
	{
		newPos[0] = start[0] + 1;
		newPos[1] = start[1] + 1;
		break;
	}
	default:
	{
		newPos[0] = -1;
		newPos[1] = -1;
		break;
	}

	}


}

//finds all segments in the img and returns them as an array of imgPosList.
int findAllSegments(grayImage *img, unsigned char threshold, imgPosList **segments)
{
	BYTE* flagMatrix;
	int i, j, logicSize = 0, physicalSize = 1;
	Segment seg;
	imgPosList *listArray, currList;
	imgPos pos;

	listArray = (imgPosList*)malloc(sizeof(imgPosList));
	checkMA(listArray);

	flagMatrix = createBitArr(img->rows, img->cols);

	//nested for loops that go through every pixel in the grayImage.
	for (i = 0; i < img->rows; i++)
	{
		for (j = 0; j < img->cols; j++)
		{
			pos[0] = i;
			pos[1] = j;

			if (isFlagged(pos, img->cols ,flagMatrix) == FALSE)
			{
				//the position is not in any segment yet.
				seg = findSegment(img, pos, threshold, flagMatrix);
				//converting to imgPosList and releasing the Segment.
				currList = convertSegmentToImgPosList(seg);
				freeSegment(seg);

				//expanding the array in necessary
				if (physicalSize = logicSize)
				{
					physicalSize *= 2;
					listArray = realloc(listArray, physicalSize * sizeof(imgPosList));
					checkMA(listArray);
				}

				listArray[logicSize] = currList;
				logicSize++;
			}
		}
	}

	freeBitArray(flagMatrix);

	//trimming excess memory allocation.
	listArray = (imgPosList*)realloc(listArray, logicSize * sizeof(imgPosList));
	checkMA(listArray);

	//return.
	*segments = listArray;
	return logicSize;
	
}

//finding segment from start position
Segment findSingleSegment(grayImage *img, imgPos start, unsigned char threshold)
{
	Segment seg;
	treeNode *root;
	int rows, cols;
	BYTE *flagMatrix;

	rows = img->rows;
	cols = img->cols;
	flagMatrix = createBitArr(rows, cols);

	root = createTreeNode(start, NULL);
	flagPosition(start, cols, flagMatrix);
	root->next_possible_positions = findNextPositions(img, start, img->pixels[start[0]][start[1]], threshold, flagMatrix);

	seg.root = root;
	freeBitArray(flagMatrix);
	return seg;
}

//finds a single segment in the img.
Segment findSegment(grayImage *img, imgPos start, unsigned char threshold, BYTE *flagArr)
{
	Segment seg;
	treeNode *root;
	int rows, cols;

	rows = img->rows;
	cols = img->cols;

	root = createTreeNode(start, NULL);
	flagPosition(start, cols, flagArr);
	root->next_possible_positions = findNextPositions(img, start, img->pixels[start[0]][start[1]], threshold, flagArr);

	seg.root = root;

	return seg;

}

//assists findSingleSegment. 
treeNodeListCell* findNextPositions(grayImage *img, imgPos start, unsigned char startValue, unsigned char threshold, BYTE *flagArr)
{
	imgPos tempPos;
	int i;
	BOOL inTree;
	unsigned char grayLev;
	treeNode *newTreeNode;
	treeNodeListCell *listCellHead = NULL, *tempTreeNodeListCell, *newTreeNodeListCell;

	//adding the suitable neighboring pixels to the tree as new treeNodes and treeNodeListCells.

	//generating neighboring positions.
	for (i = 1; i < MAX_POS_CODE; i++)
	{
		generateImgPos(start, i, tempPos);
		if (isValidPosition(tempPos, img))
		{
			//position exists in img.
			inTree = isFlagged(tempPos, img->cols, flagArr);

			if (inTree == FALSE)
			{
				//position is not in the tree.
				grayLev = img->pixels[tempPos[0]][tempPos[1]];

				if (grayLev <= startValue + threshold && grayLev >= startValue - threshold)
				{
					//gray level within allowed margin.
					flagPosition(tempPos, img->cols,  flagArr);
					newTreeNode = createTreeNode(tempPos, NULL);
					newTreeNodeListCell = createTreeNodeListCell(newTreeNode, NULL);

					//inserting the treeNodeListCell as the head of the list or at its tail.
					if (listCellHead == NULL)
						listCellHead = newTreeNodeListCell;
					else
						tempTreeNodeListCell->next = newTreeNodeListCell;

					tempTreeNodeListCell = newTreeNodeListCell;

				}//end if.

			}//end if.

		}//end if.

	}//end for.


	 //addition of neighboring pixels done. the function is called to find their neighboring pixles and add them to the tree.

	 //the recursive call of the function.
	tempTreeNodeListCell = listCellHead;
	while (tempTreeNodeListCell != NULL)
	{
		tempPos[0] = tempTreeNodeListCell->node->position[0];
		tempPos[1] = tempTreeNodeListCell->node->position[1];
		tempTreeNodeListCell->node->next_possible_positions = findNextPositions(img, tempPos, startValue, threshold, flagArr);

		tempTreeNodeListCell = tempTreeNodeListCell->next;
	}


	return listCellHead;

}

//converts a segment to an imgPosList.
imgPosList convertSegmentToImgPosList(Segment seg)
{
	imgPosList list;
	imgPosCell *head, *tail;

	head = createImgPosCell(seg.root->position, NULL, NULL);
	tail = head;
	chainNodes(seg.root->next_possible_positions, &tail);

	list.head = head;
	list.tail = tail;

	return list;
}

//creates imgPosCells for every treeNode in the tree and connects them to each other.
void chainNodes(treeNodeListCell *tNode, imgPosCell **nodePtrPtr)
{
	if (tNode != NULL)
	{
		imgPosCell *newCell;
		newCell = createImgPosCell(tNode->node->position, NULL, NULL);

		(**nodePtrPtr).next = newCell;
		newCell->prev = *nodePtrPtr;
		*nodePtrPtr = newCell;

		chainNodes(tNode->node->next_possible_positions, nodePtrPtr);
		chainNodes(tNode->next, nodePtrPtr);
	}
} 

//builds and returns a grayImage from an imgPosList 
grayImage *colorSegments(imgPosList *segments, unsigned int size)
{
	unsigned int i;
	grayImage *img;
	unsigned short rows, cols;

	img = (grayImage*)malloc(sizeof(grayImage));
	checkMA(img);

	findMaxRowsAndCols(segments, &rows, &cols, size);
	img->rows = rows;
	img->cols = cols;

	img->pixels = (unsigned char**)malloc(rows * sizeof(unsigned char*));
	checkMA(img->pixels);

	for (i = 0; i < rows; i++)
	{
		img->pixels[i] = (unsigned char*)malloc(cols * sizeof(unsigned char));
		checkMA(img->pixels[i]);
	}


	for (i = 0; i < size; i++)
		changePixels(segments, i, size, img);

	return img;
}

//updates the rows and cols to the max value
void findMaxRowsAndCols(imgPosList *segments, unsigned short *rows, unsigned short *cols, unsigned int size)
{
	unsigned int i;
	imgPosCell *current;

	unsigned short maxRowIndex = 0, maxColIndex = 0;

	for (i = 0; i < size; i++)
	{
		current = segments[i].head;
		while (current != NULL)
		{
			if (current->position[0] > maxRowIndex)
				maxRowIndex = current->position[0];
			if (current->position[1] > maxColIndex)
				maxColIndex = current->position[1];
			current = current->next;
		}
	}

	//highest index is not the size.
	*rows = maxRowIndex + 1;
	*cols = maxColIndex + 1;
}

//changes the pixels of each position in the segment
void changePixels(imgPosList *segments, int i, unsigned int size, grayImage *img)
{
	imgPosCell *current;
	current = segments[i].head;
	unsigned char newGrayLevel = (i*255 / (size - 1));

	while (current != NULL)
	{
		img->pixels[current->position[0]][current->position[1]] = newGrayLevel;
		current = current->next;
	}
}

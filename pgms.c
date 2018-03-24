#include "pgms.h"


void printSegLDR(treeNode *root, int lev, grayImage *img)
{
	treeNodeListCell *temp;

	temp = root->next_possible_positions;

	while (temp != NULL)
	{
		printSegLDR(temp->node, lev + 1, img);
		temp = temp->next;
	}
	unsigned char gray = img->pixels[root->position[0]][root->position[1]];
	printf("\n Val: %hhu (%hhu,%hhu) Lv: %d ", gray, root->position[0], root->position[1], lev);

}

void printList(imgPosList list)
{
	imgPosCell *currNode = list.head;

	while (currNode != NULL)
	{
		printf("(%d,%d)\n", currNode->position[0], currNode->position[1]);
		currNode = currNode->next;
	}


}


//checks memory allocation.
void checkMA(void *ptr)
{
	if (ptr == NULL)
	{
		printf("memory allocation failed");
		exit(1);
	}

}

//checks file opening.
void checkFopen(FILE *fptr)
{
	if (fptr == NULL)
	{
		printf("file access failed.");
		exit(1);
	}
}

//creates a BYTE sized mask with the bits leftSetBit to rightSetBit set.
BYTE createMask(int leftSetBit, int rightSetBit)
{
	int i;
	BYTE mask = 0;
	mask = 1; // [00000001]

	for (i = 0; i < leftSetBit - rightSetBit; i++)
		mask = mask * 2 + 1;	//bumping to the right and adding a 1.

	for (i = 0; i < rightSetBit; i++)
		mask = mask * 2;		//bumping to the right.

	return mask;

}

//prints the menu.
void printMenu()
{
	printf("\nWelcome, please enter your desired action:\n\n");
	printf("1. Read an image file in PGM format.\n");
	printf("2. Find all segments.\n");
	printf("3. Color the segments.\n");
	printf("4. Save the colored image in a compressed format.\n");
	printf("5. Compress and save the original image in compressed format.\n");
	printf("6. Convert a compressed image to PGM format.\n");
	printf("7. Exit.\n");
}
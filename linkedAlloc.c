#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define SIZE 20

struct fileDetails {
	char filename[SIZE];
	float filesize;
	float no_fileBlocks;
	int fileBlock[100];
} file[SIZE];

int totalBlocks, availableBlocks;
int block[100];
float sizeOfBlock;

void main() {
	
	int fileno=0;
	int currentBlock;
	char choice;
	
	printf("Enter the total number of the memory blocks: ");
	scanf("%d", &totalBlocks);
	printf("Enter the size of one block: ");
	scanf("%f", &sizeOfBlock);
	
	availableBlocks = totalBlocks;
	
	do {
		printf("\nEnter the file name: ");
		scanf("%s", file[fileno].filename);
		printf("Enter the size of the file: ");
		scanf("%f", &file[fileno].filesize);
		
		file[fileno].no_fileBlocks = ceil(file[fileno].filesize / sizeOfBlock);
		
		if(file[fileno].no_fileBlocks <= availableBlocks){
			for(int i=0; i<file[fileno].no_fileBlocks; ){
				currentBlock = rand() % totalBlocks;
				if(block[currentBlock] != 1){
					file[fileno].fileBlock[i] = currentBlock;
					block[currentBlock] = 1;
					i++;
				}
			}
			availableBlocks -= file[fileno].no_fileBlocks;
			fileno++;
		} else
			printf("There are no free blocks to allocate this file.\n");
		printf("Do you want to add more files? (Y:yes, n:no): ");
		scanf(" %c", &choice);
	} while(choice == 'Y' || choice == 'y');
	
	printf("\nFile Name \t File Size \t Blocks Allocation\n");
	for(int i=0; i<fileno; i++){
		printf("%s \t %.0f \t\t ", file[i].filename, file[i].filesize);
		for(int j=0; j<file[i].no_fileBlocks; j++)
			printf("%d ", file[i].fileBlock[j]);
		printf("\n");
	}
	
}

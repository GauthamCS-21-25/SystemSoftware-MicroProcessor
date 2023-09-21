#include <stdio.h>
#include <math.h>

#define SIZE 20

struct fileDetails {
	char filename[SIZE];
	float filesize;
	float no_fileBlocks;
	int startBlock;
	int endBlock;
} file[SIZE];

int totalBlocks, availableBlocks;
float sizeOfBlock;

void main() {
	
	int fileno=0;
	int currentBlock=0;
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
			file[fileno].startBlock = currentBlock;
			file[fileno].endBlock = currentBlock+(file[fileno].no_fileBlocks - 1);
			currentBlock = file[fileno].endBlock + 1;
			availableBlocks -= file[fileno].no_fileBlocks;
			fileno++;
		} else
			printf("There are no free blocks to allocate this file.\n");
		printf("Do you want to add more files? (Y:yes, n:no): ");
		scanf(" %c", &choice);
	} while(choice == 'Y' || choice == 'y');
	
	printf("File Name \t File Size \t Blocks Allocation\n");
	for(int i=0; i<fileno; i++){
		printf("%s \t %.0f \t\t ", file[i].filename, file[i].filesize);
		for(int j=file[i].startBlock; j<=file[i].endBlock; j++)
			printf("%d ", j);
		printf("\n");
	}
	
}

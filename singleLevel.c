#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h> 
#include <string.h>

#define SIZE 20

char path[]="/home/S5CSB/GauthamCSudheer";
char dirpath[100];
struct stat st = {0};

void createDirectory(){
	char dir_name[SIZE];
	
	printf("Enter the directory name: ");
	scanf("%s", dir_name);
	
	strcpy(dirpath, path);
	strcat(dirpath, "/");
	strcat(dirpath, dir_name);
	
	if (stat(dirpath, &st) == -1) {
    	mkdir(dirpath, 0777);
    	printf("Directory created successfully.\n");
	} else
		printf("Directory already exists.\n");
}

void createFile(){
	char filename[SIZE];
	char destination[100];
	FILE *fptr;
	
	printf("Enter the file name: ");
	scanf("%s", filename);
	
	strcpy(destination, dirpath);
	strcat(destination, "/");
	strcat(destination, filename);
	
	if (stat(destination, &st) == -1) {
    	fptr = fopen(destination, "w");
    	fclose(fptr);
    	printf("File created successfully.\n");
	} else
		printf("File already exists.\n");
}

void deleteFile(){
	char filename[SIZE], filepath[100];
	
	printf("Enter the file name: ");
	scanf("%s", filename);
	
	strcpy(filepath, dirpath);
	strcat(filepath, "/");
	strcat(filepath, filename);
	
	if (remove(filepath) == 0)
        printf("File deleted successfully.\n");
    else
        printf("File to be deleted not found in the directory.\n");
}

void searchFile(){
	char filename[SIZE], filepath[100];
	
	printf("Enter the file name: ");
	scanf("%s", filename);
	
	strcpy(filepath, dirpath);
	strcat(filepath, "/");
	strcat(filepath, filename);
	
	if (stat(filepath, &st) != -1) 
		printf("File found in the directory.\n");
	else
		printf("File not found in the directory.\n");
}

void displayFiles(){
	DIR *dir;
	dir = opendir(dirpath);
	struct dirent *entries;
	if(dir){
    	while ((entries = readdir(dir)) != NULL)
      		printf("%s\n", entries->d_name);
    	closedir(dir);
 	} else
 		printf("Directory not selected.\n");
}

void main(){
	int choice;
	char repeat;
	do {
		printf("\n1. Create Directory\n");
		printf("2. Create File\n");
		printf("3. Delete File\n");
		printf("4. Search File\n");
		printf("5. Display All Files\n\n");

		printf("Enter your chioce: ");
		scanf("%d", &choice);
		
		printf("\n");
		
		switch(choice){
			case 1:
				createDirectory();
				break;
			case 2:
				createFile();
				break;
			case 3:
				deleteFile();
				break;
			case 4:
				searchFile();
				break;
			case 5:
				displayFiles();
				break;
			default:
				printf("\nInvalid input.\n");
				break;
		}
		printf("\nDo you wish to continue? (Y-yes, n-no): ");
		scanf(" %c", &repeat);
	} while (repeat=='Y' || repeat=='y');
}

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h> 
#include <string.h>

#define SIZE 20

char path[]="/home/csgautham/Programming/SSMP_LAB";
char dirpath[100];

char dir_name[SIZE];
char user_dir_name[SIZE];

char filename[SIZE], filepath[100];

struct stat st = {0};

void createDirectory(){
	int type, valid = 1;
	
	printf("Enter the directory name: ");
	scanf("%s", dir_name);
	
	printf("How do you want to create this direcory?\n");
	printf("1. As root directory\n");
	printf("2. Inside root directory\n");
	printf("Enter your choice: ");
	scanf("%d", &type);
	
	strcpy(dirpath, path);
	strcat(dirpath, "/");
	
	if(type==1){
		strcat(dirpath, dir_name);
		strcpy(user_dir_name, dir_name);
	}
	else if(type==2){
		strcat(dirpath, user_dir_name);
		strcat(dirpath, "/");
		strcat(dirpath, dir_name);
	}
	else {
		printf("Invalid choice.\n");
		valid = 0;
	}
	
	if(valid){
		if (stat(dirpath, &st) == -1) {
			mkdir(dirpath, 0777);
			printf("Directory created successfully.\n");
		} else
			printf("Directory already exists.\n");
	}
}

void createFile(){
	char filename[SIZE];
	char destination[100];
	char dest_dir[SIZE];
	int valid = 0;
	FILE *fptr;
	
	printf("Enter the file name: ");
	scanf("%s", filename);
	
	printf("Enter the directory name to which this new file is to be created: ");
	scanf("%s", dest_dir);
	
	strcpy(destination, path);
	
	strcat(destination, "/");
	strcat(destination, user_dir_name);
	strcat(destination, "/");
	
	if(strcmp(dest_dir, user_dir_name)==0){
		printf("Root Folder.\n");
		valid = 1;
	}
	else {
		printf("Sub-dir\n");
		strcat(destination, dest_dir);
		strcat(destination, "/");
	}
	
	printf("destination: %s\n", destination);
	
	if (stat(destination, &st) != -1)
		valid = 1;
	
	if(valid){
		strcat(destination, filename);
		if (stat(destination, &st) == -1) {
			fptr = fopen(destination, "w");
			fclose(fptr);
			printf("File created successfully.\n");
		} else
			printf("File already exists.\n");
	} else
		printf("Directory not found.");
}

int checkForValidity(int mode){
	char search_dir[SIZE];
	int valid = 0;
	
	if(mode == 0){
		printf("Enter the file name: ");
		scanf("%s", filename);
	}
	
	printf("Enter the directory name: ");
	scanf("%s", search_dir);
	
	strcpy(filepath, path);
	
	strcat(filepath, "/");
	strcat(filepath, user_dir_name);
	strcat(filepath, "/");
	
	if(strcmp(search_dir, user_dir_name)==0){
		printf("Root Folder.\n");
		valid = 1;
	}
	else {
		printf("Sub-dir\n");
		strcat(filepath, search_dir);
		strcat(filepath, "/");
	}
	
	printf("Deletion path: %s\n", filepath);
	
	if (stat(filepath, &st) != -1)
		valid = 1;
	
	
	return valid;
}

void deleteFile(){

	if(checkForValidity(0)){
		strcat(filepath, filename);
		if (remove(filepath) == 0)
        	printf("File deleted successfully.\n");
	    else
    	    printf("File to be deleted not found in the directory.\n");
	} else
		printf("Directory not found.");
	
}

void searchFile(){
	
	if(checkForValidity(0)){
		strcat(filepath, filename);
		if (stat(filepath, &st) != -1) 
			printf("File found in the directory.\n");
		else
			printf("File not found in the directory.\n");
	} else
		printf("Directory not found.");

}

void displayFiles(){

	if(checkForValidity(1)){
		DIR *dir;
		dir = opendir(filepath);
		struct dirent *entries;
		if(dir){
			while ((entries = readdir(dir)) != NULL)
		  		printf("%s\n", entries->d_name);
			closedir(dir);
	 	} else
	 		printf("Directory not selected.\n");
	 } else
	 	printf("Directory not found.\n");
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

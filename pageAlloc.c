#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#define MAX 20

struct process {
	char name[3];
	int size;
	int frames[MAX];
	double requiredFrames;
} proc[MAX];

static const struct process EmptyStruct;

int pnum=0;
int totalMemory, frameSize, totalFrames, availableFrames, fragmentation=0;
int frame[100];

void deleteProcess(int mode) {
    char pid[3];
    int i;

    printf("\nEnter the PID of the process to be deleted: ");
    scanf("%s", pid);

    for (i = 0; i < pnum; i++) {
        if (strcmp(proc[i].name, pid) == 0) {
            for (int j = 0; j < proc[i].requiredFrames; j++)
                frame[proc[i].frames[j]] = 0;
            availableFrames += proc[i].requiredFrames;
            fragmentation-=(frameSize-(proc[i].size%frameSize))%frameSize;
            proc[i] = EmptyStruct;
            printf("Process deleted.\n");
            break;
        }
    }

    if (i == pnum) {
        printf("Process not found.\n");
        if (mode == 1) {
            printf("Try again.\n");
            deleteProcess(1);
        }
    }
}


void insertProcess(){
	int current, fnum=0;
	
	printf("\nEnter the PID of the process: ");
	scanf("%s", proc[pnum].name);
	printf("Enter the size of the process: ");
	scanf("%d", &proc[pnum].size);
	
	proc[pnum].requiredFrames = ceil((double)proc[pnum].size / (double)frameSize);
	
	while (proc[pnum].requiredFrames > availableFrames) {
        printf("\nThere are not enough frames. Delete an existing process.\n");
        deleteProcess(1);
    }
	
	for(int i=0; i<proc[pnum].requiredFrames; ){
		current = rand() % totalFrames;
		if(frame[current]!=1){
			proc[pnum].frames[fnum++]=current;
			frame[current]=1;
			i++;
		}
	}
	
	fragmentation+=(frameSize-(proc[pnum].size%frameSize))%frameSize;
	
	availableFrames-=proc[pnum].requiredFrames;
	pnum++;
}



void display(){
	printf("\nPID\tSize\t\tFrames\n");
	for(int i=0; i<pnum; i++){
		if(proc[i].size!=0){
			printf("%s", proc[i].name);
			printf("\t%d\t\t", proc[i].size);
			for(int j=0; j<proc[i].requiredFrames; j++)
				printf("%d ", proc[i].frames[j]);
			printf("\n");
		}
	}
	printf("\nTotal fragmentation: %d\n", fragmentation);
}

void main(){
	int option;
	char choice;
	
	printf("\nEnter the total memory: ");
	scanf("%d", &totalMemory);
	printf("Enter the frame size: ");
	scanf("%d", &frameSize);
	
	totalFrames = availableFrames = totalMemory/frameSize;
	
	
	do {
		printf("\n1. Inserta process\n2. Delete a process\n3. Display the page allocation table\n\n");
		
		printf("Enter your choice: ");
		scanf("%d", &option);
		
		switch(option){
			case 1:
				insertProcess();
				break;
			case 2:
				deleteProcess(0);
				break;
			case 3:
				display();
				break;
			default:
				printf("Invalid case.\n");
				break;
		}
		
		printf("\nDo you wish to continue? (Y:yes, n:no): ");
		scanf(" %c", &choice);
	} while(choice=='Y' || choice=='y');
}

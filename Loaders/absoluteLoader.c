#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct memory {
	char addr[25];
	char code[25];
} m[50];

void main() {
	FILE *object = fopen("al_input.txt","r");
	int ptr = 0;
	char RECORD[50], PROG[25], STARTADDR[25], LENGTH[25] = "", LOCCTR[25] = "", OBJECT[5][25] = {"","","","",""}, SIZE[25] = "";
	
	while(feof(object)!=1){
		fscanf(object, "%s", RECORD);

		if(RECORD[0] == 'H') {
			int count = 0; 
			for(int i = 2; RECORD[i] != '\0'; i++) {
				char str[2] = "\0"; 
				str[0] = RECORD[i];
				if(RECORD[i] != '^') {
					if(count == 0) strcat(PROG, str);
					else if(count == 1) strcat(STARTADDR, str);				
					else if(count == 2) strcat(LENGTH, str);
				} else {
					count++;
				}
			}	
			strcat(PROG, "\0");
			strcat(STARTADDR, "\0");
			strcat(LENGTH, "\0");
		
	
		} else if(RECORD[0] == 'T') {
			strcpy(LOCCTR, "");
			strcpy(SIZE, "");
			for(int i = 0; i < 5; i++) {
				strcpy(OBJECT[i], "");
			}
			int count = 0, i = 2;
			do {
				char str[2] = "\0"; 
				str[0] = RECORD[i];
				if(RECORD[i] != '^' && RECORD[i] != '\0') {
					if(count == 0) strcat(LOCCTR, str);
					else if(count == 1) strcat(SIZE, str);				
					else strcat(OBJECT[count - 2], str);
				} else {
					if (count >= 2) {
						sprintf(m[ptr].addr, "%x", (int)strtol(LOCCTR, NULL, 16));
						strcpy(m[ptr].code, OBJECT[count - 2]);
						sprintf(LOCCTR, "%x", (int)strtol(LOCCTR, NULL, 16) + 3);
						//printf("\naddr: %s code: %s", m[ptr].addr, m[ptr].code);
						ptr++;
					}
					count++;
				}
				i++;
			} while(RECORD[i] != '\0');
	
			strcpy(m[ptr].addr, LOCCTR);
			strcpy(m[ptr].code, OBJECT[count - 2]);
			sprintf(LOCCTR, "%x", (int)strtol(LOCCTR, NULL, 16) + 3);
			//printf("\naddr: %s code: %s\n", m[ptr].addr, m[ptr].code);
			ptr++;

		} else if(RECORD[0] == 'E') {
			break;
		}
		
	}
	printf("\n");
	for(int i = 0; i < ptr; i++) {
		printf("\n%s : %s",m[i].addr, m[i].code);
	}
	
	fclose(object);
	
	printf("\n");
}

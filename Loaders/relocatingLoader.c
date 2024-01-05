#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct memory {
	char addr[50];
	char code[50];
} m[50];

void main() {
	FILE *object = fopen("rl_input.txt","r");
	int ptr = 0;
	char RECORD[50], PROG[25], STARTADDR[25], LENGTH[25] = "", LOCCTR[25] = "",MASK[25] = "",MASKBIN[50] = "", OBJECT[50][50], SIZE[25] = "";
	
	for(int i = 0; i < 50; i++) {

		strcpy(OBJECT[i], "");
	}
	
	printf("Enter starting address in HEX : ");
	scanf("%s",STARTADDR);
	
	while(feof(object)!=1){
		fscanf(object, "%s", RECORD);
		if(RECORD[0] == 'H') {
			int count = 0; 
			for(int i = 2; RECORD[i] != '\0'; i++) {
				char str[2] = "\0"; 
				str[0] = RECORD[i];
				if(RECORD[i] != '^') {
					if(count == 0) strcat(PROG, str);
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
			int count = 0, i = 2;
			do {
				strcpy(OBJECT[i], "");
				char str[2] = "\0"; 
				str[0] = RECORD[i];
				if(RECORD[i] != '^' && RECORD[i] != '\0') {
					if(count == 0) strcat(LOCCTR, str);
					else if(count == 1) strcat(SIZE, str);				
					else if(count == 2) strcat(MASK, str);				
					else strcat(OBJECT[count - 3], str);
				} else {
					if(count == 2) {
					  int j = 0;
						while(MASK[j] != '\0') {
							switch(MASK[j]) {
								case '0': strcat(MASKBIN, "0000"); break;
								case '1': strcat(MASKBIN, "0001"); break;
								case '2': strcat(MASKBIN, "0010"); break;
								case '3': strcat(MASKBIN, "0011"); break;
								case '4': strcat(MASKBIN, "0100"); break;
								case '5': strcat(MASKBIN, "0101"); break;
								case '6': strcat(MASKBIN, "0110"); break;
								case '7': strcat(MASKBIN, "0111"); break;
								case '8': strcat(MASKBIN, "1000"); break;
								case '9': strcat(MASKBIN, "1001"); break;
								case 'A': strcat(MASKBIN, "1010"); break;
								case 'B': strcat(MASKBIN, "1011"); break;
								case 'C': strcat(MASKBIN, "1100"); break;
								case 'D': strcat(MASKBIN, "1101"); break;
								case 'E': strcat(MASKBIN, "1110"); break;
								case 'F': strcat(MASKBIN, "1111"); break;
							}	
							j++;
						}

					} else if (count >= 3) {
						sprintf(m[ptr].addr, "%x", (int)strtol(LOCCTR, NULL, 16) + (int)strtol(STARTADDR, NULL, 16));

						if(MASKBIN[count - 3] == '1') sprintf(m[ptr].code, "%x", (int)strtol(OBJECT[count - 3], NULL, 16) + (int)strtol(STARTADDR, NULL, 16));
						else strcpy(m[ptr].code, OBJECT[count - 3]);
						m[ptr].code[6] = '\0';
						OBJECT[count - 3][6] = '\0';

						sprintf(LOCCTR, "%x", (int)strtol(LOCCTR, NULL, 16) + 3);

						ptr++;
					}
					
					count++;
				}
				i++;
			} while(RECORD[i] != '\0');
			
	
			sprintf(m[ptr].addr, "%x", (int)strtol(LOCCTR, NULL, 16) + (int)strtol(STARTADDR, NULL, 16));

			if(MASKBIN[count - 3] == '1') sprintf(m[ptr].code, "%x", (int)strtol(OBJECT[count - 3], NULL, 16) + (int)strtol(STARTADDR, NULL, 16));
			else strcpy(m[ptr].code, OBJECT[count - 3]);

			sprintf(LOCCTR, "%x", (int)strtol(LOCCTR, NULL, 16) + (int)strtol(STARTADDR, NULL, 16) + 3);
			m[ptr].code[6] = '\0';
			OBJECT[count - 3][6] = '\0';
			//printf("\naddr: %s code: %s", m[ptr].addr, m[ptr].code);
			//printf("\n%s : %s",m[ptr].addr, m[ptr].code);
			ptr++;


			strcpy(MASK,"");

			strcpy(MASKBIN,"");

			strcpy(OBJECT[i],"");
	
		} else if(RECORD[0] == 'E') {
			break;
		}
		
	}
	printf("\n");
	for(int i = 0; i < ptr; i++) {
		m[i].code[6] = '\0';
		printf("\n%s : %s",m[i].addr, m[i].code);
	}
	
	fclose(object);
	
	printf("\n");
}

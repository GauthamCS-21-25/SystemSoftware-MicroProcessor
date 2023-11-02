#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>


char OPCODE[3][25], ADDRESS[3][25];
int count = 0;

int searchSYMTAB(char src_symbol[], int LOCCTR) {
	char flag = -1;
	char symbol[25], address[25];
	FILE *symtab = fopen("symtab.txt","r");
	
	while(feof(symtab)!=1){
		fscanf(symtab, "%s %s\n", symbol, address);		
		if(strcmp(src_symbol, symbol) == 0) {
			strcpy(ADDRESS[count], address);

			return (int)strtol(address, NULL, 16);
		}
	}
	return flag;
	fclose(symtab);
}

int searchOPTAB(char src_opcode[], int LOCCTR) {
	char flag = -1;
	char opcode[25], code[25];
	FILE *optab = fopen("optab.txt","r");
	
	while(feof(optab)!=1){
		fscanf(optab, "%s %s\n", opcode, code);		
		if(strcmp(opcode, src_opcode) == 0) {
			strcpy(OPCODE[count],code);

			return (int)strtol(code, NULL, 16);
		}
	}
	return flag;
	fclose(optab);
}

void main() {
	int LOCCTR, rsize, START, FIRST;

	FILE *intermediate = fopen("intermediate.txt","r");
	FILE *length = fopen("length.txt","r");
	
	FILE *output = fopen("output.txt","w");
	
	char address[25], size[25], label[25], src_opcode[25], operand[25], flength[25];
	fscanf(intermediate, "%s %s %s", label, src_opcode, operand);

	fscanf(length, "%s", flength);

	
	if (strcmp(src_opcode, "START") == 0) {
			fprintf(output, "H^%s^%s^%s\n", label, operand, flength);
	}
	
	LOCCTR = (int)strtol(operand, NULL, 16); 
	FIRST = (int)strtol(operand, NULL, 16); 
	while(feof(intermediate)!=1){
		fscanf(intermediate, "%s %s %s %s %s", address, size, label, src_opcode, operand);
		
		int opStatus = searchOPTAB(src_opcode, LOCCTR);

		
			
		if(count == 0) {
			START = LOCCTR;
			rsize = 0;
		}
			
		int symStatus = searchSYMTAB(operand, LOCCTR);


		if (strcmp(src_opcode, "END") == 0) {
				strcpy(OPCODE[count],"");
				strcpy(ADDRESS[count],"");
				fprintf(output, "T^%x^%x^%s%s^%s%s^%s%s\n", START, rsize, OPCODE[0],ADDRESS[0],OPCODE[1],ADDRESS[1],OPCODE[2],ADDRESS[2]);	
				break;
		} 

		if(opStatus != -1) rsize += 3;
		else if(strcmp(src_opcode, "WORD") == 0) {
			strcpy(OPCODE[count], operand);
			rsize += 3;
		} else if(strcmp(src_opcode, "RESW") == 0) {
			rsize += 3 * (int)strtol(operand, NULL, 16);
		}	else if(strcmp(src_opcode, "RESB") == 0) {
			rsize += (int)strtol(operand, NULL, 16);
		} else if(strcmp(src_opcode, "BYTE") == 0) {
			int loop = 2, i = 0;
   		char ascii[25];
   		for(int loop = 2; loop < strlen(operand) - 1; i+=2) {
      	sprintf((char*)(ascii + i), "%02X", operand[loop]);
      	loop += 1;
   		}
   		ascii[i++] = '\0';

   		strcat(OPCODE[count], ascii); 			
			rsize += strlen(operand) - 3;
		} else if(opStatus == -1 || symStatus == -1) {
			printf("\nERROR : Invalid Code");
			break;
		}

			
		if(count == 2) {
			fprintf(output, "T^%x^%x^%s%s^%s%s^%s%s\n", START, rsize, OPCODE[0],ADDRESS[0],OPCODE[1],ADDRESS[1],OPCODE[2],ADDRESS[2]);	
			LOCCTR += rsize;
			for(int i=0; i<3; i++){
				strcpy(OPCODE[i],"");
				strcpy(ADDRESS[i],"");
			}

		}
		
		if(strcmp(src_opcode, "END") == 0) break;
		
		count = (count + 1) % 3;
	}		
	
	fprintf(output, "E^%x",FIRST);	
		
	fclose(intermediate);
	fclose(output);
	fclose(length);
}

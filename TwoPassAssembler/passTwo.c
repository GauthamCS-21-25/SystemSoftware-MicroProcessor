#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

char OP[3][25], ADDR[3][25];
int count = 0;

int searchSYM(char sym1[], int LOCCTR) {
	char flag = -1;
	char sym[25], addr[25];
	FILE *symtab = fopen("symtab.txt","r");
	
	while(feof(symtab)!=1){
		fscanf(symtab, "%s %s\n", sym, addr);	
		if(strcmp(sym1, sym) == 0) {
			strcpy(ADDR[count], addr);
			return (int)strtol(addr, NULL, 16);
		}
	}
	return flag;
	fclose(symtab);
}

int searchOP(char opcode[], int LOCCTR) {
	char flag = -1;
	char op[25], code[25];
	FILE *optab = fopen("optab.txt","r");
	
	while(feof(optab)!=1){
		fscanf(optab, "%s %s\n", op, code);		
		if(strcmp(op, opcode) == 0) {
			strcpy(OP[count],code);
			return (int)strtol(code, NULL, 16);
		}
	}
	return flag;
	fclose(optab);
}

void main() {
	int LOCCTR, rsize, START, FIRST;

	// Input
	FILE *intermediate = fopen("intermediate.txt","r");
	FILE *length = fopen("length.txt","r");
	
	// Output
	FILE *object = fopen("object.txt","w");
	
	char address[25], size[25], label[25], opcode[25], operand[25], flength[25];
	fscanf(intermediate, "%s %s %s", label, opcode, operand);

	fscanf(length, "%s", flength);

	
	if (strcmp(opcode, "START") == 0) {
		fprintf(object, "H^%s^%s^%s\n", label, operand, flength);
	}
	
	LOCCTR = (int)strtol(operand, NULL, 16); 
	FIRST = (int)strtol(operand, NULL, 16); 
	while(feof(intermediate)!=1){
		fscanf(intermediate, "%s %s %s %s %s", address, size, label, opcode, operand);
		
		int opStatus = searchOP(opcode, LOCCTR);
			
		if(count == 0) {
			START = LOCCTR;
			rsize = 0;
		}
			
		int symStatus = searchSYM(operand, LOCCTR);

		if (strcmp(opcode, "END") == 0) {
				strcpy(OP[count],"");
				strcpy(ADDR[count],"");
				fprintf(object, "T^%x^%x^%s%s^%s%s^%s%s\n", START, rsize, OP[0],ADDR[0],OP[1],ADDR[1],OP[2],ADDR[2]);	
				break;
		} 

		if(opStatus != -1) rsize += 3; 
		else if(strcmp(opcode, "WORD") == 0) {
			strcpy(OP[count], operand);
			rsize += 3;
		} else if(strcmp(opcode, "RESW") == 0) {
			//rsize += 3 * (int)strtol(operand, NULL, 16);
		}	else if(strcmp(opcode, "RESB") == 0) {
			//rsize += (int)strtol(operand, NULL, 16);
		} else if(strcmp(opcode, "BYTE") == 0) {
			int loop = 2, i = 0;
   			char ascii[25];
   			for(int loop = 2; loop < strlen(operand) - 1; i+=2) {
      			sprintf((char*)(ascii + i), "%02X", operand[loop]);
      			loop += 1;
   			}
   			ascii[i++] = '\0';

   			strcat(OP[count], ascii); 			
			rsize += strlen(operand) - 3;
		} else if(opStatus == -1) {
			printf("\nERROR : Invalid Code");
			break;
		} else if(symStatus == -1) {
			printf("\nERROR : Invalid Symbol");
			break;
		} 

			
		if(count == 2) {
			fprintf(object, "T^%x^%x^%s%s^%s%s^%s%s\n", START, rsize, OP[0],ADDR[0],OP[1],ADDR[1],OP[2],ADDR[2]);	
			LOCCTR += rsize;
			strcpy(OP[0],"");
			strcpy(ADDR[0],"");
			strcpy(OP[1],"");
			strcpy(ADDR[1],"");
			strcpy(OP[2],"");
			strcpy(ADDR[2],"");
		}
		
		if(strcmp(opcode, "END") == 0) break;
		
		count = (count + 1) % 3;
	}		
	
	fprintf(object, "E^%x",FIRST);	
		
	fclose(intermediate);
	fclose(object);
	fclose(length);
}

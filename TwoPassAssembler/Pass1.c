#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int count = 0;

int updateSYMTAB(char in_symbol[], int LOCCTR) {
	int flag = 0;
	char symbol[25], address[25];
	FILE *symtab = fopen("symtab.txt","a+");
	if(count == 0) {
		fprintf(symtab, "%s %x\n", in_symbol, LOCCTR);
		count++;
		fclose(symtab);
		return -1;
	}	
	
	while(feof(symtab)!=1){
		fscanf(symtab, "%s %s\n", symbol, address);		
		if(strcmp(symbol, in_symbol) == 0) {
			printf("\n%s %x", in_symbol,  (int)strtol(address, NULL, 16));
			flag = 1;
			break;
		}
	}
	
	if (flag == 1) {
		fclose(symtab);
		return 0;
	} else if(flag == 0) {
		fprintf(symtab, "%s %x\n", in_symbol, LOCCTR);
		count++;
		fclose(symtab);
		return 1;	
	}
	
}

int searchOPTAB(char in_operator[], int LOCCTR) {
	int flag = 0;
	char operator[25], opcode[25];
	FILE *optab = fopen("optab.txt","r");
	
	while(feof(optab)!=1){
		fscanf(optab, "%s %s\n", operator, opcode);		
		if(strcmp(operator, in_operator) == 0) {
			flag = 1;
			return flag;
		}
	}
	return flag;
	fclose(optab);
}

void main() {
	int LOCCTR, size;
	char START[25];

	FILE *input = fopen("input.txt","r");
	
	FILE *intermediate = fopen("intermediate.txt","w");
	FILE *length = fopen("length.txt","w");
	
	char label[25], in_operator[25], operand[25];
	while(feof(input)!=1){
		fscanf(input, "%s\t\t%s\t\t%s", label, in_operator, operand);
		
		if(strcmp(in_operator, "START") == 0) {
			strcpy(START, operand);
			LOCCTR = (int)strtol(operand, NULL, 16); 

			fprintf(intermediate, "%s %s %x\n", label, in_operator, LOCCTR);
		} else {			
			int opStatus = searchOPTAB(in_operator, LOCCTR);
			
			
			if (strcmp(label, "$") != 0) {
				int symStatus = updateSYMTAB(label, LOCCTR);
				if(symStatus == 0) {
					printf("\nERROR : Duplicate Symbol\n");
					break;
				}
			} 
			
			if(opStatus == 1 || strcmp(in_operator, "END") == 0) 
				size = 3;
			else if(strcmp(in_operator, "WORD") == 0) 
				size = 3;
			else if(strcmp(in_operator, "RESW") == 0) 
				size = 3 * (int)strtol(operand, NULL, 16);
			else if(strcmp(in_operator, "RESB") == 0) 
				size = (int)strtol(operand, NULL, 16);
			else if(strcmp(in_operator, "BYTE") == 0) 
				size = strlen(operand) - 3;
			else if(opStatus == 0) {
				printf("\nERROR : Invalid opcode\n");
				break;
			} 
			
			fprintf(intermediate, "%x %d %s %s %s\n", LOCCTR, size, label, in_operator, operand);	
			
			if(strcmp(in_operator, "END") == 0)
				break;
			
			LOCCTR += size;
		}			
	}
	
	fprintf(length, "%x\n", LOCCTR - (int)strtol(START, NULL, 16) );
	
	fclose(input);
	fclose(intermediate);
	fclose(length);
}

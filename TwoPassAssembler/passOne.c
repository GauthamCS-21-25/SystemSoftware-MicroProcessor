#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Global variable to keep track of the number of symbols processed
int symbolCount = 0;

// Function to update the symbol table
int updateSymbolTable(char symbol[], int locationCounter) {
    int flag = 0;
    char sym1[25], address[25];
    
    // Open the symbol table file in append mode
    FILE *symbolTable = fopen("symtab.txt", "a+");

    // If it's the first symbol, directly add it to the symbol table
    if (symbolCount == 0) {
        fprintf(symbolTable, "%s %x\n", symbol, locationCounter);
        symbolCount++;
        fclose(symbolTable);
        return -1;
    }

    // Check for duplicate symbols in the symbol table
    while (feof(symbolTable) != 1) {
        fscanf(symbolTable, "%s %s\n", sym1, address);

        if (strcmp(sym1, symbol) == 0) {
            flag = 1;
            break;
        }
    }

    // If the symbol is found, return 0
    if (flag == 1) {
        fclose(symbolTable);
        return 0;
    }
    // If the symbol is not found, add it to the symbol table and return 1
    else if (flag == 0) {
        fprintf(symbolTable, "%s %x\n", symbol, locationCounter);
        fclose(symbolTable);
        symbolCount++;
        return 1;
    }
}

// Function to search for opcode in the opcode table
int searchOpcode(char opcode[], int locationCounter) {
    int flag = 0;
    char op[25], code[25];

    // Open the opcode table file in read mode
    FILE *opcodeTable = fopen("optab.txt", "r");

    // Search for the opcode in the opcode table
    while (feof(opcodeTable) != 1) {
        fscanf(opcodeTable, "%s %s\n", op, code);

        if (strcmp(op, opcode) == 0) {
            flag = 1;
            return flag;
        }
    }

    // If opcode is not found, return 0
    return flag;
    fclose(opcodeTable);
}

// Main function
void main() {
    int locationCounter, instructionSize;
    char programStart[25];

    // Open the input file in read mode
    FILE *inputFile = fopen("input.txt", "r");

    // Open intermediate and length files for writing
    FILE *intermediateFile = fopen("intermediate.txt", "w");
    FILE *lengthFile = fopen("length.txt", "w");

    char label[25], opcode[25], operand[25];

    // Process input file until the end
    while (feof(inputFile) != 1) {
        fscanf(inputFile, "%s\t\t%s\t\t%s", label, opcode, operand);

        // Check if opcode is START
        if (strcmp(opcode, "START") == 0) {
            strcpy(programStart, operand);
            locationCounter = (int) strtol(operand, NULL, 16);

            fprintf(intermediateFile, "%s %s %x\n", label, opcode, locationCounter);
        } else {
            // Search for opcode in the opcode table
            int opcodeStatus = searchOpcode(opcode, locationCounter);

            // Update symbol table if a label is present
            if (strcmp(label, "$") != 0) {
                int symbolStatus = updateSymbolTable(label, locationCounter);
                if (symbolStatus == 0) {
                    printf("\nERROR : Duplicate Symbol");
                    break;
                }
            }

            // Determine the instruction size based on opcode
            if (opcodeStatus == 1 || strcmp(opcode, "END") == 0) instructionSize = 3;
            else if (strcmp(opcode, "WORD") == 0) instructionSize = 3;
            else if (strcmp(opcode, "RESW") == 0) instructionSize = 3 * (int) strtol(operand, NULL, 16);
            else if (strcmp(opcode, "RESB") == 0) instructionSize = (int) strtol(operand, NULL, 16);
            else if (strcmp(opcode, "BYTE") == 0) instructionSize = strlen(operand) - 3;
            else if (opcodeStatus == 0) {
                printf("\nERROR : Invalid Code");
                break;
            }

            // Write the intermediate line to the intermediate file
            fprintf(intermediateFile, "%x %d %s %s %s\n", locationCounter, instructionSize, label, opcode, operand);

            // Check if opcode is END to exit the loop
            if (strcmp(opcode, "END") == 0) break;

            // Update the location counter
            locationCounter += instructionSize;
        }
    }

    // Write the final length to the length file
    fprintf(lengthFile, "%x\n", locationCounter - (int) strtol(programStart, NULL, 16));

    // Close the files
    fclose(inputFile);
    fclose(intermediateFile);
    fclose(lengthFile);
}


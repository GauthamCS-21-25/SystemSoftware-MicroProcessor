#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Arrays to store opcode and corresponding addresses for three instructions
char opcodeBuffer[3][25], addressBuffer[3][25];
int bufferIndex = 0; // Counter to keep track of the position in opcodeBuffer and addressBuffer arrays

// Function to search for a symbol in the symbol table
int searchSymbol(char symbol[], int locationCounter) {
    char notFound = -1;
    char sym[25], addr[25];
    FILE *symbolTable = fopen("symtab.txt", "r");

    while (fscanf(symbolTable, "%s %s", sym, addr) != EOF) {
        if (strcmp(symbol, sym) == 0) {
            // Store the address in the address buffer
            strcpy(addressBuffer[bufferIndex], addr);
            return (int)strtol(addr, NULL, 16);
        }
    }

    return notFound;
    fclose(symbolTable);
}

// Function to search for an opcode in the opcode table
int searchOpcode(char opcode[], int locationCounter) {
    char notFound = -1;
    char op[25], code[25];
    FILE *opcodeTable = fopen("optab.txt", "r");

    while (fscanf(opcodeTable, "%s %s", op, code) != EOF) {
        if (strcmp(op, opcode) == 0) {
            // Store the opcode in the opcode buffer
            strcpy(opcodeBuffer[bufferIndex], code);
            return (int)strtol(code, NULL, 16);
        }
    }

    return notFound;
    fclose(opcodeTable);
}

// Main function for Pass Two
void passTwoAssembler() {
    int locationCounter, recordSize, startAddress, programStartAddress;

    FILE *intermediateFile = fopen("intermediate.txt", "r");
    FILE *lengthFile = fopen("length.txt", "r");
    FILE *objectFile = fopen("object.txt", "w");

    char address[25], size[25], label[25], opcode[25], operand[25], programLength[25];
    fscanf(intermediateFile, "%s %s %s", label, opcode, operand);

    fscanf(lengthFile, "%s", programLength);

    if (strcmp(opcode, "START") == 0) {
        // Write the header record for the object file
        fprintf(objectFile, "H^%s^%s^%s\n", label, operand, programLength);
    }

    // Initialize location counter and program start address
    locationCounter = (int)strtol(operand, NULL, 16);
    programStartAddress = (int)strtol(operand, NULL, 16);

    while (fscanf(intermediateFile, "%s %s %s %s %s", address, size, label, opcode, operand) != EOF) {
        // Search for opcode and symbol
        int opcodeStatus = searchOpcode(opcode, locationCounter);
        int symbolStatus = searchSymbol(operand, locationCounter);

        // Initialize values when bufferIndex is 0
        if (bufferIndex == 0) {
            startAddress = locationCounter;
            recordSize = 0;
        }

        if (strcmp(opcode, "END") == 0) {
            // Write the last text record and end record
            strcpy(opcodeBuffer[bufferIndex], "");
            strcpy(addressBuffer[bufferIndex], "");
            fprintf(objectFile, "T^%x^%x^%s%s^%s%s^%s%s\n", startAddress, recordSize,
                    opcodeBuffer[0], addressBuffer[0], opcodeBuffer[1], addressBuffer[1], opcodeBuffer[2], addressBuffer[2]);
            break;
        }

        // Update record size based on the instruction type
        if (opcodeStatus != -1) {
            recordSize += 3;
        } else if (strcmp(opcode, "WORD") == 0) {
            // Store the operand in the opcode buffer
            strcpy(opcodeBuffer[bufferIndex], operand);
            recordSize += 3;
        } else if (strcmp(opcode, "RESW") == 0) {
            // Uncomment and modify as needed if RESW should affect recordSize
            // recordSize += 3 * strtol(operand, NULL, 16);
        } else if (strcmp(opcode, "RESB") == 0) {
            // Uncomment and modify as needed if RESB should affect recordSize
            // recordSize += strtol(operand, NULL, 16);
        } else if (strcmp(opcode, "BYTE") == 0) {
            // Convert ASCII characters to hexadecimal
            int loop = 2, i = 0;
            char ascii[25];
            for (int loop = 2; loop < strlen(operand) - 1; i += 2) {
                sprintf((char *)(ascii + i), "%02X", operand[loop]);
                loop += 1;
            }
            ascii[i++] = '\0';

            // Concatenate the converted ASCII to the current opcode value
            strcat(opcodeBuffer[bufferIndex], ascii);
            recordSize += strlen(operand) - 3;
        } else if (opcodeStatus == -1) {
            printf("\nERROR : Invalid Code");
            break;
        } else if (symbolStatus == -1) {
            printf("\nERROR : Invalid Symbol");
            break;
        }

        // Write a text record every three instructions
        if (bufferIndex == 2) {
            fprintf(objectFile, "T^%x^%x^%s%s^%s%s^%s%s\n", startAddress, recordSize,
                    opcodeBuffer[0], addressBuffer[0], opcodeBuffer[1], addressBuffer[1], opcodeBuffer[2], addressBuffer[2]);
            locationCounter += recordSize;

            // Reset opcodeBuffer and addressBuffer arrays
            strcpy(opcodeBuffer[0], "");
            strcpy(addressBuffer[0], "");
            strcpy(opcodeBuffer[1], "");
            strcpy(addressBuffer[1], "");
            strcpy(opcodeBuffer[2], "");
            strcpy(addressBuffer[2], "");
        }

        // Break if END is encountered
        if (strcmp(opcode, "END") == 0) break;

        // Increment bufferIndex in a circular manner
        bufferIndex = (bufferIndex + 1) % 3;
    }

    // Write the end record for the object file
    fprintf(objectFile, "E^%x", programStartAddress);

    // Close the files
    fclose(intermediateFile);
    fclose(objectFile);
    fclose(lengthFile);
}

int main() {
    // Execute the Pass Two Assembler
    passTwoAssembler();
    return 0;
}


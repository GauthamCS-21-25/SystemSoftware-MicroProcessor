#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define a structure to hold address and code information
struct MemoryEntry {
    char address[25];
    char code[25];
} memoryEntries[50];

// Function to process the object file and extract address and code
void processObjectFile(FILE *objectFile) {
    int memoryPointer = 0; // Pointer for the memory array
    char record[50], programName[25] = "", startAddress[25] = "", programLength[25] = "", currentAddress[25] = "", objectCode[5][25] = {"", "", "", "", ""}, recordSize[25] = "";

    // Read records from the object file
    while (feof(objectFile) != 1) {
        fscanf(objectFile, "%s", record);

        if (record[0] == 'H') {
            // Process header record
            int fieldCount = 0;
            for (int i = 2; record[i] != '\0'; i++) {
                char str[2] = "\0";
                str[0] = record[i];
                if (record[i] != '^') {
                    if (fieldCount == 0)
                        strcat(programName, str);
                    else if (fieldCount == 1)
                        strcat(startAddress, str);
                    else if (fieldCount == 2)
                        strcat(programLength, str);
                } else {
                    fieldCount++;
                }
            }
            strcat(programName, "\0");
            strcat(startAddress, "\0");
            strcat(programLength, "\0");
        } else if (record[0] == 'T') {
            // Process text record
            strcpy(currentAddress, "");
            strcpy(recordSize, "");
            for (int i = 0; i < 5; i++) {
                strcpy(objectCode[i], "");
            }
            int fieldCount = 0, i = 2;
            do {
                char str[2] = "\0";
                str[0] = record[i];
                if (record[i] != '^' && record[i] != '\0') {
                    if (fieldCount == 0)
                        strcat(currentAddress, str);
                    else if (fieldCount == 1)
                        strcat(recordSize, str);
                    else
                        strcat(objectCode[fieldCount - 2], str);
                } else {
                    if (fieldCount >= 2) {
                        // Store the memory entry
                        sprintf(memoryEntries[memoryPointer].address, "%x", (int)strtol(currentAddress, NULL, 16));
                        strcpy(memoryEntries[memoryPointer].code, objectCode[fieldCount - 2]);
                        sprintf(currentAddress, "%x", (int)strtol(currentAddress, NULL, 16) + 3);
                        memoryPointer++;
                    }
                    fieldCount++;
                }
                i++;
            } while (record[i] != '\0');

            // Store the last memory entry in the text record
            strcpy(memoryEntries[memoryPointer].address, currentAddress);
            strcpy(memoryEntries[memoryPointer].code, objectCode[fieldCount - 2]);
            sprintf(currentAddress, "%x", (int)strtol(currentAddress, NULL, 16) + 3);
            memoryPointer++;
        } else if (record[0] == 'E') {
            break; // End of the program
        }
    }

    // Print the memory entries
    printf("\nMemory Entries:\n");
    for (int i = 0; i < memoryPointer; i++) {
        printf("%s : %s\n", memoryEntries[i].address, memoryEntries[i].code);
    }
}

int main() {
    // Open the object file for reading
    FILE *objectFile = fopen("al_input.txt", "r");

    if (objectFile == NULL) {
        fprintf(stderr, "Error opening the object file.\n");
        return 1; // Exit with an error code
    }

    // Process the object file
    processObjectFile(objectFile);

    // Close the object file
    fclose(objectFile);

    return 0; // Exit successfully
}


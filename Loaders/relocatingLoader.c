#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure to represent memory entries
struct MemoryEntry {
    char address[50];
    char code[50];
};

// Function to convert hexadecimal characters to binary
void hexToBinary(char hexChar, char* binary) {
    switch (hexChar) {
        case '0': strcpy(binary, "0000"); break;
        case '1': strcpy(binary, "0001"); break;
        case '2': strcpy(binary, "0010"); break;
        case '3': strcpy(binary, "0011"); break;
        case '4': strcpy(binary, "0100"); break;
        case '5': strcpy(binary, "0101"); break;
        case '6': strcpy(binary, "0110"); break;
        case '7': strcpy(binary, "0111"); break;
        case '8': strcpy(binary, "1000"); break;
        case '9': strcpy(binary, "1001"); break;
        case 'A': strcpy(binary, "1010"); break;
        case 'B': strcpy(binary, "1011"); break;
        case 'C': strcpy(binary, "1100"); break;
        case 'D': strcpy(binary, "1101"); break;
        case 'E': strcpy(binary, "1110"); break;
        case 'F': strcpy(binary, "1111"); break;
    }
}

int main() {
    FILE *object = fopen("rl_input.txt", "r");
    int entryIndex = 0;
    
    char record[50], programName[25], startAddress[25], programLength[25] = "", locationCounter[25] = "";
    char mask[25] = "", maskBinary[50] = "", objectCode[50][50], size[25] = "";

    struct MemoryEntry memoryEntries[50];

    // Initialize objectCode array
    for (int i = 0; i < 50; i++) {
        strcpy(objectCode[i], "");
    }

    printf("Enter starting address in HEX : ");
    scanf("%s", startAddress);

    while (feof(object) != 1) {
        fscanf(object, "%s", record);

        if (record[0] == 'H') {
            int count = 0;

            for (int i = 2; record[i] != '\0'; i++) {
                char str[2] = "\0";
                str[0] = record[i];

                if (record[i] != '^') {
                    if (count == 0) strcat(programName, str);
                    else if (count == 2) strcat(programLength, str);
                } else {
                    count++;
                }
            }

            strcat(programName, "\0");
            strcat(startAddress, "\0");
            strcat(programLength, "\0");
        } else if (record[0] == 'T') {
            strcpy(locationCounter, "");
            strcpy(size, "");
            int count = 0, i = 2;

            do {
                strcpy(objectCode[i], "");
                char str[2] = "\0";
                str[0] = record[i];

                if (record[i] != '^' && record[i] != '\0') {
                    if (count == 0) strcat(locationCounter, str);
                    else if (count == 1) strcat(size, str);
                    else if (count == 2) strcat(mask, str);
                    else strcat(objectCode[count - 3], str);
                } else {
                    if (count == 2) {
                        int j = 0;

                        while (mask[j] != '\0') {
                            hexToBinary(mask[j], maskBinary + j * 4);
                            j++;
                        }
                    } else if (count >= 3) {
                        sprintf(memoryEntries[entryIndex].address, "%x", (int)strtol(locationCounter, NULL, 16) + (int)strtol(startAddress, NULL, 16));

                        if (maskBinary[count - 3] == '1') {
                            sprintf(memoryEntries[entryIndex].code, "%x", (int)strtol(objectCode[count - 3], NULL, 16) + (int)strtol(startAddress, NULL, 16));
                        } else {
                            strcpy(memoryEntries[entryIndex].code, objectCode[count - 3]);
                        }

                        memoryEntries[entryIndex].code[6] = '\0';
                        objectCode[count - 3][6] = '\0';

                        sprintf(locationCounter, "%x", (int)strtol(locationCounter, NULL, 16) + 3);

                        entryIndex++;
                    }

                    count++;
                }

                i++;
            } while (record[i] != '\0');

            sprintf(memoryEntries[entryIndex].address, "%x", (int)strtol(locationCounter, NULL, 16) + (int)strtol(startAddress, NULL, 16));

            if (maskBinary[count - 3] == '1') {
                sprintf(memoryEntries[entryIndex].code, "%x", (int)strtol(objectCode[count - 3], NULL, 16) + (int)strtol(startAddress, NULL, 16));
            } else {
                strcpy(memoryEntries[entryIndex].code, objectCode[count - 3]);
            }

            sprintf(locationCounter, "%x", (int)strtol(locationCounter, NULL, 16) + (int)strtol(startAddress, NULL, 16) + 3);
            memoryEntries[entryIndex].code[6] = '\0';
            objectCode[count - 3][6] = '\0';

            entryIndex++;

            strcpy(mask, "");
            strcpy(maskBinary, "");
            strcpy(objectCode[i], "");
        } else if (record[0] == 'E') {
            break;
        }
    }

    printf("\nMemory Entries:\n");

    for (int i = 0; i < entryIndex; i++) {
        memoryEntries[i].code[6] = '\0';
        printf("\n%s : %s", memoryEntries[i].address, memoryEntries[i].code);
    }

    fclose(object);

    printf("\n");

    return 0;
}


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Line {
    char label[10];
    char opcode[10];
    char operand[10];
};

int isOpcodeValid(char *opcode, FILE *fp2) {
    char code[10], mnemonic[10];
    rewind(fp2);

    while (fscanf(fp2, "%s\t%s", code, mnemonic) == 2) {
        if (strcmp(opcode, code) == 0) {
            return 1;
        }
    }
    return 0;
}

int main() {
    struct Line line;
    int start, length;
    unsigned int locctr;

    FILE *fp1, *fp2, *fp3, *fp4;

    fp1 = fopen("input.txt", "r");
    fp2 = fopen("optab.txt", "r");
    fp3 = fopen("sym.txt", "w");
    fp4 = fopen("op.txt", "w");

    if (fp1 == NULL || fp2 == NULL || fp3 == NULL || fp4 == NULL) {
        printf("file not found");
        return 1;
    }

    while (fscanf(fp1, "%s\t%s\t%s", line.label, line.opcode, line.operand) != EOF) {
        if (strcmp(line.opcode, "START") == 0) {
            start = (int)strtol(line.operand, NULL, 16);
            locctr = (unsigned int)start;
            fprintf(fp4, "\t%s\t%s\t%s\n", line.label, line.opcode, line.operand);
        }

        if (strcmp(line.opcode, "START") != 0) {
            fprintf(fp4, "%X\t%s\t%s\t%s\n", locctr, line.label, line.opcode, line.operand);

            if (strcmp(line.label, "**") != 0 && strcmp(line.opcode, "EQU") != 0) {
                fprintf(fp3, "%s\t%X\n", line.label, locctr);
            }

            if (strcmp(line.opcode, "EQU") == 0) {
                fprintf(fp3, "%s\t%s\n", line.label, line.operand);
            }

            if (isOpcodeValid(line.opcode, fp2))
                {
                locctr += 3;
            }
            else if (strcmp(line.opcode, "WORD") == 0)
                {
                locctr += 3;
            } else if (strcmp(line.opcode, "RESW") == 0)
            {
                locctr += 3 * atoi(line.operand);
            }
            else if (strcmp(line.opcode, "RESB") == 0)
                {
                locctr += atoi(line.operand);
            }
            else if (strcmp(line.opcode, "BYTE") == 0) {
                ++locctr;
            }
            else if (strcmp(line.opcode, "ORG") == 0)
                {
                locctr = (int)strtol(line.operand, NULL, 16);
                fprintf(fp4, "%X\t%s\t%s\t%s\n", locctr, line.label, line.opcode, line.operand);

            }
        }
        printf("%X\t%s\t%s\t%s\n", locctr, line.label, line.opcode, line.operand);
    }

    fclose(fp1);
    fclose(fp2);
    fclose(fp3);
    fclose(fp4);

    return 0;
}


//diya's code

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main() {
    FILE *inputFile, *symtabFile, *optabFile;
    char line[100], label[10], opcode[10], operand[10];
    int locationCounter = 0;
    int startAddress = 0; // Initialize the start address to 0

    // Open the input, symbol table, and opcode table files
    inputFile = fopen("Input.txt", "r");
    symtabFile = fopen("symtab1.txt", "w");
    optabFile = fopen("optab1.txt", "w");

    if (inputFile == NULL || symtabFile == NULL || optabFile == NULL) {
        printf("Error opening files.\n");
        return 1;
    }

    // Initialize an array to keep track of optab entries
    char optabEntries[100][10];
    int optabCount = 0;
    int cnt=0;
    int s;

    // Process the input file line by line
    while (fgets(line, sizeof(line), inputFile) != NULL) {
        // Tokenize the line into label, opcode, and operand
        sscanf(line, "%s %s %s", label, opcode, operand);

        // Check for a comment or empty line
        if (line[0] == '.' || line[0] == '\n') {
            continue;
        }

        if (strcmp(opcode, "START") == 0) {
            // Set the start address from the operand
            startAddress = strtol(operand, NULL, 16);
            s = startAddress;
            locationCounter = startAddress;
        } else if (locationCounter == 0) {
            // If START was not encountered, start from 0
            locationCounter = 0;
        }

        if (strcmp(label, "-") != 0) { // Check if the label is not "-"
            // Write label and location counter to symbol table
            if(strcmp(opcode,"EQU")==0)
            {
                cnt=strtol(operand,NULL,16);
                fprintf(symtabFile, "%s\t%04X\n", label,cnt);

            }
            else{
                fprintf(symtabFile, "%s\t%04X\n", label, locationCounter);
            }

        }

        // Check if the opcode is not in optabEntries
        int isOpcodeInOptab = 0;
        for (int i = 0; i < optabCount; i++) {
            if (strcmp(opcode, optabEntries[i]) == 0) {
                isOpcodeInOptab = 1;
                break;
            }
        }

        if (!isOpcodeInOptab) {
            // Write opcode and its value to the opcode table
            fprintf(optabFile, "%s\t%02X\n", opcode, locationCounter);
            strcpy(optabEntries[optabCount], opcode);
            optabCount++;
        }




        // Increment location counter based on the opcode
        if (strcmp(opcode, "WORD") == 0) {
            locationCounter += 3;
        } else if (strcmp(opcode, "RESW") == 0) {
            int reswValue = atoi(operand);
            locationCounter += 3 * reswValue;
        } else if (strcmp(opcode, "BYTE") == 0) {
            if (operand[0] == 'C') {
                locationCounter += strlen(operand) - 3; // Adjust for 'C' and quotes
            } else if (operand[0] == 'X') {
                locationCounter += (strlen(operand) - 3) / 2; // Adjust for 'X' and quotes
            }
        } else if (strcmp(opcode, "RESB") == 0) {
            int resbValue = atoi(operand);
            locationCounter += resbValue;
        } else if(strcmp(opcode,"EQU") != 0){
            locationCounter += 3; // Default increment
        }
        if(strcmp(opcode,"ORG")==0)
        {
           startAddress = strtol(operand, NULL, 16);
            locationCounter = startAddress;
        }

    }

    printf("\nThe length of the code is: %d",locationCounter-s);



    // Close the files
    fclose(inputFile);
    fclose(symtabFile);
    fclose(optabFile);

    return 0;
}


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

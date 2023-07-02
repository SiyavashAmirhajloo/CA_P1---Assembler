#include "assemble.h"

#pragma warning(disable : 4996)

int main(int argc, char** argv)
{
    FILE* assp, * machp, * fopen();
    struct symbolTable* pSymTab;
    int symTabLen;
    int i = 0, j = 0, found, nuInsts;
    struct instruction* currInst = (struct instruction*)malloc(sizeof(struct instruction));
    size_t lineSize = 100;
    char* line = (char*)malloc(72);
    char* token;
    char* instructions[] = { "add","sub","slt","or","nand","addi","slti","ori","lui","lw","sw","beq","jalr","j","halt" };
    int instCount = 0;
    char hexTable[16] = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F' };
    char lower[5];
    if (argc < 3) {
        printf("*****Please run this program as follows:\n");
        printf("*****%s assprog.as machprog.mc\n", argv[0]);
        printf("*****where assprog.as is your assembly program\n");
        printf("and machprog.mc will be your machine code.\n");
        exit(1);
    }
    if ((assp = fopen(argv[1], "r")) == NULL) {
        printf(" % s cannot be opened\n", argv[1]);
        exit(1);
    }
    if ((machp = fopen(argv[2], "w+")) == NULL) {
        printf("%s cannot be opened\n", argv[2]);
        exit(1);
    }
    nuInsts = findprogLen(assp);
    pSymTab = (struct symbolTable*)malloc(nuInsts * sizeof(struct symbolTable));
    for (i; i < nuInsts; i++)
        pSymTab[i].symbol = (char*)malloc(10);
    fillSymTab(pSymTab, assp);
    struct instruction* out_inst = (struct instruction*)malloc(nuInsts * sizeof(struct instruction));
    int index = 0;
    while (fgets(line, lineSize, assp) != NULL) {
        currInst->PC = instCount;
        instCount++;
        token = strtok(line, "\t ,\n");
        for (i = 0; i < nuInsts; i++) {
            if (strcmp(pSymTab[i].symbol, token) == 0) {
                token = strtok(NULL, "\t, \n");
                break;
            }
        }
        for (j = 0; j < 15; j++) {
            if (strcmp(instructions[j], token) == 0)
                break;
        }
        if (j == 15 && strcmp(".fill", token) != 0) {
            printf("Error: Invalid Instruction.\n");
            exit(1);
        }
        strcpy(currInst->mnemonic, token);
        currInst->instType = -1;
        if (strcmp(currInst->mnemonic, ".fill") == 0) {
            token = strtok(NULL, "\t, \n");
            if (token[0] <= '9' && token[0] >= '0' || token[0] == '-') {
                int temp;
                if (token[0] == '-') {
                    for (i = 1; token[i] != '\0'; i++)
                        token[i - 1] = token[i];
                    token[i - 1] = '\0';
                    temp = -1 * atoi(token);
                }
                else
                    temp = atoi(token);
                currInst->intInst = temp;
            }
            else {
                for (j = 0; j < nuInsts; j++) {
                    if (strcmp(pSymTab[j].symbol, token) == 0) {
                        currInst->intInst = pSymTab[j].value;
                        break;
                    }
                    if (j == nuInsts) {
                        printf("Error: Using undefined label.\n");
                        exit(1);
                    }
                }
            }
            int_to_hex_imm(currInst->inst, currInst->intInst, 9);
        }
        for (i = 0; i < 15; i++) {
            if (strcmp(currInst->mnemonic, instructions[i]) == 0)
                break;
        }
        if (i < 5)
            currInst->instType = 0;
        else if (i < 13)
            currInst->instType = 1;
        else if (i < 15)
            currInst->instType = 2;
        if(i<15)
            formInst(currInst, token, pSymTab, nuInsts);
        out_inst[index] = *currInst;
        index++;
    }        
    for (i = 0; i < nuInsts; i++) {
        printf("(address %d) --> Decimal of instruction: %d,\t(0X%s)\n", i, out_inst[i].intInst, out_inst[i].inst);
        if (fprintf(machp, "%d\n",out_inst[i].intInst) < 0) {
            fclose(machp);
            perror("Couldn't write to it");
            fprintf(stderr, "errno:  %d\n", errno);
            exit(1);
        }
    }
    fclose(assp);
    fclose(machp);
    exit(0);
}

int findprogLen(FILE* inputFile)
{
    int lineNu = 0;
    size_t lineSize = 100;
    char* line;
    line = (char*)malloc(72);
    while (fgets(line, lineSize, inputFile) != NULL)
        lineNu++;
    rewind(inputFile);
    free(line);
    return lineNu;
}

void fillSymTab(struct symbolTable* symT, FILE* inputFile)
{
    size_t lineSize = 100;
    char* line;
    int i = 0;
    char* token;
    line = (char*)malloc(72);
    while (fgets(line, lineSize, inputFile) != NULL) {
        if (line[0] != ' ' && line[0] != '\t') {
            token = strtok(line, "\t, \n");
            for (int j = 0; j < i; j++) {
                if (strcmp(symT[j].symbol, token) == 0) {
                    printf("Error: the label '%s' is defined before.", token);
                    exit(1);
                }
            }
            strcpy(symT[i].symbol, token);
            symT[i].value = i;
        }
        i++;
    }
    rewind(inputFile);
    free(line);
}

void formInst(struct instruction* in, char* token, struct symbolTable* symT, int n)
{
    in->inst[0] = '0';
    switch (in->instType)
    {
    case 0:
        token = strtok(NULL, "\t, \n");
        if (token[0] >= '0' && token[0] <= '9')
            in->rd = atoi(token);
        else {
            int i;
            for (i = 0; i < n; i++) {
                if (strcmp(symT[i].symbol, token) == 0) {
                    in->rd = symT[i].value;
                    break;
                }
            }
            if (i == n) {
                printf("Error: Using undefined label.\n");
                exit(1);
            }
        }
        token = strtok(NULL, "\t, \n");
        if (token[0] >= '0' && token[0] <= '9')
            in->rs = atoi(token);
        else {
            int i;
            for (i = 0; i < n; i++) {
                if (strcmp(symT[i].symbol, token) == 0) {
                    in->rs = symT[i].value;
                    break;
                }
            }
            if (i == n) {
                printf("Error: Using undefined label.\n");
                exit(1);
            }
        }
        token = strtok(NULL, "\t, \n");
        if (token[0] >= '0' && token[0] <= '9')
            in->rt = atoi(token);
        else {
            int i;
            for (i = 0; i < n; i++) {
                if (strcmp(symT[i].symbol, token) == 0) {
                    in->rt = symT[i].value;
                    break;
                }
            }
            if (i == n) {
                printf("Error: Using undefined label.\n");
                exit(1);
            }
        }
        if (strcmp(in->mnemonic, "add") == 0)
            in->inst[1] = '0';
        if (strcmp(in->mnemonic, "sub") == 0)
            in->inst[1] = '1';
        if (strcmp(in->mnemonic, "slt") == 0)
            in->inst[1] = '2';
        if (strcmp(in->mnemonic, "or") == 0)
            in->inst[1] = '3';
        if (strcmp(in->mnemonic, "nand") == 0)
            in->inst[1] = '4';
        sprintf(&in->inst[2], "%x", in->rs);
        sprintf(&in->inst[3], "%x", in->rt);
        sprintf(&in->inst[4], "%x", in->rd);
        in->inst[5] = '0';
        in->inst[6] = '0';
        in->inst[7] = '0';
        break;
    case 1:
        token = strtok(NULL, "\t, \n");
        if (token[0] >= '0' && token[0] <= '9')
            in->rt = atoi(token);
        else {
            int i;
            for (i = 0; i < n; i++) {
                if (strcmp(symT[i].symbol, token) == 0) {
                    in->rt = symT[i].value;
                    break;
                }
            }
            if (i == n) {
                printf("Error: Using undefined label.\n");
                exit(1);
            }
        }
        token = strtok(NULL, "\t, \n");
        if (strcmp(in->mnemonic, "lui") == 0) {
            if (token[0] >= '0' && token[0] <= '9' || token[0] == '-')
                in->imm = atoi(token);
            else {
                int i;
                for (i = 0; i < n; i++) {
                    if (strcmp(symT[i].symbol, token) == 0) {
                        in->imm = symT[i].value;
                        break;
                    }
                }
                if (i == n) {
                    printf("Error: Using undefined label.\n");
                    exit(1);
                }
            }
            in->rs = 0;
        }
        else {
            if (token[0] >= '0' && token[0] <= '9')
                in->rs = atoi(token);
            else {
                int i;
                for (i = 0; i < n; i++) {
                    if (strcmp(symT[i].symbol, token) == 0) {
                        in->rs = symT[i].value;
                        break;
                    }
                }
                if (i == n) {
                    printf("Error: Using undefined label.\n");
                    exit(1);
                }
            }
        }
        if (strcmp(in->mnemonic, "lui") != 0 && strcmp(in->mnemonic, "jalr") != 0) {
            token = strtok(NULL, "\t, \n");
            if (token[0] >= '0' && token[0] <= '9' || token[0] == '-')
                in->imm = atoi(token);
            else {
                int i;
                for (i = 0; i < n; i++) {
                    if (strcmp(symT[i].symbol, token) == 0) {
                        in->imm = symT[i].value;
                        break;
                    }
                }
                if (i == n) {
                    printf("Error: Using undefined label.\n");
                    exit(1);
                }
            }
        }
        if (strcmp(in->mnemonic, "jalr") == 0)
            in->imm = 0;
        if (strcmp(in->mnemonic, "beq") == 0) {
            in->imm = in->imm - in->PC - 1;
            in->inst[1] = 'B';
            sprintf(&in->inst[2], "%x", in->rt);
            sprintf(&in->inst[3], "%x", in->rs);
        }
        else {    
            sprintf(&in->inst[2], "%x", in->rs);
            sprintf(&in->inst[3], "%x", in->rt);
        }
        if (strcmp(in->mnemonic, "addi") == 0)
            in->inst[1] = '5';
        if (strcmp(in->mnemonic, "slti") == 0)
            in->inst[1] = '6';
        if (strcmp(in->mnemonic, "ori") == 0)
            in->inst[1] = '7';
        if (strcmp(in->mnemonic, "lui") == 0)
            in->inst[1] = '8';
        if (strcmp(in->mnemonic, "lw") == 0)
            in->inst[1] = '9';
        if (strcmp(in->mnemonic, "sw") == 0)
            in->inst[1] = 'A';            
        if (strcmp(in->mnemonic, "jalr") == 0)
            in->inst[1] = 'C';
        char imm_value[5];
        int_to_hex_imm(imm_value, in->imm, 5);
        if (imm_value[4] != '\0')
            imm_value[4] = '\0';
        int i;
        for (i = 0; imm_value[i] != '\0'; i++);
        int j;
        for (j = 4; j < 8 - i; j++) {
            in->inst[j] = '0';
        }
        for (j, i = 0; j < 8 && i < 4; j++, i++) {
            in->inst[j] = imm_value[i];
        }
        break;
    case 2:
        if (strcmp(in->mnemonic, "j") == 0) {
            in->inst[1] = 'D';
            token = strtok(NULL, "\t, \n");
            if (token[0] >= '0' && token[0] <= '9' || token[0] == '-')
                in->imm = atoi(token);
            else {
                int i;
                for (i = 0; i < n; i++) {
                    if (strcmp(symT[i].symbol, token) == 0) {
                        in->imm = symT[i].value;
                        break;
                    }
                }
                if (i == n) {
                    printf("Error: Using undefined label.\n");
                    exit(1);
                }
            }
            char imm_value[5];
            int_to_hex_imm(imm_value, in->imm, 5);
            int i = 0;
            while (imm_value[i] != '\0')
                i++;
            for (j = 4; j < 8-i; j++)
                in->inst[j] = '0';
            for (j,i=0; j < 8; j++,i++)
                in->inst[j] = imm_value[i];
        }
        if (strcmp(in->mnemonic, "halt") == 0) {
            in->inst[1] = 'E';
            for (int i = 4; i < 8; i++)
                in->inst[i] = '0';
        }
        in->inst[2] = '0';
        in->inst[3] = '0';
        break;
    default:
        break;
    }
    in->inst[8] = '\0';
    in->intInst = hex_to_int(in->inst);
}

int hex_to_int(char* hex)
{
    int result = 0;
    while (*hex != '\0') {
        if (*hex >= '0' && *hex <= '9')
            result = result * 16 + *hex - '0';
        else if (*hex >= 'a' && *hex <= 'f')
            result = result * 16 + *hex - 'a' + 10;
        else if (*hex >= 'A' && *hex <= 'F')
            result = result * 16 + *hex - 'A' + 10;
        hex++;
    }
    return result;
}

void int_to_hex_imm(char* lower, int a, int size)
{
    if (a >= 65536) {
        printf("Error: Offset must be smaller than 65536 bigger than -65537.\n");
        exit(1);
    }
    if (a < 0) {
        a += 65536;
        char temp[9];
        sprintf(temp, "%x", a);
        if (size == 9) {
            for (int i = 0; i < 4; i++)
                lower[i] = temp[0];
            for (int i = 4; i < 8; i++)
                lower[i] = temp[i - 4];
        }
        if (size == 5) {
            for (int i = 0; i < 4; i++)
                lower[i] = temp[i];
        }
    }
    else
        sprintf(lower, "%x", a);
}
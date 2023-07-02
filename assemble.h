#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NORTYPE 5
#define NOITYPE 8
#define NOJTYPE 2
#define RTYPE 0
#define ITYPE 1
#define JTYPE 2

struct symbolTable
{
	int value;
	char* symbol;
};

struct instruction
{
	size_t instType; // 0->r, 1->i, 2->j
	size_t intInst;  // decimal of inst
	char mnemonic[7];
	char inst[9];  // hex of inst
	int rs, rt, rd;
	int imm;
	int PC;
};

int findprogLen(FILE*);
void fillSymTab(struct symbolTable*, FILE*);
void formInst(struct instruction*, char*, struct symbolTable*, int);
int hex_to_int(char*);
void int_to_hex_imm(char*, int, int);
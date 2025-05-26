
#ifndef SIMULATER_H__
#define SIMULATER_H__

#include "common.h"
#include "semantic.h"
#include "inter.h"

struct Register{
	int state;//0表示空闲，1表示使用中
	char *name;
};

// struct Register _reg[32];


struct VarDesc
{
	int reg_num;
	pOperand op;
	struct VarDesc* next;
};


void s_code_generate(FILE* fp);

#endif
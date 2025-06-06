
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

typedef struct MemVarDesc {
    pOperand op;
    char* global_label;     // 全局标签名
    struct MemVarDesc* next;
} MemVarDesc;//生成溢出到内存的变量



void s_code_generate(FILE* fp);

#endif
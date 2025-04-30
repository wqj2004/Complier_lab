

#ifndef INTER_H
#define INTER_H

#include "common.h"

// Operand types
typedef enum {
    VARIABLE,    // Named variable
    CONSTANT,    // Immediate value
    TEMPORARY,   // Compiler-generated temporary
    ADDRESS,     // Memory address
    LABEL_OP     // Label operand
} OperandKind;

// Operand structure
typedef struct Operand_ {
    OperandKind kind;
    union {
        int var_no;        // For temporaries and labels
        char* name;        // For variables
        int value;         // For constants
    } u;
} Operand;

typedef struct Operand_* pOperand;

// Instruction types
typedef enum {
    LABEL=42,      // LABEL x:
    FUNCTION_IR_,   // FUNCTION f:
    ASSIGN,     // x := y
    ADD,        // x := y + z
    SUB,        // x := y - z
    MUL,        // x := y * z
    DIV,        // x := y / z
    ADDR,       // x := &y
    DEREF_L,    // *x := y
    DEREF_R,    // x := *y
    GOTO,       // GOTO x
    IF_GOTO,    // IF x [relop] y GOTO z
    RETURN,     // RETURN x
    DEC,        // DEC x [size]
    ARG,        // ARG x
    CALL,       // x := CALL f
    PARAM,      // PARAM x
    READ,       // READ x
    WRITE       // WRITE x
} InstructionKind;

// Instruction structure
typedef struct Instruction_ {
    InstructionKind kind;
    union {
        struct { pOperand result, op1; } assign;               // x := y
        struct { pOperand result, op1, op2; } binop;           // x := y + z
        struct { pOperand op; } singleop;                      // GOTO/ARG/RETURN/etc.
        struct { pOperand x, y; char* relop; pOperand z; } if_goto; // IF x [relop] y GOTO z
        struct { pOperand op; int size; } dec;                 // DEC x [size]
        struct { pOperand result, op; } call;                  // x := CALL f
    } u;
    struct Instruction_* prev;
    struct Instruction_* next;
} Instruction;

typedef struct Instruction_* pInstruction;

// Code list structure
typedef struct CodeList_ {
    pInstruction head;
    pInstruction tail;
} CodeList;

// Initialize intermediate code generator
void initInterCodeGen();

// Create operands
pOperand newVariable(char* name);
pOperand newConstant(int value);
pOperand newTemporary();
pOperand newLabel();

// Create instructions
pInstruction newInstruction(InstructionKind kind);
void appendInstruction(pInstruction instr);

// Code generation from AST
void translateProgram(Node* node);
void translateExtDefList(Node* node);
void translateExtDef(Node* node);
void translateFunDec(Node* node);
void translateCompSt(Node* node);
void translateDefList(Node* node);
void translateStmtList(Node* node);
void translateStmt(Node* node);
void translateExp(Node* node, pOperand place);
void translateCond(Node* node, pOperand label_true, pOperand label_false);
void translateArgs(Node* node);

// Output
void outputInterCode(FILE* out);

// Uitls
#endif
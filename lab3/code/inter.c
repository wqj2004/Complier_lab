


#include "inter.h"
#include "semantic.h"
#include "common.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Global variables
static CodeList code_list = {NULL, NULL};
static int temp_var_count = 0;
static int label_count = 0;

// Initialize the intermediate code generator
void initInterCodeGen() {
    code_list.head = NULL;
    code_list.tail = NULL;
    temp_var_count = 0;
    label_count = 0;
}

// Create a new variable operand
pOperand newVariable(char* name) {
    pOperand op = (pOperand)malloc(sizeof(Operand));
    op->kind = VARIABLE;
    op->u.name = name;
    return op;
}

// Create a new constant operand
pOperand newConstant(int value) {
    pOperand op = (pOperand)malloc(sizeof(Operand));
    op->kind = CONSTANT;
    op->u.value = value;
    return op;
}

// Create a new temporary variable operand
pOperand newTemporary() {
    pOperand op = (pOperand)malloc(sizeof(Operand));
    op->kind = TEMPORARY;
    op->u.var_no = temp_var_count++;
    return op;
}

// Create a new label operand
pOperand newLabel() {
    pOperand op = (pOperand)malloc(sizeof(Operand));
    op->kind = LABEL_OP;
    op->u.var_no = label_count++;
    return op;
}

// Create a new instruction
pInstruction newInstruction(InstructionKind kind) {
    pInstruction instr = (pInstruction)malloc(sizeof(Instruction));
    instr->kind = kind;
    instr->prev = NULL;
    instr->next = NULL;
    return instr;
}

// Append an instruction to the code list
void appendInstruction(pInstruction instr) {
    if (code_list.head == NULL) {
        code_list.head = instr;
        code_list.tail = instr;
    } else {
        instr->prev = code_list.tail;
        code_list.tail->next = instr;
        code_list.tail = instr;
    }
}

// Helper function to get string representation of operand
static char* operandToString(pOperand op) {
    char* str = (char*)malloc(50); // Allocate enough space
    switch (op->kind) {
        case VARIABLE:
            sprintf(str, "%s", op->u.name);
            break;
        case CONSTANT:
            sprintf(str, "#%d", op->u.value);
            break;
        case TEMPORARY:
            sprintf(str, "t%d", op->u.var_no);
            break;
        case ADDRESS:
            sprintf(str, "&t%d", op->u.var_no);
            break;
        case LABEL_OP:
            sprintf(str, "label%d", op->u.var_no);
            break;
        default:
            strcpy(str, "unknown");
    }
    return str;
}

// Output the intermediate code
void outputInterCode(FILE* out) {
    pInstruction curr = code_list.head;
    while (curr != NULL) {
        switch (curr->kind) {
            case LABEL:
                fprintf(out, "LABEL %s :\n", operandToString(curr->u.singleop.op));
                break;
            case FUNCTION_IR_:
                fprintf(out, "FUNCTION %s :\n", operandToString(curr->u.singleop.op));
                break;
            case ASSIGN:
                fprintf(out, "%s := %s\n", 
                        operandToString(curr->u.assign.result),
                        operandToString(curr->u.assign.op1));
                break;
            case ADD:
                fprintf(out, "%s := %s + %s\n", 
                        operandToString(curr->u.binop.result),
                        operandToString(curr->u.binop.op1),
                        operandToString(curr->u.binop.op2));
                break;
            case SUB:
                fprintf(out, "%s := %s - %s\n", 
                        operandToString(curr->u.binop.result),
                        operandToString(curr->u.binop.op1),
                        operandToString(curr->u.binop.op2));
                break;
            case MUL:
                fprintf(out, "%s := %s * %s\n", 
                        operandToString(curr->u.binop.result),
                        operandToString(curr->u.binop.op1),
                        operandToString(curr->u.binop.op2));
                break;
            case DIV:
                fprintf(out, "%s := %s / %s\n", 
                        operandToString(curr->u.binop.result),
                        operandToString(curr->u.binop.op1),
                        operandToString(curr->u.binop.op2));
                break;
            case ADDR:
                fprintf(out, "%s := &%s\n", 
                        operandToString(curr->u.assign.result),
                        operandToString(curr->u.assign.op1));
                break;
            case DEREF_L:
                fprintf(out, "*%s := %s\n", 
                        operandToString(curr->u.assign.result),
                        operandToString(curr->u.assign.op1));
                break;
            case DEREF_R:
                fprintf(out, "%s := *%s\n", 
                        operandToString(curr->u.assign.result),
                        operandToString(curr->u.assign.op1));
                break;
            case GOTO:
                fprintf(out, "GOTO %s\n", operandToString(curr->u.singleop.op));
                break;
            case IF_GOTO:
                fprintf(out, "IF %s %s %s GOTO %s\n",
                        operandToString(curr->u.if_goto.x),
                        curr->u.if_goto.relop,
                        operandToString(curr->u.if_goto.y),
                        operandToString(curr->u.if_goto.z));
                break;
            case RETURN:
                fprintf(out, "RETURN %s\n", operandToString(curr->u.singleop.op));
                break;
            case DEC:
                fprintf(out, "DEC %s %d\n", 
                        operandToString(curr->u.dec.op),
                        curr->u.dec.size);
                break;
            case ARG:
                fprintf(out, "ARG %s\n", operandToString(curr->u.singleop.op));
                break;
            case CALL:
                fprintf(out, "%s := CALL %s\n", 
                        operandToString(curr->u.call.result),
                        operandToString(curr->u.call.op));
                break;
            case PARAM:
                fprintf(out, "PARAM %s\n", operandToString(curr->u.singleop.op));
                break;
            case READ:
                fprintf(out, "READ %s\n", operandToString(curr->u.singleop.op));
                break;
            case WRITE:
                fprintf(out, "WRITE %s\n", operandToString(curr->u.singleop.op));
                break;
            default:
                fprintf(out, "Unknown instruction\n");
        }
        curr = curr->next;
    }
}

// Translate the entire program
void translateProgram(Node* node) {
    if (!node) return;
    Node* extDefList = find_node(node, "ExtDefList");
    if (extDefList)
        translateExtDefList(extDefList);
}

// Translate external definitions list
void translateExtDefList(Node* node) {
    if (!node) return;
    Node* extDef = node->firstchild;
    if (extDef)
        translateExtDef(extDef);
    
    Node* nextExtDefList = extDef->nextsib;
    if (nextExtDefList)
        translateExtDefList(nextExtDefList);
}

// Translate a single external definition
void translateExtDef(Node* node) {
    if (!node) return;
    
    Node* specifier = node->firstchild;
    Node* secondChild = specifier->nextsib;
    
    if (!strcmp_safe_(secondChild->name, "FunDec")) {
        // Function definition
        translateFunDec(secondChild);
        
        Node* compSt = secondChild->nextsib;
        if (!strcmp_safe_(compSt->name, "CompSt")) {
            translateCompSt(compSt);
        }
    }
    // Handle global variables if needed
}

// Translate function declaration
void translateFunDec(Node* node) {
    if (!node) return;
    
    // Create FUNCTION instruction
    pInstruction instr = newInstruction(FUNCTION);
    instr->u.singleop.op = newVariable(node->firstchild->val.id_val);
    appendInstruction(instr);
    
    // Handle parameters
    Node* varList = find_node(node, "VarList");
    if (varList) {
        Node* paramDec = varList->firstchild;
        while (paramDec) {
            // Create PARAM instruction for each parameter
            Node* varDec = paramDec->firstchild->nextsib; // Skip specifier
            Node* id = find_node(varDec, "ID");
            if (id) {
                pInstruction paramInstr = newInstruction(PARAM);
                paramInstr->u.singleop.op = newVariable(id->val.id_val);
                appendInstruction(paramInstr);
            }
            
            // Move to next parameter
            if (paramDec->nextsib && paramDec->nextsib->nextsib) {
                paramDec = paramDec->nextsib->nextsib->firstchild; // Skip COMMA
            } else {
                paramDec = NULL;
            }
        }
    }
}

// Translate compound statement
void translateCompSt(Node* node) {
    if (!node) return;
    
    // Find the DefList and StmtList nodes
    Node* defList = find_node(node, "DefList");
    if (defList)
        translateDefList(defList);
    
    Node* stmtList = find_node(node, "StmtList");
    if (stmtList)
        translateStmtList(stmtList);
}

// Translate definition list
void translateDefList(Node* node) {
    if (!node) return;
    Node* def = node->firstchild;
    
    while (def) {
        Node* decList = def->firstchild->nextsib;
        if (decList) {
            Node* dec = decList->firstchild;
            while (dec) {
                Node* varDec = dec->firstchild;
                
                // Check if this is an array declaration
                if (find_node(varDec, "LB")) {
                    // Handle array allocation (DEC instruction)
                    Node* idNode = find_node(varDec, "ID");
                    Node* sizeNode = find_node(varDec, "INT");
                    if (idNode && sizeNode) {
                        int size = sizeNode->val.int_val * 4; // 4 bytes per element
                        
                        pInstruction decInstr = newInstruction(DEC);
                        decInstr->u.dec.op = newVariable(idNode->val.id_val);
                        decInstr->u.dec.size = size;
                        appendInstruction(decInstr);
                    }
                }
                
                // Check if there's an initializer
                if (dec->firstchild->nextsib && !strcmp_safe_(dec->firstchild->nextsib->name, "ASSIGNOP")) {
                    Node* exp = dec->firstchild->nextsib->nextsib;
                    Node* idNode = find_node(varDec, "ID");
                    if (idNode && exp) {
                        pOperand varOp = newVariable(idNode->val.id_val);
                        translateExp(exp, varOp);
                    }
                }
                
                // Move to next declaration
                if (dec->nextsib && !strcmp_safe_(dec->nextsib->name, "COMMA")) {
                    dec = dec->nextsib->nextsib;
                } else {
                    break;
                }
            }
        }
        
        def = def->nextsib;
    }
}

// Translate statement list
void translateStmtList(Node* node) {
    if (!node) return;
    
    Node* stmt = node->firstchild;
    while (stmt) {
        translateStmt(stmt);
        stmt = stmt->nextsib;
    }
}

// Translate a statement
void translateStmt(Node* node) {
    if (!node || !node->firstchild) return;
    
    if (!strcmp_safe_(node->firstchild->name, "Exp")) {
        // Expression statement
        translateExp(node->firstchild, NULL);
    }
    else if (!strcmp_safe_(node->firstchild->name, "CompSt")) {
        // Compound statement
        translateCompSt(node->firstchild);
    }
    else if (!strcmp_safe_(node->firstchild->name, "RETURN")) {
        // Return statement
        Node* exp = node->firstchild->nextsib;
        pOperand place = newTemporary();
        translateExp(exp, place);
        
        pInstruction returnInstr = newInstruction(RETURN);
        returnInstr->u.singleop.op = place;
        appendInstruction(returnInstr);
    }
    else if (!strcmp_safe_(node->firstchild->name, "IF")) {
        // If statement
        pOperand label_true = newLabel();
        pOperand label_false = newLabel();
        pOperand label_end = newLabel();
        
        // Find the condition expression
        Node* exp = node->firstchild->nextsib->nextsib; // Skip IF LP
        
        // Generate condition code
        translateCond(exp, label_true, label_false);
        
        // True branch
        pInstruction labelTrueInstr = newInstruction(LABEL);
        labelTrueInstr->u.singleop.op = label_true;
        appendInstruction(labelTrueInstr);
        
        // Find and translate the "then" statement
        Node* stmt1 = exp->nextsib->nextsib; // Skip RP
        translateStmt(stmt1);
        
        // Generate jump to end
        pInstruction gotoEndInstr = newInstruction(GOTO);
        gotoEndInstr->u.singleop.op = label_end;
        appendInstruction(gotoEndInstr);
        
        // False branch
        pInstruction labelFalseInstr = newInstruction(LABEL);
        labelFalseInstr->u.singleop.op = label_false;
        appendInstruction(labelFalseInstr);
        
        // Check if there's an "else" branch
        Node* elseNode = stmt1->nextsib;
        if (elseNode && !strcmp_safe_(elseNode->name, "ELSE")) {
            Node* stmt2 = elseNode->nextsib;
            translateStmt(stmt2);
        }
        
        // End label
        pInstruction labelEndInstr = newInstruction(LABEL);
        labelEndInstr->u.singleop.op = label_end;
        appendInstruction(labelEndInstr);
    }
    else if (!strcmp_safe_(node->firstchild->name, "WHILE")) {
        pOperand label_start = newLabel();
        pOperand label_true = newLabel();
        pOperand label_false = newLabel();
        
        // Start label
        pInstruction labelStartInstr = newInstruction(LABEL);
        labelStartInstr->u.singleop.op = label_start;
        appendInstruction(labelStartInstr);
        
        // Translate condition
        Node* exp = node->firstchild->nextsib->nextsib; // Skip WHILE LP
        translateCond(exp, label_true, label_false);
        
        // True branch
        pInstruction labelTrueInstr = newInstruction(LABEL);
        labelTrueInstr->u.singleop.op = label_true;
        appendInstruction(labelTrueInstr);
        
        // Translate loop body
        Node* stmt = exp->nextsib->nextsib; // Skip RP
        translateStmt(stmt);
        
        // Jump back to condition
        pInstruction gotoStartInstr = newInstruction(GOTO);
        gotoStartInstr->u.singleop.op = label_start;
        appendInstruction(gotoStartInstr);
        
        // False branch (loop exit)
        pInstruction labelFalseInstr = newInstruction(LABEL);
        labelFalseInstr->u.singleop.op = label_false;
        appendInstruction(labelFalseInstr);
    }
}

// Translate expression
void translateExp(Node* node, pOperand place) {
    if (!node) return;
    
    if (!strcmp_safe_(node->firstchild->name, "INT")) {
        if (place) {
            pInstruction instr = newInstruction(ASSIGN);
            instr->u.assign.result = place;
            instr->u.assign.op1 = newConstant(node->firstchild->val.int_val);
            appendInstruction(instr);
        }
    }
    else if (!strcmp_safe_(node->firstchild->name, "ID") && node->firstchild->nextsib == NULL) {
        if (place) {
            pInstruction instr = newInstruction(ASSIGN);
            instr->u.assign.result = place;
            instr->u.assign.op1 = newVariable(node->firstchild->val.id_val);
            appendInstruction(instr);
        }
    }
    else if (!strcmp_safe_(node->firstchild->name, "Exp") && 
             node->firstchild->nextsib && 
             !strcmp_safe_(node->firstchild->nextsib->name, "ASSIGNOP")) {
        // Assignment expression: Exp ASSIGNOP Exp
        Node* left = node->firstchild;
        Node* right = left->nextsib->nextsib;
        
        // Get the variable name from left side
        Node* id = find_node(left, "ID");
        if (id) {
            pOperand leftOp = newVariable(id->val.id_val);
            
            // Translate the right expression
            pOperand rightOp = place ? place : newTemporary();
            translateExp(right, rightOp);
            
            // Assign right to left
            if (!place || (place && place->kind != VARIABLE) || 
                (place && place->kind == VARIABLE && strcmp_safe_(place->u.name, id->val.id_val))) {
                pInstruction instr = newInstruction(ASSIGN);
                instr->u.assign.result = leftOp;
                instr->u.assign.op1 = rightOp;
                appendInstruction(instr);
            }
        }
    }
    else if (!strcmp_safe_(node->firstchild->name, "Exp") && 
             node->firstchild->nextsib && 
             node->firstchild->nextsib->nextsib &&
             (!strcmp_safe_(node->firstchild->nextsib->name, "PLUS") || 
              !strcmp_safe_(node->firstchild->nextsib->name, "MINUS") ||
              !strcmp_safe_(node->firstchild->nextsib->name, "STAR") || 
              !strcmp_safe_(node->firstchild->nextsib->name, "DIV"))) {
        // Binary operation: Exp OP Exp
        if (place) {
            Node* left = node->firstchild;
            Node* op = left->nextsib;
            Node* right = op->nextsib;
            
            pOperand t1 = newTemporary();
            pOperand t2 = newTemporary();
            
            translateExp(left, t1);
            translateExp(right, t2);
            
            pInstruction instr;
            if (!strcmp_safe_(op->name, "PLUS"))
                instr = newInstruction(ADD);
            else if (!strcmp_safe_(op->name, "MINUS"))
                instr = newInstruction(SUB);
            else if (!strcmp_safe_(op->name, "STAR"))
                instr = newInstruction(MUL);
            else // DIV
                instr = newInstruction(DIV);
                
            instr->u.binop.result = place;
            instr->u.binop.op1 = t1;
            instr->u.binop.op2 = t2;
            appendInstruction(instr);
        }
    }
    else if (!strcmp_safe_(node->firstchild->name, "ID") && 
             node->firstchild->nextsib && 
             !strcmp_safe_(node->firstchild->nextsib->name, "LP")) {
        // Function call: ID LP RP or ID LP Args RP
        if (node->firstchild->nextsib->nextsib && 
            !strcmp_safe_(node->firstchild->nextsib->nextsib->name, "Args")) {
            // Function call with arguments
            Node* args = node->firstchild->nextsib->nextsib;
            translateArgs(args);
        }
        
        // Function call instruction
        pInstruction callInstr = newInstruction(CALL);
        callInstr->u.call.op = newVariable(node->firstchild->val.id_val);
        callInstr->u.call.result = place ? place : newTemporary();
        appendInstruction(callInstr);
    }
    char* func_name = node->firstchild->val.id_val;
    
    if (!strcmp_safe_(func_name, "read")) {
        // Handle read system call
        if (place) {
            pInstruction readInstr = newInstruction(READ);
            readInstr->u.singleop.op = place;
            appendInstruction(readInstr);
        }
    }
    else if (!strcmp_safe_(func_name, "write")) {
        // Handle write system call - should have one argument
        if (node->firstchild->nextsib->nextsib && 
            !strcmp_safe_(node->firstchild->nextsib->nextsib->name, "Args")) {
            Node* args = node->firstchild->nextsib->nextsib;
            Node* exp = args->firstchild;
            
            // Evaluate the expression to be written
            pOperand t = newTemporary();
            translateExp(exp, t);
            
            // Generate WRITE instruction
            pInstruction writeInstr = newInstruction(WRITE);
            writeInstr->u.singleop.op = t;
            appendInstruction(writeInstr);
        }
    }
    else {
        // Regular function call
        if (node->firstchild->nextsib->nextsib && 
            !strcmp_safe_(node->firstchild->nextsib->nextsib->name, "Args")) {
            // Function call with arguments
            Node* args = node->firstchild->nextsib->nextsib;
            translateArgs(args);
        }
        
        // Function call instruction
        pInstruction callInstr = newInstruction(CALL);
        callInstr->u.call.op = newVariable(func_name);
        callInstr->u.call.result = place ? place : newTemporary();
        appendInstruction(callInstr);
    }
    // Add more expression types as needed
}

// Translate arguments
void translateArgs(Node* node) {
    if (!node) return;
    
    // Current argument
    Node* exp = node->firstchild;
    pOperand t = newTemporary();
    translateExp(exp, t);
    
    // Process other arguments first (reverse order for stack)
    if (exp->nextsib && exp->nextsib->nextsib) {
        translateArgs(exp->nextsib->nextsib);
    }
    
    // Add current argument
    pInstruction argInstr = newInstruction(ARG);
    argInstr->u.singleop.op = t;
    appendInstruction(argInstr);
}

// Translate condition
void translateCond(Node* node, pOperand label_true, pOperand label_false) {
    if (!node) return;
    
    if (!strcmp_safe_(node->firstchild->name, "Exp") && 
        node->firstchild->nextsib && 
        node->firstchild->nextsib->nextsib &&
        !strcmp_safe_(node->firstchild->nextsib->name, "RELOP")) {
        // Relational operation: Exp RELOP Exp
        Node* exp1 = node->firstchild;
        Node* relop = exp1->nextsib;
        Node* exp2 = relop->nextsib;
        
        pOperand t1 = newTemporary();
        pOperand t2 = newTemporary();
        
        translateExp(exp1, t1);
        translateExp(exp2, t2);
        
        pInstruction ifGotoInstr = newInstruction(IF_GOTO);
        ifGotoInstr->u.if_goto.x = t1;
        ifGotoInstr->u.if_goto.y = t2;
        ifGotoInstr->u.if_goto.z = label_true;
        // Copy the relop from the node (==, !=, >, <, >=, <=)
        strncpy(ifGotoInstr->u.if_goto.relop, relop->val.id_val, 3);
        appendInstruction(ifGotoInstr);
        
        pInstruction gotoInstr = newInstruction(GOTO);
        gotoInstr->u.singleop.op = label_false;
        appendInstruction(gotoInstr);
    }
    else if (!strcmp_safe_(node->firstchild->name, "Exp") && 
             node->firstchild->nextsib && 
             node->firstchild->nextsib->nextsib &&
             !strcmp_safe_(node->firstchild->nextsib->name, "AND")) {
        // AND operation: Exp AND Exp
        pOperand label_mid = newLabel();
        
        translateCond(node->firstchild, label_mid, label_false);
        
        pInstruction labelMidInstr = newInstruction(LABEL);
        labelMidInstr->u.singleop.op = label_mid;
        appendInstruction(labelMidInstr);
        
        translateCond(node->firstchild->nextsib->nextsib, label_true, label_false);
    }
    else if (!strcmp_safe_(node->firstchild->name, "Exp") && 
             node->firstchild->nextsib && 
             node->firstchild->nextsib->nextsib &&
             !strcmp_safe_(node->firstchild->nextsib->name, "OR")) {
        // OR operation: Exp OR Exp
        pOperand label_mid = newLabel();
        
        translateCond(node->firstchild, label_true, label_mid);
        
        pInstruction labelMidInstr = newInstruction(LABEL);
        labelMidInstr->u.singleop.op = label_mid;
        appendInstruction(labelMidInstr);
        
        translateCond(node->firstchild->nextsib->nextsib, label_true, label_false);
    }
    else if (!strcmp_safe_(node->firstchild->name, "NOT") && node->firstchild->nextsib) {
        // NOT operation: NOT Exp
        translateCond(node->firstchild->nextsib, label_false, label_true);
    }
    else {
        // Other expressions treated as a boolean value
        pOperand t = newTemporary();
        translateExp(node, t);
        
        pOperand zero = newConstant(0);
        
        pInstruction ifGotoInstr = newInstruction(IF_GOTO);
        ifGotoInstr->u.if_goto.x = t;
        ifGotoInstr->u.if_goto.y = zero;
        ifGotoInstr->u.if_goto.z = label_true;
        strcpy(ifGotoInstr->u.if_goto.relop, "!=");
        appendInstruction(ifGotoInstr);
        
        pInstruction gotoInstr = newInstruction(GOTO);
        gotoInstr->u.singleop.op = label_false;
        appendInstruction(gotoInstr);
    }
}
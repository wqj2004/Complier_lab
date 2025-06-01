#include "inter.h"
#include "semantic.h"
#include "common.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Global variables
CodeList code_list = {NULL, NULL};
static int temp_var_count = 0;
static int label_count = 0;
static int var_count = 0;

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
            case FUNCTION:
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
                fprintf(out, "Unknown instruction -> %d\n", curr->kind);
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
    
    if (!strcmp_safe_(node->name, "ExtDefList")) {
        Node* extDef = node->firstchild;
        if (extDef)
            translateExtDefList(extDef);
        
        Node* nextExtDefList = node->nextsib;
        if (nextExtDefList)
            translateExtDefList(nextExtDefList);
    }else if(!strcmp_safe_(node->name, "ExtDef")){
        translateExtDef(node);
        
        Node* nextExtDefList = node->nextsib;
        if (nextExtDefList)
            translateExtDefList(nextExtDefList);
    }

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
        while (decList) {
            Node* dec = decList->firstchild;
            if (dec) {
                Node* varDec = dec->firstchild;
                Type varType = NULL;
                Node* idNode = find_node_recursive(varDec, "ID");
                if (idNode) {
                    //printf("initializing %s\n", idNode->val.id_val);
                    char *varName = idNode->val.id_val;
                    pobj varObj = searchtab(table, varName);
                    if (varObj) {
                        varType = varObj->type;
                        int size = getTypeSize(varType);

                        //DEC指令生成
                        if (size > 4) {
                            //printf("gen dec for %s\n", varName);
                            // Allocate memory for large variables
                            pInstruction decInstr = newInstruction(DEC);
                            decInstr->u.dec.op = newVariable(varName);
                            decInstr->u.dec.size = size;
                            appendInstruction(decInstr);
                        }
                    }
                }
                                
                // Check if there's an initializer
                if (dec->firstchild->nextsib && !strcmp_safe_(dec->firstchild->nextsib->name, "ASSIGNOP")) {
                    Node* exp = dec->firstchild->nextsib->nextsib;
                    Node* idNode = find_node(varDec, "ID");
                    //printf("initializer from %s to %s\n", exp->name, idNode->val.id_val);
                    if (idNode && exp) {
                        pOperand varOp = newVariable(idNode->val.id_val);
                        translateExp(exp, varOp);
                    }
                }
                // Simple variable declaration without initialization
                else if (!find_node(varDec, "LB")) {
                    // We don't need to generate any IR for simple variables
                    // They are handled implicitly in memory allocation
                    Node* idNode = find_node(varDec, "ID");
                    if (idNode) {
                        // You could add debugging here if needed
                        // printf("Simple variable declaration: %s\n", idNode->val.id_val);
                    }
                }
                
                // Move to next declaration
                if (dec->nextsib && !strcmp_safe_(dec->nextsib->name, "COMMA")) {
                    decList = dec->nextsib->nextsib;
                } else {
                    break;
                }
            }
        }
        
        if(def->nextsib && strcmp_safe_(def->nextsib->name, "DefList") == 0) {
            def = def->nextsib->firstchild; // Move to the first child of DefList
        } else {
            break; // No more definitions
        }
    }
}

// Helper function to calculate array size considering multi-dimensional arrays
int calculateArraySize(Node* varDec) {
    int totalSize = 1;
    Node* current = varDec;
    
    // Navigate through all dimensions
    while (find_node(current, "LB")) {
        Node* sizeNode = find_node(current, "INT");
        if (sizeNode) {
            totalSize *= sizeNode->val.int_val;
        } else {
            return 0; // Error: array dimension not specified
        }
        
        // For multi-dimensional arrays, the LB and INT nodes
        // are inside the nested VarDec nodes
        current = current->firstchild;
    }
    
    // Multiply by 4 (assuming each element is 4 bytes)
    return totalSize * 4;
}

// Translate statement list
void translateStmtList(Node* node) {
    if (!node) return;
    
    Node* stmt = node->firstchild;
    while (stmt) {
        if (!strcmp_safe_(stmt->name, "StmtList")) {
            translateStmtList(stmt);
        } else {
            translateStmt(stmt);
        }
        stmt = stmt->nextsib;
    }
}

// Translate a statement
void translateStmt(Node* node) {
    if (!node || !node->firstchild) return;
    
    // 表达式语句
    if (!strcmp_safe_(node->firstchild->name, "Exp")) {
        translateExp(node->firstchild, NULL);
    }
    // 复合语句
    else if (!strcmp_safe_(node->firstchild->name, "CompSt")) {
        translateCompSt(node->firstchild);
    }
    // 返回语句
    else if (!strcmp_safe_(node->firstchild->name, "RETURN")) {
        Node* exp = node->firstchild->nextsib;
        pOperand place = newTemporary();
        translateExp(exp, place);
        
        pInstruction returnInstr = newInstruction(RETURN);
        returnInstr->u.singleop.op = place;
        appendInstruction(returnInstr);
    }
    // IF语句
    else if (!strcmp_safe_(node->firstchild->name, "IF")) {
        pOperand label_true = newLabel();
        pOperand label_false = newLabel();
        pOperand label_end = newLabel();
        
        // 处理条件
        Node* exp = node->firstchild->nextsib->nextsib; // Skip IF LP
        translateCond(exp, label_true, label_false);
        
        // True分支
        pInstruction labelTrueInstr = newInstruction(LABEL);
        labelTrueInstr->u.singleop.op = label_true;
        appendInstruction(labelTrueInstr);
        
        // 翻译then语句
        Node* stmt1 = exp->nextsib->nextsib; // Skip RP
        translateStmt(stmt1);
        
        // 跳转到结束
        pInstruction gotoEndInstr = newInstruction(GOTO);
        gotoEndInstr->u.singleop.op = label_end;
        appendInstruction(gotoEndInstr);
        
        // False分支
        pInstruction labelFalseInstr = newInstruction(LABEL);
        labelFalseInstr->u.singleop.op = label_false;
        appendInstruction(labelFalseInstr);
        
        // 检查是否有else分支
        Node* elseNode = stmt1->nextsib;
        if (elseNode && !strcmp_safe_(elseNode->name, "ELSE")) {
            Node* stmt2 = elseNode->nextsib;
            translateStmt(stmt2); // 这会递归处理嵌套的if-else
        }
        
        // 结束标签
        pInstruction labelEndInstr = newInstruction(LABEL);
        labelEndInstr->u.singleop.op = label_end;
        appendInstruction(labelEndInstr);
    }
    // WHILE语句
    else if (!strcmp_safe_(node->firstchild->name, "WHILE")) {
        pOperand label_start = newLabel();
        pOperand label_true = newLabel();
        pOperand label_false = newLabel();
        
        // 开始标签
        pInstruction labelStartInstr = newInstruction(LABEL);
        labelStartInstr->u.singleop.op = label_start;
        appendInstruction(labelStartInstr);
        
        // 处理循环条件
        Node* exp = node->firstchild->nextsib->nextsib; // Skip WHILE LP
        translateCond(exp, label_true, label_false);
        
        // 循环体
        pInstruction labelTrueInstr = newInstruction(LABEL);
        labelTrueInstr->u.singleop.op = label_true;
        appendInstruction(labelTrueInstr);
        
        // 翻译循环体
        Node* stmt = exp->nextsib->nextsib; // Skip RP
        translateStmt(stmt);
        
        // 跳回条件检查
        pInstruction gotoStartInstr = newInstruction(GOTO);
        gotoStartInstr->u.singleop.op = label_start;
        appendInstruction(gotoStartInstr);
        
        // 循环结束标签
        pInstruction labelFalseInstr = newInstruction(LABEL);
        labelFalseInstr->u.singleop.op = label_false;
        appendInstruction(labelFalseInstr);
    }
}

// Translate expression
void translateExp(Node* node, pOperand place) {
    if (!node) return;
    // INT常量
    if (!strcmp_safe_(node->firstchild->name, "INT")) {
        if (place) {
            pInstruction instr = newInstruction(ASSIGN);
            instr->u.assign.result = place;
            instr->u.assign.op1 = newConstant(node->firstchild->val.int_val);
            appendInstruction(instr);
        }
    }
    // FLOAT常量
    else if (!strcmp_safe_(node->firstchild->name, "FLOAT")) {
        if (place) {
            pInstruction instr = newInstruction(ASSIGN);
            instr->u.assign.result = place;
            instr->u.assign.op1 = newConstant(node->firstchild->val.float_val);
            appendInstruction(instr);
        }
    }
    // 变量引用
    else if (!strcmp_safe_(node->firstchild->name, "ID") && node->firstchild->nextsib == NULL) {
        if (place) {
            pInstruction instr = newInstruction(ASSIGN);
            instr->u.assign.result = place;
            instr->u.assign.op1 = newVariable(node->firstchild->val.id_val);
            appendInstruction(instr);
        }
    }
    // 赋值表达式
    else if (!strcmp_safe_(node->firstchild->name, "Exp") && 
             node->firstchild->nextsib && 
             !strcmp_safe_(node->firstchild->nextsib->name, "ASSIGNOP")) {
        Node* left = node->firstchild;
        Node* right = left->nextsib->nextsib;
        
        // 处理左值
        if (left->firstchild && !strcmp_safe_(left->firstchild->name, "ID")) {
            // 简单变量赋值
            pOperand leftOp = newVariable(left->firstchild->val.id_val);
            
            // 翻译右边表达式
            pOperand rightOp = place ? place : newTemporary();
            translateExp(right, rightOp);
            
            // 生成赋值指令
            if (!place || (place && place->kind != VARIABLE) || 
                (place && place->kind == VARIABLE && strcmp_safe_(place->u.name, left->firstchild->val.id_val))) {
                pInstruction instr = newInstruction(ASSIGN);
                instr->u.assign.result = leftOp;
                instr->u.assign.op1 = rightOp;
                appendInstruction(instr);
            }
        } 
        else if (left->firstchild && !strcmp_safe_(left->firstchild->name, "Exp") && 
                 left->firstchild->nextsib && !strcmp_safe_(left->firstchild->nextsib->name, "LB")) {
            // 数组元素赋值: Exp[Exp] = Exp
            pOperand array = newTemporary();
            translateExp(left->firstchild, array);
            
            pOperand index = newTemporary();
            translateExp(left->firstchild->nextsib->nextsib, index);
            
            // 计算地址
            pOperand offset = newTemporary();
            pInstruction mulInstr = newInstruction(MUL);
            mulInstr->u.binop.result = offset;
            mulInstr->u.binop.op1 = index;
            mulInstr->u.binop.op2 = newConstant(4); // 假设每个元素4字节
            appendInstruction(mulInstr);
            
            pOperand addr = newTemporary();
            pInstruction addInstr = newInstruction(ADD);
            addInstr->u.binop.result = addr;
            addInstr->u.binop.op1 = array;
            addInstr->u.binop.op2 = offset;
            appendInstruction(addInstr);
            
            // 右值
            pOperand rightVal = newTemporary();
            translateExp(right, rightVal);
            
            // 间接赋值
            pInstruction assignInstr = newInstruction(DEREF_L);
            assignInstr->u.assign.result = addr;
            assignInstr->u.assign.op1 = rightVal;
            appendInstruction(assignInstr);
            
            if (place) {
                pInstruction copyInstr = newInstruction(ASSIGN);
                copyInstr->u.assign.result = place;
                copyInstr->u.assign.op1 = rightVal;
                appendInstruction(copyInstr);
            }
        }
        else if (left->firstchild && !strcmp_safe_(left->firstchild->name, "Exp") && 
                 left->firstchild->nextsib && !strcmp_safe_(left->firstchild->nextsib->name, "DOT")) {
            // 结构体字段赋值: Exp.ID = Exp
            // 结构体访问需要计算字段偏移
            pOperand structAddr = newTemporary();
            translateExp(left->firstchild, structAddr);
            
            // 获取字段名和结构体类型
            Node* fieldNode = left->firstchild->nextsib->nextsib; // 获取ID节点
            
            // Check if it's really an ID node
            if (!fieldNode || strcmp_safe_(fieldNode->name, "ID") != 0) {
                //printf("Error: Expected ID node for struct field, got %s\n", 
                    //fieldNode ? fieldNode->name : "NULL");
                return;
            }
            
            char* fieldName = fieldNode->val.id_val;
            //printf("Field name in assignment: %s\n", fieldName); // Debug print
            
            // 通过符号表查找表达式的类型
            Type structType = NULL;
            if (left->firstchild->firstchild && !strcmp_safe_(left->firstchild->firstchild->name, "ID")) {
                char* structVarName = left->firstchild->firstchild->val.id_val;
                //printf("Find struct variable: %s\n", structVarName);
                pobj structObj = searchtab(table, structVarName);
                if (structObj) {
                    structType = structObj->type;
                }
            }
            
            // 计算字段偏移
            int offset = getStructFieldOffset(structType, fieldName);
            //printf("Field offset: %d\n", offset);
            
            // 计算字段地址
            pOperand fieldAddr = newTemporary();
            pInstruction addInstr = newInstruction(ADD);
            addInstr->u.binop.result = fieldAddr;
            addInstr->u.binop.op1 = structAddr;
            addInstr->u.binop.op2 = newConstant(offset);
            appendInstruction(addInstr);
            
            // 右值
            pOperand rightVal = newTemporary();
            translateExp(right, rightVal);
            
            // 间接赋值
            pInstruction assignInstr = newInstruction(DEREF_L);
            assignInstr->u.assign.result = fieldAddr;
            assignInstr->u.assign.op1 = rightVal;
            appendInstruction(assignInstr);
            
            if (place) {
                pInstruction copyInstr = newInstruction(ASSIGN);
                copyInstr->u.assign.result = place;
                copyInstr->u.assign.op1 = rightVal;
                appendInstruction(copyInstr);
            }
        }
    }
    // 二元运算
    else if (!strcmp_safe_(node->firstchild->name, "Exp") && 
             node->firstchild->nextsib && 
             node->firstchild->nextsib->nextsib &&
             (!strcmp_safe_(node->firstchild->nextsib->name, "PLUS") || 
              !strcmp_safe_(node->firstchild->nextsib->name, "MINUS") ||
              !strcmp_safe_(node->firstchild->nextsib->name, "STAR") || 
              !strcmp_safe_(node->firstchild->nextsib->name, "DIV"))) {
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
    // 函数调用
    else if (!strcmp_safe_(node->firstchild->name, "ID") && 
             node->firstchild->nextsib && 
             !strcmp_safe_(node->firstchild->nextsib->name, "LP")) {
        // 获取函数名
        char* func_name = node->firstchild->val.id_val;
        
        if (!strcmp_safe_(func_name, "read")) {
            // 处理read系统调用
            if (place) {
                pInstruction readInstr = newInstruction(READ);
                readInstr->u.singleop.op = place;
                appendInstruction(readInstr);
            }
            return;
        }
        else if (!strcmp_safe_(func_name, "write")) {
            // 处理write系统调用
            if (node->firstchild->nextsib->nextsib && 
                !strcmp_safe_(node->firstchild->nextsib->nextsib->name, "Args")) {
                Node* args = node->firstchild->nextsib->nextsib;
                Node* exp = args->firstchild;
                
                // 计算要输出的表达式
                pOperand t = newTemporary();
                translateExp(exp, t);
                
                // 生成WRITE指令
                pInstruction writeInstr = newInstruction(WRITE);
                writeInstr->u.singleop.op = t;
                appendInstruction(writeInstr);
            }
            return;
        }
        
        // 普通函数调用
        if (node->firstchild->nextsib->nextsib && 
            !strcmp_safe_(node->firstchild->nextsib->nextsib->name, "Args")) {
            // 处理参数
            translateArgs(node->firstchild->nextsib->nextsib);
        }
        
        // 生成函数调用指令
        pInstruction callInstr = newInstruction(CALL);
        callInstr->u.call.op = newVariable(func_name);
        callInstr->u.call.result = place ? place : newTemporary();
        appendInstruction(callInstr);
    }
    // 一元运算符(+, -)
    else if ((!strcmp_safe_(node->firstchild->name, "PLUS") || !strcmp_safe_(node->firstchild->name, "MINUS")) && 
             node->firstchild->nextsib) {
        if (place) {
            Node* op = node->firstchild;
            Node* right = op->nextsib;
            pOperand t1 = newConstant(0);
            pOperand t2 = newTemporary();
            translateExp(right, t2);
            
            pInstruction instr;
            if (!strcmp_safe_(op->name, "PLUS"))
                instr = newInstruction(ADD);
            else // MINUS
                instr = newInstruction(SUB);

            instr->u.binop.result = place;
            instr->u.binop.op1 = t1;
            instr->u.binop.op2 = t2;
            appendInstruction(instr);
        }
    }
    // 括号表达式
    else if (!strcmp_safe_(node->firstchild->name, "LP") && 
             node->firstchild->nextsib && 
             !strcmp_safe_(node->firstchild->nextsib->name, "Exp")) {
        if (place) {
            translateExp(node->firstchild->nextsib, place);
        }
    }
    // 数组访问 Exp[Exp]
    else if (!strcmp_safe_(node->firstchild->name, "Exp") && 
             node->firstchild->nextsib && 
             !strcmp_safe_(node->firstchild->nextsib->name, "LB")) {
        if (place) {
            // 处理数组基址
            pOperand array = newTemporary();
            translateExp(node->firstchild, array);
            
            // 处理索引
            pOperand index = newTemporary();
            translateExp(node->firstchild->nextsib->nextsib, index);
            
            // 获取数组类型以计算元素大小
            Type arrayType = NULL;
            int elemSize = 4; // 默认大小为4字节
            
            // 尝试从符号表中获取数组类型
            if (node->firstchild->firstchild && !strcmp_safe_(node->firstchild->firstchild->name, "ID")) {
                char* arrayName = node->firstchild->firstchild->val.id_val;
                pobj arrayObj = searchtab(table, arrayName);
                if (arrayObj && arrayObj->type->kind == ARRAY) {
                    arrayType = arrayObj->type;
                    // 计算元素大小
                    elemSize = getTypeSize(arrayType->u.array.elem);
                    //printf("Array element size: %d bytes\n", elemSize);
                }
            }
            
            // 计算偏移 (index * elemSize)
            pOperand offset = newTemporary();
            pInstruction mulInstr = newInstruction(MUL);
            mulInstr->u.binop.result = offset;
            mulInstr->u.binop.op1 = index;
            mulInstr->u.binop.op2 = newConstant(elemSize); // 使用实际元素大小
            appendInstruction(mulInstr);

            //printf("calculate offset with element size %d\n", elemSize);
            
            // 计算元素地址
            pOperand addr = newTemporary();
            pInstruction addInstr = newInstruction(ADD);
            addInstr->u.binop.result = addr;
            addInstr->u.binop.op1 = array;
            addInstr->u.binop.op2 = offset;
            appendInstruction(addInstr);
            
            // 加载元素值
            pInstruction loadInstr = newInstruction(DEREF_R);
            loadInstr->u.assign.result = place;
            loadInstr->u.assign.op1 = addr;
            appendInstruction(loadInstr);
        }
    }
    // 结构体成员访问 Exp.ID
    else if (!strcmp_safe_(node->firstchild->name, "Exp") && 
             node->firstchild->nextsib && 
             !strcmp_safe_(node->firstchild->nextsib->name, "DOT")) {
        if (place) {
            // 获取结构体地址
            pOperand structAddr = newTemporary();
            translateExp(node->firstchild, structAddr);
            
            // 获取字段名和结构体类型
            Node* fieldNode = node->firstchild->nextsib->nextsib; // 获取ID节点
            char* fieldName = fieldNode->val.id_val;

            // 通过符号表查找结构体类型
            Type structType = NULL;
            if (node->firstchild->firstchild && !strcmp_safe_(node->firstchild->firstchild->name, "ID")) {
                char* structVarName = node->firstchild->firstchild->val.id_val;
                pobj structObj = searchtab(table, structVarName);
                if (structObj) {
                    structType = structObj->type;
                }
            }
            
            // 计算字段偏移
            int offset = getStructFieldOffset(structType, fieldName);

            // 计算字段地址
            pOperand fieldAddr = newTemporary();
            pInstruction addInstr = newInstruction(ADD);
            addInstr->u.binop.result = fieldAddr;
            addInstr->u.binop.op1 = structAddr;
            addInstr->u.binop.op2 = newConstant(offset);
            appendInstruction(addInstr);
            
            // 加载字段值
            pInstruction loadInstr = newInstruction(DEREF_R);
            loadInstr->u.assign.result = place;
            loadInstr->u.assign.op1 = fieldAddr;
            appendInstruction(loadInstr);
        }
    }
    // 取地址操作 &Exp
    else if (!strcmp_safe_(node->firstchild->name, "AND") && node->firstchild->nextsib) {
        if (place) {
            // 假设AND操作符是取地址符
            Node* exp = node->firstchild->nextsib;
            
            if (exp->firstchild && !strcmp_safe_(exp->firstchild->name, "ID")) {
                // 获取变量地址
                pInstruction addrInstr = newInstruction(ADDR);
                addrInstr->u.assign.result = place;
                addrInstr->u.assign.op1 = newVariable(exp->firstchild->val.id_val);
                appendInstruction(addrInstr);
            }
        }
    }
    // 解引用操作 *Exp
    else if (!strcmp_safe_(node->firstchild->name, "STAR") && node->firstchild->nextsib) {
        if (place) {
            // 获取指针
            pOperand ptr = newTemporary();
            translateExp(node->firstchild->nextsib, ptr);
            
            // 解引用加载值
            pInstruction derefInstr = newInstruction(DEREF_R);
            derefInstr->u.assign.result = place;
            derefInstr->u.assign.op1 = ptr;
            appendInstruction(derefInstr);
        }
    }
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
    
    // 首先检查是否是括号表达式：LP Exp RP
    if (node->firstchild && 
        !strcmp_safe_(node->firstchild->name, "LP") && 
        node->firstchild->nextsib && 
        !strcmp_safe_(node->firstchild->nextsib->name, "Exp") &&
        node->firstchild->nextsib->nextsib &&
        !strcmp_safe_(node->firstchild->nextsib->nextsib->name, "RP")) {
        
        // 括号表达式，递归处理内部表达式
        translateCond(node->firstchild->nextsib, label_true, label_false);
        return;
    }
    
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
        ifGotoInstr->u.if_goto.relop = (char*)malloc(10 * sizeof(char));
        ifGotoInstr->u.if_goto.relop[0] = '\0'; // Initialize to empty string
        strcpy(ifGotoInstr->u.if_goto.relop, relop->val.id_val);
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


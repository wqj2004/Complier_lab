#include "semantic.h"

#define hashsz 0x3fff
#define max_st_depth 20

hashfn(char *name)
{
    unsigned val = 0, i;
    for (; *name; ++name)
    {
        val = (val << 2) + *name;
        if (i = val & ~hashsz)
            val = (val ^ (i >> 12)) & hashsz;
    }
    return val;
}
// TYPE
Type newTYPE(TypeKind kind, ...)
{
    Type newtype = (Type)malloc(sizeof(struct Type_));
    assert(newtype != NULL);
    newtype->kind = kind;
    va_list valist;
    assert(kind == BASIC || kind == ARRAY || kind == STRUCTURE || kind == FUNCTION);
    switch (kind)
    {
    case BASIC:
        va_start(valist, kind);
        newtype->u.basic = va_arg(valist, BasicType);
        break;
    case ARRAY:
        va_start(valist, kind);
        newtype->u.array.elem = va_arg(valist, Type);
        newtype->u.array.size = va_arg(valist, int);
        break;
    case STRUCTURE:
        va_start(valist, kind);
        newtype->u.structure = va_arg(valist, FieldList);
        break;
    case FUNCTION:
        va_start(valist, kind);
        newtype->u.function.argc = va_arg(valist, int);
        newtype->u.function.args = va_arg(valist, FieldList);
        newtype->u.function.ret_type = va_arg(valist, Type);
        newtype->u.function.state = va_arg(valist, Funstate);
        newtype->u.function.lineno = va_arg(valist, int);
        break;
    }
    return newtype;
}

Type copyTYPE(Type type)
{
    TypeKind kind = type->kind;

    switch (kind)
    {
    case BASIC:
        return newTYPE(kind, type->u.basic);
        break;
    case ARRAY:
        return newTYPE(kind, copyTYPE(type->u.array.elem), type->u.array.size);
        break;
    case STRUCTURE:
        return newTYPE(kind, copyFieldList(type->u.structure));
        break;
    case FUNCTION:
        return newTYPE(kind, type->u.function.argc, copyFieldList(type->u.function.args), copyTYPE(type->u.function.ret_type), type->u.function.state, type->u.function.lineno);
        break;
    }
}

int equalType(Type type1, Type type2)
{
    if (type1 == NULL && type2 == NULL)
        return 1;
    if (type1 == NULL || type2 == NULL)
        return 0;
    if (type1->kind != type2->kind)
        return 0;

    switch (type1->kind)
    {
    case BASIC:
        return type1->u.basic == type2->u.basic;
    case ARRAY:
        return equalType(type1->u.array.elem, type2->u.array.elem);
    case STRUCTURE:
        return equalFieldList(type1->u.structure, type2->u.structure);
    case FUNCTION:
        if (type1->u.function.state == defined && type2->u.function.state == defined)
            return 0;

        return type1->u.function.argc == type2->u.function.argc &&
               equalFieldList(type1->u.function.args, type2->u.function.args) &&
               equalType(type1->u.function.ret_type, type2->u.function.ret_type);
    }
}

void freeType(Type type)
{
    // TODO
}

void freeFieldList(FieldList field)
{
    // TODO
}

void freeObj(pobj obj)
{
    // TODO
}

// FieldList
FieldList newFieldList(char *name, Type type)
{
    FieldList newfield = (FieldList)malloc(sizeof(fieldlist));
    assert(newfield != NULL);
    newfield->name = copystr(name);
    newfield->type = type;
    newfield->tail = NULL;
    return newfield;
}

FieldList copyFieldList(FieldList field)
{
    if (field == NULL)
        return NULL;
    FieldList newfield = newFieldList(field->name, copyTYPE(field->type));
    newfield->tail = copyFieldList(field->tail);
    return newfield;
}

void append_FieldList(FieldList pField, FieldList F_to_append)
{
    assert(pField != NULL);
    FieldList cur = pField;
    while (cur->tail != NULL)
    {
        cur = cur->tail;
    }
    cur->tail = F_to_append;
}

int equalFieldList(FieldList field1, FieldList field2)
{
    if (field1 == NULL && field2 == NULL)
        return 1;
    if (field1 == NULL || field2 == NULL)
        return 0;
    return equalType(field1->type, field2->type) && equalFieldList(field1->tail, field2->tail);
}

// Object
pobj newObj(char *name, int st_depth, Type type)
{
    pobj newobj = (pobj)malloc(sizeof(obj));
    assert(newobj != NULL);
    newobj->name = copystr(name);
    newobj->stack_depth = st_depth;
    newobj->type = type;
    newobj->hash_next = NULL;
    newobj->stack_next = NULL;
    return newobj;
}

// HashTable
phash newHash()
{
    pobj *objlist = (pobj *)malloc(sizeof(pobj) * hashsz);
    assert(objlist != NULL);
    for (int i = 0; i < hashsz; i++)
    {
        objlist[i] = NULL;
    }
    phash p = (phash)malloc(sizeof(hashtab));
    p->hashlist = objlist;
    return p;
}
void insert_hashobj(phash hashtab, pobj obj)
{
    unsigned index = hashfn(obj->name);
    pobj *objlist = hashtab->hashlist;
    obj->hash_next = objlist[index];
    objlist[index] = obj;
}
void delete_hashobj(phash hashtab, pobj obj)
{
    unsigned index = hashfn(obj->name);
    pobj *objlist = hashtab->hashlist;
    pobj cur_obj = objlist[index];
    pobj prev_obj = NULL;
    while (cur_obj != NULL)
    {
        if (cur_obj == obj)
        {
            if (prev_obj == NULL)
                objlist[index] = cur_obj->hash_next;
            else
                prev_obj->hash_next = cur_obj->hash_next;
            break;
        }
        prev_obj = cur_obj;
        cur_obj = cur_obj->hash_next;
    }
}
obj *searchhash(phash hashtab, char *name)
{
    unsigned index = hashfn(name);
    pobj *objlist = hashtab->hashlist;
    pobj obj = objlist[index];
    while (obj != NULL)
    {
        if (strcmp(obj->name, name) == 0)
            return obj;
        obj = obj->hash_next;
    }
    return NULL;
}

// Stack
pstack newStack()
{
    pobj *objlist = (pobj *)malloc(sizeof(pobj) * max_st_depth);
    assert(objlist != NULL);
    for (int i = 0; i < max_st_depth; i++)
    {
        objlist[i] = NULL;
    }
    pstack p = (pstack)malloc(sizeof(stack));
    p->stacklist = objlist;
    p->cur_stack_depth = 0;
    return p;
}
void insert_stackobj(pstack stack, pobj obj)
{
    int cur_depth = stack->cur_stack_depth;
    pobj *obj_stack = stack->stacklist;
    obj->stack_next = obj_stack[cur_depth];
    obj_stack[cur_depth] = obj;
}

void incStackDepth(pstack stack)
{
    if (stack->cur_stack_depth < max_st_depth)
        stack->cur_stack_depth++;
}

void decStackDepth(pstack stack)
{
    stack->cur_stack_depth--;
    assert(stack->cur_stack_depth >= 0);
}

// ObjTable
ptab newTable()
{
    phash hashtab = newHash();
    pstack stack = newStack();
    ptab p = (ptab)malloc(sizeof(objtab));
    p->hashtab = hashtab;
    p->st = stack;
    p->unnamed_struct_num = 0;
    return p;
}
void insert_tabobj(ptab table, pobj obj)
{

    insert_hashobj(table->hashtab, obj);
    insert_stackobj(table->st, obj);
}
void delete_stack_curdepth(ptab table)
{
    pstack stack = table->st;
    int cur_depth = stack->cur_stack_depth;
    pobj cur_obj = stack->stacklist[cur_depth];
    while (cur_obj != NULL)
    {
        pobj next = cur_obj->stack_next;
        delete_hashobj(table->hashtab, cur_obj);
        free(cur_obj);
        cur_obj = next;
    }
    stack->stacklist[cur_depth] = NULL;
    stack->cur_stack_depth--;
}
obj *searchtab(ptab table, char *name)
{
    return searchhash(table->hashtab, name);
}

obj *searchtab_func(ptab table, char *name)
{
    unsigned index = hashfn(name);
    pobj *objlist = table->hashtab->hashlist;
    pobj obj = objlist[index];
    while (obj != NULL)
    {
        if (strcmp(obj->name, name) == 0 && obj->type->kind == FUNCTION)
            return obj;
        obj = obj->hash_next;
    }
    return NULL;
}

int objConflict(ptab table, pobj obj)
{
    pobj curobj = searchtab(table->hashtab, obj->name);
    if (curobj == NULL)
        return 0;
    while (curobj != NULL)
    {
        if (!strcmp(curobj->name, obj->name))
        {
            if (curobj->type->kind == STRUCTURE || obj->type->kind == STRUCTURE)
            {
                return 1;
            }
            else if (curobj->stack_depth == obj->stack_depth)
            {
                if ((curobj->type->kind == FUNCTION && obj->type->kind != FUNCTION) || (curobj->type->kind != FUNCTION && obj->type->kind == FUNCTION))
                {
                    continue;
                }
                else
                    return 1;
            }
        }
        curobj = curobj->hash_next;
    }
}

// Program     : ExtDefList
void Program(Node *node)
{
    if (node == NULL)
        return;
    if (!strcmp('ExtDefList', node->firstchild->name))
    {
        ExtDefList(node->firstchild);
    }
}

// ExtDefList : ExtDef ExtDefList
//              |  /* empty */
//              ;
void ExtDefList(Node *node)
{
    if (node == NULL)
        return;
    if (!strcmp(node->name, "ExtDef"))
    {
        ExtDef(node->firstchild);
        ExtDefList(node->firstchild->nextsib);
    }
    else
    {
        return;
    }
}

// ExtDef      : Specifier ExtDecList SEMI
//             | Specifier SEMI
//             | Specifier FunDec CompSt
//             | Specifier FunDec SEMI
void ExtDef(Node *node)
{
    Type spec = Specifier(node->firstchild);
    if (!strcmp(node->firstchild->nextsib->name, "ExtDecList"))
    {
        ExtDecList(node->firstchild->nextsib, spec);
    }
    else if (!strcmp(node->firstchild->nextsib->name, "FunDec"))
    {
        if (!strcmp(node->firstchild->nextsib->nextsib->name, "CompSt"))
        {
            pobj funobj = FunDec(node->firstchild->nextsib, spec, defined);
            CompSt(node->firstchild->nextsib->nextsib, funobj);
        }
        else
        {
            FunDec(node->firstchild->nextsib, spec, undefined);
        }
    }
}

// ExtDecList  : VarDec
//             | VarDec COMMA ExtDecList
//             ;
void ExtDecList(Node *node, Type deftype)
{
    if (node == NULL)
        return;
    while (node->firstchild->nextsib != NULL)
    {
        pobj obj = (pobj)VarDec(node->firstchild, deftype, NULL);
        if (objConflict(table, obj))
        {
            semanticError(REDEF_VAR, node->fline, "Redefined variable");
            freeObj(obj);
        }
        else
        {
            insert_tabobj(table, obj);
        }
        node = node->firstchild->nextsib->nextsib;
    }
    pobj obj = (pobj)VarDec(node->firstchild, deftype, NULL);
    if (objConflict(table, obj))
    {
        semanticError(REDEF_VAR, node->fline, "Redefined variable");
        freeObj(obj);
    }
    else
    {
        insert_tabobj(table, obj);
    }
}

// FunDec      : ID LP VarList RP
//             | ID LP RP
//             ;
pobj FunDec(Node *node, Type rettype, Funstate isdef)
{
    if (table->st->cur_stack_depth != 0)
    {
        sematicError(NEST_FUNC_DEF, node->fline, "Function defined in a block");
    }
    pobj funobj = searchtab_func(table, node->firstchild->val.id_val);
    if (funobj == NULL)
    {
        Type funtype = newTYPE(FUNCTION, 0, NULL, rettype, isdef, node->fline);
        if (!strcmp(node->firstchild->nextsib->nextsib->val.id_val, "VarList"))
        {
            VarList(node->firstchild->nextsib->nextsib, funtype);
        }
        funobj = newObj(node->firstchild->val.id_val, 0, funtype);
        insert_tabobj(table, funobj);
        return funobj;
    }
    else
    {
        if (funobj->type->u.function.state == defined && isdef == defined)
        {
            semanticError(REDEF_FUNC, node->firstchild->val.id_val, "Redefined function");
            return NULL;
        }
        else
        {
            Type funtype = newTYPE(FUNCTION, 0, NULL, rettype, isdef, node->fline);
            if (!strcmp(node->firstchild->nextsib->nextsib->val.id_val, "VarList"))
            {
                VarList(node->firstchild->nextsib->nextsib, funtype);
            }
            if (equalType(funtype, funobj->type))
            {
                if (isdef == defined)
                {
                    funobj->type->u.function.state = defined;
                }
                freeType(funtype);
                return funobj;
            }
            else
            {
                freeType(funtype);
                funobj->type->u.function.state = isdef;
                semanticError(DISMATCH_DECLARE_FUNC, node->fline, "Function declaration mismatch");
                return NULL;
            }
        }
    }
}

// VarList:        ParamDec COMMA VarList
//            |       ParamDec
//            ;
void VarList(Node *node, Type funtype)
{
    FieldList newparam = ParamDec(node->firstchild);
    assert(funtype->u.function.args == NULL);
    funtype->u.function.args = newparam;
    funtype->u.function.argc++;
    node = node->firstchild->nextsib;
    while (node != NULL)
    {
        node = node->nextsib;
        FieldList newparam = ParamDec(node->firstchild);
        append_FieldList(funtype->u.function.args, newparam);
        funtype->u.function.argc++;
        node = node->firstchild->nextsib;
    }
}
// ParamDec    : Specifier VarDec
//             ;
FieldList ParamDec(Node *node)
{
    Type paramtype = Specifier(node->firstchild);
    FieldList _ = (FieldList)malloc(sizeof(fieldlist));
    FieldList paramfield = (FieldList)VarDec(node->firstchild->nextsib, paramtype, _);
    free(_);
    return paramfield;
}

// CompSt      : LC DefList StmtList RC
//             ;
void CompSt(Node *node, pobj funobj)
{
    // funobj用于指示当前的复合语句是否是函数定义的复合语句
    incStackDepth(table->st);
    if (funobj != NULL)
    {
        FieldList argv = funobj->type->u.function.args;
        while (argv != NULL)
        {
            pobj newobj = newObj(argv->name, table->st->cur_stack_depth, argv->type);
            insert_tabobj(table, newobj);
            argv = argv->tail;
        }
    }
    if (node->firstchild->nextsib != NULL)
    {
        DefList(node->firstchild->nextsib, NULL);
    }
    if (node->firstchild->nextsib->nextsib != NULL)
    {
        StmtList(node->firstchild->nextsib->nextsib, funobj);
    }
    delete_stack_curdepth(table);
    decStackDepth(table->st);
}

// StmtList    : Stmt StmtList
//         | /* empty */
//         ;
void StmtList(Node *node, pobj funobj)
{
    while (node != NULL)
    {
        Stmt(node->firstchild, funobj);
        node = node->firstchild->nextsib;
    }
}

// Stmt    : Exp SEMI
//         | CompSt
//         | RETURN Exp SEMI
//         | IF LP Exp RP Stmt %prec LOWER_THAN_ELSE
//         | IF LP Exp RP Stmt ELSE Stmt
//         | WHILE LP Exp RP Stmt
//         ;
void Stmt(Node *node, pobj funobj)
{
    Type type = NULL;
    Type rettype = NULL;
    if (!strcmp(node->firstchild->name, "Exp"))
    {
        int lval = 0;
        type = Exp(node->firstchild, &lval);
    }
    else if (!strcmp(node->firstchild->name, "CompSt"))
    {
        CompSt(node->firstchild, funobj);
    }
    else if (!strcmp(node->firstchild->name, "RETURN"))
    {
        if (funobj != NULL)
        {
            rettype = funobj->type->u.function.ret_type;
            int lval = 0;
            type = Exp(node->firstchild->nextsib, &lval);
            if (!equalType(rettype, type))
            {
                semanticError(TYPE_MISMATCH_RETURN, node->fline, "Return type mismatch");
            }
        }
    }
    else if (!strcmp(node->firstchild->name, "IF"))
    {
        int lval = 0;
        type = Exp(node->firstchild->nextsib->nextsib, &lval);
        Node *stmtnode = node->firstchild->nextsib->nextsib->nextsib->nextsib;
        Stmt(stmtnode, funobj);
        if (stmtnode->nextsib)
        {
            Stmt(stmtnode->nextsib->nextsib, funobj);
        }
    }
    else if (!strcmp(node->firstchild->name, "WHILE"))
    {
        int lval = 0;
        type = Exp(node->firstchild->nextsib->nextsib, &lval);
        Node *stmtnode = node->firstchild->nextsib->nextsib->nextsib;
        Stmt(stmtnode, funobj);
    }
    else
    {
        assert(0);
    }
    if (type != NULL)
    {
        freeType(type);
        type = NULL;
    }
}

// DefList : Def DefList
//         | e
//         ;
// 每个Def就是一条变量定义
void DefList(Node *node, FieldList structfield)
{
    while (node != NULL)
    {
        node = node->firstchild;
        Def(node, structfield);
        node = node->nextsib;
    }
}

// Def     : Specifier DecList SEMI
//         ;
void Def(Node *node, FieldList structfield)
{
    assert(node != NULL);
    Node *specnode = node->firstchild;
    Node *declistnode = specnode->nextsib;
    Type deftype = Specifier(specnode);
    DecList(declistnode, deftype, structfield);
}

// DecList : Dec
//         | Dec COMMA DecList
//         ;
void DecList(Node *node, Type deftype, FieldList structfield)
{
    while (node->firstchild->nextsib != NULL)
    {
        Dec(node->firstchild, deftype, structfield);
        node = node->firstchild->nextsib->nextsib;
    }
    Dec(node->firstchild, deftype, structfield);
}

// Dec     : VarDec
//         | VarDec ASSIGNOP Exp  a = 5
//         ;
void Dec(Node *node, Type deftype, FieldList structfield)
{
    if (node->firstchild->nextsib == NULL)
    {
        if (structfield != NULL)
        {
            // 在结构体内部,需要把每个Dec对应的fieldlist加到structfield的后面串起来
            FieldList field = (FieldList)VarDec(node->firstchild, deftype, structfield);
            FieldList curfield = structfield->tail;
            // 在我们的设置中,struct的field的第一个是指示这个struct名字的虚拟field,从这个虚拟field之后的才是真正的struct的FieldList
            // structfield是Type_中的structure这个值,是一个指针,所以需要用到它的tail
            while (curfield != NULL)
            {
                if (!strcmp(curfield->name, field->name))
                {
                    semanticError(REDEF_FIELD, node->firstchild->val.id_val, "Redefined field");
                    freeFieldList(field);
                    return;
                }
                curfield = curfield->tail;
            }
            append_FieldList(structfield, field);
            return;
        }
        else
        {
            // 不在结构体内部
            pobj newobj = (pobj)VarDec(node->firstchild, deftype, NULL);
            if (objConflict(table, newobj))
            {
                semanticError(REDEF_VAR, node->fline, "Redefined variable");
                freeObj(newobj);
            }
            else
            {
                insert_tabobj(table, newobj);
            }
        }
    }
    else
    {
        if (structfield != NULL)
        {
            semanticError(REDEF_FIELD, node->fline, "Assignment in struct");
        }
        else
        {
            int lval = 0;
            Type expType = Exp(node->firstchild->nextsib->nextsib, &lval);
            pobj newobj = (pobj)VarDec(node->firstchild, deftype, NULL);
            if (expType != NULL)
            {

                if (objConflict(table, newobj))
                {
                    semanticError(REDEF_VAR, node->fline, "Redefined variable");
                    freeObj(newobj);
                }
                else if (newobj->type->kind != BASIC && newobj->type->kind != STRUCTURE)
                {
                    semanticError(LEFT_VAR_ASSIGN, node->fline, "Left value of assignment must be a variable");
                    freeObj(newobj);
                }
                else if (!equalType(newobj->type, expType))
                {
                    semanticError(TYPE_MISMATCH_ASSIGN, node->fline, "Type mismatched for assignment");
                    freeObj(newobj);
                }
                else
                {
                    insert_tabobj(table, newobj);
                }
                freeType(expType);
                expType = NULL;
            }
        }
    }
}

// VarDec      : ID
//             | VarDec LB INT RB
//             ;
void *VarDec(Node *node, Type deftype, FieldList structfield)
{
    Node *tmpnode = node;
    while (tmpnode->firstchild)
    {
        tmpnode = tmpnode->firstchild;
    }
    char *varname = tmpnode->val.id_val;
    tmpnode = node;
    if (structfield != NULL)
    {
        FieldList newfield = newFieldList(varname, deftype);
        // 在struct内部
        if (!strcmp(node->firstchild->name, "ID"))
        {
            // 普通变量
            return newfield;
        }
        else
        {
            // 数组变量
            Type lasttype = deftype;
            while (tmpnode->firstchild->nextsib != NULL)
            {
                int size = tmpnode->firstchild->nextsib->nextsib->val.int_val;
                lasttype = newTYPE(ARRAY, lasttype, size);
                tmpnode = tmpnode->firstchild;
            }
            newfield->type = lasttype;
            return newfield;
        }
    }
    else
    {
        // 不在struct内部
        pobj newobj = newObj(varname, table->st->cur_stack_depth, deftype);
        if (!strcmp(node->firstchild->name, "ID"))
        {
            // 普通变量
            return newobj;
        }
        else
        {
            // 数组变量
            Type lasttype = deftype;
            while (tmpnode->firstchild->nextsib != NULL)
            {
                int size = tmpnode->firstchild->nextsib->nextsib->val.int_val;
                lasttype = newTYPE(ARRAY, lasttype, size);
                tmpnode = tmpnode->firstchild;
            }
            newobj->type = lasttype;
            return newobj;
        }
    }
}

// Exp     : Exp ASSIGNOP Exp  a = 5
//         | Exp AND Exp       a&b
//         | Exp OR Exp        a|b
//         | Exp RELOP Exp     a>b
//         | Exp PLUS Exp      a+b
//         | Exp MINUS Exp     a-b
//         | Exp STAR Exp      a*b
//         | Exp DIV Exp       a/b
//         | Exp LB Exp RB     a[b]  左值
//         | Exp DOT ID        a.b   左值

//         | LP Exp RP         (a)
//         | MINUS Exp         -a
//         | NOT Exp           ~a

//         | ID LP Args RP     a(b)
//         | ID LP RP          a()

//         | ID                a     左值
//         | INT               1
//         | FLOAT             1.0
//         ;
Type Exp(Node *node, int *plval)
{
    int exp1_lval = 0, exp2_lval = 0;
    Type exp1_type = NULL, exp2_type = NULL;
    if (!strcmp(node->firstchild->name, "Exp"))
    {
        //           Exp ASSIGNOP Exp  a = 5
        //         | Exp AND Exp       a&b
        //         | Exp OR Exp        a|b
        //         | Exp RELOP Exp     a>b
        //         | Exp PLUS Exp      a+b
        //         | Exp MINUS Exp     a-b
        //         | Exp STAR Exp      a*b
        //         | Exp DIV Exp       a/b
        //         | Exp LB Exp RB     a[b]  左值
        //         | Exp DOT ID        a.b   左值
        exp1_type = Exp(node->firstchild, &exp1_lval);
        exp2_type = Exp(node->firstchild->nextsib->nextsib, &exp2_lval);
        if (exp1_type == NULL || exp2_type == NULL)
        {
            return NULL;
        }
        Node *opnode = node->firstchild->nextsib;
        if (!strcmp(opnode->val.id_val, "LB"))
        {
            // 数组下标
            // Exp LB Exp RB     a[b]
            if (exp1_type->kind != ARRAY)
            {
                semanticError(NOT_A_ARRAY, node->fline, "Array type mismatch");
                return NULL;
            }
            else if ((exp2_type->kind != BASIC) || (exp2_type->u.basic != INT_TYPE))
            {
                semanticError(NOT_A_INT, node->fline, "Array type mismatch");
                return NULL;
            }
            TypeKind kind = exp1_type->u.array.elem->kind;
            if (kind == ARRAY || kind == FUNCTION)
            {
                *plval = 0; // 非左值
            }
            else
            {
                *plval = 1; // 左值
            }
            return copyTYPE(exp1_type->u.array.elem);
        }
        else if (!strcmp(opnode->val.id_val, "DOT"))
        {
            // 结构体成员
            // Exp DOT ID
            if (exp1_type->kind != STRUCTURE)
            {
                semanticError(ILLEGAL_USE_DOT, node->fline, "illegal use of . ");
                return NULL;
            }
            char *idname = node->firstchild->nextsib->nextsib->val.id_val;
            FieldList field = exp1_type->u.structure;
            while (field != NULL)
            {
                if (!strcmp(field->name, idname))
                {
                    break;
                }
                field = field->tail;
            }
            if (field == NULL)
            {
                semanticError(NONEXISTFIELD, node->fline, "Field not exist");
                return NULL;
            }
            TypeKind kind = field->type->kind;
            if (kind == ARRAY || kind == FUNCTION)
            {
                *plval = 0; // 非左值
            }
            else
            {
                *plval = 1; // 左值
            }
            return copyTYPE(field->type);
        }
        else if (!strcmp(opnode->val.id_val, "ASSIGNOP"))
        {
            // 赋值
            // Exp ASSIGNOP Exp  a = 5
            if (exp1_lval == 0)
            {
                semanticError(LEFT_VAR_ASSIGN, node->fline, "Left value of assignment must be a variable");
                return NULL;
            }
            else if (exp1_type->kind != BASIC && exp1_type->kind != STRUCTURE)
            {
                semanticError(LEFT_VAR_ASSIGN, node->fline, "Left value of assignment must be a variable");
                return NULL;
            }
            else if (!equalType(exp1_type, exp2_type))
            {
                semanticError(TYPE_MISMATCH_ASSIGN, node->fline, "Type mismatched for assignment");
                return NULL;
            }
            *plval = 0; // 右值
            return copyTYPE(exp1_type);
        }
        else
        {
            // 其他运算符
            // Exp AND Exp       a&b
            // Exp OR Exp        a|b
            // Exp RELOP Exp     a>b
            // Exp PLUS Exp      a+b
            // Exp MINUS Exp     a-b
            // Exp STAR Exp      a*b
            // Exp DIV Exp       a/b
            if (exp1_type->kind != BASIC || exp2_type->kind != BASIC)
            {
                semanticError(TYPE_MISMATCH_OP, node->fline, "Type mismatched for operator");
                return NULL;
            }
            char *opname = opnode->val.id_val;
            if (!strcmp(opname, "AND") || !strcmp(opname, "OR") || !strcmp(opname, "RELOP"))
            {
                return newTYPE(BASIC, INT_TYPE);
            }
            else
            {
                return copyTYPE(exp1_type);
            }
        }
    }
    else if (!strcmp(node->firstchild->name, "LP"))
    {
        // LP Exp RP
        // (a)
        return Exp(node->firstchild->nextsib, plval);
    }
    else if (!strcmp(node->firstchild->name, "MINUS") || !strcmp(node->firstchild->name, "NOT"))
    {
        // MINUS Exp
        // -a
        // NOT Exp
        // ~a
        exp1_type = Exp(node->firstchild->nextsib, plval);
        if (exp1_type == NULL)
        {
            return NULL;
        }
        if (exp1_type->kind != BASIC)
        {
            semanticError(TYPE_MISMATCH_OP, node->fline, "Type mismatched for operator");
            return NULL;
        }
        *plval = 0;
        if (!strcmp(node->firstchild->name, "MINUS"))
        {
            return newType(BASIC, INT_TYPE);
        }
        return copyTYPE(exp1_type);
    }
    else if (!strcmp(node->firstchild->name, "ID"))
    {
        // ID LP Args RP
        // ID LP RP
        // ID
        pobj idobj = searchtab(table, node->firstchild->val.id_val);
        if (idobj == NULL || isStructType(idobj))
        {
            if (node->firstchild->nextsib != NULL)
            {
                semanticError(UNDEF_FUNC, node->fline, "Undefined function");
                return NULL;
            }
            else
            {
                semanticError(UNDEF_VAR, node->fline, "Undefined variable");
                return NULL;
            }
        }
        else if (node->firstchild->nextsib != NULL)
        {
            // ID LP Args RP
            // ID LP RP
            if (idobj->type->kind != FUNCTION)
            {
                semanticError(NOT_A_FUNC, node->fline, "Undefined function");
                return NULL;
            }
            else
            {
                Node *argsnode = NULL;
                if (!strcmp(node->firstchild->nextsib->nextsib->name, "Args"))
                {
                    argsnode = node->firstchild->nextsib->nextsib;
                }
                if (!Args(argsnode, idobj->type->u.function.args, node->fline))
                {
                    return NULL;
                }
                *plval = 0;
                return copyTYPE(idobj->type->u.function.ret_type);
            }
        }
        else
        {
            // ID
            // a
            *plval = 1;
            return copyTYPE(idobj->type);
        }
    }
    else if (!strcmp(node->firstchild->name, "INT"))
    {
        *plval = 0;
        return newTYPE(BASIC, INT_TYPE);
    }
    else if (!strcmp(node->firstchild->name, "FLOAT"))
    {
        *plval = 0;
        return newTYPE(BASIC, FLOAT_TYPE);
    }
    // TODO
    return NULL;
}

// Args        :   Exp COMMA Args
//             |   Exp
//             ;
int Args(Node *node, FieldList args, int fline)
{
    // 用于判断调用时的参数和实际定义时的参数是否一致
    Node *curnode = node;
    FieldList curarg = args;
    while (curnode != NULL && curarg != NULL)
    {
        int lval = 0;
        Type argtype = Exp(curnode->firstchild, &lval);
        if (!equalType(argtype, curarg->type))
        {
            semanticError(DISMATCH_DECLARE_FUNC, fline, "Function declaration mismatch");
            return 0;
        }
        if (curnode->firstchild->nextsib != NULL)
        {
            curnode = curnode->firstchild->nextsib->nextsib;
        }
        else
        {
            curnode = NULL;
        }
        curarg = curarg->tail;
    }
    if (curnode != NULL || curarg != NULL)
    {
        semanticError(DISMATCH_DECLARE_FUNC, fline, "Function declaration mismatch");
        return 0;
    }
    return 1;
}

// Specifier   : TYPE
//             | StructSpecifier
//             ;
Type Specifier(Node *node)
{
    if (!strcmp(node->firstchild->name, "TYPE"))
    {
        Node *typenode = node->firstchild;
        char *type_str = typenode->val.id_val;
        if (!strcmp(type_str, "int"))
        {
            return newTYPE(BASIC, INT_TYPE);
        }
        else if (!strcmp(type_str, "float"))
        {
            return newTYPE(BASIC, FLOAT_TYPE);
        }
        else
        {
            print("Error: Unknown type %s\n", type_str);
            return NULL;
        }
    }
    else if (!strcmp(node->firstchild->name, 'StructSpecifier'))
    {
        return StructSpecifier(node->firstchild);
    }
    else
    {
        printf("Error: Unknown specifier %s\n", node->firstchild->name);
        return NULL;
    }
}

// StructSpecifier : STRUCT OptTag LC DefList RC
//                 | STRUCT Tag
//                 ;
// struct Complex { int real, image; }
// 或 struct { int real, image; }
// struct Complex(Tag) a, b;
Type StructSpecifier(Node *node)
{

    Node *tagnode = node->firstchild->nextsib;
    if (!strcmp(tagnode->name, "Tag"))
    {
        char *tagname = tagnode->firstchild->val.id_val;
        pobj tagobj = searchtab(table, tagname);
        if (tagobj == NULL || isStructType(tagobj) == 0)
        {

            semanticError(UNDEF_STRUCT, node->fline, "Undefined struct");
            return NULL;
        }
        else
        {
            return tagobj->type;
        }
    }
    else
    {
        // OptTag
        Type rettype = newTYPE(STRUCTURE, NULL);
        char *tagname = NULL;
        if (tagnode->firstchild != NULL)
        {
            // 有名结构体
            tagname = tagnode->firstchild->val.id_val;
        }
        else
        {
            // 无名结构体
            table->unnamed_struct_num++;
            tagname = (char *)malloc(sizeof(char) * 20);
            snprintf(tagname, 20, "%d", table->unnamed_struct_num);
        }
        FieldList field = newFieldList(tagname, NULL);
        rettype->u.structure = field;
        Node *defnode = tagnode->nextsib->nextsib;
        if (!strcmp(defnode->name, "DefList"))
        {
            DefList(defnode, rettype->u.structure);
        }
        if (tagnode->firstchild != NULL)
        {
            pobj structobj = newObj(tagname, 0, rettype);
            if (searchtab(table, tagname))
            {
                semanticError(DUPLICATED_NAME, node->fline, "Redefined struct");
                freeObj(structobj);
            }
            else
            {
                insert_tabobj(table, structobj);
            }
        }
        return rettype;
    }
}

int isStructType(pobj obj)
{
    if (obj == NULL)
        return 0;
    return obj->type->kind == STRUCTURE;
}

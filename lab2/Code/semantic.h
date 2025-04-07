#include "common.h"

typedef struct Type_ *Type;
typedef struct FieldList_ *FieldList;
typedef struct Object *pobj;
typedef struct ObjTable *ptab;
typedef struct HashTable *phash;
typedef struct Stack *pstack;

typedef enum
{
    INT_TYPE,
    FLOAT_TYPE
} BasicType;

typedef enum
{
    undefined,
    defined
} Funstate;

typedef enum _errorType
{
    UNDEF_VAR = 1,         // Undefined Variable
    UNDEF_FUNC,            // Undefined Function
    REDEF_VAR,             // Redefined Variable
    REDEF_FUNC,            // Redefined Function
    TYPE_MISMATCH_ASSIGN,  // Type mismatchedfor assignment.
    LEFT_VAR_ASSIGN,       // The left-hand side of an assignment must be a variable.
    TYPE_MISMATCH_OP,      // Type mismatched for operands.
    TYPE_MISMATCH_RETURN,  // Type mismatched for return.
    FUNC_AGRC_MISMATCH,    // Function is not applicable for arguments
    NOT_A_ARRAY,           // Variable is not a Array
    NOT_A_FUNC,            // Variable is not a Function
    NOT_A_INT,             // Variable is not a Integer
    ILLEGAL_USE_DOT,       // Illegal use of "."
    NONEXISTFIELD,         // Non-existentfield
    REDEF_FIELD,           // Redefined field
    DUPLICATED_NAME,       // Duplicated name
    UNDEF_STRUCT,          // Undefined structure
    ONLY_DECLARED_FUNC,    // Only declared function
    DISMATCH_DECLARE_FUNC, // Function declaration mismatch
    NEST_FUNC_DEF,         // Function defined in a block
} ErrorType;

typedef enum
{
    BASIC,
    ARRAY,
    STRUCTURE,
    FUNCTION,
} TypeKind;

struct Type_
{
    TypeKind kind;
    union
    {
        // 基本类型
        BasicType basic;
        // 数组类型信息包括元素类型与数组大小构成
        struct
        {
            Type elem;
            int size;
        } array;
        // 结构体类型信息是一个链表
        FieldList structure;
        // 函数的类型
        struct
        {
            FieldList args; // 参数
            Type ret_type;  // 返回值类型
            int argc;       // 参数数量
            Funstate state; // 状态
            int lineno;     // 行数
        } function;
    } u;
};

// Struct中每个域的定义
typedef struct FieldList_
{
    char *name;     // 域的名字
    Type type;      // 域的类型
    FieldList tail; // 下一个域
} fieldlist;

// 每个符号的定义
typedef struct Object
{
    char *name;
    int stack_depth;
    Type type;
    pobj hash_next;
    pobj stack_next;
} obj;

// 符号表中hash table的定义
typedef struct HashTable
{
    pobj *hashlist
} hashtab;

// 符号表中stack的定义
typedef struct Stack
{
    pobj *stacklist;
    int cur_stack_depth;
} stack;

// 符号表的定义
typedef struct ObjTable
{
    hashtab *hashtab;
    stack *st;
    int unnamed_struct_num; // 匿名结构体
} objtab;

extern ptab table;

static inline void semanticError(ErrorType type, int line, char *msg)
{
    printf("Error type %d at Line %d: %s\n", type, line, msg);
}

// TYPE
Type newTYPE(TypeKind kind, ...);
Type copyTYPE(Type type);
int equalType(Type type1, Type type2);
void freeType(Type type);

// Field
FieldList newFieldList(char *name, Type type);
FieldList copyFieldList(FieldList field);
void append_FieldList(FieldList pField, FieldList F_to_append);
int equalFieldList(FieldList field1, FieldList field2);
void freeFieldList(FieldList field);

// Object
pobj newObj(char *name, int st_depth, Type type);
void freeObj(pobj obj);

// Hashtable
phash newHash();
void insert_hashobj(phash hashtab, pobj obj);
obj *searchhash(phash hashtab, char *name);

// Stack
pstack newStack();
void insert_stackobj(pstack stack, pobj obj);
void incStackDepth(pstack stack);
void decStackDepth(pstack stack);

// ObjTable
ptab newTable();
void insert_tabobj(ptab table, pobj obj);
void delete_stack_curdepth(ptab table);
obj *searchtab(ptab table, char *name);
obj *searchtab_func(ptab table, char *name);
int objConflict(ptab table, pobj obj);

// SDD
void Program(Node *node);
void ExtDefList(Node *node);
void ExtDef(Node *node);
void ExtDecList(Node *node, Type deftype);
pobj FunDec(Node *node, Type rettype, Funstate isdef);
void VarList(Node *node, Type funtype);
FieldList ParamDec(Node *node);
void CompSt(Node *node, pobj funobj);
void StmtList(Node *node, pobj funobj);
void Stmt(Node *node, pobj funobj);
void DefList(Node *node, FieldList structfield);
void Def(Node *node, FieldList structfield);
void DecList(Node *node, Type deftype, FieldList structfield);
void Dec(Node *node, Type deftype, FieldList structfield);
void *VarDec(Node *node, Type deftype, FieldList structfield);
Type Exp(Node *node, int *plval);
int Args(Node *node, FieldList args, int fline);

Type Specifier(Node *node);
Type StructSpecifier(Node *node);

// utils
int isStructType(pobj obj);
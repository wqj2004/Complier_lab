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

typedef enum _errorType
{
    UNDEF_VAR = 1,        // Undefined Variable
    UNDEF_FUNC,           // Undefined Function
    REDEF_VAR,            // Redefined Variable
    REDEF_FUNC,           // Redefined Function
    TYPE_MISMATCH_ASSIGN, // Type mismatchedfor assignment.
    LEFT_VAR_ASSIGN,      // The left-hand side of an assignment must be a variable.
    TYPE_MISMATCH_OP,     // Type mismatched for operands.
    TYPE_MISMATCH_RETURN, // Type mismatched for return.
    FUNC_AGRC_MISMATCH,   // Function is not applicable for arguments
    NOT_A_ARRAY,          // Variable is not a Array
    NOT_A_FUNC,           // Variable is not a Function
    NOT_A_INT,            // Variable is not a Integer
    ILLEGAL_USE_DOT,      // Illegal use of "."
    NONEXISTFIELD,        // Non-existentfield
    REDEF_FEILD,          // Redefined field
    DUPLICATED_NAME,      // Duplicated name
    UNDEF_STRUCT          // Undefined structure
} ErrorType;

struct Type_
{
    enum
    {
        BASIC,
        ARRAY,
        STRUCTURE,
        FUNCTION,
        STRUCTURE_DEF
    } kind;
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
            int state;      // 状态
            int lineno;     // 行数
        } function;
    } u;
};

// Struct中每个域的定义
struct FieldList_
{
    char *name;     // 域的名字
    Type type;      // 域的类型
    FieldList tail; // 下一个域
};

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

// Hashtable
phash newHash();
void insert_hashobj(phash hashtab, pobj obj);
obj *searchhash(phash hashtab, char *name);

// Stack
pstack newStack();
void insert_stackobj(pstack stack, pobj obj);
void addStackDepth(pstack stack);
void delete_curstack(pstack stack);

// ObjTable
ptab newTable();
void insert_tabobj(ptab table, pobj obj);
void delete_stack(ptab table);
obj *searchtab(ptab table, char *name);

void checkTree(Node *node);
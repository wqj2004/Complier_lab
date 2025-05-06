#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
typedef struct Node Node;

typedef enum datatype
{
    type_nter = -1,
    type_ch = 0,
    type_int = 1,
    type_float = 2,
    type_id = 3,
    type_type = 4
} dtype;

struct Node
{
    char *name;
    dtype type;
    int fline;
    union Val
    {
        int int_val;
        float float_val;
        char id_val[33];
    } val;
    struct Node *firstchild;
    struct Node *nextsib;
};

Node *create_node(char *nodename, dtype type, int fline, int child_num, ...);

void print_tree(Node *root, int dep);

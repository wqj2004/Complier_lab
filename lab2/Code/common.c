#include "common.h"
#include <assert.h> 

char *copystr(char *str)
{
    if (str == NULL)
    {
        return NULL;
    }
    char *newstr = (char *)malloc(sizeof(char) * (strlen(str) + 1));
    assert(newstr != NULL);
    strncpy(newstr, str, strlen(str) + 1);
    return str;
}

Node *create_node(char *nodename, dtype type, int fline, int child_num, ...)
{
    Node *node = (Node *)malloc(sizeof(Node));
    node->name = copystr(nodename);
    node->type = type;
    node->fline = fline;
    node->nextsib = NULL;
    node->firstchild = NULL;
    if (child_num > 0)
    {
        va_list valist;
        va_start(valist, child_num);
        Node *cur = va_arg(valist, Node *);
        int count = 1;
        while (cur == NULL && count < child_num)
        {
            cur = va_arg(valist, Node *);
            count++;
        }
        node->firstchild = cur;
        Node *lastsib = cur;
        if (lastsib != NULL)
        {
            for (; count < child_num; count++)
            {
                cur = va_arg(valist, Node *);
                if (cur != NULL)
                {
                    lastsib->nextsib = cur;
                    lastsib = cur;
                }
            }
        }
        va_end(valist);
    }
    return node;
}

void print_tree(Node *root, int dep)
{
    if (root == NULL)
    {
        return;
    }
    for (int i = 0; i < dep; i++)
    {
        printf("  ");
    }
    if (root->type == type_nter)
    {
        printf("%s (%d)\n", root->name, root->fline);
    }
    else
    {
        if (root->type == type_ch)
        {
            printf("%s\n", root->name);
        }
        else if (root->type == type_float)
        {
            printf("%s: %f\n", root->name, root->val.float_val);
        }
        else if ((root->type == type_id) || (root->type == type_type))
        {
            printf("%s: %s\n", root->name, root->val.id_val);
        }
        else if (root->type == type_int)
        {
            printf("%s: %d\n", root->name, root->val.int_val);
        }
        else
        {
            printf("%s\n", root->name);
        }
    }
    print_tree(root->firstchild, dep + 1);
    print_tree(root->nextsib, dep);
}
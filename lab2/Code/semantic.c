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
    obj->stack_depth = cur_depth;
    obj->stack_next = obj_stack[cur_depth];
    obj_stack[cur_depth] = obj;
}

void addStackDepth(pstack stack)
{
    if (stack->cur_stack_depth < max_st_depth)
        stack->cur_stack_depth++;
}

void delete_curstack(pstack stack)
{
    int cur_depth = stack->cur_stack_depth;
    pobj *obj_stack = stack->stacklist;
    pobj obj = obj_stack[cur_depth];
    while (obj != NULL)
    {
        pobj next = obj->stack_next;
        free(obj);
        obj = next;
    }
    obj_stack[cur_depth] = NULL;
    cur_depth--;
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
void delete_stack(ptab table)
{
}
obj *searchtab(ptab table, char *name);

void checkTree(Node *node);
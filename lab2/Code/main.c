#include "semantic.h"
extern int yyparse();
extern FILE *yyin;
extern void yyrestart(FILE *input_file);
extern int synerror_count;
extern int lexerror_count;
extern Node *root;
ptab table = NULL;////////CHANGE 改成了全局变量

int main(int argc, char **argv)
{
    if (argc <= 1)
    {
        return 1;
    }
    FILE *f = fopen(argv[1], "r");
    if (!f)
    {
        perror(argv[1]);
        return 1;
    }
    yyrestart(f);
    yyparse();

    if (lexerror_count == 0 && synerror_count == 0)
    {
        table=newTable();///////////CHANGE
        Program(root);
    }

    // print_tree(root, 0);
    return 0;
}
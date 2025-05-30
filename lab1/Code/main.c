/*
#include "common.h"
extern int yyparse();
extern FILE *yyin;
extern int yydebug;
extern void yyrestart(FILE *input_file);
extern int synerror_count;
extern int lexerror_count;
extern Node *root;

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
    yydebug = 1;
    yyparse();

    if (lexerror_count == 0 && synerror_count == 0)
    {
        print_tree(root, 0);
    }

    // print_tree(root, 0);
    return 0;
}
    */
#include "common.h"
extern int yyparse();
extern FILE *yyin;
extern void yyrestart(FILE *input_file);
extern int synerror_count;
extern int lexerror_count;
extern Node *root;

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
        print_tree(root, 0);
    }

    // print_tree(root, 0);
    return 0;
}
#include "semantic.h"
#include "inter.h"
#include "simulator.h"
FILE *tmpout;
extern int yyparse();
extern FILE *yyin;
extern void yyrestart(FILE *input_file);
extern int synerror_count;
extern int lexerror_count;
extern Node *root;
ptab table; ////////CHANGE 改成了全局变量

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

    FILE *AST = fopen("ASTo1.txt", "w");
    print_tree(root, 0, AST);

    tmpout = fopen("finalout.s", "w");

    FILE *out_ir_ = fopen("ircode.txt", "w");

    char *out2_name = argv[2];

    if (lexerror_count == 0 && synerror_count == 0)
    {
        table = newTable(); ///////////CHANGE
        Program(root);

        initInterCodeGen();
        translateProgram(root);
        // FILE *out = fopen(argv[2], "w");
        // FILE *out = stdout;
        if (out_ir_)
        {   
            outputInterCode(out_ir_);
            fclose(out_ir_);
        }
        // else
        // {
        //     fprintf(stderr, "Error opening output file.\n");
        // }
        FILE *out2 = fopen(argv[2], "w");
        s_code_generate(out2);
        fclose(out2);
    }
    fclose(tmpout);
    fclose(AST);

    // 将out1.s中的内容写入到tmp.txt的后面,记得要保留原先tmp.txt中的内容。
    FILE *out1_file = fopen(out2_name, "r");
    FILE *tmp_append = fopen("finalout.s", "a");

    if (out1_file && tmp_append)
    {
        int ch;
        while ((ch = fgetc(out1_file)) != EOF)
        {
            fputc(ch, tmp_append);
        }

        // 强制刷新缓冲区
        fflush(tmp_append);

        printf("File copy completed\n");
    }
    else
    {
        if (!out1_file)
            printf("Failed to open out1.s\n");
        if (!tmp_append)
            printf("Failed to open tmp.txt\n");
    }

    if (out1_file)
        fclose(out1_file);
    if (tmp_append)
        fclose(tmp_append);
    // 删除out.s文件
    remove(out2_name);
    rename("finalout.s", out2_name);

    return 0;
}
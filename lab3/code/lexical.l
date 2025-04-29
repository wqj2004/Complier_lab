%{
    #include "syntax.tab.h"
    #include <stdio.h>
    #include <stdarg.h>
    #include <string.h>
    #include <stdlib.h>
    #include "common.h"

    #define debug 0

    int lexerror_count=0;
    extern int last_error_line;
    int yycolumn = 1;
    #define YY_USER_ACTION \
        yylloc.first_line = yylloc.last_line = yylineno; \
	    yylloc.first_column = yycolumn; \
	    yylloc.last_column = yycolumn + yyleng - 1; \
	    yycolumn += yyleng;

    void create_lexnode(char*name,dtype type,int fline)
    {
        yylval.node=create_node(name,type,fline,0);
    }

    void print_error(char type , int line, const char* msg){
        if(type=='A')
        {
            lexerror_count++;
        }
        if(line>last_error_line)
        {
            printf("Error type %c at Line %d: %s.\n", type,line, msg);
            last_error_line=line;
        }
    }
    
%}

%option yylineno
%x COMMENT
blank     [ \t\r\f\v]+
digit     [0-9]
letter    [_a-zA-Z]
INT       0|([1-9]{digit}*)
FLOAT     {digit}+"."{digit}+|{digit}*"."{digit}+[eE][+-]?{digit}+|{digit}+"."{digit}*[eE][+-]?{digit}+
ID        {letter}({digit}|{letter})*
OCT       0[0-7]+
HEX       0[Xx][0-9a-fA-F]+
SEMI      ";"
COMMA     ","
ASSIGNOP  "="
RELOP     ">"|"<"|">="|"<="|"=="|"!="
PLUS      "+"
MINUS     "-"
STAR      "*"
DIV       "/"
AND       "&&"
OR        "||"
DOT       "."
NOT       "!"
TYPE      int|float
LP        \(
RP        \)
LB        \[
RB        \]
LC        \{
RC        \}
STRUCT    struct
RETURN    return
IF        if
ELSE      else
WHILE     while
STRING      \"([^\"\n]|\\\")*\"



%%

{RELOP}       {create_lexnode("RELOP",type_ch,yylineno);strcpy((yylval.node)->val.id_val,yytext);return RELOP;}
{SEMI}        {create_lexnode("SEMI",type_ch,yylineno);return SEMI;}
{COMMA}       {create_lexnode("COMMA",type_ch,yylineno);return COMMA;}
{ASSIGNOP}    {create_lexnode("ASSIGNOP",type_ch,yylineno);return ASSIGNOP;}
{PLUS}        {create_lexnode("PLUS",type_ch,yylineno);return PLUS;}
{MINUS}       {create_lexnode("MINUS",type_ch,yylineno);return MINUS;}
{STAR}        {create_lexnode("STAR",type_ch,yylineno);return STAR;}
{DIV}         {create_lexnode("DIV",type_ch,yylineno);return DIV;}
{AND}         {create_lexnode("AND",type_ch,yylineno);return AND;}
{OR}          {create_lexnode("OR",type_ch,yylineno); return OR;}
{DOT}         {create_lexnode("DOT",type_ch,yylineno);return DOT;}
{NOT}         {create_lexnode("NOT",type_ch,yylineno);return NOT;}
{LP}          {create_lexnode("LP",type_ch,yylineno);return LP;}
{RP}          {create_lexnode("RP",type_ch,yylineno);return RP;}
{LB}          {create_lexnode("LB",type_ch,yylineno);return LB;}
{RB}          {create_lexnode("RB",type_ch,yylineno);return RB;}
{LC}          {create_lexnode("LC",type_ch,yylineno);return LC;}
{RC}          {create_lexnode("RC",type_ch,yylineno);return RC;}
{STRUCT}      {create_lexnode("STRUCT",type_ch,yylineno);return STRUCT;}
{RETURN}      {create_lexnode("RETURN",type_ch,yylineno);return RETURN;}
{IF}          {create_lexnode("IF",type_ch,yylineno); return IF;}
{ELSE}        {create_lexnode("ELSE",type_ch,yylineno);return ELSE;}
{WHILE}       {create_lexnode("WHILE",type_ch,yylineno);return WHILE;}
{TYPE}        {create_lexnode("TYPE",type_type,yylineno);strcpy((yylval.node)->val.id_val,yytext);return TYPE;}
{FLOAT}       {create_lexnode("FLOAT",type_float,yylineno);(yylval.node)->val.float_val=atof(yytext);return FLOAT;}
{INT}         {create_lexnode("INT",type_int,yylineno);(yylval.node)->val.int_val=atoi(yytext);return INT;}
{OCT}         {create_lexnode("INT",type_int,yylineno);(yylval.node)->val.int_val=(int)strtol(yytext,NULL,0);return INT;}
{HEX}         {create_lexnode("INT",type_int,yylineno);(yylval.node)->val.int_val=(int)strtol(yytext,NULL,0);return INT;}
{STRING}      {create_lexnode("STRING",type_id,yylineno);return STRING;}

0[0-7]*[89_a-zA-Z]+[0-7]*   {print_error('A',yylineno,"Illegal OCT");return INT;}
0[Xx][0-9a-fA-F]*[g-zG-Z]*[0-9a-fA-F]*    {print_error('A',yylineno,"Illegal HEX");return INT;}

{ID}          {create_lexnode("ID",type_id,yylineno);strcpy((yylval.node)->val.id_val,yytext);return ID;}
{digit}+({letter}|{digit})*           {print_error('A',yylineno,"Illegal ID");return ID;}           

\n            {yycolumn=1;}
{blank}       {}


"//"[^\n]*    {}
"/*"          {BEGIN(COMMENT);}
<COMMENT>"*/" { BEGIN(INITIAL); }
<COMMENT>\n   {yycolumn=1;}
<COMMENT>.    {}
.             {print_error('A',yylineno,"Illegal character");}

%%

























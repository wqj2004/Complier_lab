%{
    #include "lex.yy.c" 
    int synerror_count=0;
    Node* root=NULL;
    int last_error_line=0;
    extern void print_error(char type , int line, char* msg);
    extern void yyerror(char* msg);
    #define YYERROR_VERBOSE 1
%}

%union {
    struct Node* node;
}

%token <node> INT      
%token <node> FLOAT    
%token <node> ID            
%token <node> STRING
%token <node> SEMI COMMA    
%token <node> ASSIGNOP RELOP    
%token <node> PLUS MINUS STAR DIV      
%token <node> AND OR NOT       
%token <node> DOT           
%token <node> TYPE     
%token <node> LP RP LB RB LC RC       
%token <node> STRUCT RETURN IF ELSE WHILE    

%type <node> Program ExtDefList ExtDef ExtDecList   //High-level Definitions
%type <node> Specifier StructSpecifier OptTag Tag //Specifiers
%type <node> VarDec FunDec VarList ParamDec       //Declarators
%type <node> CompSt StmtList Stmt                 //Statements
%type <node> DefList Def DecList Dec              //Local Definitions
%type <node> Exp Args                             //Expressions

/*

Program                 整个程序
ExtDefList              外部/全局变量定义列表,递归定义
ExtDef                  具体的,全局变量定义列表中的每一个定义,可能是全局变量、函数声明或函数定义
ExtDecList              外部声明列表
Specifier               类型说明符
StructSpecifier         结构体说明符
OptTag                  结构体可选标识符，如果存在则为结构体名，否则为空
Tag                     结构体标识符(结构体名)
VarDec                  表示变量声明，包含变量名及其类型信息。(只能是普通变量/数组变量)
FunDec                  函数声明,由函数名,(,),还有参数列表组成
VarList                 参数列表,用于定义函数的参数
ParamDec                参数声明,定义单个参数的类型和名称
CompSt                  复合语句,表示由花括号括起来的部分
StmtList                语句列表,包含多个语句
Stmt                    表示特定的单个语句
DefList                 局部定义列表
Def                     单个局部定义
DecList                 局部声明列表
Dec                     单个局部声明
Exp                     表达式
Args                    实际参数列表

*/


%right ASSIGNOP
%left OR
%left AND 
%left RELOP
%left PLUS MINUS
%left STAR DIV
%right NOT 
%left DOT 
%left LB RB 
%left LP RP
%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

%%

// High-level Definitions
Program : ExtDefList    {   $$=create_node("Program",type_nter,@$.first_line,1,$1);
                            root = $$;
                        }//程序就是由一个外部定义列表(一系列外部定义/全局变量、函数声明、函数定义)组成的                                             
    ;           

ExtDefList : ExtDef ExtDefList  { $$=create_node("ExtDefList",type_nter,@$.first_line,2,$1,$2);} //外部定义列表的递归表示
    |  /*empty*/                { $$=NULL; }
    ;

ExtDef : Specifier ExtDecList SEMI  { $$=create_node("ExtDef",type_nter,@$.first_line,3,$1,$2,$3); } //外部定义要么是一个类型声明+外部(变量)声明
    | Specifier SEMI                { $$=create_node("ExtDef",type_nter,@$.first_line,2,$1,$2); } //要么是一个类型声明+;(一般和struct一起使用)
    | Specifier FunDec CompSt       { $$=create_node("ExtDef",type_nter,@$.first_line,3,$1,$2,$3); }//要么是一个类型声明+函数声明+{...}
    | error SEMI                    {yyerror("syntax vardef error"); yyerrok;}
    ;  

ExtDecList : VarDec             { $$=create_node("ExtDecList",type_nter, @$.first_line,1,$1); }  //外部声明要么是一个单独的变量声明
    | VarDec COMMA ExtDecList   { $$=create_node("ExtDecList",type_nter, @$.first_line,3,$1,$2,$3); }//要么就是多个变量声明而中间用,连接
    ;

// Specifiers
Specifier : TYPE        { $$=create_node("Specifier",type_nter, @$.first_line,1,$1); }//要么就是基本类型(int/float)
    | StructSpecifier   { $$=create_node("Specifier",type_nter, @$.first_line,1,$1); }//要么就是复合类型(struct)
    ;

StructSpecifier : STRUCT OptTag LC DefList RC   { $$=create_node("StructSpecifier",type_nter, @$.first_line,5,$1,$2,$3,$4,$5); }
    | STRUCT Tag                                { $$=create_node("StructSpecifier",type_nter, @$.first_line,2,$1,$2); }//结构体标识可以只是一个声明,也可以是一个完整的定义(有定义就可以是匿名结构体)
    ;

OptTag : ID     { $$=create_node("OptTag",type_nter, @$.first_line,1,$1); }//结构体可选标识符，主要用于给结构体命名
    | /*empty*/           { $$=NULL; }
    ;

Tag : ID    { $$=create_node("Tag",type_nter, @$.first_line,1,$1); }
    ;


//Declarators
VarDec : ID                 { $$=create_node("VarDec",type_nter, @$.first_line,1,$1); } //普通变量
    | VarDec LB INT RB      { $$=create_node("VarDec",type_nter, @$.first_line,4,$1,$2,$3,$4); }//数组变量
    | error RB              { yyerror("syntax VarDec error"); yyerrok;}
    ; 

FunDec : ID LP VarList RP   { $$=create_node("FunDec",type_nter, @$.first_line,4,$1,$2,$3,$4); } //有参数的函数声明
    | ID LP RP              { $$=create_node("FunDec",type_nter, @$.first_line,3,$1,$2,$3); }    //无参数的函数声明
    | error RP              { yyerror("syntax FunDec error"); yyerrok;}
    ;

VarList : ParamDec COMMA VarList    { $$=create_node("VarList",type_nter, @$.first_line,3,$1,$2,$3); }//多个变量，采用递归定义
    | ParamDec                      { $$=create_node("VarList",type_nter, @$.first_line,1,$1); }      //单个变量
    ;

ParamDec : Specifier VarDec     { $$=create_node("ParamDec",type_nter, @$.first_line,2,$1,$2); } //单个参数声明的具体形式:标识符+变量
    ;

/* Statements */
CompSt : LC DefList StmtList RC     { $$=create_node("CompSt",type_nter, @$.first_line,4,$1,$2,$3,$4); }//复合语句
    | error RC                      { yyerror("syntax Comp_LC error"); yyerrok;}
    ;

StmtList : Stmt StmtList    { $$=create_node("StmtList",type_nter, @$.first_line,2,$1,$2); }//语句列表
    |                       { $$=NULL; }
    ;

Stmt : Exp SEMI                                 { $$=create_node("Stmt",type_nter, @$.first_line,2,$1,$2); }//表达式
    | CompSt                                    { $$=create_node("Stmt",type_nter, @$.first_line,1,$1); }//复合语句(嵌套)
    | RETURN Exp SEMI                           { $$=create_node("Stmt",type_nter, @$.first_line,3,$1,$2,$3); }//返回语句
    | IF LP Exp RP Stmt %prec LOWER_THAN_ELSE   { $$=create_node("Stmt",type_nter, @$.first_line,5,$1,$2,$3,$4,$5); }//if语句,无else
    | IF LP Exp RP Stmt ELSE Stmt               { $$=create_node("Stmt",type_nter, @$.first_line,7,$1,$2,$3,$4,$5,$6,$7); }//if...else
    | WHILE LP Exp RP Stmt                      { $$=create_node("Stmt",type_nter, @$.first_line,5,$1,$2,$3,$4,$5); }//while语句 
    | error SEMI                                { yyerror("syntax Stmt error"); yyerrok;}
    ;

// Local Definitions
DefList : Def DefList       { $$=create_node("DefList",type_nter, @$.first_line,2,$1,$2); }//局部定义
    |                       { $$=NULL; }
    ;

Def : Specifier DecList SEMI    { $$=create_node("Def",type_nter, @$.first_line,3,$1,$2,$3); } //单个局部定义,由标识符+声明列表+;组成
    ;

DecList : Dec               { $$=create_node("DecList",type_nter, @$.first_line,1,$1); }//声明列表,由若干单个声明组成，中间由,连接
    | Dec COMMA DecList     { $$=create_node("DecList",type_nter, @$.first_line,3,$1,$2,$3); }
    ;

Dec : VarDec                { $$=create_node("Dec",type_nter, @$.first_line,1,$1); }//单个变量声明,分为带初值/不带初值地声明
    | VarDec ASSIGNOP Exp   { $$=create_node("Dec",type_nter, @$.first_line,3,$1,$2,$3); }
    ;

// Expressions
Exp : Exp ASSIGNOP Exp      { $$=create_node("Exp",type_nter, @$.first_line,3,$1,$2,$3); }
    | Exp AND Exp           { $$=create_node("Exp",type_nter, @$.first_line,3,$1,$2,$3); }
    | Exp OR Exp            { $$=create_node("Exp",type_nter, @$.first_line,3,$1,$2,$3); }
    | Exp RELOP Exp         { $$=create_node("Exp",type_nter, @$.first_line,3,$1,$2,$3); }
    | Exp PLUS Exp          { $$=create_node("Exp",type_nter, @$.first_line,3,$1,$2,$3); }
    | Exp MINUS Exp         { $$=create_node("Exp",type_nter, @$.first_line,3,$1,$2,$3); }
    | Exp STAR Exp          { $$=create_node("Exp",type_nter, @$.first_line,3,$1,$2,$3); }
    | Exp DIV Exp           { $$=create_node("Exp",type_nter, @$.first_line,3,$1,$2,$3); }
    | LP Exp RP             { $$=create_node("Exp",type_nter, @$.first_line,3,$1,$2,$3); }
    | MINUS Exp             { $$=create_node("Exp",type_nter, @$.first_line,2,$1,$2); }
    | NOT Exp               { $$=create_node("Exp",type_nter, @$.first_line,2,$1,$2); }
    | ID LP Args RP         { $$=create_node("Exp",type_nter, @$.first_line,4,$1,$2,$3,$4); }//函数调用(有参数)
    | ID LP RP              { $$=create_node("Exp",type_nter, @$.first_line,3,$1,$2,$3); }//函数调用(无参数)
    | Exp LB Exp RB         { $$=create_node("Exp",type_nter, @$.first_line,4,$1,$2,$3,$4); }//数组元素
    | Exp DOT ID            { $$=create_node("Exp",type_nter, @$.first_line,3,$1,$2,$3); }//结构体取元素
    | ID                    { $$=create_node("Exp",type_nter, @$.first_line,1,$1); }
    | INT                   { $$=create_node("Exp",type_nter, @$.first_line,1,$1); }
    | FLOAT                 { $$=create_node("Exp",type_nter, @$.first_line,1,$1); }
    | STRING                { $$=create_node("Exp",type_nter, @$.first_line,1,$1); }
    ;
Args : Exp COMMA Args       { $$=create_node("Args",type_nter, @$.first_line,3,$1,$2,$3); }//非空参数列表,由","间隔，每一个参数是一个exp
    | Exp                   { $$=create_node("Args",type_nter, @$.first_line,1,$1); }
    ;
%%

void yyerror(char* msg)
{
    synerror_count++;
    print_error('B',yylineno,msg);
}































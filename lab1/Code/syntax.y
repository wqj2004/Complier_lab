%{
    #include "lex.yy.c" 
    //#include "common.h"   
    //extern void yyerrok;
    int synerror_count=0;
    Node* root=NULL;
    int last_error_line=0;
    extern void print_error(char type , int line, char* msg);
    void yyerror(char* msg)
    {
        if(yylineno>last_error_line)
        {
            synerror_count++;
            print_error('B',yylineno,msg);
            //printf("lastline:%d\n",last_error_line);
            //last_error_line=yylineno;
        }
    }

    
    
%}

%union {
    struct Node* node;
}

%token <node> INT      
%token <node> FLOAT    
%token <node> ID            
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


%nonassoc error  //傻逼吧
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
//    | error SEMI                    {yyerrok;}
//    | Specifier error               {yyerrok;}
//    | Specifier error SEMI          {yyerrok;}
    //| Specifier error CompSt        {yyerrok;}
    | error SEMI                    {yyerror("syntax vardef error");}
    | error FunDec CompSt           {print_error('B',@1.first_line,"syntax ExtDef spec error");}

    | error CompSt                  {print_error('B',@1.first_line,"syntax ExtDef error");}

    ;  

ExtDecList : VarDec             { $$=create_node("ExtDecList",type_nter, @$.first_line,1,$1); }  //外部声明要么是一个单独的变量声明
    | VarDec COMMA ExtDecList   { $$=create_node("ExtDecList",type_nter, @$.first_line,3,$1,$2,$3); }//要么就是多个变量声明而中间用,连接
    ;

// Specifiers
Specifier : TYPE        { $$=create_node("Specifier",type_nter, @$.first_line,1,$1); }//要么就是基本类型(int/float)
    | StructSpecifier   { $$=create_node("Specifier",type_nter, @$.first_line,1,$1); }//要么就是复合类型(struct)
//    | error             {yyerror("syntax specifier error");yyerrok; }          
    ;

StructSpecifier : STRUCT OptTag LC DefList RC   { $$=create_node("StructSpecifier",type_nter, @$.first_line,5,$1,$2,$3,$4,$5); }
    | STRUCT Tag                                { $$=create_node("StructSpecifier",type_nter, @$.first_line,2,$1,$2); }//结构体标识可以只是一个声明,也可以是一个完整的定义(有定义就可以是匿名结构体)
    //| error RC                                  {yyerror("syntax error");yyerrok;}
//    | STRUCT OptTag LC DefList error RC          {yyerrok;}
    | STRUCT error LC DefList RC                {yyerror("syntax struct_optag error");}
    | STRUCT error RC                           {yyerror("syntax struct_def error");} 
    | STRUCT OptTag LC DefList error            {yyerror("syntax struct_RC error");}
    ;

OptTag : ID     { $$=create_node("OptTag",type_nter, @$.first_line,1,$1); }//结构体可选标识符，主要用于给结构体命名
    | /*empty*/           { $$=NULL; }
    ;

Tag : ID    { $$=create_node("Tag",type_nter, @$.first_line,1,$1); }
    ;


//Declarators
VarDec : ID                 { $$=create_node("VarDec",type_nter, @$.first_line,1,$1); } //普通变量
    | VarDec LB INT RB      { $$=create_node("VarDec",type_nter, @$.first_line,4,$1,$2,$3,$4); }//数组变量
    | VarDec LB error RB    { yyerror("syntax array_error error");}
    | VarDec LB INT error   { yyerror("syntax VarDec_RB error");}
//    | error LB INT RB       {yyerrok;}
    ; 

FunDec : ID LP VarList RP   { $$=create_node("FunDec",type_nter, @$.first_line,4,$1,$2,$3,$4); } //有参数的函数声明
    | ID LP RP              { $$=create_node("FunDec",type_nter, @$.first_line,3,$1,$2,$3); }    //无参数的函数声明
    | ID LP error RP        { yyerror("syntax FunDec_VarList error");}
//    | error RP              { yyerror("syntax FunDec error");}

//    | ID error RP           {yyerrok;}
    | ID LP VarList error           {yyerror("syntax FunDec_RP error");/*yyerrok;*/}
//    | ID LP error RP        {yyerror("syntax FunDec_varlst error");/*yyerrok;*/}
//    | error RP              {yyerrok;}
//    | error LP VarList RP   {yyerrok;}
    ;

VarList : ParamDec COMMA VarList    { $$=create_node("VarList",type_nter, @$.first_line,3,$1,$2,$3); }//多个变量，采用递归定义
    | ParamDec                      { $$=create_node("VarList",type_nter, @$.first_line,1,$1); }      //单个变量
    ;

ParamDec : Specifier VarDec     { $$=create_node("ParamDec",type_nter, @$.first_line,2,$1,$2); } //单个参数声明的具体形式:标识符+变量
    ;

/* Statements */
CompSt : LC DefList StmtList RC     { $$=create_node("CompSt",type_nter, @$.first_line,4,$1,$2,$3,$4); }//复合语句
    | error DefList StmtList RC     {yyerror("syntax Comp_LC error");};
//    | LC DefList error RC            {yyerrok;}
//    | LC error StmtList RC                   {yyerrok;}
    

StmtList : Stmt StmtList    { $$=create_node("StmtList",type_nter, @$.first_line,2,$1,$2); }//语句列表
    |                       { $$=NULL; }
    ;

Stmt : Exp SEMI                                 { $$=create_node("Stmt",type_nter, @$.first_line,2,$1,$2); }//表达式
    | CompSt                                    { $$=create_node("Stmt",type_nter, @$.first_line,1,$1); }//复合语句(嵌套)
    | RETURN Exp SEMI                           { $$=create_node("Stmt",type_nter, @$.first_line,3,$1,$2,$3); }//返回语句
    | IF LP Exp RP Stmt %prec LOWER_THAN_ELSE   { $$=create_node("Stmt",type_nter, @$.first_line,5,$1,$2,$3,$4,$5); }//if语句,无else
    | IF LP Exp RP Stmt ELSE Stmt               { $$=create_node("Stmt",type_nter, @$.first_line,7,$1,$2,$3,$4,$5,$6,$7); }//if...else
    | WHILE LP Exp RP Stmt                      { $$=create_node("Stmt",type_nter, @$.first_line,5,$1,$2,$3,$4,$5); }//while语句 
//    | error RP                                  {yyerror("syntax Stmt_exp error");yyerrok;} 
//    | Exp error                                 {}
    | error SEMI                                {print_error('B',@1.first_line,"syntax stmt_semi error");}
//    | error CompSt                              {print_error('B',@1.first_line,"syntax stmt_error_com error");}
 //    | RETURN Exp error                          {yyerror("syntax Stmt_ret_semi error");}
    | IF LP error RP Stmt %prec LOWER_THAN_ELSE   {print_error('B',@3.first_line,"syntax stmt_if error");}  
    //| IF error  Stmt %prec LOWER_THAN_ELSE   {yyerror("syntax Stmt_if_RP error");} 
    | IF LP error RP Stmt ELSE Stmt             {print_error('B',@3.first_line,"syntax stmt_if_else error");}  
//    | IF LP Exp error Stmt ELSE Stmt            {yyerror("syntax Stmt_ifel_RP error");}  
    | WHILE LP error RP Stmt                    {print_error('B',@3.first_line,"syntax stmt_while error");}
    | WHILE error Stmt                          {print_error('B',@2.first_line,"syntax stmt_while error");}
 //    | WHILE LP Exp error Stmt                    {yyerror("syntax Stmt_while_RP error");}
//    | error LP Exp RP Stmt %prec LOWER_THAN_ELSE   {yyerrok;} 
//    | error LP Exp RP Stmt ELSE Stmt             {yyerrok;} 
    ;//具体的语句

// Local Definitions
DefList : Def DefList       { $$=create_node("DefList",type_nter, @$.first_line,2,$1,$2); }//局部定义
    |                       { $$=NULL; }
    ;

Def : Specifier DecList SEMI    { $$=create_node("Def",type_nter, @$.first_line,3,$1,$2,$3); } //单个局部定义,由标识符+声明列表+;组成
//    | Specifier error SEMI                {yyerrok;}
//    | Specifier DecList error              {yyerrok;}
    | Specifier error SEMI                {yyerror("syntax Def error");}
    | Specifier DecList error             {yyerror("syntax Def_semi error");}
    ;

DecList : Dec               { $$=create_node("DecList",type_nter, @$.first_line,1,$1); }//声明列表,由若干单个声明组成，中间由,连接
    | Dec COMMA DecList     { $$=create_node("DecList",type_nter, @$.first_line,3,$1,$2,$3); }
    ;

Dec : VarDec                { $$=create_node("Dec",type_nter, @$.first_line,1,$1); }//单个变量声明,分为带初值/不带初值地声明
    | VarDec ASSIGNOP Exp   { $$=create_node("Dec",type_nter, @$.first_line,3,$1,$2,$3); }
//    | VarDec ASSIGNOP error { yyerror("syntax Dec_exp error");}
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
//    | Exp ASSIGNOP error    {}
//    | Exp AND error         {}
//    | Exp OR error          {}
//    | Exp RELOP error       {}
//    | Exp PLUS error        {}
//    | Exp MINUS error       {}
//    | Exp STAR error        {}
//    | Exp DIV error         {}
//    | LP error RP           {}
//    | LP Exp error          {}
//    | MINUS error           {}
//    | NOT error             {}
//    | ID LP error RP        {}
//    | ID LP error SEMI      {}
//    | Exp LB error RB       {}

//    | ID LP error SEMI      {yyerrok;}
    //| error RP              {yyerror("syntax error");yyerrok;}
    ;
Args : Exp COMMA Args       { $$=create_node("Args",type_nter, @$.first_line,3,$1,$2,$3); }//非空参数列表,由","间隔，每一个参数是一个exp
    | Exp                   { $$=create_node("Args",type_nter, @$.first_line,1,$1); }
//    | error COMMA Exp                               {}
    ;
%%

































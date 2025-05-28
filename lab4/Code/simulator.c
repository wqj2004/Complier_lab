#include "simulator.h"

extern CodeList code_list;
//extern struct Register _reg[32];
struct Register _reg[32];

int in_func=0;//表示当前是否在函数中
char *cur_fun_name;

extern FILE *tmpout;

static int spill_counter = 0;

int last_changed_reg=0;
struct VarDesc* var_head;
struct VarDesc* var_cur;
struct VarDesc* memory_var_head;
struct VarDesc* memory_var_cur;

// 全局变量存储溢出变量信息
static MemVarDesc* spill_mem_head = NULL;
static MemVarDesc* spill_mem_cur = NULL;

// 操作数比较函数
int operand_equal(pOperand op1, pOperand op2) {
    if (op1->kind != op2->kind) return 0;
    
    switch (op1->kind) {
        case VARIABLE:
            return strcmp(op1->u.name, op2->u.name) == 0;
        case TEMPORARY:
            return op1->u.var_no == op2->u.var_no;
        case CONSTANT:
            return op1->u.value == op2->u.value;
        default:
            return 0;
    }
}

void add_var_desc(int reg,pOperand op){
	struct VarDesc* new_var = (struct VarDesc*)malloc(sizeof(struct VarDesc));
	new_var->op=op;
	new_var->next=NULL;
	new_var->reg_num=reg;
	if(var_head==NULL)
	{
		var_head = new_var;
		var_cur=new_var;
	}
	else
	{
		var_cur->next=new_var;
		var_cur=new_var;
	}
}

void del_var_desc(struct VarDesc* var){
	if(var == var_head)
	{
		var_head=var_head->next;
	}
	else
	{
		struct VarDesc* p = var_head;
		while(p!=NULL && p->next!=var)
		{
			p=p->next;
		}
		if(var_cur == var)
			var_cur = p;
		p->next=var->next;
	}
}

void add_memory_var_desc(pOperand op){//只用来添加函数参数
	struct VarDesc* new_var = (struct VarDesc*)malloc(sizeof(struct VarDesc));
	new_var->op=op;
	new_var->next=NULL;
	if(memory_var_head==NULL)
	{
		memory_var_head = new_var;
		memory_var_cur=new_var;
	}
	else
	{
		memory_var_cur->next=new_var;
		memory_var_cur=new_var;
	}
}

void del_memory_var_desc(struct VarDesc* var){
	if(var == memory_var_head)
	{
		memory_var_head=memory_var_head->next;
	}
	else
	{
		struct VarDesc* p = memory_var_head;
		while(p!=NULL && p->next!=var)
		{
			p=p->next;
		}
		if(memory_var_cur == var)
			memory_var_cur = p;
		p->next=var->next;
	}
}

// 添加溢出变量到内存描述符链表
void add_spill_mem_var_desc(pOperand op, char* label) {
    MemVarDesc* new_mem_var = (MemVarDesc*)malloc(sizeof(MemVarDesc));
    new_mem_var->op = op;
    new_mem_var->global_label = label;
    new_mem_var->next = NULL;
    
    if (spill_mem_head == NULL) {
        spill_mem_head = new_mem_var;
        spill_mem_cur = new_mem_var;
    } else {
        spill_mem_cur->next = new_mem_var;
        spill_mem_cur = new_mem_var;
    }
}

// 查找溢出变量
MemVarDesc* find_mem_var(pOperand op) {
    MemVarDesc* p = spill_mem_head;
    while (p != NULL) {
        if (operand_equal(p->op, op)) {
            return p;
        }
        p = p->next;
    }
    return NULL;
}

// 删除溢出变量描述符
void del_spill_mem_var_desc(MemVarDesc* mem_var) {
    if (mem_var == spill_mem_head) {
        spill_mem_head = spill_mem_head->next;
        if (spill_mem_cur == mem_var) {
            spill_mem_cur = spill_mem_head;
        }
    } else {
        MemVarDesc* p = spill_mem_head;
        while (p != NULL && p->next != mem_var) {
            p = p->next;
        }
        if (p != NULL) {
            if (spill_mem_cur == mem_var) {
                spill_mem_cur = p;
            }
            p->next = mem_var->next;
        }
    }
    free(mem_var->global_label);
    free(mem_var);
}


void spill_to_memory(FILE *fp,struct VarDesc* var_desc) {
    pOperand op = var_desc->op;
    int reg_num = var_desc->reg_num;
    
    // 生成唯一的全局标签
    char* label = malloc(32);
    sprintf(label, "_spill_%d", spill_counter++);
    
    // 在数据段声明变量
    fprintf(tmpout, "%s: .word 0\n", label);
    
    // 存储到全局地址
    fprintf(fp, "  sw %s, %s\n", _reg[reg_num].name, label);
    
    add_spill_mem_var_desc(op, label);
}


int allocate(pOperand op,FILE *fp){
	for(int i=8;i<26;i++)
	{
		if(!_reg[i].state)//空闲
		{
			_reg[i].state=1;
			add_var_desc(i,op);
			return i;
		}
	}
	//没有空闲寄存器
	struct VarDesc* p = var_head;
	while(p!=NULL)
	{
		if(p->op->kind == CONSTANT && p->reg_num!=last_changed_reg){//如果是常量且不是最后改变的寄存器
			last_changed_reg = p->reg_num;
			del_var_desc(p);
			add_var_desc(last_changed_reg,op);
			return last_changed_reg;
		}
		p=p->next;
	}
	
	p = var_head;
	while(p!=NULL)
	{
		if(p->op->kind==TEMPORARY && p->reg_num != last_changed_reg)//遍历变量描述符，临时变量且不是最后改变的直接释放
		{
			last_changed_reg = p->reg_num;
			del_var_desc(p);
			add_var_desc(last_changed_reg,op);
			return last_changed_reg;
		}
		p=p->next;
	}

	p = var_head;
    while (p != NULL) {
        if (p->op->kind == VARIABLE && p->reg_num != last_changed_reg) {
            // 生成溢出代码：将寄存器内容存储到内存
            spill_to_memory(fp, p);
            
            int spilled_reg = p->reg_num;
            last_changed_reg = spilled_reg;
            del_var_desc(p);
            add_var_desc(spilled_reg, op);
            return spilled_reg;
        }
        p = p->next;
    }
    
    // 5. 最后的保底策略 - 溢出第一个非最近使用的寄存器
    p = var_head;
    if (p != NULL && p->reg_num != last_changed_reg) {
        spill_to_memory(fp, p);
        
        int spilled_reg = p->reg_num;
        last_changed_reg = spilled_reg;
        del_var_desc(p);
        add_var_desc(spilled_reg, op);
        return spilled_reg;
    }

	return 8;

}


int load_from_memory(FILE *fp,pOperand op) {
    MemVarDesc* mem_var = find_mem_var(op);
    if (mem_var) {
        int reg = allocate(op,fp);
        // 从全局地址加载
        fprintf(fp, "  lw %s, %s\n", 
                _reg[reg].name, mem_var->global_label);
        del_spill_mem_var_desc(mem_var);
        return reg;
    }
    return 0;
}

int ensure(pOperand op,FILE *fp){//保证op在寄存器中，返回寄存器编号
	struct VarDesc* p = var_head;
	while (p != NULL) {
        if (operand_equal(p->op, op)) {
            return p->reg_num;
        }
        p = p->next;
    }


	int tmp = load_from_memory(tmpout, op);
	if (tmp != 0) {
		// 如果从内存加载成功，返回寄存器编号
		return tmp;
	}

	int result = allocate(op,fp);
	return result;

}

void init_reg(){
    for(int i=0;i<32;++i){
        _reg[i].state=0;
    }
    _reg[0].name="$zero";
    _reg[1].name="$at";
    _reg[2].name="$v0";
	_reg[3].name="$v1";
	_reg[4].name="$a0";
	_reg[5].name="$a1";
	_reg[6].name="$a2";
	_reg[7].name="$a3";
	_reg[8].name="$t0";
	_reg[9].name="$t1";
	_reg[10].name="$t2";
	_reg[11].name="$t3";
	_reg[12].name="$t4";
	_reg[13].name="$t5";
	_reg[14].name="$t6";
	_reg[15].name="$t7";
	_reg[16].name="$s0";
	_reg[17].name="$s1";
	_reg[18].name="$s2";
	_reg[19].name="$s3";
	_reg[20].name="$s4";
	_reg[21].name="$s5";
	_reg[22].name="$s6";
	_reg[23].name="$s7";
	_reg[24].name="$t8";
	_reg[25].name="$t9";
	_reg[26].name="$k0";
	_reg[27].name="$k1";
	_reg[28].name="$gp";
	_reg[29].name="$sp";
	_reg[30].name="$fp";
	_reg[31].name="$ra";


}

void init_data_text(FILE* fp){
    
	fprintf(fp,".data\n");
	fprintf(fp, "_prompt: .asciiz \"Enter an integer:\"\n");
    fprintf(fp, "_ret: .asciiz \"\\n\"\n");
    fprintf(fp, ".globl main\n");
	pInstruction curr = code_list.head;
    while (curr != NULL) {
    	if(curr->kind == DEC)
    	{
    		fprintf(fp, "%s: .space %d\n",curr->u.dec.op->u.name, curr->u.dec.size);
    	} 			
        curr = curr->next;
    }

}

void init_var_list(){
	var_head=NULL;
	var_cur=NULL;
	memory_var_head=NULL;
	memory_var_cur=NULL;
}

void store_reg(FILE* fp){
	//将寄存器中的变量存入内存
	fprintf(fp, "  addi $sp, $sp, -72\n");
	for(int i=8;i<26;i++)
	{
		fprintf(fp, "  sw %s, %d($sp)\n", _reg[i].name,(i-8)*4);
	}
}

void load_reg(FILE* fp){
	//将内存中的变量加载到寄存器
	for(int i=8;i<26;i++)
	{
		fprintf(fp, "  lw %s, %d($sp)\n", _reg[i].name,(i-8)*4);
	}
	fprintf(fp, "  addi $sp, $sp, 72\n");
}

void print_relop(char*relop,FILE*fp){
	if(strcmp(relop,"==")==0){
		fprintf(fp,"beq ");
	}else if(strcmp(relop,"!=")==0){
		fprintf(fp,"bne ");
	}else if(strcmp(relop,">")==0){
		fprintf(fp,"bgt ");
	}else if(strcmp(relop,"<")==0){
		fprintf(fp,"blt ");
	}else if(strcmp(relop,">=")==0){
		fprintf(fp,"bge ");
	}else if(strcmp(relop,"<=")==0){
		fprintf(fp,"ble ");
	}
}

pInstruction find_pre_instr(pInstruction curr){
	pInstruction pre = code_list.head;
	while(pre != NULL && pre->next!=NULL && pre->next != curr)
	{
		pre = pre->next;
	}
	if(pre == NULL)
	{
		return NULL;
	}
	return pre;
}

struct VarDesc* find_the_var_desc_in_mem(int num){
	//输出全部op的value值
	printf("%d\n",num);
	struct VarDesc* p1 = memory_var_head;
	while(p1!=NULL)
	{
		printf("op: %s, reg_num: %d\n", p1->op->u.name, p1->reg_num);
		p1 = p1->next;
	}
	
	//vacdesc next n-1 次之后是memory_var_cur
	struct VarDesc* p = memory_var_head;
	while(p!=NULL)
	{
		struct VarDesc* tmp = p;
		for(int i=0;i<num-1;i++)
		{
			tmp = tmp->next;
		}
		if(tmp == memory_var_cur)
		{
			return p;
		}
		p=p->next;
	}
	assert(0);//没有找到
}


void trans_one_line(pInstruction curr,FILE *fp){
	switch (curr->kind) {
		case LABEL:{
			fprintf(fp,"label%d:\n",curr->u.singleop.op->u.var_no);
			break;
		}      // LABEL x:
		//case FUNCTION_IR_:{
		case 3:{
			fprintf(fp,"\n%s:\n",curr->u.singleop.op->u.name);
			//清空寄存器
			for(int i=8;i<26;i++)
			{
				_reg[i].state=0;
			}
			//清空变量描述符
			var_head=NULL;
			var_cur=NULL;
			memory_var_head=NULL;
			memory_var_cur=NULL;
			spill_mem_head = NULL;
			spill_mem_cur = NULL;

			//加载参数
			if(strcmp(curr->u.singleop.op->u.name,"main")==0)//main函数
			{
				in_func=0;
				cur_fun_name=NULL;
			}
			else
			{
				in_func=1;
				cur_fun_name = curr->u.singleop.op->u.name;
				//找到当前函数的参数数量
				int param_num = find_param_num(cur_fun_name);
				int cnt = 0;
				pInstruction tmp = curr->next;
				while(tmp != NULL && tmp->kind == PARAM)
				{	
					if(cnt<4)//直接存入寄存器
					{
						add_var_desc(4+cnt,tmp->u.singleop.op);
					}
					else//参数大于四个则加载到栈中，再加入到寄存器
					{
						int result_reg_num = ensure(tmp->u.singleop.op,fp);
						fprintf(fp, "  lw %s, %d($fp)\n", _reg[result_reg_num].name,(param_num-1-cnt)*4);
						add_var_desc(result_reg_num,tmp->u.singleop.op);
					}
					cnt++;
					tmp = tmp->next;
				}
			}
			break;
		}   // FUNCTION f:
		case ASSIGN:{
			int result_reg_num = ensure(curr->u.assign.result,fp);
			if(curr->u.assign.op1->kind == CONSTANT){
				fprintf(fp,"  li %s, %d\n",_reg[result_reg_num].name,curr->u.assign.op1->u.value);
			}
			else{
				int op_reg_num = ensure(curr->u.assign.op1,fp);
				fprintf(fp, "  move %s, %s\n", _reg[result_reg_num].name, _reg[op_reg_num].name);  
			}
			break;
		}     // x := y
		case ADD:{
			pOperand result = curr->u.binop.result;
			pOperand op1 = curr->u.binop.op1;
			pOperand op2 = curr->u.binop.op2;
			int result_reg_num = ensure(result,fp);
			if(op1->kind == CONSTANT && op2->kind == CONSTANT){
				fprintf(fp,"  li %s, %d\n",_reg[result_reg_num].name,op1->u.value + op2->u.value);
			}
			else if(op1->kind == CONSTANT){
				int op2_reg_num = ensure(op2,fp);
				fprintf(fp,"  addi %s, %s, %d\n",_reg[result_reg_num].name,_reg[op2_reg_num].name,op1->u.value);
			}
			else if(op2->kind == CONSTANT){
				int op1_reg_num = ensure(op1,fp);
				fprintf(fp,"  addi %s, %s, %d\n",_reg[result_reg_num].name,_reg[op1_reg_num].name,op2->u.value);
			}
			else{
				int op1_reg_num = ensure(op1,fp);
				int op2_reg_num = ensure(op2,fp);
				fprintf(fp,"  add %s, %s, %s\n",_reg[result_reg_num].name,_reg[op1_reg_num].name,_reg[op2_reg_num].name);
			}
			break;
		}        // x := y + z
		case SUB:{
			pOperand result = curr->u.binop.result;
			pOperand op1 = curr->u.binop.op1;
			pOperand op2 = curr->u.binop.op2;
			int result_reg_num = ensure(result,fp);
			if(op1->kind == CONSTANT && op2->kind == CONSTANT){
				fprintf(fp,"  li %s, %d\n",_reg[result_reg_num].name,op1->u.value - op2->u.value);
			}
			// else if(op1->kind == CONSTANT){
			// 	int op2_reg_num = ensure(op2);
			// 	fprintf(fp,"  addi %s, %s, %d\n",_reg[result_reg_num].name,_reg[op2_reg_num].name,op1->u.value);//TOCHANGE
			// }//TODO 第一个是不是不能是常数？翻译模式里面好像没有这个选项
			else if(op2->kind == CONSTANT){
				int op1_reg_num = ensure(op1,fp);
				fprintf(fp,"  addi %s, %s, %d\n",_reg[result_reg_num].name,_reg[op1_reg_num].name,-op2->u.value);
			}
			else{//如果有第二种情况会多用一个寄存器，但是不会出错
				int op1_reg_num = ensure(op1,fp);
				int op2_reg_num = ensure(op2,fp);
				fprintf(fp,"  sub %s, %s, %s\n",_reg[result_reg_num].name,_reg[op1_reg_num].name,_reg[op2_reg_num].name);
			}
			break;
		}        // x := y - z
		case MUL:{
			pOperand result = curr->u.binop.result;
			pOperand op1 = curr->u.binop.op1;
			pOperand op2 = curr->u.binop.op2;
			int result_reg_num = ensure(result,fp);
			if(op1->kind == CONSTANT && op2->kind == CONSTANT){
				fprintf(fp,"  li %s, %d\n",_reg[result_reg_num].name,op1->u.value * op2->u.value);
			}
			else{//其中一个是常量也必须加载到寄存器中，否则不满足MIPS32语法规范
				int op1_reg_num = ensure(op1,fp);
				int op2_reg_num = ensure(op2,fp);
				fprintf(fp,"  mul %s, %s, %s\n",_reg[result_reg_num].name,_reg[op1_reg_num].name,_reg[op2_reg_num].name);
			}
			break;
		}        // x := y * z
		case DIV:{
			pOperand result = curr->u.binop.result;
			pOperand op1 = curr->u.binop.op1;
			pOperand op2 = curr->u.binop.op2;
			int result_reg_num = ensure(result,fp);
			int op1_reg_num = ensure(op1,fp);
			int op2_reg_num = ensure(op2,fp);
			fprintf(fp,"  div %s, %s\n",_reg[op1_reg_num].name,_reg[op2_reg_num].name);
			fprintf(fp,"  mflo %s\n",_reg[result_reg_num].name);
			break;
		}        // x := y / z
		case ADDR:{
			int result_reg_num = ensure(curr->u.assign.result,fp);
			int op1_reg_num = ensure(curr->u.assign.op1,fp);
			fprintf(fp,"  la %s, %s\n",_reg[op1_reg_num].name,curr->u.assign.op1->u.name);//TODO 不确定对不对，回头问一下晁哥怎么实现的
			fprintf(fp,"  move %s, %s\n",_reg[result_reg_num].name,_reg[op1_reg_num].name);
			break;
		}       // x := &y
		case DEREF_L:{
			int result_reg_num = ensure(curr->u.assign.result,fp);
			int op1_reg_num = ensure(curr->u.assign.op1,fp);
			fprintf(fp,"  lw %s, 0(%s)\n",_reg[result_reg_num].name,_reg[op1_reg_num].name);
			break;
		}    // *x := y
		case DEREF_R:{
			int result_reg_num = ensure(curr->u.assign.result,fp);
			int op1_reg_num = ensure(curr->u.assign.op1,fp);
			fprintf(fp,"  sw %s, 0(%s)\n",_reg[op1_reg_num].name,_reg[result_reg_num].name);
			break;
		}    // x := *y
		case GOTO:{
			fprintf(fp, "  j label%d\n",curr->u.singleop.op->u.var_no);//TODO label%d 应该对吧，再检查一下
			break;
		}       // GOTO x
		case IF_GOTO:{
			int x_num = ensure(curr->u.if_goto.x,fp);
			int y_num = ensure(curr->u.if_goto.y,fp);
			fprintf(fp,"  ");
			print_relop(curr->u.if_goto.relop,fp);
			fprintf(fp,"%s, %s, label%d\n",_reg[x_num].name,_reg[y_num].name,curr->u.if_goto.z->u.var_no);//TODO label%d 应该对吧，再检查一下
			break;
		}    // IF x [relop] y GOTO z
		case RETURN:{
			if(curr->u.singleop.op->kind == CONSTANT)
				fprintf(fp, "  li $v0, %d\n", curr->u.singleop.op->u.value);//常数直接加载
			else
			{
				int reg_num = ensure(curr->u.singleop.op,fp);
				fprintf(fp, "  move $v0, %s\n", _reg[reg_num].name);
			}
			fprintf(fp, "  jr $ra\n");
			break;
		}     // RETURN x
		case DEC:{
			//在init_data_text中已经完成
			break;
		}        // DEC x [size]
		case ARG:{
			//在CALL中调用，已完成
			break;
		}        // ARG x
		case CALL:{
			int result_reg_num = ensure(curr->u.singleop.op,fp);
			fprintf(fp, "  addi $sp, $sp, -4\n");
			fprintf(fp, "  sw $ra, 0($sp)\n");
			store_reg(fp);
			//如果在函数中调用，则需要将原函数参数压栈保存
			if(in_func){
				int param_num_origin = find_param_num(cur_fun_name);
				fprintf(fp, "  addi $sp, $sp, -%d\n", (4 < param_num_origin ? 4 : param_num_origin) * 4);//保存原先函数的参数
				for(int i=0;i<(4 < param_num_origin ? 4 : param_num_origin);i++)
				{
					fprintf(fp, "  sw %s, %d($sp)\n", _reg[4+i].name, i*4);//加入栈
					//在变量描述符中删除，并加入到内存变量描述符中
					struct VarDesc* p = var_head;
					while(p!=NULL)
					{
						//if(p->op->kind != CONSTANT && p->reg_num==4+i)
						if(p->reg_num==4+i)
						{
							break;
						}
						p=p->next;
					}
					//p是找到的变量描述符
					if(p!=NULL)
					{
						del_var_desc(p);//删除变量描述符并加载到内存中暂存
						add_memory_var_desc(p->op);
					}
				}
			}
				//加载参数ARG
			pInstruction tmp = find_pre_instr(curr);
			int param_reg_num = 0;
			while(tmp != NULL && tmp->kind == ARG)
			{
				if(param_reg_num < 4)//4个以内的参数存到a0-a3
				{
					int reg_num = ensure(tmp->u.singleop.op,fp);
					fprintf(fp, "  move %s, %s\n", _reg[4+param_reg_num].name, _reg[reg_num].name);
					param_reg_num++;
				}
				else//超过的参数存到栈中
				{
					fprintf(fp, "  addi $sp, $sp, -4\n");
					fprintf(fp, "  sw %s, 0($sp)\n", _reg[4+param_reg_num].name);
					fprintf(fp, "  move $fp, $sp\n");
					param_reg_num++;
				}
				tmp = find_pre_instr(tmp);
			}
			// fprintf(fp, "  jal %s\n", curr->u.singleop.op->u.name);//调用函数 ////segmentation fault
			fprintf(fp, "  jal %s\n", curr->u.call.op->u.name);//调用函数 ////segmentation fault
			//恢复栈
			fprintf(fp, "  addi $sp, $sp, %d\n", (param_reg_num-4>0?param_reg_num-4:0) * 4);//将被调用的函数参数弹出
			//如果在函数中则恢复原函数参数
			if(in_func){
				int param_num_origin = find_param_num(cur_fun_name);
				for(int i=0;i<(4 < param_num_origin ? 4 : param_num_origin);i++)
				{
					fprintf(fp, "  lw %s, %d($sp)\n", _reg[4+i].name, i*4);//从栈中恢复
				}
				//在内存变量描述符中找到对应的变量描述符，删除后添加到当前的变量描述符中
				struct VarDesc* p = find_the_var_desc_in_mem(4 < param_num_origin ? 4 : param_num_origin);
				for(int i=0;i<(4 < param_num_origin ? 4 : param_num_origin);i++){
					fprintf(fp, "  lw %s, %d($sp)\n", _reg[4+i].name, i*4);
					del_memory_var_desc(p);
					add_var_desc(4+i,p->op);
					p = p->next;
				}//恢复原参数
				fprintf(fp, "  addi $sp, $sp, %d\n", (4 < param_num_origin ? 4 : param_num_origin) * 4);//恢复栈指针
			}
			load_reg(fp);//恢复寄存器
			fprintf(fp, "  lw $ra, 0($sp)\n");//恢复返回值
			fprintf(fp, "  addi $sp, $sp, 4\n");//恢复栈指针
			fprintf(fp, "  move %s, $v0\n", _reg[result_reg_num].name);//将返回值存入寄存器
			break;
		}       // x := CALL f
		case PARAM:{
			//在函数中调用，已完成
			break;
		}     // PARAM x
		case READ:{
			fprintf(fp, "  addi $sp, $sp, -4\n");
			fprintf(fp, "  sw $ra, 0($sp)\n");
			fprintf(fp, "  jal read\n");
			fprintf(fp, "  lw $ra, 0($sp)\n");
			fprintf(fp, "  addi $sp, $sp, 4\n");
			int reg_num = ensure(curr->u.singleop.op,fp);
			fprintf(fp, "  move %s, $v0\n", _reg[reg_num].name);
			break;
		}       // READ x
		case WRITE:{
			if(in_func==0)
			{
				int result_num = ensure(curr->u.singleop.op,fp);
				fprintf(fp, "  move $a0, %s\n", _reg[result_num].name);
				fprintf(fp, "  addi $sp, $sp, -4\n");//内容写入a0寄存器
				fprintf(fp, "  sw $ra, 0($sp)\n");//保护返回值
				fprintf(fp, "  jal write\n");
				fprintf(fp, "  lw $ra, 0($sp)\n");//恢复返回值以及栈指针
				fprintf(fp, "  addi $sp, $sp, 4\n");
			}
			else
			{
				int result_num = ensure(curr->u.singleop.op,fp);
				fprintf(fp, "  addi $sp, $sp, -8\n");
				fprintf(fp, "  sw $a0, 0($sp)\n");
				fprintf(fp, "  sw $ra, 4($sp)\n");//保护a0值与返回值
				fprintf(fp, "  move $a0, %s\n", _reg[result_num].name);			
				fprintf(fp, "  jal write\n");
				fprintf(fp, "  lw $a0, 0($sp)\n");
				fprintf(fp, "  lw $ra, 4($sp)\n");//恢复a0值与返回值	
				fprintf(fp, "  addi $sp, $sp, 8\n");		
			}
			break;
		}       // WRITE x
	}


}


void trans(FILE* fp){
	pInstruction curr = code_list.head;
    while (curr != NULL) {
	printf("translating %d\n",curr->kind);
	trans_one_line(curr,fp);
	curr = curr->next;
	}
}

void print_spill_labels(FILE* fp){
	//进行文件读写，把tmpout中的内容输出到fp文件的后面
	
	fprintf(fp, ".text\n");
	fprintf(fp, "read:\n");
	fprintf(fp, "  li $v0, 4\n");
	fprintf(fp, "  la $a0, _prompt\n");
	fprintf(fp, "  syscall\n");
	fprintf(fp, "  li $v0, 5\n");
	fprintf(fp, "  syscall\n");
	fprintf(fp, "  jr $ra\n\n");
	fprintf(fp, "write:\n");
	fprintf(fp, "  li $v0, 1\n");
	fprintf(fp, "  syscall\n");
	fprintf(fp, "  li $v0, 4\n");
	fprintf(fp, "  la $a0, _ret\n");
	fprintf(fp, "  syscall\n");
	fprintf(fp, "  move $v0, $0\n");
	fprintf(fp, "  jr $ra\n");
	return;
}


void s_code_generate(FILE* fp){
    init_reg();
    init_data_text(tmpout);
    init_var_list();
	trans(fp);
	print_spill_labels(tmpout);
}
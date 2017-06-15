#include "semantic_utils.h"
#include <cstdlib>
REG expInit(char* value)
{
	REG regName = regPool.getReg();
	CodeBuffer::instance().emit("li "+RegPool::regToString(regName)+", "+string(value));
	return  regName;
}

REG expMakeBinOp1(REG exp1,REG exp2,char* op){
	string mipsOp;
	if(string(op) == "*"){
		mipsOp = "mult";
	}
	else{// op = "/"
		//TODO put code to verify exp2 not 0
		mipsOp = "div";
	}
	CodeBuffer::instance().emit(mipsOp + " " + RegPool::regToString(exp1) + ", " + RegPool::regToString(exp2));
	CodeBuffer::instance().emit("mflo " + RegPool::regToString(exp1));
	if(exp2 != exp1)
		regPool.freeReg(exp2);
	free(op);
	return exp1;
}

REG expMakeBinOp2(REG exp1,REG exp2,char* op)
{
	string mipsOp;
	if(string(op) == "+"){
		mipsOp = "add";
	}
	else {//op == "-"
		mipsOp = "sub";
	}
	
	CodeBuffer::instance().emit(mipsOp + " " + RegPool::regToString(exp1) + ", " + RegPool::regToString(exp1) + ", " + RegPool::regToString(exp2));
	if(exp2 != exp1)
		regPool.freeReg(exp2);
	free(op);
	return exp1;
}

void insert_printi(){
	CodeBuffer::instance().emit("printi:");
	CodeBuffer::instance().emit("lw $a0,0($sp)");
	 CodeBuffer::instance().emit("li $v0,1");
	 CodeBuffer::instance().emit("syscall");
	 CodeBuffer::instance().emit("jr $ra");
}

void insert_print(){
	CodeBuffer::instance().emit("print:");
	CodeBuffer::instance().emit("lw $a0,0($sp)");
	 CodeBuffer::instance().emit("li $v0,0");
	 CodeBuffer::instance().emit("syscall");
	 CodeBuffer::instance().emit("jr $ra");
}
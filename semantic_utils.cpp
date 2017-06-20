#include "semantic_utils.h"
#include <cstdlib>
#include <assert.h>
#include <sstream>

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
		REG tmp0Reg = regPool.getReg();
		CodeBuffer::instance().emit("li " + RegPool::regToString(tmp0Reg) + ", 0");
		CodeBuffer::instance().emit("beq " +RegPool::regToString(tmp0Reg) + ", " + RegPool::regToString(exp2) + ", div0error");
		regPool.freeReg(tmp0Reg);
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

void expMakeRelOp2(REG place1, REG place2 ,string op, vector<int>** trueList,vector<int>** falseList){
	string jumpOp;
	if(op == "<=")
		jumpOp = "ble ";
	else if (op == "<")
		jumpOp = "blt ";
	else if (op == ">=")
		jumpOp = "bge ";
	else if (op == ">")
		jumpOp = "bgt ";
	else if (op == "!=")
		jumpOp = "bne ";
	else if (op == "==")
		jumpOp = "beq ";
	else assert(false);//should never happen
	
	/*
	so op should look somthing like:
	beq $t1, $t2, _ <---command is trueList value
	b _				<---command is falseList value
	*/
	
	jumpOp += RegPool::regToString(place1) + ", " + RegPool::regToString(place2) + ", ";
	int trueTargetInst = CodeBuffer::instance().emit(jumpOp);
	vector<int> tempTrueList = CodeBuffer::makelist(trueTargetInst);
	*trueList = new vector<int>(tempTrueList.begin(),tempTrueList.end());
	
	int falseTargetInst = CodeBuffer::instance().emit("b ");//else
	vector<int> tempFalseList = CodeBuffer::makelist(falseTargetInst);
	*falseList = new vector<int>(tempFalseList.begin(),tempFalseList.end());
	
	regPool.freeReg(place1);
	regPool.freeReg(place2);
}

void moveValueInVar (string varName, REG valPlace)
{
	int pos = symbolTable.getPosition(varName);
	int offFromFP = pos*4;
	std::ostringstream ostr;
	ostr << offFromFP;
	CodeBuffer::instance().emit("sw " + RegPool::regToString(valPlace) + ", " + ostr.str() +"($fp)");
	regPool.freeReg(valPlace);
}

REG loadValueFromVar (string varName)
{
	int pos = symbolTable.getPosition(varName);
	int offFromFP = pos*4;
	REG dest = regPool.getReg();
	std::ostringstream ostr;
	ostr << offFromFP;
	CodeBuffer::instance().emit("lw " + RegPool::regToString(dest) + ", " + ostr.str() +"($fp)");
	return dest;
}

void initFP()
{	
	CodeBuffer::instance().emit("subu $fp, $sp, 4");
}

REG putSetRegCode(vector<int>* trueList, vector<int>* falseList)
{
	CodeBuffer& cbuff = CodeBuffer::instance();
	REG valPlace = regPool.getReg();
	string trueLabel = cbuff.next();
	cbuff.emit("li " + RegPool::regToString(valPlace) + ", 1");
	int branchPoint = cbuff.emit("b ");
	string falseLabel = cbuff.next();
	cbuff.emit("li " + RegPool::regToString(valPlace) + ", 0");
	string branchTarget = cbuff.next();
	cbuff.bpatch(CodeBuffer::makelist(branchPoint), branchTarget);
	
	//patch all lists.
	cbuff.bpatch(*trueList, trueLabel);
	cbuff.bpatch(*falseList, falseLabel);
	
	//cleanups
	delete trueList;
	delete falseList;
	
	return valPlace;
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
	CodeBuffer::instance().emit("li $v0,4");
	CodeBuffer::instance().emit("syscall");
	CodeBuffer::instance().emit("jr $ra");
}

void insert_div0error(){
	CodeBuffer::instance().emitData("div0errormsg: .asciiz \"Error division by zero\\n\"");
	CodeBuffer::instance().emit("div0error:");
	CodeBuffer::instance().emit("la $a0,div0errormsg");
	CodeBuffer::instance().emit("li $v0,4");
	CodeBuffer::instance().emit("syscall");
	CodeBuffer::instance().emit("li $v0,10");
	CodeBuffer::instance().emit("syscall");
}
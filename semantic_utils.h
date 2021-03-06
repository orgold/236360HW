#include "RegPool.h"
#include "bp.hpp"
#include "hw4_table_utils.h"
#include <vector>
using std::vector;

extern RegPool regPool;
extern SymbolTable symbolTable;
REG expInit(char* value);
REG expMakeBinOp1(REG exp1,REG exp2,char* op);
REG expMakeBinOp2(REG exp1,REG exp2,char* op);
void expMakeRelOp2(REG place1, REG place2 ,string op, vector<int>** trueList,vector<int>** falseList);
void moveValueInVar (string varName, REG valPlace);
REG loadValueFromVar (string varName);//oposite of moveValueInVar
void initFP();
REG putSetRegCode(vector<int>* trueList, vector<int>* falseList);
void makeSwitchCode(REG expPlace, vector<pair<int,string> > caseStack, vector<int> nextList);
void setRaOnStack(int argsSize);
void pushArgsList(vector<REG> argsList);
//insert functions inst-by-inst
void insert_print();
void insert_printi();
void insert_div0error();

class whileStack{
vector<pair<int,vector<int> > > _whileStack;
public:
	whileStack();
	void addVector();
	void removeVector(string label);
	void addAddress();

};

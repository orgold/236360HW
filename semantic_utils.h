#include "RegPool.h"
#include "bp.hpp"

#include <vector>
using std::vector;

extern RegPool regPool;
REG expInit(char* value);
REG expMakeBinOp1(REG exp1,REG exp2,char* op);
REG expMakeBinOp2(REG exp1,REG exp2,char* op);
void expMakeRelOp2(REG place1, REG place2 ,string op, vector<int>** trueList,vector<int>** falseList);
//insert functions inst-by-inst
void insert_print();
void insert_printi();
void insert_div0error();

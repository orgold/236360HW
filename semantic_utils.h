#include "RegPool.h"
#include "bp.hpp"
extern RegPool regPool;
REG expInit(char* value);
REG expMakeBinOp1(REG exp1,REG exp2,char* op);
REG expMakeBinOp2(REG exp1,REG exp2,char* op);

//insert functions inst-by-inst
void insert_print();
void insert_printi();
//void insert_div0error();//TODO

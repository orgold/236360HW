#include "RegPool.h"
#include <assert.h>

RegPool::RegPool()
{
	for(int i=0;i<18;i++)
		pool[REG(i)] = false;
}

 REG RegPool::getReg()
{
	for(map<REG,bool>::iterator ite=pool.begin(); ite != pool.end();ite++)
	{
		if(false == ite->second)
		{
			pool[ite->first]= true;
			return ite->first;
		}
	}
	assert(false);
	return invalidReg;
}

void RegPool::freeReg(REG regKey)
{
	pool[regKey] = false;
}

string  RegPool::regToString(REG reg)
{
	switch(reg)
	{
		case(t0): return "$t0";
		case(t1): return "$t1";
		case(t2): return "$t2";
		case(t3): return "$t3";
		case(t4): return "$t4";
		case(t5): return "$t5";
		case(t6): return "$t6";
		case(t7): return "$t7";
		case(t8): return "$t8";
		case(t9): return "$t9";
		case(s0): return "$s0";
		case(s1): return "$s1";
		case(s2): return "$s2";
		case(s3): return "$s3";
		case(s4): return "$s4";
		case(s5): return "$s5";
		case(s6): return "$s6";
		case(s7): return "$s7";
		default : return "";
	}
}

void RegPool::saveAll()
{
	CodeBuffer::instance().emit("subu $sp , $sp,72 ");

	for(int i=0;i<18;i++)
	{
		std::ostringstream ostr;
		ostr<< i*4;
		CodeBuffer::instance().emit("sw " +regToString(REG(i))+", " + ostr.str()+  "($sp)");
	}
}
void RegPool::loadAll()
{
	for(int i=0;i<18;i++)
	{
		std::ostringstream ostr;
		ostr<< i*4;
		CodeBuffer::instance().emit("lw "+regToString(REG(i))+ " ," +ostr.str()+  "($sp)");
	}
	CodeBuffer::instance().emit("addu $sp , $sp,72 ");
}
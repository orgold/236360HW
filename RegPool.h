#ifndef _REGPOOL_H_
#define _REGPOOL_H_

#include <map>
#include <string>
#include "bp.hpp"
#include <sstream>

using std::string;
using std::map;

enum REG {t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,s0,s1,s2,s3,s4,s5,s6,s7,invalidReg} ;

class RegPool {
map<REG,bool> pool;

public:
	static string  regToString(REG);
	RegPool();
	REG getReg();
	void freeReg(REG);
	static void saveAll();
	static void loadAll();
};

#endif
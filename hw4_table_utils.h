#ifndef _hw4_table_utils_h
#define _hw4_table_utils_h

#include <iostream>
#include "output.hpp"
#include <map>
#include <exception>
#include <assert.h>
#include <algorithm>
#include "RegPool.h"
#include "bp.hpp"


using std::vector;
using std::string;
using std::pair;
using  namespace output;
enum TYPE {INT_T, BOOL_T, BYTE_T, VOID_T, STRING_T};

const char* toCString(TYPE type);

struct newYystype 
{
	int hasBreak;
	TYPE type;
	TYPE* typeList;
	char** idList;
	int typeListSize;
	char* value;
	char * idName;
	int lineno;
	int seenReturn;
	REG place;
	vector<int>* trueList;
	vector<int>* falseList;
	char* nextInst;
	int asmLineNo;
	vector<pair<int,string> >* caseStack;
	vector<int>* nextList;
	vector<REG>* argListRegs;
};
typedef struct newYystype  YYSTYPE;
#define YYSTYPE_IS_DECLARED = 1

class errorDefException  
{
public:
	string id;
	errorDefException(const string &id) :id(id){}
};

class errorUndefException  
{
public:
	string id;
	errorUndefException(const string &id) :id(id){}
	errorUndefException(){};
};

class errorUnInitilizeVar  
{
public:
	errorUnInitilizeVar(){};
};

class errorUndefFuncException  
{
public:
	string id;
	errorUndefFuncException(const string &id) :id(id){}
	errorUndefFuncException(){};
};

class errorPrototypeMismatchException  
{
public:
	string id;
	vector<const char*> argTypes;
	
	errorPrototypeMismatchException(const string &id,const vector<TYPE>& typesVector) :id(id),
	argTypes(vector<const char*>(typesVector.size()))
	{
		std::transform(
			typesVector.begin(),
			typesVector.end(),
			argTypes.begin(),
			toCString
		);
	}
	errorPrototypeMismatchException(){};
};

struct VarEntryData
{
	TYPE type;
	int position;
	int initScope;
};

class ScopeTable
{
	std::vector<string> orderIdsByArrival; 
	std::map<string,VarEntryData> nameToData;
	int currentOffset;
	
public:
	ScopeTable(int currentOffset,bool isWhile = false) : currentOffset(currentOffset) {}
	ScopeTable(const ScopeTable& st) : 
		orderIdsByArrival(st.orderIdsByArrival),nameToData(st.nameToData), currentOffset (st.currentOffset) {}
	bool isVarInScope(const string& name)
	{
		return nameToData.find(name) != nameToData.end(); 
	}
	void insertVar(const string& name, TYPE type)
	{
		orderIdsByArrival.push_back(name);
		VarEntryData data = {type, currentOffset++,-1};
		nameToData[name] = data;
	}
	void insertParam(const string& name, TYPE type, int position)
	{
		if(nameToData.find(name) != nameToData.end())
			throw errorDefException(name);
		orderIdsByArrival.push_back(name);
		VarEntryData data = {type, position,0};//d
		nameToData[name] = data;
	}
	int getCurrentOffset()
	{
		return currentOffset;
	}
	void sendToPrint()
	{
		for(std::vector<string>::iterator ite = orderIdsByArrival.begin(); ite != orderIdsByArrival.end() ; ite++)
			printID(ite->c_str(),nameToData[*ite].position,toCString(nameToData[*ite].type));
	}
	TYPE getType (string name)
	{
		return nameToData[name].type;
	}
	void assignVar(int scopeNumber,string name)
	{
		VarEntryData dataNew = nameToData[name];
		if(dataNew.initScope == -1)
			dataNew.initScope = scopeNumber;
		nameToData[name] = dataNew;
	}
	void invalidAssign(int scopeNumber)
	{
		for( std::map<string,VarEntryData>::iterator itr = nameToData.begin(); itr != nameToData.end(); itr++){
			VarEntryData dataNew = itr->second;
			if(dataNew.initScope == scopeNumber) {
				dataNew.initScope = -1;
				nameToData[itr->first] = dataNew;
			}
		}
	}
	bool isInit(string name)
	{
		VarEntryData dataNew = nameToData[name];
		return -1 != dataNew.initScope;

	}
	int getPosition(string name) { return nameToData[name].position; }
#ifdef DEBUG
	void print()
	{
		std::cout << "\tcurrentOffset: " << currentOffset << std::endl;
		std::cout << "\tnameToData: " << std::endl;
		for( auto itr = nameToData.begin(); itr != nameToData.end(); itr++){
			std::cout <<"\t\tName:" << itr->first << ", ";
			std::cout << "Type: " + print_TYPE(itr->second.type);
			std::cout << "Position: " << itr->second.position;
			std::cout << std:: endl;
		}
	}
#endif //DEBUG

	size_t getNumOfSymbolsInScope() {
		size_t cnt = 0;
		for(std::map<string,VarEntryData>::iterator it = nameToData.begin(); it != nameToData.end(); it++){
			cnt++;
		}
		return cnt;
	}
	
};

struct FunctionData
{

	TYPE returnType;
	vector<TYPE> paramTypeList;
};

class SymbolTable
{
	vector<ScopeTable> scopeStack;
	std::map<string,FunctionData> functionMap;
	std::vector<string> funcsByOrderArrival;
	void checkNameNotDefinedAsVar(const string& name);
	void checkNameNotDefinedAsFunction(const string& name);
	void insertFunction(const string& name,const vector<TYPE>& paramList,TYPE retType,const vector<string>& paramNames);
	void invalidAssign(int scopeNumber);
public:
	SymbolTable();
	~SymbolTable();
	void coverInsertFunction(const string& name,TYPE* typeList,int paramNum, TYPE retType, char** paramNames);
	void insertScope();
	void functionCallCode();
	void removeScope();
	void functionRemoveScope(int argSize);
	void insertVar(const string& name, TYPE type);
	TYPE checkFuncTypeAndArgs(string, TYPE*,int);
	//getPosition(name);
	TYPE getType(string name);
	void removeGlobalScope();
	int getPosition(string name);
	size_t getStackSize();
	size_t numOfVarsUpToScopeNumber(size_t scopeNumber);
	void assignVar(string name);
	bool isInit(string name);

#ifdef DEBUG
	void print()
	{
		std::cout << "functionMap: " << std::endl;
		for( auto itr= functionMap.begin();itr!=functionMap.end();itr++){
			std::cout << itr->first << " :" << std::endl;
			std::cout << "\tretType: " ;print_TYPE(itr->second.returnType);
			std::cout << "\tparamTypes:";
			for (auto paramType = itr->second.paramTypeList.begin();
			     paramType !=itr->second.paramTypeList.end();
			      paramType++){
				print_TYPE(*paramType);
				std::cout <<  ", ";
			}
			std::cout << std:: endl;
		}
		for( int i = 0; i < scopeStack.size(); i++){
			std::cout << "scope " << i << ":" << std::endl;
			scopeStack[i].print();
		}
	}
#endif //DEBUG
};
#endif //_hw4_table_utils_h

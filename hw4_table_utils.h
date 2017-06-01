#include <iostream>
#include <vector>
using std::vector;
#include <string>
#include <map>
#include <exception>
#include <assert.h>
using std::string;
enum TYPE {INT, BOOL, BYTE, VOID, STRING};

#ifdef DEBUG
void print_TYPE(TYPE type){
	switch(type){
		case(INT):
			std::cout << "int";
			break;
		case(BOOL):
			std::cout << "bool";
			break;
		case(BYTE):
			std::cout << "byte";
			break;
		case(STRING):
			std::cout << "string";
			break;
		case(VOID):
			std::cout << "void";
			break;
	}
}
#endif //DEBUG

class AlreadyDefinedException  
{
public:
	string id;
	AlreadyDefinedException(const string &id) :id(id){}
};

struct VarEntryData
{
	
	TYPE type;
	int position;
};

class ScopeTable
{
	std::map<string,VarEntryData> nameToData;
	int currentOffset;
	
	
public:
	ScopeTable(int currentOffset) : currentOffset(currentOffset) {}
	ScopeTable(const ScopeTable& st) : 
		nameToData(st.nameToData), currentOffset (st.currentOffset) {}
	bool isVarInScope(const string& name)
	{
		return nameToData.find(name) != nameToData.end(); 
	}
	void insertVar(const string& name, TYPE type)
	{
		VarEntryData data = {type, currentOffset++};
		nameToData[name] = data;
	}
	void insertParam(const string& name, TYPE type, int position)
	{
		if(nameToData.find(name) != nameToData.end())
			throw AlreadyDefinedException(name);
		VarEntryData data = {type, position};///d
		nameToData[name] = data;
	}
	int getCurrentOffset()
	{
		return currentOffset;
	}
#ifdef DEBUG
	void print()
	{
		std::cout << "\tcurrentOffset: " << currentOffset << std::endl;
		std::cout << "\tnameToData: " << std::endl;
		for( auto itr = nameToData.begin(); itr != nameToData.end(); itr++){
			std::cout <<"\t\tName:" << itr->first << ", ";
			std::cout << "Type: "; print_TYPE(itr->second.type);
			std::cout << "Position: " << itr->second.position;
			std::cout << std:: endl;
		}
	}
#endif //DEBUG
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
	void checkNameNotDefinedAsVar(const string& name);
	void checkNameNotDefinedAsFunction(const string& name);
public:
	SymbolTable();
	void insertFunction(const string& name,const vector<TYPE>& paramList,TYPE retType,const vector<string>& paramNames);
	void insertScope();
	void removeScope();
	void insertVar(const string& name, TYPE type);
	//getPosition(name);
	//getType(name);
	
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
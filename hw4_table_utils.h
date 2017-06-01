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

class AlreadyDefinedException : public exception 
{
	string id;
}

class VarEntryData
{
	public:
	TYPE type;
	int position;
};

class ScopeTable
{
	map<string,VarEntryData> nameToData;
	int currentOffset;
	
	void checkNameNotDefinedAsVar(const string& name);
	void checkNameNotDefinedAsFunction(const string& name)
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
		VarEntryData data(type, currentOffset++);
		nameToData[name] = data;
	}
	void insertParam(const string& name, TYPE type, int position)
	{
		if(nameToData.find(name) == nameToData.end())
			throw AlreadyDefinedException(name);
		VarEntryData data(type, position);
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
		for( auto& itr : nameToData){
			std::cout <<"\t\tName:" << itr->first << ", ";
			std::cout << "Type: " << print_TYPE(int->second.type);
			std::cout << "Position: " << print_TYPE(int->second.position);
			std::cout << std:: endl;
		}
	}
#endif //DEBUG
}

class FunctionData
{
public:
	TYPE returnType;
	vector<TYPE> paramTypeList;
}

class SymbolTable
{
	vector<scopeTable> scopeStack;
	map<string,FunctionData> functionMap;
public:
	void insertFunction(const string& name,const vector& paramList,TYPE retType);
	void insertScope();
	void removeScope();
	void insertVar(const string& name, TYPE type);
	//getPosition(name);
	//getType(name);
	
#ifdef DEBUG
	void print()
	{
		std::cout << "functionMap: " << std::endl;
		for( auto& itr : functionMap){
			std::cout << itr->first << " :" << std::endl;
			std::cout << "\tretType: " << print_TYPE(int->second.returnType);
			std::cout << "\tparamTypes:";
			for (auto& paramType : itr->second.paramTypeList)
				std::cout << print_TYPE(*paramType) << ", ";
			std::cout << std:: endl;
		}
		for( int i = 0; i < scopeStack.size(); i++){
			std::cout << "scope " << i << ":" << std::endl;
			scopeStack[i].print();
		}
	}
#endif //DEBUG
}
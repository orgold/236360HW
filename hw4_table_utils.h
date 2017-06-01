enum TYPE {INT, BOOL, BYTE, VOID, STRING};

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
	removeScope();
	insertVar(name,type);
	//getPosition(name);
	//getType(name);
}
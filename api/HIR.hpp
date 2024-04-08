#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>

namespace ozToy::HIR{

	class TranslationUnit;

	class ModuleBase;
	class ModuleImpl;
	class ModuleRef;

	class StructBase;
	class StructImpl;
	class StructRef;
	
	class ClassBase;
	class ClassImpl;
	class ClassRef;
	
	class FunctionBase;
	class FunctionImpl;
	class FunctionRef;

	class Type;

	class UnresolvedName;
	class UnresolvedClass;
	class UnresolvedType;
	class UnresolvedVariable;

	class UnresolvedName {
	public:
		virtual const std::string& getName() = 0;
	};

	class Type {
	public:
		Type() = default;
	};

	class UnresolvedType : public UnresolvedName, public Type {
		std::string name;
	public:
		UnresolvedType(std::string name);
		const std::string& getName() override;
	};

	class TranslationUnit {
		ModuleImpl* rootModule;
		std::vector<UnresolvedName*> unresolvedNames;
	public:
		TranslationUnit();
		ModuleImpl* getRootModule();
		void addUnresolvedName(UnresolvedName* name);
		void print(std::ostream& out);
	};

	class ModuleBase {

	};

	class ModuleImpl : public ModuleBase {
		TranslationUnit* tu;
		std::string name;
		ModuleImpl* parent = nullptr;
		std::map<std::string, ModuleBase*> modules;
		std::map<std::string, StructBase*> structs;
		std::map<std::string, ClassBase*> classes;
		std::map<std::string, FunctionBase*> functions;
	public:
		ModuleImpl(std::string name, TranslationUnit* tu);
		ModuleImpl* createModule(std::string name);
		FunctionImpl* createFunction(std::string name);
	};

	class ModuleRef : public ModuleBase {
		ModuleImpl* impl;
	};

	class FunctionBase {

	};

	class FunctionBody;
	class TypeConstraint;
	class Scope;
	class Argument;
	class Variable;
	class Value;
	class Block;

	class FunctionImpl : public FunctionBase {
		TranslationUnit* tu;
		ModuleImpl* parentModule;
		std::string name;
		std::vector<Argument*> arguments;
		Block* rootBlock;
		Type* returnType;
	public:
		FunctionImpl(std::string name,ModuleImpl* parentModule, TranslationUnit* tu);
		void setReturnType(Type* type);
		void addArgument(Argument* arg);
		Type* getType(std::string name);
		Variable* getVariableOutside(std::string name);
		Block* getRootBlock();
	};
	
	class Scope {
		FunctionImpl* function;
		std::size_t depth;
		Scope* parent = nullptr;
		std::vector<Scope*> children;
		std::vector<Variable*> variables;
		std::map<std::string, Variable*> accessibleVariables;
		Scope(FunctionImpl* function, Scope* parent);
	public:
		Scope(FunctionImpl* function);
		bool isRoot();
		Scope* createChild();
		Scope* getParent();
		void addVariable(std::string id, Variable* var);
		Value* getVariable(std::string id);
	};

	class Argument {
		std::string name;
		Type* type;
	public:
		Argument(std::string name, Type* type);
		Variable* createVariable();
	};
	
	class Value {
		TypeConstraint* typeConstraint;
	public:
		Value(TypeConstraint* typeConstraint);
	};

	class Variable : public Value {
	protected:
		std::string name;
	public:
		Variable(std::string name);
		Variable(std::string name, Type* type);
	};

	class UnresolvedVariable : public Variable, public UnresolvedName {
		FunctionImpl* function;
	public:
		UnresolvedVariable(FunctionImpl* function, std::string name);
		const std::string& getName() override;
	};

	enum class LiteralType {
		STRING,
		CHAR,
		INT,
		FLOAT,
	};

	class Literal : public Value {
		std::string value;
		LiteralType type;
	public:
		Literal(std::string value, LiteralType type);
	};

	class Call : public Value {
		Value* callee;
		std::vector<Value*> arguments;
	public:
		Call(Value* callee, std::vector<Value*> arguments);
	};

	class Block : public Value {
		Scope* linkedScope;
		std::vector<Value*> values;
	public:
		Block(Scope* linkedScope);
		void addValue(Value* value);
		Scope* getScope();
	};

	class UnitTypeValue : public Value {
		UnitTypeValue();
	public:
		static UnitTypeValue* getInstance();
	};

	class TypeConstraint {
		Type* type = nullptr;
	public:
		TypeConstraint();
		TypeConstraint(Type* type);
	};
} // namespace ozToy::HIR

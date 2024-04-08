#pragma once
#include <stack>

#include "HIR.hpp"

namespace ozToy::HIR {

	class ModuleBuilder;
	class ClassBuilder;
	class StructBuilder;
	class FunctionBuilder;

	class ModuleBuilder {
		ModuleImpl* module;
	public:
		ModuleBuilder(ModuleImpl* module);
		ModuleImpl& getModule();
	};

	class ClassBuilder {
		ClassImpl* clazz;
	};

	class StructBuilder {
		StructImpl* strct;
	};

	class FunctionBuilder {
		FunctionImpl* function;
		std::stack<Block*> blockStack;
	public:
		FunctionBuilder(FunctionImpl* function);
		void setReturnType(std::string type);
		void addArgument(std::string name, std::string type);
		void addArguments(std::vector<std::pair<std::string, std::string>> args);
		Value* declVariable(std::string name, std::string type, bool isMutable);
		Value* declVariable(std::string name, bool isMutable);
		Value* getVariable(std::string name);
		Value* getLiteral(std::string value, LiteralType type);
		void createBlock();
		void addInstruction(Value* value);
		Value* exitBlock();
	};
}
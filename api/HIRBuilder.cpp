#include "HIRBuilder.hpp"

namespace ozToy::HIR {
	ModuleBuilder::ModuleBuilder(ModuleImpl* module) : module(module) {
	}
	ModuleImpl& ModuleBuilder::getModule()
	{
		return *module;
	}
	FunctionBuilder::FunctionBuilder(FunctionImpl* function) : function(function) {
		blockStack.push(function->getRootBlock());
	}

	void FunctionBuilder::setReturnType(std::string type)
	{
		function->setReturnType(function->getType(type));
	}
	void FunctionBuilder::addArgument(std::string name, std::string type)
	{
		function->addArgument(new Argument(name, function->getType(type)));
	}
	void FunctionBuilder::addArguments(std::vector<std::pair<std::string, std::string>> args)
	{
		for (auto&& arg : args) {
			function->addArgument(new Argument(arg.first, function->getType(arg.second)));
		}
	}
	Value* FunctionBuilder::declVariable(std::string name, std::string type, bool isMutable)
	{
		Variable* var = new Variable(name, function->getType(type));
		Scope* scope = blockStack.top()->getScope();
		scope->addVariable(name, var);
		return var;
	}
	Value* FunctionBuilder::declVariable(std::string name, bool isMutable)
	{
		Variable* var = new Variable(name);
		Scope* scope = blockStack.top()->getScope();
		scope->addVariable(name, var);
		return var;
	}
	Value* FunctionBuilder::getVariable(std::string name)
	{
		return blockStack.top()->getScope()->getVariable(name);
	}
	Value* FunctionBuilder::getLiteral(std::string value, LiteralType type)
	{
		return new Literal(value, type);
	}
	void FunctionBuilder::createBlock()
	{
		blockStack.push(new Block(blockStack.top()->getScope()->createChild()));
	}
	void FunctionBuilder::addInstruction(Value* value)
	{
		blockStack.top()->addValue(value);
	}
	Value* FunctionBuilder::exitBlock()
	{
		Value* block = blockStack.top();
		blockStack.pop();
		return block;
	}
} // namespace ozToy::HIR
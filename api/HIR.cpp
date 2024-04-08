#include "HIR.hpp"

namespace ozToy::HIR {

	TranslationUnit::TranslationUnit()
	{
		rootModule = new ModuleImpl("root", this);
	}

	ModuleImpl* ozToy::HIR::TranslationUnit::getRootModule()
	{
		return rootModule;
	}

	void TranslationUnit::addUnresolvedName(UnresolvedName* name)
	{
		unresolvedNames.push_back(name);
	}

	void TranslationUnit::print(std::ostream& out)
	{
		for (auto&& name : unresolvedNames) {
			out << name->getName() << std::endl;
		}
	}

	ModuleImpl::ModuleImpl(std::string name, TranslationUnit* tu) : name(name), tu(tu)
	{
	}

	ModuleImpl* ModuleImpl::createModule(std::string name)
	{
		ModuleImpl* module = new ModuleImpl(name, tu);
		modules[name] = module;
		module->parent = this;
		return module;
	}

	FunctionImpl* ModuleImpl::createFunction(std::string name)
	{
		FunctionImpl* function = new FunctionImpl(name, this, tu);
		functions[name] = function;
		return function;
	}

	FunctionImpl::FunctionImpl(std::string name, ModuleImpl* parentModule, TranslationUnit* tu) : name(name), parentModule(parentModule), tu(tu)
	{
		rootBlock = new Block(new Scope(this));
	}

	void FunctionImpl::setReturnType(Type* type)
	{
		returnType = type;
	}

	void FunctionImpl::addArgument(Argument* arg)
	{
		arguments.push_back(arg);
	}

	Type* FunctionImpl::getType(std::string name)
	{
		UnresolvedType* unresolved = new UnresolvedType(name);
		tu->addUnresolvedName(unresolved);

		return unresolved;
	}

	Variable* FunctionImpl::getVariableOutside(std::string name)
	{
		UnresolvedVariable* unresolved = new UnresolvedVariable(this, name);
		tu->addUnresolvedName(unresolved);
		return unresolved;
	}

	Block* FunctionImpl::getRootBlock()
	{
		return rootBlock;
	}

	UnresolvedType::UnresolvedType(std::string name) : name(name)
	{
	}

	const std::string& UnresolvedType::getName()
	{
		return name;
	}

	Scope::Scope(FunctionImpl* function) : function(function)
	{
		depth = 0;
	}

	bool Scope::isRoot()
	{
		return parent == nullptr;
	}

	Scope* Scope::createChild()
	{
		Scope* child = new Scope(function, this);
		children.push_back(child);
		return child;
	}

	Scope* Scope::getParent()
	{
		return parent;
	}

	void Scope::addVariable(std::string id, Variable* var)
	{
		variables.push_back(var);
		accessibleVariables[id] = var;
	}

	Value* Scope::getVariable(std::string id)
	{
		auto it = accessibleVariables.find(id);
		if (it != accessibleVariables.end())
		{
			return it->second;
		}
		else if (parent != nullptr)
		{
			return parent->getVariable(id);
		}
		else
		{
			return function->getVariableOutside(id);
		}
	}

	Scope::Scope(FunctionImpl* function, Scope* parent) : function(function), parent(parent)
	{
		depth = parent->depth + 1;
	}

	Argument::Argument(std::string name, Type* type) : name(name), type(type)
	{
	}

	Variable* Argument::createVariable()
	{
		return new Variable(name, type);
	}

	Value::Value(TypeConstraint* typeConstraint) : typeConstraint(typeConstraint)
	{
	}

	Variable::Variable(std::string name) : Value(new TypeConstraint()), name(name)
	{
	}

	Variable::Variable(std::string name, Type* type) : Value(new TypeConstraint(type)), name(name)
	{
	}

	UnresolvedVariable::UnresolvedVariable(FunctionImpl* function, std::string name) : Variable(name), function(function)
	{
	}

	const std::string& UnresolvedVariable::getName()
	{
		return name;
	}

	Literal::Literal(std::string value, LiteralType type) : Value(new TypeConstraint()), value(value), type(type)
	{
	}
	
	Call::Call(Value* callee, std::vector<Value*> arguments) : Value(new TypeConstraint()), callee(callee), arguments(arguments)
	{
	}
	Block::Block(Scope* linkedScope) : Value(new TypeConstraint()), linkedScope(linkedScope)
	{
	}
	void Block::addValue(Value* value)
	{
		values.push_back(value);
	}
	Scope* Block::getScope()
	{
		return linkedScope;
	}
	UnitTypeValue* UnitTypeValue::getInstance()
	{
		static UnitTypeValue* instance = new UnitTypeValue();
		return instance;
	}
	TypeConstraint::TypeConstraint() : type(nullptr)
	{
	}

	TypeConstraint::TypeConstraint(Type* type) : type(type)
	{
	}

	UnitTypeValue::UnitTypeValue() : Value(new TypeConstraint())
	{
	}
} // namespace ozToy::HIR
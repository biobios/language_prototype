#include "AST.hpp"

#include <stack>

namespace ozToy::AST {

	void Module::addTopLevel(TopLevel* topLevel)
	{
		this->topLevel.push_back(topLevel);
	}

	void Module::generateHIR(HIR::ModuleBuilder& mBuilder)
	{
		auto&& m = *new HIR::ModuleBuilder(mBuilder.getModule().createModule(this->name));
		for (auto& topLevel : this->topLevel)
		{
			topLevel->generateHIR(m);
		}
		delete &m;
	}

	Module::~Module()
	{
		for (auto& topLevel : this->topLevel)
		{
			delete topLevel;
		}
	}

	Module* Module::parse(Scanner* scanner, std::ostream& errorOut)
	{
		scanner->consumeToken(); // Consume the module keyword
		auto name = scanner->getToken();
		if (name.type != TokenType::IDENTIFIER)
		{
			errorOut << "Expected module name, got " << name.toString() << std::endl;
			return nullptr;
		}
		auto module = new Module(name.text);
		auto leftBrace = scanner->getToken();
		
		if (leftBrace.type != TokenType::LEFT_BRACE)
		{
			errorOut << "Expected {, got " << leftBrace.toString() << std::endl;
			delete module;
			return nullptr;
		}
		
		while (true)
		{
			if (scanner->peekToken().type == TokenType::RIGHT_BRACE)
				break;
			auto topLevel = TopLevel::parse(scanner, errorOut);
			if (topLevel == nullptr)
			{
				delete module;
				return nullptr;
			}
			module->addTopLevel(topLevel);
		}
		return module;
	}

	Root::Root() {}

	void Root::addTopLevel(TopLevel* topLevel)
	{
		this->topLevel.push_back(topLevel);
	}

	void Root::generateHIR(HIR::ModuleBuilder& mBuilder)
	{
		for (auto& topLevel : this->topLevel)
		{
			topLevel->generateHIR(mBuilder);
		}
	}

	Root::~Root()
	{
		for (auto& topLevel : this->topLevel)
		{
			delete topLevel;
		}
	}

	Root* Root::parse(Scanner* scanner, std::ostream& errorOut)
	{
		auto root = new Root();
		while (true)
		{
			if (scanner->peekToken().type == TokenType::END_OF_FILE)
				break;
			auto topLevel = TopLevel::parse(scanner, errorOut);
			if (topLevel == nullptr)
			{
				delete root;
				return nullptr;
			}
			root->addTopLevel(topLevel);
		}
		return root;
	}

	TopLevel* TopLevel::parse(Scanner* scanner, std::ostream& errorOut)
	{
		Token token = scanner->peekToken();
		switch (token.type)
		{
		case TokenType::MODULE:
			return Module::parse(scanner, errorOut);
		case TokenType::FN:
			return DeclarationFunction::parse(scanner, errorOut);
		default:
			errorOut << "Expected top level declaration, got " << token.toString() << std::endl;
			return nullptr;
		}
	}

	void DeclarationFunction::addArgument(Argument* argument)
	{
		this->arguments.push_back(argument);
	}

	void DeclarationFunction::setReturnType(std::string returnType)
	{
		this->returnType = returnType;
	}

	void DeclarationFunction::setBody(BlockExpression* body)
	{
		this->body = body;
	}

	void DeclarationFunction::generateHIR(HIR::ModuleBuilder& mBuilder)
	{
		auto&& fBuilder = *new HIR::FunctionBuilder(mBuilder.getModule().createFunction(this->name));

		for (auto& argument : this->arguments)
		{
			fBuilder.addArgument(argument->getName(), argument->getType());
		}

		this->body->generateHIR(fBuilder);

		delete &fBuilder;
	}

	DeclarationFunction::~DeclarationFunction()
	{
		for (auto& argument : this->arguments)
		{
			delete argument;
		}
		delete this->body;
	}

	DeclarationFunction* DeclarationFunction::parse(Scanner* scanner, std::ostream& errorOut)
	{
		scanner->consumeToken(); // Consume the fn keyword
		auto name = scanner->getToken();
		if (name.type != TokenType::IDENTIFIER)
		{
			errorOut << "Expected function name, got " << name.toString() << std::endl;
			return nullptr;
		}

		auto function = new DeclarationFunction(name.text);

		if (scanner->getToken().type != TokenType::LEFT_PAREN)
		{
			errorOut << "Expected (, got " << name.toString() << std::endl;
			delete function;
			return nullptr;
		}

		if (scanner->peekToken().type != TokenType::RIGHT_PAREN)
		{
			while (true)
			{
				auto argument = Argument::parse(scanner, errorOut);
				if (argument == nullptr)
				{
					delete function;
					return nullptr;
				}
				function->addArgument(argument);
				if (scanner->peekToken().type == TokenType::RIGHT_PAREN)
					break;
				if (scanner->getToken().type != TokenType::COMMA)
				{
					errorOut << "Expected , or ), got " << scanner->getToken().toString() << std::endl;
					delete function;
					return nullptr;
				}
			}
		}

		scanner->consumeToken(); // Consume the )

		if (scanner->getToken().type != TokenType::ARROW)
		{
			errorOut << "Expected ->, got " << scanner->getToken().toString() << std::endl;
			delete function;
			return nullptr;
		}

		auto returnType = scanner->getToken();
		if (returnType.type != TokenType::IDENTIFIER)
		{
			errorOut << "Expected return type, got " << returnType.toString() << std::endl;
			delete function;
			return nullptr;
		}

		function->setReturnType(returnType.text);

		if (scanner->peekToken().type != TokenType::LEFT_BRACE)
		{
			errorOut << "Expected {, got " << scanner->getToken().toString() << std::endl;
			delete function;
			return nullptr;
		}

		auto body = BlockExpression::parse(scanner, errorOut);

		if (body == nullptr)
		{
			delete function;
			return nullptr;
		}

		function->setBody(body);

		return function;
	}

	Argument* Argument::parse(Scanner* scanner, std::ostream& errorOut)
	{
		auto name = scanner->getToken();
		if (name.type != TokenType::IDENTIFIER)
		{
			errorOut << "Expected argument name, got " << name.toString() << std::endl;
			return nullptr;
		}

		if (scanner->getToken().type != TokenType::COLON)
		{
			errorOut << "Expected :, got " << scanner->getToken().toString() << std::endl;
			return nullptr;
		}

		auto type = scanner->getToken();
		if (type.type != TokenType::IDENTIFIER)
		{
			errorOut << "Expected argument type, got " << type.toString() << std::endl;
			return nullptr;
		}

		return new Argument(name.text, type.text);
	}

	Expression* Expression::parsePrimary(Scanner* scanner, std::ostream& errorOut)
	{
		Token token = scanner->peekToken();
		switch (token.type) {
		case TokenType::IDENTIFIER:
			scanner->consumeToken();
			return new IdentifierExpression(token.text);
		case TokenType::NUMBER:
			scanner->consumeToken();
			return new NumberExpression(token.text);
		case TokenType::STRING:
			scanner->consumeToken();
			return new StringExpression(token.text);
		case TokenType::CHAR:
			scanner->consumeToken();
			return new CharExpression(token.text);
		case TokenType::LEFT_PAREN:
			scanner->consumeToken();
			{
				auto expression = parse(scanner, errorOut);
				if (expression == nullptr)
					return nullptr;
				if (scanner->getToken().type != TokenType::RIGHT_PAREN)
				{
					errorOut << "Expected ), got " << scanner->getToken().toString() << std::endl;
					delete expression;
					return nullptr;
				}
				return expression;
			}
		case TokenType::LEFT_BRACE:
			scanner->consumeToken();
			{
				auto expression = parse(scanner, errorOut);
				if (expression == nullptr)
					return nullptr;
				if (scanner->getToken().type != TokenType::RIGHT_BRACE)
				{
					errorOut << "Expected }, got " << scanner->getToken().toString() << std::endl;
					delete expression;
					return nullptr;
				}
				return expression;
			}
		case TokenType::LET:
		case TokenType::VAR:
		{
			auto expression = DeclarationVariable::parse(scanner, errorOut);
			if (expression == nullptr)
				return nullptr;
			return expression;
		}
		default:
			return new NOPExpression();
		}
	}

	Expression* Expression::parseLine(Scanner* scanner, std::ostream& errorOut)
	{
		std::stack<Expression*> expressionStack;
		std::stack<BinaryOperatorType> operatorStack;

		auto primary = parsePrimary(scanner, errorOut);
		if (primary == nullptr)
			return nullptr;
		expressionStack.push(primary);
		operatorStack.push(BinaryOperatorType::START);

		while (true) {
			Token token = scanner->peekToken();
			BinaryOperatorType operatorType = binaryOperatorTypeFromTokenType(token.type);
			BinaryOperatorType topOperatorType = operatorStack.top();

			ExpressionParseOperation operation = getOperation(topOperatorType, operatorType);
			switch (operation) {
			case ExpressionParseOperation::SHIFT:
				scanner->consumeToken();
				operatorStack.push(operatorType);
				{
					auto primary = parsePrimary(scanner, errorOut);
					if (primary == nullptr) {
						while (!expressionStack.empty()) {
							delete expressionStack.top();
							expressionStack.pop();
						}
						return nullptr;
					}
					expressionStack.push(primary);
				}
				continue;
			case ExpressionParseOperation::REDUCE:
			{
				auto right = expressionStack.top();
				expressionStack.pop();
				auto left = expressionStack.top();
				expressionStack.pop();
				auto operatorType = operatorStack.top();
				operatorStack.pop();
				expressionStack.push(new BinaryExpression(left, right, operatorType));
			}
			continue;
			case ExpressionParseOperation::ACCEPT:
				break;
			default:
				errorOut << "Unexpected token " << token.toString() << std::endl;
				while (!expressionStack.empty()) {
					delete expressionStack.top();
					expressionStack.pop();
				}
				return nullptr;
			}

			break;
		}

		return expressionStack.top();
	}

	Expression* Expression::parse(Scanner* scanner, std::ostream& errorOut)
	{
		Expression* expression = parseLine(scanner, errorOut);
		if (expression == nullptr)
			return nullptr;

		while (true) {
			if (scanner->peekToken().type != TokenType::SEMICOLON) break;

			scanner->consumeToken(); // Consume the ;

			auto nextExpression = parseLine(scanner, errorOut);
			if (nextExpression == nullptr)
			{
				delete expression;
				return nullptr;
			}
			expression = new SemicolonExpression(expression, nextExpression);
		}

		return expression;
	}

	ozToy::HIR::Value* DeclarationVariable::generateHIR(HIR::FunctionBuilder& fBuilder)
	{
		if (typeIsInferred)
			return fBuilder.declVariable(this->name, false);
		else
			return fBuilder.declVariable(this->name, this->type, false);
	}

	DeclarationVariable* DeclarationVariable::parse(Scanner* scanner, std::ostream& errorOut)
	{
		bool isMutable = scanner->getToken().type == TokenType::VAR;

		auto name = scanner->getToken();
		if (name.type != TokenType::IDENTIFIER)
		{
			errorOut << "Expected variable name, got " << name.toString() << std::endl;
			return nullptr;
		}

		if (scanner->peekToken().type != TokenType::COLON) {
			return new DeclarationVariable(name.text, isMutable);
		}

		scanner->consumeToken(); // Consume the :

		auto type = scanner->getToken();
		if (type.type != TokenType::IDENTIFIER)
		{
			errorOut << "Expected variable type, got " << type.toString() << std::endl;
			return nullptr;
		}

		return new DeclarationVariable(name.text, type.text, isMutable);
	}

	ozToy::HIR::Value* BlockExpression::generateHIR(HIR::FunctionBuilder& fBuilder)
	{
		fBuilder.createBlock();
		fBuilder.addInstruction(this->inner->generateHIR(fBuilder));
		return fBuilder.exitBlock();
	}

	BlockExpression::~BlockExpression()
	{
		delete this->inner;
	}

	BlockExpression* BlockExpression::parse(Scanner* scanner, std::ostream& errorOut)
	{
		scanner->consumeToken(); // Consume the {

		auto expression = Expression::parse(scanner, errorOut);
		if (expression == nullptr)
			return nullptr;

		if (scanner->getToken().type != TokenType::RIGHT_BRACE)
		{
			errorOut << "Expected }, got " << scanner->getToken().toString() << std::endl;
			delete expression;
			return nullptr;
		}

		return new BlockExpression(expression);
	}

	ozToy::HIR::Value* SemicolonExpression::generateHIR(HIR::FunctionBuilder& fBuilder)
	{
		fBuilder.addInstruction(this->previous->generateHIR(fBuilder));
		return this->next->generateHIR(fBuilder);
	}

	SemicolonExpression::~SemicolonExpression()
	{
		delete this->previous;
		delete this->next;
	}

	ozToy::HIR::Value* NumberExpression::generateHIR(HIR::FunctionBuilder& fBuilder)
	{
		return fBuilder.getLiteral(this->value, HIR::LiteralType::INT);
	}

	HIR::Value* StringExpression::generateHIR(HIR::FunctionBuilder& fBuilder)
	{
		return fBuilder.getLiteral(this->value, HIR::LiteralType::STRING);
	}

	HIR::Value* CharExpression::generateHIR(HIR::FunctionBuilder& fBuilder)
	{
		return fBuilder.getLiteral(this->value, HIR::LiteralType::CHAR);
	}

	HIR::Value* BinaryExpression::generateHIR(HIR::FunctionBuilder& fBuilder)
	{
		HIR::Value* hir_left, *hir_right;
		if (BinaryOperatorPriority[static_cast<size_t>(type)] == 1) {
			hir_right = right->generateHIR(fBuilder);
			hir_left = left->generateHIR(fBuilder);
		}
		else {
			hir_left = left->generateHIR(fBuilder);
			hir_right = right->generateHIR(fBuilder);
		}

		return new HIR::Call(fBuilder.getVariable("binary"), { hir_left, hir_right });
	}

	HIR::Value* NOPExpression::generateHIR(HIR::FunctionBuilder& fBuilder)
	{
		return HIR::UnitTypeValue::getInstance();
	}

	HIR::Value* IdentifierExpression::generateHIR(HIR::FunctionBuilder& fBuilder)
	{
		return fBuilder.getVariable(this->value);
	}

}
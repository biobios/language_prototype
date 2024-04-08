#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "HIRBuilder.hpp"

#include "Scanner.hpp"

namespace ozToy::AST {
	class Node {
	public:
		virtual ~Node() = default;
	};

	class TopLevel : public virtual Node {
	public:
		virtual ~TopLevel() = default;
		virtual void generateHIR(HIR::ModuleBuilder& mBuilder) = 0;
		static TopLevel* parse(Scanner* scanner, std::ostream& errorOut = std::cerr);
	};

	class Root : public virtual Node {
		std::vector<TopLevel*> topLevel;
	public:
		Root();
		void addTopLevel(TopLevel* topLevel);
		void generateHIR(HIR::ModuleBuilder& builder);
		virtual ~Root();
		static Root* parse(Scanner* scanner, std::ostream& errorOut = std::cerr);
	};

	class Expression : public virtual Node {
		static Expression* parsePrimary(Scanner* scanner, std::ostream& errorOut = std::cerr);
		static Expression* parseLine(Scanner* scanner, std::ostream& errorOut = std::cerr);
	public:
		virtual HIR::Value* generateHIR(HIR::FunctionBuilder & fBuilder) = 0;
		virtual ~Expression() = default;
		static Expression* parse(Scanner* scanner, std::ostream& errorOut = std::cerr);
	};

	class Argument : public virtual Node {
		std::string name;
		std::string type;
	public:
		Argument(std::string name, std::string type) : name(name), type(type) {}
		virtual ~Argument() = default;
		const std::string& getName() const { return name; }
		const std::string& getType() const { return type; }
		static Argument* parse(Scanner* scanner, std::ostream& errorOut = std::cerr);
	};

	class BlockExpression;

	class DeclarationFunction : public virtual TopLevel {
		std::string name;
		std::vector<Argument*> arguments;
		std::string returnType;
		BlockExpression* body = nullptr;
	public:
		DeclarationFunction(std::string name) : name(name) {}
		void addArgument(Argument* argument);
		void setReturnType(std::string returnType);
		void setBody(BlockExpression* body);
		void generateHIR(HIR::ModuleBuilder& mBuilder) override;
		virtual ~DeclarationFunction();
		static DeclarationFunction* parse(Scanner* scanner, std::ostream& errorOut = std::cerr);
	};

	class Module : public virtual TopLevel {
		std::string name;
		std::vector<TopLevel*> topLevel;
	public:
		Module(std::string name) : name(name) {}
		void addTopLevel(TopLevel* topLevel);
		void generateHIR(HIR::ModuleBuilder& mBuilder) override;
		virtual ~Module();
		static Module* parse(Scanner* scanner, std::ostream& errorOut = std::cerr);
	};

	class DeclarationVariable : public virtual Expression {
		std::string name;
		std::string type;
		bool isMutable;
		bool typeIsInferred;
	public:
		DeclarationVariable(std::string name, std::string type, bool isMutable) : name(name), type(type), isMutable(isMutable), typeIsInferred(false) {}
		DeclarationVariable(std::string name, bool isMutable) : name(name), isMutable(isMutable), typeIsInferred(true) {}
		HIR::Value* generateHIR(HIR::FunctionBuilder& fBuilder) override;
		virtual ~DeclarationVariable() = default;
		static DeclarationVariable* parse(Scanner* scanner, std::ostream& errorOut = std::cerr);
	};

	class BlockExpression : public virtual Expression {
		Expression* inner;
	public:
		BlockExpression(Expression* inner) : inner(inner) {}
		HIR::Value* generateHIR(HIR::FunctionBuilder& fBuilder) override;
		virtual ~BlockExpression();
		static BlockExpression* parse(Scanner* scanner, std::ostream& errorOut = std::cerr);
	};

	class SemicolonExpression : public virtual Expression {
		Expression* previous;
		Expression* next;
	public:
		SemicolonExpression(Expression* prev, Expression* next) : previous(prev), next(next) {}
		HIR::Value* generateHIR(HIR::FunctionBuilder& fBuilder) override;
		virtual ~SemicolonExpression();
	};

	class NumberExpression : public virtual Expression {
		std::string value;
	public:
		NumberExpression(std::string value) : value(value) {}
		HIR::Value* generateHIR(HIR::FunctionBuilder& fBuilder) override;
		virtual ~NumberExpression() = default;
	};

	class StringExpression : public virtual Expression {
		std::string value;
	public:
		StringExpression(std::string value) : value(value) {}
		HIR::Value* generateHIR(HIR::FunctionBuilder& fBuilder) override;
		virtual ~StringExpression() = default;
	};

	class CharExpression : public virtual Expression {
		std::string value;
	public:
		CharExpression(std::string value) : value(value) {}
		HIR::Value* generateHIR(HIR::FunctionBuilder& fBuilder) override;
		virtual ~CharExpression() = default;
	};

	class IdentifierExpression : public virtual Expression {
		std::string value;
	public:
		IdentifierExpression(std::string value) : value(value) {}
		HIR::Value* generateHIR(HIR::FunctionBuilder& fBuilder) override;
		virtual ~IdentifierExpression() = default;
	};

	class CallExpression : public virtual Expression {
		Expression* callee;
		std::vector<Expression*> arguments;
	public:
		virtual ~CallExpression() = default;
		static CallExpression* parse(Scanner* scanner, std::ostream& errorOut = std::cerr);
	};

	class BinaryExpression : public virtual Expression {
		Expression* left;
		Expression* right;
		BinaryOperatorType type;
	public:
		BinaryExpression(Expression* left, Expression* right, BinaryOperatorType type) : left(left), right(right), type(type) {}
		HIR::Value* generateHIR(HIR::FunctionBuilder& fBuilder) override;
		virtual ~BinaryExpression() = default;
	};

	class NOPExpression : public virtual Expression {

	public:
		NOPExpression() {}
		HIR::Value* generateHIR(HIR::FunctionBuilder& fBuilder) override;
		virtual ~NOPExpression() = default;
	};
}

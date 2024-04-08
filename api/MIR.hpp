#pragma once

#include <vector>
#include <string>

namespace ozToy::MIR {

	class Module;
	class ModuleDef;

	class Class;
	class ClassDef;

	class Struct;
	class StructDef;

	class Function;
	class FunctionDef;

	class Module {
		std::string name;
		ModuleDef* def;
	public:
	};

	class ModuleDef {
		Module* parent;
		std::vector<Module*> children;
		std::vector<Class*> classes;
		std::vector<Struct*> structs;
		std::vector<Function*> functions;
	public:
	};

	class Class {
		std::string name;
		ClassDef* def;
	public:
	};

	class ClassDef {
		Module* parent;
		std::vector<Function*> functions;
	public:
	};

	class Function {
		std::string name;
		FunctionDef* def;
	public:
	};

	class FunctionDef {
		Module* parent;
	};
}
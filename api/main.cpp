#include <iostream>
#include <string>
#include <fstream>
#include "Scanner.hpp"
#include "AST.hpp"
#include "HIRBuilder.hpp"

int main() {
	std::string fileNmae = "test.txt";
	std::ifstream file(fileNmae);
	if (!file.is_open()) {
		std::cout << "Error opening file: " << fileNmae << std::endl;
		return 1;
	}
	ozToy::Scanner scanner(&file);
	ozToy::AST::Root* root = ozToy::AST::Root::parse(&scanner);

	if(root != nullptr)
		std::cout << "Parsing successful!" << std::endl;
	else
		std::cout << "Parsing failed!" << std::endl;

	ozToy::HIR::TranslationUnit tu;
	ozToy::HIR::ModuleBuilder mBuilder(tu.getRootModule());

	root->generateHIR(mBuilder);

	std::cout << "HIR generation successful!" << std::endl;

	tu.print(std::cout);

	return 0;
}
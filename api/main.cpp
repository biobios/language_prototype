#include <iostream>
#include <string>
#include <fstream>
#include "Scanner.hpp"

int main() {
	std::string fileNmae = "test.txt";
	std::ifstream file(fileNmae);
	if (!file.is_open()) {
		std::cout << "Error opening file: " << fileNmae << std::endl;
		return 1;
	}
	ozToy::Scanner scanner(&file);
	while(true){
		ozToy::Token token = scanner.getToken();
		std::cout << token.toString() << std::endl;
		if (token.type == ozToy::TokenType::END_OF_FILE) break;
	}
	return 0;
}
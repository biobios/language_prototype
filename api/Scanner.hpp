#pragma once

#include <iostream>
#include <string>

namespace ozToy {

	enum class TokenType {
		END_OF_FILE,
		IDENTIFIER,
		NUMBER,
		STRING,
		CHAR,
		LEFT_PAREN,
		RIGHT_PAREN,
		LEFT_BRACE,
		RIGHT_BRACE,
		LEFT_BRACKET,
		RIGHT_BRACKET,
		COMMA,
		DOT,
		SEMICOLON,
		PLUS,
		MINUS,
		STAR,
		SLASH,
		PERCENT,
		CARET,
		AMPERSAND,
		PIPE,
		AMPERSAND_AMPERSAND,
		PIPE_PIPE,
		TILDE,
		BANG,
		QUESTION,
		COLON,
		EQUAL,
		LESS,
		GREATER,
		PLUS_EQUAL,
		MINUS_EQUAL,
		STAR_EQUAL,
		SLASH_EQUAL,
		PERCENT_EQUAL,
		AMPERSAND_EQUAL,
		PIPE_EQUAL,
		CARET_EQUAL,
		ASSIGN,
		PLUS_PLUS,
		MINUS_MINUS,
		LESS_LESS,
		GREATER_GREATER,
		LESS_EQUAL,
		GREATER_EQUAL,
		LESS_LESS_EQUAL,
		GREATER_GREATER_EQUAL,
		EQUAL_EQUAL,
		BANG_EQUAL,
		ARROW,
		LET,
		CLASS,
		STRUCT,
		ENUM,
		FN,
		IF,
		THEN,
		ELSE,
		TRUE,
		FALSE,
		OTHER,
		ERROR,
	};

	const std::string TokenTypeNames[] = {
		"END_OF_FILE",
		"IDENTIFIER",
		"NUMBER",
		"STRING",
		"CHAR",
		"LEFT_PAREN",
		"RIGHT_PAREN",
		"LEFT_BRACE",
		"RIGHT_BRACE",
		"LEFT_BRACKET",
		"RIGHT_BRACKET",
		"COMMA",
		"DOT",
		"SEMICOLON",
		"PLUS",
		"MINUS",
		"STAR",
		"SLASH",
		"PERCENT",
		"CARET",
		"AMPERSAND",
		"PIPE",
		"AMPERSAND_AMPERSAND",
		"PIPE_PIPE",
		"TILDE",
		"BANG",
		"QUESTION",
		"COLON",
		"EQUAL",
		"LESS",
		"GREATER",
		"PLUS_EQUAL",
		"MINUS_EQUAL",
		"STAR_EQUAL",
		"SLASH_EQUAL",
		"PERCENT_EQUAL",
		"AMPERSAND_EQUAL",
		"PIPE_EQUAL",
		"CARET_EQUAL",
		"ASSIGN",
		"PLUS_PLUS",
		"MINUS_MINUS",
		"LESS_LESS",
		"GREATER_GREATER",
		"LESS_EQUAL",
		"GREATER_EQUAL",
		"LESS_LESS_EQUAL",
		"GREATER_GREATER_EQUAL",
		"EQUAL_EQUAL",
		"BANG_EQUAL",
		"ARROW",
		"LET",
		"CLASS",
		"STRUCT",
		"ENUM",
		"FN",
		"IF",
		"THEN",
		"ELSE",
		"TRUE",
		"FALSE",
		"OTHER",
		"ERROR",
	};

	inline std::string* TokenTypeToString(TokenType type) {
		return (std::string*)&TokenTypeNames[(int)type];
	}

	struct Token
	{
		TokenType type;
		std::string text;
		std::string toString();
	};

	class Scanner
	{
		std::istream* input;
		Token lastToken;
		bool tokenUnget;

		TokenType scanKeywordOrIdentifier(std::string& text);
		Token scanIdentifier(char triggerChar);
		Token scanNumber(char triggerChar);
		Token scanString(char triggerChar);
		Token scanChar(char triggerChar);

	public:
		Scanner(std::istream* input);
		Token getToken();
		void putBackToken(Token token);
		void ungetToken();
	};
}


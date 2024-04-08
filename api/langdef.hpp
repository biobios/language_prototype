#pragma once

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
		VAR,
		CLASS,
		STRUCT,
		ENUM,
		MODULE,
		FN,
		IF,
		THEN,
		ELSE,
		TRUE,
		FALSE,
		OTHER,
		ERROR,
	};

	enum class BinaryOperatorType {
		PLUS, // +
		MINUS, // -
		MULTIPLY, // *
		DIVIDE, // /
		MODULO, // %
		PLUS_EQUAL, // +=
		MINUS_EQUAL, // -=
		MULTIPLY_EQUAL, // *=
		DIVIDE_EQUAL, // /=
		MODULO_EQUAL, // %=
		LESS, // <
		GREATER, // >
		LESS_EQUAL, // <=
		GREATER_EQUAL, // >=
		EQUAL, // ==
		NOT_EQUAL, // !=
		ASSIGN, // :=
		SUBSTITUTE, // =
	    COND_AND, // &&
		COND_OR, // ||
		AND, // &
		OR, // |
		XOR, // ^
		AND_EQUAL, // &=
		OR_EQUAL, // |=
		XOR_EQUAL, // ^=
		LEFT_SHIFT, // <<
		RIGHT_SHIFT, // >>
		LEFT_SHIFT_EQUAL, // <<=
		RIGHT_SHIFT_EQUAL, // >>=
	    START,
		END,
	};

	const std::uint8_t BinaryOperatorPriority[] = {
		10, // +
		10, // -
		11, // *
		11, // /
		11, // %
		1, // +=
		1, // -=
		1, // *=
		1, // /=
		1, // %=
		8, // <
		8, // >
		8, // <=
		8, // >=
		7, // ==
		7, // !=
		1, // :=
		1, // =
		3, // &&
		2, // ||
		6, // &
		4, // |
		5, // ^
		1, // &=
		1, // |=
		1, // ^=
		9, // <<
		9, // >>
		1, // <<=
		1, // >>=
		0, // START
		0, // END
	};

	enum class ExpressionParseOperation {
		SHIFT,
		REDUCE,
		ACCEPT,
		ERROR,
	};

	inline BinaryOperatorType binaryOperatorTypeFromTokenType(TokenType type) {
		switch (type) {
		case TokenType::PLUS:
			return BinaryOperatorType::PLUS;
		case TokenType::MINUS:
			return BinaryOperatorType::MINUS;
		case TokenType::STAR:
			return BinaryOperatorType::MULTIPLY;
		case TokenType::SLASH:
			return BinaryOperatorType::DIVIDE;
		case TokenType::PERCENT:
			return BinaryOperatorType::MODULO;
		case TokenType::PLUS_EQUAL:
			return BinaryOperatorType::PLUS_EQUAL;
		case TokenType::MINUS_EQUAL:
			return BinaryOperatorType::MINUS_EQUAL;
		case TokenType::STAR_EQUAL:
			return BinaryOperatorType::MULTIPLY_EQUAL;
		case TokenType::SLASH_EQUAL:
			return BinaryOperatorType::DIVIDE_EQUAL;
		case TokenType::PERCENT_EQUAL:
			return BinaryOperatorType::MODULO_EQUAL;
		case TokenType::LESS:
			return BinaryOperatorType::LESS;
		case TokenType::GREATER:
			return BinaryOperatorType::GREATER;
		case TokenType::LESS_EQUAL:
			return BinaryOperatorType::LESS_EQUAL;
		case TokenType::GREATER_EQUAL:
			return BinaryOperatorType::GREATER_EQUAL;
		case TokenType::EQUAL_EQUAL:
			return BinaryOperatorType::EQUAL;
		case TokenType::BANG_EQUAL:
			return BinaryOperatorType::NOT_EQUAL;
		case TokenType::ASSIGN:
			return BinaryOperatorType::ASSIGN;
		case TokenType::EQUAL:
			return BinaryOperatorType::SUBSTITUTE;
		case TokenType::AMPERSAND_AMPERSAND:
			return BinaryOperatorType::AND;
		case TokenType::PIPE_PIPE:
			return BinaryOperatorType::OR;
		case TokenType::CARET:
			return BinaryOperatorType::XOR;
		case TokenType::AMPERSAND:
			return BinaryOperatorType::AND;
		case TokenType::PIPE:
			return BinaryOperatorType::OR;
		case TokenType::CARET_EQUAL:
			return BinaryOperatorType::XOR_EQUAL;
		case TokenType::AMPERSAND_EQUAL:
			return BinaryOperatorType::AND_EQUAL;
		case TokenType::PIPE_EQUAL:
			return BinaryOperatorType::OR_EQUAL;
		case TokenType::LESS_LESS:
			return BinaryOperatorType::LEFT_SHIFT;
		case TokenType::GREATER_GREATER:
			return BinaryOperatorType::RIGHT_SHIFT;
		case TokenType::LESS_LESS_EQUAL:
			return BinaryOperatorType::LEFT_SHIFT_EQUAL;
		case TokenType::GREATER_GREATER_EQUAL:
			return BinaryOperatorType::RIGHT_SHIFT_EQUAL;
		default:
			return BinaryOperatorType::END;
		}
	}

	inline ExpressionParseOperation getOperation(BinaryOperatorType left, BinaryOperatorType right) {
		if (left == BinaryOperatorType::START && right == BinaryOperatorType::END) {
			return ExpressionParseOperation::ACCEPT;
		}
		
		std::uint8_t leftPriority = BinaryOperatorPriority[static_cast<std::uint8_t>(left)];
		std::uint8_t rightPriority = BinaryOperatorPriority[static_cast<std::uint8_t>(right)];

		if (leftPriority == 1 && rightPriority == 1) {
			return ExpressionParseOperation::SHIFT;
		}
		else if (leftPriority >= rightPriority) {
			return ExpressionParseOperation::REDUCE;
		}
		else {
			return ExpressionParseOperation::SHIFT;
		}
	}
}
#include "Scanner.hpp"

ozToy::TokenType ozToy::Scanner::scanKeywordOrIdentifier(std::string& text)
{
	if (text == "fn") return TokenType::FN;
	if (text == "if") return TokenType::IF;
	if (text == "then") return TokenType::THEN;
	if (text == "else") return TokenType::ELSE;
	if (text == "true") return TokenType::TRUE;
	if (text == "false") return TokenType::FALSE;
	if (text == "struct") return TokenType::STRUCT;
	if (text == "class") return TokenType::CLASS;
	if (text == "enum") return TokenType::ENUM;
}

ozToy::Token ozToy::Scanner::scanIdentifier(char triggerChar)
{
	Token token{ TokenType::IDENTIFIER, std::string(1, triggerChar) };
	while (true) {
		char c = input->get();
		if (('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z') || c == '_' || ('0' <= c && c <= '9')) {
			token.text += c;
		}
		else {
			input->unget();
			break;
		}
	}
	return token;
}

ozToy::Token ozToy::Scanner::scanNumber(char triggerChar)
{
	Token token{ TokenType::NUMBER, std::string(1, triggerChar) };
	while (true) {
		char c = input->get();
		if ('0' <= c && c <= '9') {
			token.text += c;
		}
		else {
			input->unget();
			break;
		}
	}
	return token;
}

ozToy::Token ozToy::Scanner::scanString(char triggerChar)
{
	Token token{ TokenType::STRING, std::string() };
	while (true) {
		char c = input->get();
		if (c == '\\') {
			c = input->get();
			switch (c) {
			case 'n':
				token.text += '\n';
				break;
			case 'r':
				token.text += '\r';
				break;
			case 't':
				token.text += '\t';
				break;
			case '\\':
				token.text += '\\';
				break;
			case '\'':
				token.text += '\'';
				break;
			case '\"':
				token.text += '\"';
				break;
			default:
				token.text += c;
				break;
			}
		}
		else if (c == triggerChar) {
			break;
		}
		else {
			token.text += c;
		}
	}
	return token;
}

ozToy::Token ozToy::Scanner::scanChar(char triggerChar)
{
	char c = input->get();
	if (c == '\\') {
		c = input->get();
		switch (c) {
		case 'n':
			c = '\n';
			break;
		case 'r':
			c = '\r';
			break;
		case 't':
			c = '\t';
			break;
		case '\\':
			c = '\\';
			break;
		case '\'':
			c = '\'';
			break;
		case '\"':
			c = '\"';
			break;
		default:
			break;
		}
	}
	char c2 = input->get();
	if (c2 != triggerChar) {
		return Token{ TokenType::ERROR, "Expected " + std::string(1, triggerChar) };
	}
	return Token{ TokenType::CHAR, std::string(1, c) };
}

ozToy::Scanner::Scanner(std::istream* input) : input(input), tokenUnget(false)
{
}

ozToy::Token ozToy::Scanner::getToken()
{
	if (tokenUnget)
	{
		tokenUnget = false;
		return lastToken;
	}

	char c = input->get();
	while (c == ' ' || c == '\t' || c == '\n' || c == '\r') {
		c = input->get();
	}
	if (('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z') || c == '_') {
		lastToken = scanIdentifier(c);
	}
	else if ('0' <= c && c <= '9') {
		lastToken = scanNumber(c);
	}
	else {
		switch (c) {
		case '+':
			c = input->get();
			if (c == '=') {
				lastToken = Token{ TokenType::PLUS_EQUAL, "+=" };
			}
			else if (c == '+') {
				lastToken = Token{ TokenType::PLUS_PLUS, "++" };
			}
			else {
				input->unget();
				lastToken = Token{ TokenType::PLUS, "+" };
			}
			break;
		case '-':
			c = input->get();
			if (c == '=') {
				lastToken = Token{ TokenType::MINUS_EQUAL, "-=" };
			}
			else if (c == '-') {
				lastToken = Token{ TokenType::MINUS_MINUS, "--" };
			}else if(c == '>') {
				lastToken = Token{ TokenType::ARROW, "->" };
			}
			else {
				input->unget();
				lastToken = Token{ TokenType::MINUS, "-" };
			}
			break;
		case '*':
			c = input->get();
			if (c == '=') {
				lastToken = Token{ TokenType::STAR_EQUAL, "*=" };
			}
			else {
				input->unget();
				lastToken = Token{ TokenType::STAR, "*" };
			}
			break;
		case '/':
			c = input->get();
			if (c == '=') {
				lastToken = Token{ TokenType::SLASH_EQUAL, "/=" };
			}
			else {
				input->unget();
				lastToken = Token{ TokenType::SLASH, "/" };
			}
			break;
		case '%':
			c = input->get();
			if (c == '=') {
				lastToken = Token{ TokenType::PERCENT_EQUAL, "%=" };
			}
			else {
				input->unget();
				lastToken = Token{ TokenType::PERCENT, "%" };
			}
			break;
		case '=':
			c = input->get();
			if (c == '=') {
				lastToken = Token{ TokenType::EQUAL_EQUAL, "==" };
			}
			else {
				input->unget();
				lastToken = Token{ TokenType::EQUAL, "=" };
			}
			break;
		case '!':
			c = input->get();
			if (c == '=') {
				lastToken = Token{ TokenType::BANG_EQUAL, "!=" };
			}
			else {
				input->unget();
				lastToken = Token{ TokenType::BANG, "!" };
			}
			break;
		case '<':
			c = input->get();
			if (c == '=') {
				lastToken = Token{ TokenType::LESS_EQUAL, "<=" };
			}
			else if (c == '<') {
				c = input->get();
				if (c == '=') {
					lastToken = Token{ TokenType::LESS_LESS_EQUAL, "<<=" };
				}
				else {
					input->unget();
					lastToken = Token{ TokenType::LESS_LESS, "<<" };
				}
			}
			else {
				input->unget();
				lastToken = Token{ TokenType::LESS, "<" };
			}
			break;
		case '>':
			c = input->get();
			if (c == '=') {
				lastToken = Token{ TokenType::GREATER_EQUAL, ">=" };
			}
			else if (c == '>') {
				c = input->get();
				if (c == '=') {
					lastToken = Token{ TokenType::GREATER_GREATER_EQUAL, ">>=" };
				}
				else {
					input->unget();
					lastToken = Token{ TokenType::GREATER_GREATER, ">>" };
				}
			}
			else {
				input->unget();
				lastToken = Token{ TokenType::GREATER, ">" };
			}
			break;
		case '&':
			c = input->get();
			if (c == '=') {
				lastToken = Token{ TokenType::AMPERSAND_EQUAL, "&=" };
			}
			else if (c == '&') {
				lastToken = Token{ TokenType::AMPERSAND_AMPERSAND, "&&" };
			}
			else {
				input->unget();
				lastToken = Token{ TokenType::AMPERSAND, "&" };
			}
			break;
		case '|':
			c = input->get();
			if (c == '=') {
				lastToken = Token{ TokenType::PIPE_EQUAL, "|=" };
			}
			else if (c == '|') {
				lastToken = Token{ TokenType::PIPE_PIPE, "||" };
			}
			else {
				input->unget();
				lastToken = Token{ TokenType::PIPE, "|" };
			}
			break;
		case '^':
			c = input->get();
			if (c == '=') {
				lastToken = Token{ TokenType::CARET_EQUAL, "^=" };
			}
			else {
				input->unget();
				lastToken = Token{ TokenType::CARET, "^" };
			}
			break;
		case '~':
			lastToken = Token{ TokenType::TILDE, "~" };
			break;
		case '?':
			lastToken = Token{ TokenType::QUESTION, "?" };
			break;
		case ':':
			c = input->get();
			if (c == '=') {
				lastToken = Token{ TokenType::ASSIGN, ":=" };
			}
			else {
				input->unget();
				lastToken = Token{ TokenType::COLON, ":" };
			}
			break;
		case ';':
			lastToken = Token{ TokenType::SEMICOLON, ";" };
			break;
		case ',':
			lastToken = Token{ TokenType::COMMA, "," };
			break;
		case '.':
			lastToken = Token{ TokenType::DOT, "." };
			break;
		case '(':
			lastToken = Token{ TokenType::LEFT_PAREN, "(" };
			break;
		case ')':
			lastToken = Token{ TokenType::RIGHT_PAREN, ")" };
			break;
		case '[':
			lastToken = Token{ TokenType::LEFT_BRACKET, "[" };
			break;
		case ']':
			lastToken = Token{ TokenType::RIGHT_BRACKET, "]" };
			break;
		case '{':
			lastToken = Token{ TokenType::LEFT_BRACE, "{" };
			break;
		case '}':
			lastToken = Token{ TokenType::RIGHT_BRACE, "}" };
			break;
		case '"':
			lastToken = scanString(c);
			break;
		case '\'':
			lastToken = scanChar(c);
			break;
		case EOF:
			lastToken = Token{ TokenType::END_OF_FILE, "" };
			break;
		default:
			lastToken = Token{ TokenType::OTHER, std::string(1, c) };
			break;
		}
	}
	return lastToken;
}

void ozToy::Scanner::putBackToken(Token token)
{
	tokenUnget = true;
	lastToken = token;
}

void ozToy::Scanner::ungetToken()
{
	tokenUnget = true;
}

std::string ozToy::Token::toString()
{
	return "Token{ type: " + *TokenTypeToString(type) + ", value: " + text + " }";
}

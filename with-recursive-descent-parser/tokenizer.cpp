#include "tokenizer.h"

Tokenizer::Tokenizer(std::string file_name) {
    std::ifstream in(file_name, std::ios::in);
	ss >> std::noskipws;
	ss << in.rdbuf();
	in.close();
}

std::string Tokenizer::readString(char end = '"') {
	int start_line_number = lineNumber;
    std::string str = "";
    char c;
    while (ss.peek() != end || c == '\\') {
		if (ss.eof()) { std::cerr << "String on line " + std::to_string(start_line_number) + " does not terminate"; exit(1); }
        ss >> c;
		if (c == '\n') { lineNumber += 1; }
        str += c;
    }
    return str;
}

Token* Tokenizer::getToken() {
    if (hasUngottenToken) {
        hasUngottenToken = false;
        return lastToken;
    }

    // Check if double quoted string
    if (lastToken->value() == "\"" && !inString) {
        Token* token = new Token(readString(), "STRING", lineNumber);
        inString = true;
        lastToken = token;
        return token;
    } else if (lastToken->value() == "\"" && inString) {
        inString = false;
    }

	// Check if single quoted string
    if (lastToken->value() == "'" && !inString) {
        Token* token = new Token(readString('\''), "STRING", lineNumber);
        inString = true;
        lastToken = token;
        return token;
    } else if (lastToken->value() == "'" && inString) {
        inString = false;
    }

    char c;
    ss >> c;

    Token* token;

    // Skip white space and get line number
    while (iswspace(c)) {
        if (ss.eof()) {
            token->eof() = true;
            return token;
        }
        if (c == '\n') lineNumber += 1;
        ss >> c;
    }

    // Check if end of file
    if (ss.eof()) {
        token->eof() = true;
        
    } else

    // Check if identifier
    if (isalpha(c) || c == '_') {
        token = new Token(c, "IDENTIFIER", lineNumber);
        while (isalpha(ss.peek()) || isdigit(ss.peek()) || ss.peek() == '_') {
            ss >> c;
            token->value() += c;
        }
		// Check if boolean true / false
		if (token->value() == "TRUE") {
			token->type() = "BOOLEAN_TRUE";
		} else if (token->value() == "FALSE") {
			token->type() = "BOOLEAN_FALSE";
		}
        
    } else

    // Check if integer
    if (isdigit(c)) {
        token = new Token(c, "INTEGER", lineNumber);
        while (isdigit(ss.peek())) {
            ss >> c;
            token->value() += c;
        }
        if (isalpha(ss.peek())) {
            std::cerr << "Syntax eror on line " << lineNumber << ": Invalid integer" << std::endl;
            exit(1);
        }
        
    } else

    // Check if left parenthesis
    if (c == '(') {
        token = new Token(c, "LEFT_PAREN", lineNumber);
        
    } else

    // Check if right parenthesis
    if (c == ')') {
        token = new Token(c, "RIGHT_PAREN", lineNumber);
        
    } else

    // Check if left bracket
    if (c == '[') {
        token = new Token(c, "LEFT_BRACKET", lineNumber);
        
    } else

    // Check if right bracket
    if (c == ']') {
        token = new Token(c, "RIGHT_BRACKET", lineNumber);
        
    } else

    // Check if left brace
    if (c == '{') {
        token = new Token(c, "LEFT_BRACE", lineNumber);
        
    } else 

    // Check if right brace
    if (c == '}') {
        token = new Token(c, "RIGHT_BRACE", lineNumber);
        
    } else

    // Check if single quote
    if (c == '\'') {
        token = new Token(c, "SINGLE_QUOTE", lineNumber);
        
    } else

    // Check if double quote
    if (c == '"') {
        token = new Token(c, "DOUBLE_QUOTE", lineNumber);
        
    } else

    // Check if semicolon
    if (c == ';') {
        token = new Token(c, "SEMICOLON", lineNumber);
        
    } else

    // Check if comma
    if (c == ',') {
        token = new Token(c, "COMMA", lineNumber);
        
    } else

    // Check if assignment operator
    if (c == '=') {
        if (ss.peek() == '=') {
            token = new Token("==", "BOOLEAN_EQUAL", lineNumber);
            ss >> c;
        } else {
            token = new Token(c, "ASSIGNMENT_OPERATOR", lineNumber);
        }
        
    } else

    // Check if plus
    if (c == '+') {
        token = new Token(c, "PLUS", lineNumber);
        
    } else

    // Check if minus
    if (c == '-') {
        if (isdigit(ss.peek())) {
            token = new Token(c, "INTEGER", lineNumber);
            while (isdigit(ss.peek())) {
                ss >> c;
                token->value() += c;
            }
            if (isalpha(ss.peek())) {
                std::cerr << "Syntax eror on line " << lineNumber << ": Invalid integer" << std::endl;
                exit(1);
            }
        } else {
            token = new Token(c, "MINUS", lineNumber);
        }
        
    } else

    // Check if asterisk
    if (c == '*') {
        token = new Token(c, "ASTERISK", lineNumber);
        
    } else

    // Check if divide
    if (c == '/') {
        token = new Token(c, "DIVIDE", lineNumber);
        
    } else

    // Check if modulo
    if (c == '%') {
        token = new Token(c, "MODULO", lineNumber);
        
    } else

    // Check if caret
    if (c == '^') {
        token = new Token(c, "CARET", lineNumber);
        
    } else

    // Check if less than
    if (c == '<') {
        if (ss.peek() == '=') {
            token = new Token("<=", "LT_EQUAL", lineNumber);
            ss >> c;
            
        } else {
            token = new Token(c, "LT", lineNumber);
            
        }
    } else

    // Check if greater than
    if (c == '>') {
        if (ss.peek() == '=') {
            token = new Token(">=", "GT_EQUAL", lineNumber);
            ss >> c;
            
        } else {
            token = new Token(c, "GT", lineNumber);
            
        }
    } else

    // Check if boolean and
    if (c == '&' && ss.peek() == '&') {
        token = new Token("&&", "BOOLEAN_AND", lineNumber);
        ss >> c;
        
    } else

    // Check if boolean or
    if (c == '|' && ss.peek() == '|') {
        token = new Token("||", "BOOLEAN_OR", lineNumber);
        ss >> c;
        
    } else

    // Check if boolean not
    if (c == '!') {
        if (ss.peek() == '=') {
            token = new Token("!=", "BOOLEAN_NOT_EQUAL", lineNumber);
            ss >> c;
        } else {
            token = new Token(c, "BOOLEAN_NOT", lineNumber);
        }
        
    } else
    
    {
        std::cerr << "Syntax error on line " << lineNumber  << ": Unidentified token" << std::endl;
        exit(1);
    }

    lastToken = token;
    return token;
}
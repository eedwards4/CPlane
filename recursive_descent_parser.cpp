#include "recursive_descent_parser.h"


// RecursiveDescentParser::RecursiveDescentParser(exec_node* head) {
// 	bool r = isProgram(head, 1);
// 	std::cout << (r ? "True" : "False " + error_message) << std::endl;
// }


bool RecursiveDescentParser::check_syntax(exec_node* head) {
	if (!isProgram(head, 1)) { // error in program
		std::cout << error_message << std::endl;
		return false;
	} else if (head != nullptr) { // more program after expected end of program
		std::cout << "Syntax error on line " + std::to_string(head->get_line()) + ": Expected EOF but got '" + head->get_value() + "' instead." << std::endl;
		return false;
	}
	return true;
}


// bool RecursiveDescentParser::isCharacter(Token*& token, int d) {
// 	if (token == nullptr) { setError(d, "Syntax error: Unexpectedly reached EOF."); return false; }
// 	Token* init = token;

// 	if (token->type() == "STRING") {
// 		if (token->value().size() == 1) {
// 			token = token->next();
// 			return true;
// 		}
// 	}

// 	setError(d, "Syntax error on line " + std::to_string(token->lineNumber()) + ": expected character, but got '" + token->value() + "' instead.");
// 	token = init;
// 	return false;
// }


// bool RecursiveDescentParser::isEscapedCharacter(Token*& token, int d) {
// 	if (token == nullptr) { setError(d, "Syntax error: Unexpectedly reached EOF."); return false; }
// 	Token* init = token;

// 	if (token->type() == "STRING" && token->value().size() > 1) {
// 		if (token->value() == "\\a" || token->value() == "\\b" || token->value() == "\\f" || token->value() == "\\n" || token->value() == "\\r" ||
// 			token->value() == "\\t" || token->value() == "\\v" || token->value() == "\\\\" || token->value() == "\\?" || token->value() == "\\'" ||
// 			token->value() == "\\\"") {
// 			token = token->next();
// 			return true;
// 		} else if (token->value()[0] == '\\' && token->value()[1] == 'x') {
// 			if (token->value().size() == 3) {
// 				if (isxdigit(token->value()[2])) {
// 					token = token->next();
// 					return true;
// 				}
// 			} else if (token->value().size() == 4) {
// 				if (isxdigit(token->value()[2]) && isxdigit(token->value()[3])) {
// 					token = token->next();
// 					return true;
// 				}
// 			}
// 		}
// 	}

// 	token = init;
// 	setError(d, "Syntax error on line " + std::to_string(token->lineNumber()) + ": expected escaped character, but got '" + token->value() + "' instead.");
// 	return false;
// }


bool RecursiveDescentParser::isWholeNumber(exec_node*& token, int d) {
	if (token == nullptr) { setError(d, "Syntax error: Unexpectedly reached EOF."); return false; }
	exec_node* init = token;

	if (token->get_type() == tokens::INT_AS_STRING && token->get_value()[0] != '-') {
		// token = token->next();
		to_next(token);
		return true;
	}

	token = init;
	setError(d, "Syntax error on line " + std::to_string(token->get_line()) + ": expected whole number, but got '" + token->get_value() + "' instead.");
	return false;
}


bool RecursiveDescentParser::isIdentifier(exec_node*& token, int d) {
	if (token == nullptr) { setError(d, "Syntax error: Unexpectedly reached EOF."); return false; }
	exec_node* init = token;

	if (token->get_type() == 9995) {
		if (token->get_value() == "TRUE" || token->get_value() == "FALSE" ||
			token->get_value() == "char" || token->get_value() == "bool" ||
			token->get_value() == "int" || token->get_value() == "if" ||
			token->get_value() == "for" || token->get_value() == "printf" ||
			token->get_value() == "getchar" || token->get_value() == "return" ||
			token->get_value() == "function" || token->get_value() == "procedure" ||
			token->get_value() == "main" || token->get_value() == "void") 
		{
			setError(d, "Syntax error on line " + std::to_string(token->get_line()) + ": identifier '" + token->get_value() + "' is reserved.");
			token = init;
			return false;
		}
		// token = token->next();
		to_next(token);
		return true;
	}

	token = init;
	setError(d, "Syntax error on line " + std::to_string(token->get_line()) + ": expected identifier, but got '" + token->get_value() + "' instead.");
	return false;
}

// <DOUBLE_QUOTE> <STRING> <DOUBLE_QUOTE>
bool RecursiveDescentParser::isDoubleQuotedString(exec_node*& token, int d) {
	if (token == nullptr) { setError(d, "Syntax error: Unexpectedly reached EOF."); return false; }
	exec_node* init = token;

	if (isType(token, 34, d + 1) && isType(token, tokens::STRING_LITERAL, d + 2) && isType(token, 34, d + 3)) {
		return true;
	}

	token = init;
	setError(d, "Syntax error on line " + std::to_string(token->get_line()) + ": expected double quoted string, but got '" + token->get_value() + "' instead.");
	return false;
}

// <SINGLE_QUOTE> <STRING> <SINGLE_QUOTE>
bool RecursiveDescentParser::isSingleQuotedString(exec_node*& token, int d) {
	if (token == nullptr) { setError(d, "Syntax error: Unexpectedly reached EOF."); return false; }
	exec_node* init = token;

	if (isType(token, 39, d + 1) && isType(token, tokens::CHAR_LITERAL, d + 2) && isType(token, 39, d + 3)) {
		return true;
	}

	token = init;
	setError(d, "Syntax error on line " + std::to_string(token->get_line()) + ": expected single quoted string, but got '" + token->get_value() + "' instead.");
	return false;
}

/*
<IDENTIFIER> <COMMA> <IDENTIFIER_LIST> |
<IDENTIFIER>
*/
bool RecursiveDescentParser::isIdentifierList(exec_node*& token, int d) {
	if (token == nullptr) { setError(d, "Syntax error: Unexpectedly reached EOF."); return false; }
	exec_node* init = token;

	if (isIdentifier(token, d + 1) && isType(token, 44, d + 2) && isIdentifierList(token, d + 3)) {
		return true;
	}
	token = init;
	if (isIdentifier(token, d + 1)) {
		return true;
	}

	token = init;
	setError(d, "Syntax error on line " + std::to_string(token->get_line()) + ": expected identifier list, but got '" + token->get_value() + "' instead.");
	return false;
}

/*
<IDENTIFIER> <L_BRACKET> <WHOLE_NUMBER> <R_BRACKET> <COMMA> <IDENTIFIER_ARRAY_LIST> |
<IDENTIFIER> <L_BRACKET> <IDENTIFIER> <R_BRACKET> <COMMA> <IDENTIFIER_ARRAY_LIST> |
<IDENTIFIER> <L_BRACKET> <WHOLE_NUMBER> <R_BRACKET> |
<IDENTIFIER> <L_BRACKET> <IDENTIFIER> <R_BRACKET> 
*/
bool RecursiveDescentParser::isIdentifierArrayList(exec_node*& token, int d) {
	if (token == nullptr) { setError(d, "Syntax error: Unexpectedly reached EOF."); return false; }
	exec_node* init = token;

	if (isIdentifier(token, d + 1) && isType(token, tokens::OPEN_BRACKET, d + 2) && isWholeNumber(token, d + 3) && isType(token, tokens::CLOSE_BRACKET, d + 4) && isType(token, 44, d + 5) && isIdentifierArrayList(token, d + 6)) {
		return true;
	}
	token = init;
	if (isIdentifier(token, d + 1) && isType(token, tokens::OPEN_BRACKET, d + 2) && isIdentifier(token, d + 3) && isType(token, tokens::CLOSE_BRACKET, d + 4) && isType(token, 44, d + 5) && isIdentifierArrayList(token, d + 6)) {
		return true;
	}
	token = init;
	if (isIdentifier(token, d + 1) && isType(token, tokens::OPEN_BRACKET, d + 2) && isWholeNumber(token, d + 3) && isType(token, tokens::CLOSE_BRACKET, d + 4)) {
		return true;
	}
	token = init;
	if (isIdentifier(token, d + 1) && isType(token, tokens::OPEN_BRACKET, d + 2) && isIdentifier(token, d + 3) && isType(token, tokens::CLOSE_BRACKET, d + 4)) {
		return true;
	}

	token = init;
	setError(d, "Syntax error on line " + std::to_string(token->get_line()) + ": expected identifier array list, but got '" + token->get_value() + "' instead.");
	return false;
}

/*
<IDENTIFIER_ARRAY_LIST> <COMMA> <IDENTIFIER_AND_IDENTIFIER_ARRAY_LIST> |
<IDENTIFIER_ARRAY_LIST> |
<IDENTIFIER> <COMMA> <IDENTIFIER_AND_IDENTIFIER_ARRAY_LIST> |
<IDENTIFIER>
*/
bool RecursiveDescentParser::isIdentifierAndIdentifierArrayList(exec_node*& token, int d) {
	if (token == nullptr) { setError(d, "Syntax error: Unexpectedly reached EOF."); return false; }
	exec_node* init = token;

	if (isIdentifierArrayList(token, d + 1) && isType(token, 44, d + 2) && isIdentifierAndIdentifierArrayList(token, d + 3)) {
		return true;
	}
	token = init;
	if (isIdentifierArrayList(token, d + 1)) {
		return true;
	}
	token = init;
	if (isIdentifier(token, d + 1) && isType(token, 44, d + 2) && isIdentifierAndIdentifierArrayList(token, d + 3)) {
		return true;
	}
	token = init;
	if (isIdentifier(token, d + 1)) {
		return true;
	}

	setError(d, "Syntax error on line " + std::to_string(token->get_line()) + ": expected identifier and identifier array list, but got '" + token->get_value() + "' instead.");
	token = init;
	return false;
}

// int | char | bool
bool RecursiveDescentParser::isDatatypeSpecifier(exec_node*& token, int d) {
	if (token == nullptr) { setError(d, "Syntax error: Unexpectedly reached EOF."); return false; }
	exec_node* init = token;

	if (token->get_value() == "int" || token->get_value() == "char" || token->get_value() == "bool") {
		// token = token->next();
		to_next(token);
		return true;
	}

	token = init;
	setError(d, "Syntax error on line " + std::to_string(token->get_line()) + ": expected datatype specifier, but got '" + token->get_value() + "' instead.");
	return false;
}

/*
<INTEGER> |
<GETCHAR_FUNCTION> |
<USER_DEFINED_FUNCTION> |
<IDENTIFIER> <L_BRACKET> <WHOLE_NUMBER> <R_BRACKET> |
<IDENTIFIER> |
<SINGLE_QUOTE> <CHARACTER> <SINGLE_QUOTE> |
<SINGLE_QUOTE> <ESCAPED_CHARACTER> <SINGLE_QUOTE> |
<DOUBLE_QUOTE> <CHARACTER> <DOUBLE_QUOTE> |
<DOUBLE_QUOTE> <ESCAPED_CHARACTER> <DOUBLE_QUOTE>
*/
bool RecursiveDescentParser::isNumericalOperand(exec_node*& token, int d) {
	if (token == nullptr) { setError(d, "Syntax error: Unexpectedly reached EOF."); return false; }
	exec_node* init = token;

	if (isType(token, tokens::INT_AS_STRING, d + 1)) {
		return true;
	}
	token = init;
	if (isGetcharFunction(token, d + 1)) {
		return true;
	}
	if (isUserDefinedFunction(token, d + 1)) {
		return true;
	}
	token = init;
	if (isIdentifier(token, d + 1) && isType(token, tokens::OPEN_BRACKET, d + 2) && isWholeNumber(token, d + 3) && isType(token, tokens::CLOSE_BRACKET, d + 4)) {
		return true;
	}
	token = init;
	if (isIdentifier(token, d + 1)) {
		return true;
	}
	token = init;
	if (isSingleQuotedString(token, d + 1)) {
		return true;
	}
	token = init;
	if (isDoubleQuotedString(token, d + 1)) {
		return true;
	}
	// if (isType(token, 39, d + 1) && isCharacter(token, d + 2) && isType(token, 39, d + 3)) {
	// 	return true;
	// }
	// token = init;
	// if (isType(token, 39, d + 1) && isEscapedCharacter(token, d + 2) && isType(token, 39, d + 3)) {
	// 	return true;
	// }
	// token = init;
	// if (isType(token, 34, d + 1) && isCharacter(token, d + 2) && isType(token, 34, d + 3)) {
	// 	return true;
	// }
	// token = init;
	// if (isType(token, 34, d + 1) && isEscapedCharacter(token, d + 2) && isType(token, 34, d + 3)) {
	// 	return true;
	// }

	token = init;
	setError(d, "Syntax error on line " + std::to_string(token->get_line()) + ": expected numerical operand, but got '" + token->get_value() + "' instead.");
	return false;
}

// <PLUS> | <MINUS> | <ASTERISK> | <DIVIDE> | <MODULO> | <CARET>
bool RecursiveDescentParser::isNumericalOperator(exec_node*& token, int d) {
	if (token == nullptr) { setError(d, "Syntax error: Unexpectedly reached EOF."); return false; }
	exec_node* init = token;

	if (token->get_type() == 43 || token->get_type() == 45 || token->get_type() == 42 ||
		token->get_type() == 47 || token->get_type() == 37 || token->get_type() == 94) {
		// token = token->next();
		to_next(token);
		return true;
	}

	token = init;
	setError(d, "Syntax error on line " + std::to_string(token->get_line()) + ": expected numerical operator, but got '" + token->get_value() + "' instead.");
	return false;

}

// <BOOLEAN_AND_OPERATOR> | <BOOLEAN_OR_OPERATOR>
bool RecursiveDescentParser::isBooleanOperator(exec_node*& token, int d) {
	if (token == nullptr) { setError(d, "Syntax error: Unexpectedly reached EOF."); return false; }
	exec_node* init = token;

	if (token->get_type() == tokens::BOOLEAN_AND || token->get_type() == tokens::BOOLEAN_OR) {
		// token = token->next();
		to_next(token);
		return true;
	}

	setError(d, "Syntax error on line " + std::to_string(token->get_line()) + ": expected boolean operator, but got '" + token->get_value() + "' instead.");
	token = init;
	return false;
}

// <BOOLEAN_EQUAL> | <BOOLEAN_NOT_EQUAL>
bool RecursiveDescentParser::isEqualityExpression(exec_node*& token, int d) {
	if (token == nullptr) { setError(d, "Syntax error: Unexpectedly reached EOF."); return false; }
	exec_node* init = token;

	if (token->get_type() == tokens::EQUALS_EQUALS || token->get_type() == tokens::NOT_EQUALS) {
		// token = token->next();
		to_next(token);
		return true;
	}

	token = init;
	setError(d, "Syntax error on line " + std::to_string(token->get_line()) + ": expected equality expression, but got '" + token->get_value() + "' instead.");
	return false;
}

// <LT> | <LT_EQUAL> | <GT> | <GT_EQUAL> | <BOOLEAN_EQUAL> | <BOOLEAN_NOT_EQUAL>
bool RecursiveDescentParser::isRelationalExpression(exec_node*& token, int d) {
	if (token == nullptr) { setError(d, "Syntax error: Unexpectedly reached EOF."); return false; }
	exec_node* init = token;

	if (token->get_type() == 60 || token->get_type() == tokens::LESS_EQUALS || token->get_type() == 62 || token->get_type() == tokens::GREATER_EQUALS ||
		token->get_type() == tokens::EQUALS_EQUALS || token->get_type() == tokens::NOT_EQUALS) {
		// token = token->next();
		to_next(token);
		return true;
	}

	token = init;
	setError(d, "Syntax error on line " + std::to_string(token->get_line()) + ": expected numerical operator, but got '" + token->get_value() + "' instead.");
	return false;
}

/*
<NUMERICAL_OPERAND> <NUMERICAL_OPERATOR> <LEFT_PAREN> <NUMERICAL_EXPRESSION> <RIGHT_PAREN> <NUMERICAL_OPERATOR> <NUMERICAL_EXPRESSION> |
<NUMERICAL_OPERAND> <NUMERICAL_OPERATOR> <LEFT_PAREN> <NUMERICAL_EXPRESSION> <RIGHT_PAREN> |
<NUMERICAL_OPERAND> <NUMERICAL_OPERATOR> <NUMERICAL_EXPRESSION> |
<NUMERICAL_OPERAND> |
<LEFT_PAREN> <NUMERICAL_OPERAND> <NUMERICAL_OPERATOR> <NUMERICAL_EXPRESSION> <RIGHT_PAREN> <NUMERICAL_OPERATOR> <NUMERICAL_EXPRESSION> |
<LEFT_PAREN> <NUMERICAL_OPERAND> <NUMERICAL_OPERATOR> <NUMERICAL_EXPRESSION> <RIGHT_PAREN> |
<LEFT_PAREN> <NUMERICAL_OPERAND> <RIGHT_PAREN>
*/
bool RecursiveDescentParser::isNumericalExpression(exec_node*& token, int d) {
	if (token == nullptr) { setError(d, "Syntax error: Unexpectedly reached EOF."); return false; }
	exec_node* init = token;

	if (isNumericalOperand(token, d + 1) && isNumericalOperator(token, d + 2) && isType(token, tokens::OPEN_PAREN, d + 3) && isNumericalExpression(token, d + 4) &&
		isType(token, tokens::CLOSE_PAREN, d + 5) && isNumericalOperator(token, d + 6) && isNumericalExpression(token, d + 7)) {
		return true;
	}
	token = init;
	if (isNumericalOperand(token, d + 1) && isNumericalOperator(token, d + 2) && isType(token, tokens::OPEN_PAREN, d + 3) && isNumericalExpression(token, d + 4) && isType(token, tokens::CLOSE_PAREN, d + 5)) {
		return true;
	}
	token = init;
	if (isNumericalOperand(token, d + 1) && isNumericalOperator(token, d + 2) && isNumericalExpression(token, d + 3)) {
		return true;
	}
	token = init;
	if (isNumericalOperand(token, d + 1)) {
		return true;
	}
	token = init;
	if (isType(token, tokens::OPEN_PAREN, d + 1) && isNumericalOperand(token, d + 2) && isNumericalOperator(token, d + 3) && isNumericalExpression(token, d + 4) &&
		isType(token, tokens::CLOSE_PAREN, d + 5) && isNumericalOperator(token, d + 6) && isNumericalExpression(token, d + 7)) {
		return true;
	}
	token = init;
	if (isType(token, tokens::OPEN_PAREN, d + 1) && isNumericalOperand(token, d + 2) && isNumericalOperator(token, d + 3) && isNumericalExpression(token, d + 4) && isType(token, tokens::CLOSE_PAREN, d + 5)) {
		return true;
	}
	token = init;
	if (isType(token, tokens::OPEN_PAREN, d + 1) && isNumericalOperand(token, d + 2) && isType(token, tokens::CLOSE_PAREN, d + 3)) {
		return true;
	}

	token = init;
	setError(d, "Syntax error on line " + std::to_string(token->get_line()) + ": expected numerical expression, but got '" + token->get_value() + "' instead.");
	return false;
}

/*
<BOOLEAN_TRUE> |
<BOOLEAN_FALSE> |
<USER_DEFINED_FUNCTION> |
<L_PAREN> <BOOLEAN_EXPRESSION> <R_PAREN> <BOOLEAN_OPERATOR> <BOOLEAN_EXPRESSION> |
<L_PAREN> <BOOLEAN_EXPRESSION> <BOOLEAN_OPERATOR> <BOOLEAN_EXPRESSION> <R_PAREN> |
<L_PAREN> <BOOLEAN_EXPRESSION> <R_PAREN> |
<NUMERICAL_EXPRESSION> <RELATIONAL_EXPRESSION> <NUMERICAL_EXPRESSION> |
<IDENTIFIER> <BOOLEAN_OPERATOR> <BOOLEAN_EXPRESSION> |
<IDENTIFIER>
*/
bool RecursiveDescentParser::isBooleanExpression(exec_node*& token, int d) {
	if (token == nullptr) { setError(d, "Syntax error: Unexpectedly reached EOF."); return false; }
	exec_node* init = token;

	if (token->get_value() == "TRUE" || token->get_value() == "FALSE") {
		// token = token->next();
		to_next(token);
		return true;
	}
	token = init;
	if (isType(token, tokens::OPEN_PAREN, d + 1) && isBooleanExpression(token, d + 2) && isType(token, tokens::CLOSE_PAREN, d + 3) && isBooleanOperator(token, d + 4) && isBooleanExpression(token, d + 5)) {
		return true;
	}
	token = init;
	if (isUserDefinedFunction(token, d + 1)) {
		return true;
	}
	token = init;
	if (isType(token, tokens::OPEN_PAREN, d + 1) && isBooleanExpression(token, d + 2) && isBooleanOperator(token, d + 3) && isBooleanExpression(token, d + 4) && isType(token, tokens::CLOSE_PAREN, d + 5)) {
		return true;
	}
	token = init;
	if (isType(token, tokens::OPEN_PAREN, d + 1) && isBooleanExpression(token, d + 2) && isType(token, tokens::CLOSE_PAREN, d + 3)) {
		return true;
	}
	token = init;
	if (isNumericalExpression(token, d + 1) && isRelationalExpression(token, d + 2) && isNumericalExpression(token, d + 3)) {
		return true;
	}
	token = init;
	if (isIdentifier(token, d + 1) && isBooleanOperator(token, d + 2) && isBooleanExpression(token, d + 3)) {
		return true;
	}
	token = init;
	if (isIdentifier(token, d + 1)) {
		return true;
	}

	token = init;
	setError(d, "Syntax error on line " + std::to_string(token->get_line()) + ": expected boolean expression, but got '" + token->get_value() + "' instead.");
	return false;
}

/*
<IDENTIFIER> <ASSIGNMENT_OPERATOR> <EXPRESSION> |
<IDENTIFIER> <ASSIGNMENT_OPERATOR> <SINGLE_QUOTED_STRING> |
<IDENTIFIER> <ASSIGNMENT_OPERATOR> <DOUBLE_QUOTED_STRING>
*/
bool RecursiveDescentParser::isInitializationExpression(exec_node*& token, int d) {
	if (token == nullptr) { setError(d, "Syntax error: Unexpectedly reached EOF."); return false; }
	exec_node* init = token;

	if (isIdentifier(token, d + 1) && isType(token, 61, d + 2) && isExpression(token, d + 3)) {
		return true;
	}
	token = init;
	if (isIdentifier(token, d + 1) && isType(token, 61, d + 2) && isSingleQuotedString(token, d + 3)) {
		return true;
	}
	token = init;
	if (isIdentifier(token, d + 1) && isType(token, 61, d + 2) && isDoubleQuotedString(token, d + 3)) {
		return true;
	}

	token = init;
	setError(d, "Syntax error on line " + std::to_string(token->get_line()) + ": expected initialization expression, but got '" + token->get_value() + "' instead.");
	return false;
}

// <NUMERICAL_EXPRESSION> | <BOOLEAN_EXPRESSION>
bool RecursiveDescentParser::isExpression(exec_node*& token, int d) {
	if (token == nullptr) { setError(d, "Syntax error: Unexpectedly reached EOF."); return false; }
	exec_node* init = token;

	if (isNumericalExpression(token, d + 1)) {
		return true;
	}
	token = init;
	if (isBooleanExpression(token, d + 1)) {
		return true;
	}

	token = init;
	setError(d, "Syntax error on line " + std::to_string(token->get_line()) + ": expected expression, but got '" + token->get_value() + "' instead.");
	return false;
}

/*
if <L_PAREN> <BOOLEAN_EXPRESSION> <R_PAREN> <STATEMENT> else <STATEMENT> |
if <L_PAREN> <BOOLEAN_EXPRESSION> <R_PAREN> <STATEMENT> else <BLOCK_STATEMENT> |
if <L_PAREN> <BOOLEAN_EXPRESSION> <R_PAREN> <STATEMENT> |
if <L_PAREN> <BOOLEAN_EXPRESSION> <R_PAREN> <BLOCK_STATEMENT> else <STATEMENT> |
if <L_PAREN> <BOOLEAN_EXPRESSION> <R_PAREN> <BLOCK_STATEMENT> else <BLOCK_STATEMENT> |
if <L_PAREN> <BOOLEAN_EXPRESSION> <R_PAREN> <BLOCK_STATEMENT>
*/
bool RecursiveDescentParser::isSelectionStatement(exec_node*& token, int d) {
	if (token == nullptr) { setError(d, "Syntax error: Unexpectedly reached EOF."); return false; }
	exec_node* init = token;

	if (isValue(token, "if", d + 1) && isType(token, tokens::OPEN_PAREN, d + 2) && isBooleanExpression(token, d + 3) && isType(token, tokens::CLOSE_PAREN, d + 4) && isStatement(token, d + 5) &&
		isValue(token, "else", d + 6) && isStatement(token, d + 7)) {
		return true;
	}
	token = init;
	if (isValue(token, "if", d + 1) && isType(token, tokens::OPEN_PAREN, d + 2) && isBooleanExpression(token, d + 3) && isType(token, tokens::CLOSE_PAREN, d + 4) && isStatement(token, d + 5) &&
		isValue(token, "else", d + 6) && isBlockStatement(token, d + 7)) {
		return true;
	}
	token = init;
	if (isValue(token, "if", d + 1) && isType(token, tokens::OPEN_PAREN, d + 2) && isBooleanExpression(token, d + 3) && isType(token, tokens::CLOSE_PAREN, d + 4) && isStatement(token, d + 5)) {
		return true;
	}
	token = init;
	if (isValue(token, "if", d + 1) && isType(token, tokens::OPEN_PAREN, d + 2) && isBooleanExpression(token, d + 3) && isType(token, tokens::CLOSE_PAREN, d + 4) && isBlockStatement(token, d + 5) &&
		isValue(token, "else", d + 6) && isStatement(token, d + 7)) {
		return true;
	}
	token = init;
	if (isValue(token, "if", d + 1) && isType(token, tokens::OPEN_PAREN, d + 2) && isBooleanExpression(token, d + 3) && isType(token, tokens::CLOSE_PAREN, d + 4) && isBlockStatement(token, d + 5) &&
		isValue(token, "else", d + 6) && isBlockStatement(token, d + 7)) {
		return true;
	}
	token = init;
	if (isValue(token, "if", d + 1) && isType(token, tokens::OPEN_PAREN, d + 2) && isBooleanExpression(token, d + 3) && isType(token, tokens::CLOSE_PAREN, d + 4) && isBlockStatement(token, d + 5)) {
		return true;
	}

	token = init;
	setError(d, "Syntax error on line " + std::to_string(token->get_line()) + ": expected selection statement, but got '" + token->get_value() + "' instead.");
	return false;
}

/*
for <L_PAREN> <INITIALIZATION_EXPRESSION> <SEMICOLON> <BOOLEAN_EXPRESSION> <SEMICOLON> <IDENTIFIER> <ASSIGNMENT_OPERATOR> <EXPRESSION> <R_PAREN> <STATEMENT> |
for <L_PAREN> <INITIALIZATION_EXPRESSION> <SEMICOLON> <BOOLEAN_EXPRESSION> <SEMICOLON> <IDENTIFIER> <ASSIGNMENT_OPERATOR> <EXPRESSION> <R_PAREN> <BLOCK_STATEMENT> |
for <L_PAREN> <INITIALIZATION_EXPRESSION> <SEMICOLON> <BOOLEAN_EXPRESSION> <SEMICOLON> <EXPRESSION> <R_PAREN> <STATEMENT> |
for <L_PAREN> <INITIALIZATION_EXPRESSION> <SEMICOLON> <BOOLEAN_EXPRESSION> <SEMICOLON> <EXPRESSION> <R_PAREN> <BLOCK_STATEMENT> |
while <L_PAREN> <BOOLEAN_EXPRESSION> <R_PAREN> <STATEMENT> |
while <L_PAREN> <BOOLEAN_EXPRESSION> <R_PAREN> <BLOCK_STATEMENT>
*/
bool RecursiveDescentParser::isIterationStatement(exec_node*& token, int d) {
	if (token == nullptr) { setError(d, "Syntax error: Unexpectedly reached EOF."); return false; }
	exec_node* init = token;

	if (isValue(token, "for", d + 1) && isType(token, tokens::OPEN_PAREN, d + 2) && isInitializationExpression(token, d + 3) && isType(token, 59, d + 4) &&
		isBooleanExpression(token, d + 5) && isType(token, 59, d + 6) && isIdentifier(token, d + 7) && isType(token, 61, d + 8) && isExpression(token, d + 9) && isType(token, tokens::CLOSE_PAREN, d + 10) && isStatement(token, d + 11)) {
		return true;
	}
	token = init;
	if (isValue(token, "for", d + 1) && isType(token, tokens::OPEN_PAREN, d + 2) && isInitializationExpression(token, d + 3) && isType(token, 59, d + 4) &&
		isBooleanExpression(token, d + 5) && isType(token, 59, d + 6) && isIdentifier(token, d + 7) && isType(token, 61, d + 8) && isExpression(token, d + 9) && isType(token, tokens::CLOSE_PAREN, d + 10) && isBlockStatement(token, d + 11)) {
		return true;
	}
	token = init;
	if (isValue(token, "for", d + 1) && isType(token, tokens::OPEN_PAREN, d + 2) && isInitializationExpression(token, d + 3) && isType(token, 59, d + 4) &&
		isBooleanExpression(token, d + 5) && isType(token, 59, d + 6) && isExpression(token, d + 7) && isType(token, tokens::CLOSE_PAREN, d + 8) && isStatement(token, d + 9)) {
		return true;
	}
	token = init;
	if (isValue(token, "for", d + 1) && isType(token, tokens::OPEN_PAREN, d + 2) && isInitializationExpression(token, d + 3) && isType(token, 59, d + 4) &&
		isBooleanExpression(token, d + 5) && isType(token, 59, d + 6) && isExpression(token, d + 7) && isType(token, tokens::CLOSE_PAREN, d + 8) && isBlockStatement(token, d + 9)) {
		return true;
	}
	token = init;
	if (isValue(token, "while", d + 1) && isType(token, tokens::OPEN_PAREN, d + 2) && isBooleanExpression(token, d + 3) && isType(token, tokens::CLOSE_PAREN, d + 4) && isStatement(token, d + 5)) {
		return true;
	}
	token = init;
	if (isValue(token, "while", d + 1) && isType(token, tokens::OPEN_PAREN, d + 2) && isBooleanExpression(token, d + 3) && isType(token, tokens::CLOSE_PAREN, d + 4) && isBlockStatement(token, d + 5)) {
		return true;
	}

	token = init;
	setError(d, "Syntax error on line " + std::to_string(token->get_line()) + ": expected iteration statement, but got '" + token->get_value() + "' instead.");
	return false;
}

/*
<IDENTIFIER_AND_IDENTIFIER_ARRAY_LIST> <ASSIGNMENT_OPERATOR> <EXPRESSION> <SEMICOLON> |
<IDENTIFIER_AND_IDENTIFIER_ARRAY_LIST> <ASSIGNMENT_OPERATOR> <SINGLE_QUOTED_STRING> <SEMICOLON> |
<IDENTIFIER_AND_IDENTIFIER_ARRAY_LIST> <ASSIGNMENT_OPERATOR> <DOUBLE_QUOTED_STRING> <SEMICOLON>
*/
bool RecursiveDescentParser::isAssignmentStatement(exec_node*& token, int d) {
	if (token == nullptr) { setError(d, "Syntax error: Unexpectedly reached EOF."); return false; }
	exec_node* init = token;

	if (isIdentifierAndIdentifierArrayList(token, d + 1) && isType(token, 61, d + 2) && isExpression(token, d + 3) && isType(token, 59, d + 4)) {
		return true;
	}
	token = init;
	if (isIdentifierAndIdentifierArrayList(token, d + 1) && isType(token, 61, d + 2) && isSingleQuotedString(token, d + 3) && isType(token, 59, d + 4)) {
		return true;
	}
	token = init;
	if (isIdentifierAndIdentifierArrayList(token, d + 1) && isType(token, 61, d + 2) && isDoubleQuotedString(token, d + 3) && isType(token, 59, d + 4)) {
		return true;
	}

	token = init;
	setError(d, "Syntax error on line " + std::to_string(token->get_line()) + ": expected assignment statement, but got '" + token->get_value() + "' instead.");
	return false;
}

/*
printf <L_PAREN> <DOUBLE_QUOTED_STRING> <R_PAREN> <SEMICOLON> |
printf <L_PAREN> <SINGLE_QUOTED_STRING> <R_PAREN> <SEMICOLON> |
printf <L_PAREN> <DOUBLE_QUOTED_STRING> <COMMA> <IDENTIFIER_AND_IDENTIFIER_ARRAY_LIST> <R_PAREN> <SEMICOLON> |
printf <L_PAREN> <SINGLE_QUOTED_STRING> <COMMA> <IDENTIFIER_AND_IDENTIFIER_ARRAY_LIST> <R_PAREN> <SEMICOLON>
*/
bool RecursiveDescentParser::isPrintfStatement(exec_node*& token, int d) {
	if (token == nullptr) { setError(d, "Syntax error: Unexpectedly reached EOF."); return false; }
	exec_node* init = token;

	if (isValue(token, "printf", d + 1) && isType(token, tokens::OPEN_PAREN, d + 2) && isDoubleQuotedString(token, d + 3) && isType(token, tokens::CLOSE_PAREN, d + 4) && isType(token, 59, d + 5)) {
		return true;
	}
	token = init;
	if (isValue(token, "printf", d + 1) && isType(token, tokens::OPEN_PAREN, d + 2) && isSingleQuotedString(token, d + 3) && isType(token, tokens::CLOSE_PAREN, d + 4) && isType(token, 59, d + 5)) {
		return true;
	}
	token = init;
	if (isValue(token, "printf", d + 1) && isType(token, tokens::OPEN_PAREN, d + 2) && isDoubleQuotedString(token, d + 3) && isType(token, 44, d + 4) &&
		isIdentifierAndIdentifierArrayList(token, d + 5) && isType(token, tokens::CLOSE_PAREN, d + 6) && isType(token, 59, d + 7)) {
		return true;
	}
	token = init;
	if (isValue(token, "printf", d + 1) && isType(token, tokens::OPEN_PAREN, d + 2) && isSingleQuotedString(token, d + 3) && isType(token, 44, d + 4) &&
		isIdentifierAndIdentifierArrayList(token, d + 5) && isType(token, tokens::CLOSE_PAREN, d + 6) && isType(token, 59, d + 7)) {
		return true;
	}

	token = init;
	setError(d, "Syntax error on line " + std::to_string(token->get_line()) + ": expected printf statement, but got '" + token->get_value() + "' instead.");
	return false;
}

// getchar <L_PAREN> <IDENTIFIER> <R_PAREN>
bool RecursiveDescentParser::isGetcharFunction(exec_node*& token, int d) {
	if (token == nullptr) { setError(d, "Syntax error: Unexpectedly reached EOF."); return false; }
	exec_node* init = token;

	if (isValue(token, "getchar", d + 1) && isType(token, tokens::OPEN_PAREN, d + 2) && isIdentifier(token, d + 3) && isType(token, tokens::CLOSE_PAREN, d + 4)) {
		return true;
	}

	token = init;
	setError(d, "Syntax error on line " + std::to_string(token->get_line()) + ": expected getchar function, but got '" + token->get_value() + "' instead.");
	return false;
}

/*
<IDENTIFIER> <L_PAREN> <EXPRESSION> <R_PAREN>
<IDENTIFIER> <L_PAREN> <IDENTIFIER_AND_IDENTIFIER_ARRAY_LIST> <R_PAREN> |
*/
bool RecursiveDescentParser::isUserDefinedFunction(exec_node*& token, int d) {
	if (token == nullptr) { setError(d, "Syntax error: Unexpectedly reached EOF."); return false; }
	exec_node* init = token;

	if (isIdentifier(token, d + 1) && isType(token, tokens::OPEN_PAREN, d + 2) && isExpression(token, d + 3) && isType(token, tokens::CLOSE_PAREN, d + 4)) {
		return true;
	}
	token = init;
	if (isIdentifier(token, d + 1) && isType(token, tokens::OPEN_PAREN, d + 2) && isIdentifierAndIdentifierArrayList(token, d + 3) && isType(token, tokens::CLOSE_PAREN, d + 4)) {
		return true;
	}

	token = init;
	setError(d, "Syntax error on line " + std::to_string(token->get_line()) + ": expected user defined function, but got '" + token->get_value() + "' instead.");
	return false;
}

/*
<DATATYPE_SPECIFIER> <IDENTIFIER> <SEMICOLON> |
<DATATYPE_SPECIFIER> <IDENTIFIER_AND_IDENTIFIER_ARRAY_LIST> <SEMICOLON>
*/
bool RecursiveDescentParser::isDeclarationStatement(exec_node*& token, int d) {
	if (token == nullptr) { setError(d, "Syntax error: Unexpectedly reached EOF."); return false; }
	exec_node* init = token;

	if (isDatatypeSpecifier(token, d + 1) && isIdentifier(token, d + 2) && isType(token, 59, d + 3)) {
		return true;
	}
	token = init;
	if (isDatatypeSpecifier(token, d + 1) && isIdentifierAndIdentifierArrayList(token, d + 2) && isType(token, 59, d + 3)) {
		return true;
	}

	token = init;
	setError(d, "Syntax error on line " + std::to_string(token->get_line()) + ": expected declaration statement, but got '" + token->get_value() + "' instead.");
	return false;
}

/*
return <EXPRESSION> <SEMICOLON> |
return <SINGLE_QUOTED_STRING> <SEMICOLON> |
return <DOUBLE_QUOTED_STRING> <SEMICOLON>
*/
bool RecursiveDescentParser::isReturnStatement(exec_node*& token, int d) {
	if (token == nullptr) { setError(d, "Syntax error: Unexpectedly reached EOF."); return false; }
	exec_node* init = token;

	if (isValue(token, "return", d + 1) && isExpression(token, d + 2) && isType(token, 59, d + 3)) {
		return true;
	}
	token = init;
	if (isValue(token, "return", d + 1) && isSingleQuotedString(token, d + 2) && isType(token, 59, d + 3)) {
		return true;
	}
	token = init;
	if (isValue(token, "return", d + 1) && isDoubleQuotedString(token, d + 2) && isType(token, 59, d + 3)) {
		return true;
	}

	token = init;
	setError(d, "Syntax error on line " + std::to_string(token->get_line()) + ": expected return statement, but got '" + token->get_value() + "' instead.");
	return false;
}

// <DECLARATION_STATEMENT> | <ASSIGNMENT_STATEMENT> | <ITERATION_STATEMENT> | <SELECTION_STATEMENT> | <PRINTF_STATEMENT> | <RETURN_STATEMENT> | <USER_DEFINED_FUNCTION> <SEMICOLON>
bool RecursiveDescentParser::isStatement(exec_node*& token, int d) {
	if (token == nullptr) { setError(d, "Syntax error: Unexpectedly reached EOF."); return false; }
	exec_node* init = token;

	if (isDeclarationStatement(token, d + 1)) {
		return true;
	}
	token = init;
	if (isAssignmentStatement(token, d + 1)) {
		return true;
	}
	token = init;
	if (isIterationStatement(token, d + 1)) {
		return true;
	}
	token = init;
	if (isSelectionStatement(token, d + 1)) {
		return true;
	}
	token = init;
	if (isPrintfStatement(token, d + 1)) {
		return true;
	}
	token = init;
	if (isReturnStatement(token, d + 1)) {
		return true;
	}
	token = init;
	if (isUserDefinedFunction(token, d + 1) && isType(token, 59, d + 2)) {
		return true;
	}

	token = init;
	setError(d, "Syntax error on line " + std::to_string(token->get_line()) + ": expected statement, but got '" + token->get_value() + "' instead.");
	return false;
}

// <STATEMENT> <COMPOUND_STATEMENT> | <STATEMENT>
bool RecursiveDescentParser::isCompoundStatement(exec_node*& token, int d) {
	if (token == nullptr) { setError(d, "Syntax error: Unexpectedly reached EOF."); return false; }
	exec_node* init = token;

	if (isStatement(token, d + 1) && isCompoundStatement(token, d + 2)) {
		return true;
	}
	token = init;
	if (isStatement(token, d + 1)) {
		return true;
	}

	token = init;
	setError(d, "Syntax error on line " + std::to_string(token->get_line()) + ": expected compound statement, but got '" + token->get_value() + "' instead.");
	return false;
}

// <L_BRACE> <COMPOUND_STATEMENT> <R_BRACE>
bool RecursiveDescentParser::isBlockStatement(exec_node*& token, int d) {
	if (token == nullptr) { setError(d, "Syntax error: Unexpectedly reached EOF."); return false; }
	exec_node* init = token;

	if (isType(token, tokens::OPEN_BRACE, d + 1) && isCompoundStatement(token, d + 2) && isType(token, tokens::CLOSE_BRACE, d + 3)) {
		return true;
	}

	token = init;
	setError(d, "Syntax error on line " + std::to_string(token->get_line()) + ": expected block statement, but got '" + token->get_value() + "' instead.");
	return false;
}

/*
<DATATYPE_SPECIFIER> <IDENTIFIER> <L_BRACKET> <WHOLE_NUMBER> <R_BRACKET> <COMMA> <PARAMETER_LIST> |
<DATATYPE_SPECIFIER> <IDENTIFIER> <L_BRACKET> <WHOLE_NUMBER> <R_BRACKET> |
<DATATYPE_SPECIFIER> <IDENTIFIER> <COMMA> <PARAMETER_LIST> |
<DATATYPE_SPECIFIER> <IDENTIFIER>
*/
bool RecursiveDescentParser::isParameterList(exec_node*& token, int d) {
	if (token == nullptr) { setError(d, "Syntax error: Unexpectedly reached EOF."); return false; }
	exec_node* init = token;

	if (isDatatypeSpecifier(token, d + 1) && isIdentifier(token, d + 2) && isType(token, tokens::OPEN_BRACKET, d + 3) && isWholeNumber(token, d + 4) &&
		isType(token, tokens::CLOSE_BRACKET, d + 5) && isType(token, 44, d + 6) && isParameterList(token, d + 7)) {
		return true;
	}
	token = init;
	if (isDatatypeSpecifier(token, d + 1) && isIdentifier(token, d + 2) && isType(token, tokens::OPEN_BRACKET, d + 3) && isWholeNumber(token, d + 4) &&
		isType(token, tokens::CLOSE_BRACKET, d + 5)) {
		return true;
	}
	token = init;
	if (isDatatypeSpecifier(token, d + 1) && isIdentifier(token, d + 2) && isType(token, 44, d + 3) && isParameterList(token, d + 4)) {
		return true;
	}
	token = init;
	if (isDatatypeSpecifier(token, d + 1) && isIdentifier(token, d + 2)) {
		return true;
	}

	token = init;
	setError(d, "Syntax error on line " + std::to_string(token->get_line()) + ": expected parameter list, but got '" + token->get_value() + "' instead.");
	return false;
}

/*
function <DATATYPE_SPECIFIER> <IDENTIFIER> <L_PAREN> <PARAMETER_LIST> <R_PAREN> <L_BRACE> <COMPOUND_STATEMENT> <R_BRACE> |
function <DATATYPE_SPECIFIER> <IDENTIFIER> <L_PAREN> void <R_PAREN> < L_BRACE> <COMPOUND_STATEMENT> <R_BRACE>
*/
bool RecursiveDescentParser::isFunctionDeclaration(exec_node*& token, int d) {
	if (token == nullptr) { setError(d, "Syntax error: Unexpectedly reached EOF."); return false; }
	exec_node* init = token;

	if (isValue(token, "function", d + 1) && isDatatypeSpecifier(token, d + 2) && isIdentifier(token, d + 3) && isType(token, tokens::OPEN_PAREN, d + 4) && isParameterList(token, d + 5) &&
		isType(token, tokens::CLOSE_PAREN, d + 6) && isType(token, tokens::OPEN_BRACE, d + 7) && isCompoundStatement(token, d + 8) && isType(token, tokens::CLOSE_BRACE, d + 9)) {
		return true;
	}
	token = init;
	if (isValue(token, "function", d + 1) && isDatatypeSpecifier(token, d + 2) && isIdentifier(token, d + 3) && isType(token, tokens::OPEN_PAREN, d + 4) && isValue(token, "void", d + 5) &&
		isType(token, tokens::CLOSE_PAREN, d + 6) && isType(token, tokens::OPEN_BRACE, d + 7) && isCompoundStatement(token, d + 8) && isType(token, tokens::CLOSE_BRACE, d + 9)) {
		return true;
	}

	token = init;
	setError(d, "Syntax error on line " + std::to_string(token->get_line()) + ": expected function declaration, but got '" + token->get_value() + "' instead.");
	return false;
}

/*
procedure <IDENTIFIER> <L_PAREN> <PARAMETER_LIST> <R_PAREN> < L_BRACE> <COMPOUND_STATEMENT> <R_BRACE> |
procedure <IDENTIFIER> <L_PAREN> void <R_PAREN> < L_BRACE> <COMPOUND_STATEMENT> <R_BRACE>
*/
bool RecursiveDescentParser::isProcedureDeclaration(exec_node*& token, int d) {
	if (token == nullptr) { setError(d, "Syntax error: Unexpectedly reached EOF."); return false; }
	exec_node* init = token;

	if (isValue(token, "procedure", d + 1) && isIdentifier(token, d + 2) && isType(token, tokens::OPEN_PAREN, d + 3) && isParameterList(token, d + 4) &&
		isType(token, tokens::CLOSE_PAREN, d + 5) && isType(token, tokens::OPEN_BRACE, d + 6) && isCompoundStatement(token, d + 7) && isType(token, tokens::CLOSE_BRACE, d + 8)) {
		return true;
	}
	token = init;
	if (isValue(token, "procedure", d + 1) && isIdentifier(token, d + 2) && isType(token, tokens::OPEN_PAREN, d + 3) && isValue(token, "void", d + 4) &&
		isType(token, tokens::CLOSE_PAREN, d + 5) && isType(token, tokens::OPEN_BRACE, d + 6) && isCompoundStatement(token, d + 7) && isType(token, tokens::CLOSE_BRACE, d + 8)) {
		return true;
	}

	token = init;
	setError(d, "Syntax error on line " + std::to_string(token->get_line()) + ": expected procedure declaration, but got '" + token->get_value() + "' instead.");
	return false;
}

// procedure main <L_PAREN> void <R_PAREN> <BLOCK_STATEMENT>
bool RecursiveDescentParser::isMainProcedure(exec_node*& token, int d) {
	if (token == nullptr) { setError(d, "Syntax error: Unexpectedly reached EOF."); return false; }
	exec_node* init = token;

	if (isValue(token, "procedure", d + 1) && isValue(token, "main", d + 2) && isType(token, tokens::OPEN_PAREN, d + 3) && isValue(token, "void", d + 4) &&
		isType(token, tokens::CLOSE_PAREN, d + 5) && isBlockStatement(token, d + 6)) {
		return true;
	}

	token = init;
	setError(d, "Syntax error on line " + std::to_string(token->get_line()) + ": expected main procedure, but got '" + token->get_value() + "' instead.");
	return false;
}

// <MAIN_PROCEDURE> | <FUNCTION_DECLARATION> <PROGRAM> | <PROCEDURE_DECLARATION> <PROGRAM> | <DECLARATION_STATEMENT> <PROGRAM>
bool RecursiveDescentParser::isProgram(exec_node*& token, int d) {
	if (token == nullptr) { setError(d, "Syntax error: Unexpectedly reached EOF."); return false; }
	exec_node* init = token;

	if (isMainProcedure(token, d + 1)) {
		return true;
	}
	token = init;
	if (isFunctionDeclaration(token, d + 1) && isProgram(token, d + 2)) {
		return true;
	}
	token = init;
	if (isProcedureDeclaration(token, d + 1) && isProgram(token, d + 2)) {
		return true;
	}
	token = init;
	if (isDeclarationStatement(token, d + 1) && isProgram(token, d + 2)) {
		return true;
	}

	token = init;
	setError(d, "Syntax error on line " + std::to_string(token->get_line()) + ": expected program, but got '" + token->get_value() + "' instead.");
	return false;
}

#ifndef RECURSIVE_DESCENT_PARSER_H
#define RECURSIVE_DESCENT_PARSER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "token.h"
#include "tokenizer.h"

class RecursiveDescentParser {
	public:
		RecursiveDescentParser(Tokenizer& tokenizer, std::string outfilepath);

		Token* head() { return _head; }
		Token* tail() { return _tail; }

		bool checkSyntax();
		void printCST();

	private:
		std::string outfile;

		Token* _head;
		Token* _tail;

		void setError(int d, std::string message) {
			if (d >= depth) {
				error_message = message;
				depth = d;
			}
		}

		bool isType(Token*& token, std::string type, int d) {
			if (token == nullptr) { return false; }
			if (token->type() == type) {
				token = token->next();
				return true;
			}
			setError(d, "Error on line " + std::to_string(token->lineNumber()) + ": Expected '" + type + "' but got '" + token->type() + "' instead.\n");
			return false;
		}

		bool isValue(Token*& token, std::string value, int d) {
			if (token == nullptr) { return false; }
			if (token->value() == value) {
				token = token->next();
				return true;
			}
			setError(d, "Error on line " + std::to_string(token->lineNumber()) + ": Expected '" + value + "' but got '" + token->type() + "' instead.\n");
			return false;
		}

		std::string error_message = "";
		int depth = 0;

		// Functions for recursive descent parser (35)
		bool isCharacter(Token*& token, int d);
		bool isEscapedCharacter(Token*& token, int d);
		bool isWholeNumber(Token*& token, int d);
		bool isDoubleQuotedString(Token*& token, int d);
		bool isSingleQuotedString(Token*& token, int d);
		bool isIdentifier(Token*& token, int d);
		bool isIdentifierList(Token*& token, int d);
		bool isIdentifierArrayList(Token*& token, int d);
		bool isIdentifierAndIdentifierArrayList(Token*& token, int d);
		bool isDatatypeSpecifier(Token*& token, int d);
		bool isNumericalOperand(Token*& token, int d);
		bool isNumericalOperator(Token*& token, int d);
		bool isBooleanOperator(Token*& token, int d);
		bool isEqualityExpression(Token*& token, int d);
		bool isRelationalExpression(Token*& token, int d);
		bool isNumericalExpression(Token*& token, int d);
		bool isBooleanExpression(Token*& token, int d);
		bool isInitializationExpression(Token*& token, int d);
		bool isExpression(Token*& token, int d);
		bool isSelectionStatement(Token*& token, int d);
		bool isIterationStatement(Token*& token, int d);
		bool isAssignmentStatement(Token*& token, int d);
		bool isPrintfStatement(Token*& token, int d);
		bool isGetcharFunction(Token*& token, int d);
		bool isUserDefinedFunction(Token*& token, int d);
		bool isDeclarationStatement(Token*& token, int d);
		bool isReturnStatement(Token*& token, int d);
		bool isStatement(Token*& token, int d);
		bool isCompoundStatement(Token*& token, int d);
		bool isBlockStatement(Token*& token, int d);
		bool isParameterList(Token*& token, int d);
		bool isFunctionDeclaration(Token*& token, int d);
		bool isProcedureDeclaration(Token*& token, int d);
		bool isMainProcedure(Token*& token, int d);
		bool isProgram(Token*& token, int d);
};

#endif

/*
Reserved Identifiers:
TRUE
FALSE
char
bool
int
if
for
printf
getchar
return
function
procedure
main
void
*/
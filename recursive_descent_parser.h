#ifndef RECURSIVE_DESCENT_PARSER_H
#define RECURSIVE_DESCENT_PARSER_H

#include <iostream>
#include <string>

#include "exec_path.h"
#include "errors_new.h"

class RecursiveDescentParser {
	public:
		RecursiveDescentParser(std::string in_filename, ERRORS& errors) {
			filename = in_filename;
			RST_errors = errors;
		}

		bool check_syntax(exec_node *head);
		void returnErrors(){
			RST_errors.STOP_SYNTAX_ERRORS();
		}

	private:
	
		//void ENC_ERROR(std::string filename, int code, exec_node*& token, std::string error_msg);
		void setErrorNew(int d, exec_node*& token, std::string message) {
			if ( d >= depth ){
				//message = "DEFAULT RDP ERROR";
				//error_message = message;
				// ERROR CODE 1 default
				if ( token == nullptr ){
					RST_errors.ENC_ERROR(filename, 1, 0, 0, message);
				}
				RST_errors.ENC_ERROR(filename, 1, token->get_line(), token->get_column(), message);
				depth = d;
			}
		}

		bool isType(exec_node*& token, int type, int d) {
			if (token == nullptr) { return false; }
			if (token->get_type() == type) {
				// token = token->next();
				to_next(token);
				return true;
			}
			//setError(d, "Error on line " + std::to_string(token->get_line()) + ": Expected '" + std::to_string(type) + "' but got '" + std::to_string(token->get_type()) + "' instead.\n");
			error_message = "expected " + std::to_string(type) + " but found '" + std::to_string(token->get_type()) + "' instead.";
			setErrorNew(d, token, error_message);
			return false;
		}

		bool isValue(exec_node*& token, std::string value, int d) {
			if (token == nullptr) { return false; }
			if (token->get_value() == value) {
				// token = token->next();
				to_next(token);
				return true;
			}
			//setError(d, "Error on line " + std::to_string(token->get_line()) + ": Expected '" + value + "' but got '" + token->get_value() + "' instead.\n");
			error_message = "expected " + value + " but found '" + token->get_value() + "' instead.";
			setErrorNew(d, token, error_message);
			return false;
		}

		void to_next(exec_node*& token) {
			// std::cout << token->get_value() << " (" << token->get_type() << ") ";
			if (token->get_next() != nullptr) {
				token = token->get_next();
			} else {
				token = token->get_fold();
			}
		}

		ERRORS RST_errors;
		std::string filename;
		std::string error_message = "";
		int depth = 0;


		// Functions for recursive descent parser (35 - 2, 33 total)
		// bool isCharacter(exec_node*& token, int d);
		// bool isEscapedCharacter(exec_node*& token, int d);
		bool isWholeNumber(exec_node*& token, int d);
		bool isDoubleQuotedString(exec_node*& token, int d);
		bool isSingleQuotedString(exec_node*& token, int d);
		bool isIdentifier(exec_node*& token, int d);
		bool isIdentifierList(exec_node*& token, int d);
		bool isIdentifierArrayList(exec_node*& token, int d);
		bool isIdentifierAndIdentifierArrayList(exec_node*& token, int d);
		bool isDatatypeSpecifier(exec_node*& token, int d);
		bool isNumericalOperand(exec_node*& token, int d);
		bool isNumericalOperator(exec_node*& token, int d);
		bool isBooleanOperator(exec_node*& token, int d);
		bool isEqualityExpression(exec_node*& token, int d);
		bool isRelationalExpression(exec_node*& token, int d);
		bool isNumericalExpression(exec_node*& token, int d);
		bool isBooleanExpression(exec_node*& token, int d);
		bool isInitializationExpression(exec_node*& token, int d);
		bool isExpression(exec_node*& token, int d);
		bool isSelectionStatement(exec_node*& token, int d);
		bool isIterationStatement(exec_node*& token, int d);
		bool isAssignmentStatement(exec_node*& token, int d);
		bool isPrintfStatement(exec_node*& token, int d);
		bool isGetcharFunction(exec_node*& token, int d);
		bool isUserDefinedFunction(exec_node*& token, int d);
		bool isDeclarationStatement(exec_node*& token, int d);
		bool isReturnStatement(exec_node*& token, int d);
		bool isStatement(exec_node*& token, int d);
		bool isCompoundStatement(exec_node*& token, int d);
		bool isBlockStatement(exec_node*& token, int d);
		bool isParameterList(exec_node*& token, int d);
		bool isFunctionDeclaration(exec_node*& token, int d);
		bool isProcedureDeclaration(exec_node*& token, int d);
		bool isMainProcedure(exec_node*& token, int d);
		bool isProgram(exec_node*& token, int d);
};

#endif //RECURSIVE_DESCENT_PARSER_H

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
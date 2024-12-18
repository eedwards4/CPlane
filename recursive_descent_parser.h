#ifndef RECURSIVE_DESCENT_PARSER_H
#define RECURSIVE_DESCENT_PARSER_H

#include <iostream>
#include <string>

#include "exec_path.h"
#include "errors.h"


class RecursiveDescentParser {
	public:
		RecursiveDescentParser(std::string in_filename, ERRORS& errors) {
			filename = in_filename;
			RST_errors = errors;
		}

		bool check_syntax(exec_node *head);
		ERRORS& returnErrors(){
			return RST_errors;
		}

	private:
		tokens tokenz;
		void setErrorNew(int d, exec_node*& token, std::string message) {
			if ( d >= depth ){
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
				to_next(token);
				return true;
			}
			
			error_message = "expected " + tokens::what_is(type) + " but found '" + tokens::what_is(token->get_type()) + "' instead.";
			setErrorNew(d, token, error_message);
			return false;
		}

		bool isValue(exec_node*& token, std::string value, int d) {
			if (token == nullptr) { return false; }
			if (token->get_value() == value) {
				to_next(token);
				return true;
			}
			
			error_message = "expected " + value + " but found '" + token->get_value() + "' instead.";
			setErrorNew(d, token, error_message);
			return false;
		}

		void to_next(exec_node*& token) {
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
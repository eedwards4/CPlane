#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <iostream>
#include <fstream>
#include <string>
#include <stack>

#include "token.h"

class Symbol {
	public:
		Symbol(std::string, std::string, Symbol*, std::string, bool, int, int, Symbol*, int ln);

		std::string& identifierName() { return identifier_name; }
		std::string& identifierType() { return identifier_type; }
		Symbol*& parameterList() { return parameter_list; }
		std::string& datatype() { return _datatype; }
		bool& isArray() { return is_array; }
		int& arraySize() { return array_size; }
		int& scope() { return _scope; }
		Symbol*& next() { return _next; }

		int& lineNumber() { return line_number; }

	private:
		std::string identifier_name;
		std::string identifier_type;
		Symbol* parameter_list;
		std::string _datatype;
		bool is_array;
		int array_size;
		// value?
		int _scope;
		Symbol* _next;

		int line_number;

};

class SymbolTable {
	public:
		SymbolTable(Token* head, std::string outfilepath);

		bool create();
		void print(Symbol* start = nullptr);

	private:
		Symbol* _head;
		Symbol* _tail;

		Token* token_head;

		std::string outfile;

		int scope_counter;
		std::stack<int> scope_stack;

		Symbol* getParameterList(Token*& token);
		bool checkRedefined(Symbol* symbol, Symbol* start = nullptr);

		std::string getValue(Token*& token) {
			std::string v = token->value();
			token = token->next();
			return v;
		}
};

#endif
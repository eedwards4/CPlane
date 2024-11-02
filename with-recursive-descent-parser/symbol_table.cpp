#include "symbol_table.h"

Symbol::Symbol(std::string name, std::string type, Symbol* parameter, std::string dt, bool isArr, int arrSize, int s, Symbol* n, int ln) {
	identifier_name = name;
	identifier_type = type;
	parameter_list = parameter;
	_datatype = dt;
	is_array = isArr;
	array_size = arrSize;
	_scope = s;
	_next = n;

	line_number = ln;
}


SymbolTable::SymbolTable(Token* token, std::string outfilepath) {
	scope_counter = 0;
	scope_stack.push(0);
	_head = _tail = nullptr;
	token_head = token;

	outfile = outfilepath;
	std::ofstream out(outfile); // clear file
	out.close();
}


void SymbolTable::print(Symbol* start) {
	Symbol* current = start;
	if (start == nullptr) {
		current = _head;
	}

	std::ofstream out(outfile, std::ios::app);

	while (current != nullptr) {
		out << "IDENTIFIER_NAME: " << current->identifierName() << std::endl;
		out << "IDENTIFIER_TYPE: " << current->identifierType() << std::endl;
		out << "DATATYPE: " << current->datatype() << std::endl;
		out << "IS_ARRAY: " << (current->isArray() ? "True" : "False") << std::endl;
		out << "ARRAY_SIZE: " << std::to_string(current->arraySize()) << std::endl;
		out << "SCOPE: " << std::to_string(current->scope()) << std::endl << std::endl;
		current = current->next();
	}

	out.close();

	if (start != nullptr) { return; }

	current = _head;
	while (current != nullptr) {
		if (current->parameterList() != nullptr) {
			std::ofstream out(outfile, std::ios::app);
			out << "PARAMETER_LIST_FOR: " << current->identifierName() << std::endl;
			out.close();
			print(current->parameterList());
		}
		current = current->next();
	}
}


Symbol* SymbolTable::getParameterList(Token*& token) {
	if (token->value() == "void") {
		token = token->next();
		return nullptr;
	}
	int temp_ln = token->lineNumber();
	std::string temp_dt = getValue(token);
	std::string temp_name = getValue(token);
	bool temp_isArr = false;
	int temp_arrSize = 0;
	Symbol* temp_next = nullptr;
	if (token->type() == "LEFT_BRACKET") {
		token = token->next(); // skip left bracket
		temp_isArr = true;
		temp_arrSize = stoi(getValue(token));
		token = token->next(); // skip right bracket
	}
	if (token->type() == "COMMA") {
		token = token->next();
		temp_next = getParameterList(token);
	}
	return new Symbol(temp_name, "datatype", nullptr, temp_dt, temp_isArr, temp_arrSize, scope_stack.top(), temp_next, temp_ln);
}

bool SymbolTable::checkRedefined(Symbol* symbol, Symbol* start) {
	Symbol* current = start;
	if (start == nullptr) {
		current = _head;
	}

	while (current != nullptr) {
		if (current->identifierName() == symbol->identifierName()) {
			if (current->scope() == 0) {
				std::ofstream out(outfile);
				out << "Error on line " + std::to_string(symbol->lineNumber()) + ": symbol '" + symbol->identifierName() + "' is already defined globally." << std::endl;
				out.close();
				return false;
			}
			
			if (current->scope() == symbol->scope()) {
				std::ofstream out(outfile);
				out << "Error on line " + std::to_string(symbol->lineNumber()) + ": symbol '" + symbol->identifierName() + "' is already defined locally." << std::endl;
				out.close();
				return false;
			}
		}
		current = current->next();
	}

	if (start != nullptr) { return true; }

	current = _head;
	while (current != nullptr) {
		if (current->parameterList() != nullptr) {
			if (!checkRedefined(symbol, current->parameterList())) {
				return false;
			}
		}
		current = current->next();
	}

	return true;
}


bool SymbolTable::create() {
	Token* current = token_head;

	while (current != nullptr) {
		if (current->type() == "IDENTIFIER" && current->value() == "function") {
			scope_counter += 1;
			scope_stack.push(scope_counter);
			int temp_ln = current->lineNumber();
			std::string temp_type = getValue(current);
			std::string temp_dt = getValue(current);
			std::string temp_name = getValue(current);
			current = current->next(); // skip left parenthesis
			Symbol* temp_param = getParameterList(current);
			current = current->next(); // skip right parenthesis
			current = current->next(); // skip left brace

			Symbol* new_symbol = new Symbol(temp_name, temp_type, temp_param, temp_dt, false, 0, scope_stack.top(), nullptr, temp_ln);
			if (!checkRedefined(new_symbol)) {
				return false;
			}

			if (_head == nullptr) {
				_head = _tail = new_symbol;
			} else {
				_tail->next() = new_symbol;
				_tail = _tail->next();
			}

		} else

		if (current->type() == "IDENTIFIER" && current->value() == "procedure") {
			scope_counter += 1;
			scope_stack.push(scope_counter);
			int temp_ln = current->lineNumber();
			std::string temp_type = getValue(current);
			std::string temp_name = getValue(current);
			current = current->next(); // skip left parenthesis
			Symbol* temp_param = getParameterList(current);
			current = current->next(); // skip right parenthesis
			current = current->next(); // skip left brace

			Symbol* new_symbol = new Symbol(temp_name, temp_type, temp_param, "NOT APPLICABLE", false, 0, scope_stack.top(), nullptr, temp_ln);
			if (!checkRedefined(new_symbol)) {
				return false;
			}

			if (_head == nullptr) {
				_head = _tail = new_symbol;
			} else {
				_tail->next() = new_symbol;
				_tail = _tail->next();
			}

		} else

		if (current->type() == "IDENTIFIER" && (current->value() == "int" || current->value() == "char" || current->value() == "bool")) {
			int temp_ln = current->lineNumber();
			std::string temp_dt = getValue(current);
			std::string temp_name = getValue(current);
			bool temp_isArr = false;
			int temp_arrSize = 0;
			Symbol* temp_next = nullptr;
			if (current->type() == "LEFT_BRACKET") {
				current = current->next(); // skip left bracket
				temp_isArr = true;
				temp_arrSize = stoi(getValue(current));
				current = current->next(); // skip right bracket
			}

			Symbol* list_head =  new Symbol(temp_name, "datatype", nullptr, temp_dt, temp_isArr, temp_arrSize, scope_stack.top(), temp_next, temp_ln);
			Symbol* list_tail = list_head;

			if (!checkRedefined(list_head)) {
				return false;
			}

			while (current->type() == "COMMA") {
				temp_ln = current->lineNumber();
				current = current->next();
				temp_name = getValue(current);
				temp_isArr = false;
				temp_arrSize = 0;
				temp_next = nullptr;
				if (current->type() == "LEFT_BRACKET") {
					current = current->next(); // skip left bracket
					temp_isArr = true;
					temp_arrSize = stoi(getValue(current));
					current = current->next(); // skip right bracket
				}
				Symbol* new_symbol = new Symbol(temp_name, "datatype", nullptr, temp_dt, temp_isArr, temp_arrSize, scope_stack.top(), temp_next, temp_ln);
				if (!checkRedefined(new_symbol)) {
					return false;
				}
				list_tail->next() = new_symbol;
				list_tail = list_tail->next();
			}

			if (_head == nullptr) {
				_head = list_head;
				_tail = list_tail; // new Symbol(temp_name, "datatype", nullptr, temp_dt, temp_isArr, temp_arrSize, scope_stack.top(), temp_next);
			} else {
				_tail->next() = list_head; // new Symbol(temp_name, "datatype", nullptr, temp_dt, temp_isArr, temp_arrSize, scope_stack.top(), temp_next);
				_tail = list_tail;
			}

		} else
		
		if (current->type() == "LEFT_BRACE") {
			scope_counter += 1;
			scope_stack.push(scope_counter);
			current = current->next();

		} else

		if (current->type() == "RIGHT_BRACE") {
			scope_stack.pop();
			current = current->next();
		}

		else { current = current->next(); }
	}
	return true;
}
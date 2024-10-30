//
// Created by Ethan Edwards on 10/24/2024.
// Edited by Logan Puntous on 10/29/2024.
//

#include "symbol_table.h"





// Destructor
symbol_table::~symbol_table() {
    for (symbol_node* current : scopes){ // 1. Iterate through each scope
        while (current != nullptr) { // 2. Iterate through each symbol in the current scope
            symbol_node* temp = current;
            current = current->getNext();
            delete temp; // 3. Delete the current symbol node
        }
    }
}




// Generates the inital symbol table using token stream.
void symbol_table::generateSymbolTable(exec_path &path) {
    exec_node* current = path.get_head(); // Get the head of the execution path
    int scope = 0; // Initialize the scope level at global (0)
    int function_scope = 0; // Track the function scope level (i.e how many functions/procedures we have)

    while (current != nullptr) {
        if (current->get_type() == tokens::TOKEN_AS_STRING) {
            // Handle symbol declaration
            if (current->get_value() == "function"){ // Check if the value is a function
                if (scope != 0) {
                    errors::E_NESTED_FUNCTION_NOT_ALLOWED( current->get_line(), current->get_column(), current->get_next()->get_next()->get_value());
                }
                auto newSymbol = new symbol_node();
                newSymbol->setIDENT_TYPE(symbols::identifiers::FUNCTION);
		function_scope++;
		// First token
                current = current->get_next();
                newSymbol->setDATATYPE(symbols::data_types::check_type(current->get_value()));

		// Second token
                current = current->get_next();
                newSymbol->setIDENT_NAME(current->get_value());
                newSymbol->set_line(current->get_line());
                newSymbol->set_column(current->get_column());
                newSymbol->setSCOPE(function_scope);

		// Adding new function symbol to symbol table
                symbol_table::addSymbol(newSymbol);
                // Consume parameters
                current = addParameters(newSymbol, current);
            }
            else if (current->get_value() == "procedure"){ // Check if the value is a procedure
                if (scope != 0) {
                    errors::E_NESTED_FUNCTION_NOT_ALLOWED( current->get_line(), current->get_column(), current->get_next()->get_next()->get_value());
                }
                auto newSymbol = new symbol_node();
                function_scope++;
                newSymbol->setIDENT_TYPE(symbols::identifiers::PROCEDURE);
                newSymbol->setDATATYPE(symbols::data_types::NA); // Procedures do not have a return type

                current = current->get_next();
                newSymbol->setIDENT_NAME(current->get_value()); // Get the procedure name
                newSymbol->setSCOPE(function_scope); // Set the scope
                symbol_table::addSymbol(newSymbol); // Add the new symbol to the symbol table
                // Add line/col info
                newSymbol->set_line(current->get_line());
                newSymbol->set_column(current->get_column());
                // Consume parameters
                current = addParameters(newSymbol, current); // Add parameters to the procedure
            }
            else if (symbols::data_types::check_type(current->get_value()) != -1) { // Check if the value is defining a new variable
                int dataType = symbols::data_types::check_type(current->get_value());
                if (scope > 0){
                    current = addVariables(current, dataType, function_scope);
                } else{
                    current = addVariables(current, dataType, scope); // Add variables to the symbol table
                }
            }
        }
        // Navigation
        if (current->get_next() == nullptr && current->get_fold() != nullptr) {
            if (current->get_type() == tokens::OPEN_BRACE || current->get_type() == '{') {
                scope++;
            } else if (current->get_type() == tokens::CLOSE_BRACE || current->get_type() == '}') {
                scope--;
            }
            current = current->get_fold();
        } else{
            current = current->get_next();
        }
    }
}

exec_node* symbol_table::addVariables(exec_node* current, int dataType, int scope) {
    auto newSymbol = new symbol_node();
    newSymbol->setIDENT_TYPE(symbols::identifiers::DATATYPE);
    newSymbol->setDATATYPE(dataType); // Get the datatype
    current = current->get_next();
    newSymbol->setIDENT_NAME(current->get_value()); // Get the variable name
    // Add line/col info
    newSymbol->set_line(current->get_line());
    newSymbol->set_column(current->get_column());
    newSymbol->setSCOPE(scope); // Set the scope
    if (current->get_next()->get_type() == tokens::OPEN_BRACKET){
        newSymbol->setIS_ARRAY(true); // Set as an array
        current = current->get_next()->get_next(); // Move to the token containing the size
        newSymbol->setARRAY_SIZE(std::stoi(current->get_value())); // Get the array size
        current = current->get_next(); // Consume the closing bracket
    }
    symbol_table::addSymbol(newSymbol); // Add the new symbol to the symbol table

    // Iterate until we reach a comma or a semicolon
    while (current->get_type() != ',' && current->get_type() != ';'){
        current = current->get_next();
    }

    if (current->get_type() == ','){
        addVariables(current, dataType, scope); // Recursive call to handle the next variable
    }
    return current;
}

exec_node* symbol_table::addParameters(symbol_node* functionSymbol, exec_node* current) {
    while (current->get_next() != nullptr){
        current = current->get_next();
        if (current->get_type() == tokens::TOKEN_AS_STRING && current->get_value() != "void") {
            auto newSymbol = new symbol_node();
            newSymbol->setIS_PARAMETER(true);
            newSymbol->setPARAMETER_FOR(functionSymbol->getIDENT_NAME());
            newSymbol->setIDENT_TYPE(symbols::identifiers::PARAMETER);
            newSymbol->setDATATYPE(symbols::data_types::check_type(current->get_value())); // Get the datatype
            current = current->get_next();
            newSymbol->setIDENT_NAME(current->get_value()); // Get the variable name
            newSymbol->setSCOPE(functionSymbol->getSCOPE()); // Set the scope
            if (current->get_next()->get_type() == tokens::OPEN_BRACKET){
                newSymbol->setIS_ARRAY(true); // Set as an array
                current = current->get_next()->get_next(); // Move to the token containing the size
                newSymbol->setARRAY_SIZE(std::stoi(current->get_value())); // Get the array size
                current = current->get_next(); // Consume the rest of the declaration
            }
            // Add line/col info
            newSymbol->set_line(current->get_line());
            newSymbol->set_column(current->get_column());

            // Add the symbol to the parameter list
            if (functionSymbol->get_params() == nullptr) {
                functionSymbol->set_params(newSymbol); // Set the first parameter
            } else {
                symbol_node* paramCurrent = functionSymbol->get_params();
                while (paramCurrent->getNext() != nullptr) {
                    paramCurrent = paramCurrent->getNext(); // Navigate to the end of the parameter list
                }
                paramCurrent->setNext(newSymbol); // Add the new parameter to the end of the list
            }
        }
    }
    return current;
}

void symbol_table::addSymbol(symbol_node* newSymbol) {
    if (newSymbol->getSCOPE() == 0){
        global_scope = true; // We have a globally scoped variable

        if (!scopes.empty() && scopes[0]->getIDENT_TYPE() != symbols::identifiers::DATATYPE) {
            // Prepend global variables if they are defined after 1st function/procedure
            scopes.insert(scopes.begin(), newSymbol);
            return;
        }
    }
    if (scopes.empty()){ // Fist symbol in the table
        scopes.push_back(newSymbol);
    }
    else {
        int scopesSize;
        if (global_scope){
            scopesSize = scopes.size() - 1;
        } else{
            scopesSize = scopes.size();
        }
        if (newSymbol->getSCOPE() > scopesSize) { // New scope
            if (newSymbol->getIDENT_TYPE() == symbols::identifiers::DATATYPE) {
                // Non-functions/Non-procedures cannot be declared as a unique scope
                errors::E_NON_FUNCTION_SCOPE_DECLARATION( newSymbol->get_line(), newSymbol->get_column(), newSymbol->getIDENT_NAME());
            }
            scopes.push_back(newSymbol);
        }
        else {
            int newScope;
            if (global_scope){
                newScope = newSymbol->getSCOPE();
            } else{
                newScope = newSymbol->getSCOPE() - 1;
            }
            symbol_node* current = scopes[newScope];

            int ident = current->getIDENT_TYPE(); // Get the identifier type for the first symbol in the current scope
            while (current != nullptr) { // Find the end of the current scope, checking for duplicates/errors
                if (current->getIDENT_NAME() == newSymbol->getIDENT_NAME()) {
                    // Handle duplicate symbol error
                    errors::E_ALREADY_DEFINED_VARIABLE_LOCAL(newSymbol->get_line(), newSymbol->get_column(), newSymbol->getIDENT_NAME());
                }
                if (current->getNext() == nullptr) break; // If we are at the end of the list
                current = current->getNext();
            }

            if (scopes[newScope]->get_params() != nullptr){ // If we have parameters, we need to check for duplicates
                symbol_node* paramCurrent = scopes[newScope]->get_params();
                while (paramCurrent != nullptr) {
                    if (paramCurrent->getIDENT_NAME() == newSymbol->getIDENT_NAME()) {
                        // Handle duplicate symbol error
                        errors::E_ALREADY_DEFINED_VARIABLE_LOCAL(newSymbol->get_line(), newSymbol->get_column(), newSymbol->getIDENT_NAME());
                    }
                    paramCurrent = paramCurrent->getNext();
                }
            }

            if (global_scope){ // If we have globally defined variables, we need to check for duplicates
                symbol_node* g_current = scopes[0];
                while (g_current != nullptr) {
                    if (g_current->getIDENT_NAME() == newSymbol->getIDENT_NAME()) {
                        // Handle duplicate symbol error
                        errors::E_ALREADY_DEFINED_VARIABLE_GLOBAL(newSymbol->get_line(), newSymbol->get_column(), newSymbol->getIDENT_NAME());
                    }
                    g_current = g_current->getNext();
                }
            }
            current->setNext(newSymbol); // Add the new symbol to the end of the current scope
        }
    }
}

void symbol_table::printSymbolTable(const std::string& filename) const {
    std::ofstream out(filename);
    if (!out) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }
    for (const auto& scope : scopes) {
        symbol_node* current = scope;
        while (current != nullptr) {
            out << "    IDENTIFIER_NAME: " << current->getIDENT_NAME() << std::endl;
            out << "    IDENTIFIER_TYPE: " << symbols::identifiers::get_type(current->getIDENT_TYPE()) << std::endl;
            out << "           DATATYPE: " << symbols::data_types::get_type(current->getDATATYPE()) << std::endl;
            if (current->getIS_ARRAY() == 0){
                out << "  DATATYPE_IS_ARRAY: no" << std::endl;
                out << "DATATYPE_ARRAY_SIZE: " << current->getARRAY_SIZE() + 1 << std::endl;
            }
            else{
                out << "  DATATYPE_IS_ARRAY: yes" << std::endl;
                out << "DATATYPE_ARRAY_SIZE: " << current->getARRAY_SIZE() << std::endl;
            }
            out << "              SCOPE: " << current->getSCOPE() << std::endl;
            out << std::endl;
            current = current->getNext();
        }
    }
    // Parameters
    for (const auto& scope : scopes) {
        if (scope->get_params() != nullptr){
            out << " PARAMETER LIST FOR: " << scope->getIDENT_NAME() << std::endl;
            symbol_node* paramCurrent = scope->get_params();
            while (paramCurrent != nullptr) {
                out << "    IDENTIFIER_NAME: " << paramCurrent->getIDENT_NAME() << std::endl;
                out << "           DATATYPE: " << symbols::data_types::get_type(paramCurrent->getDATATYPE()) << std::endl;
                if (paramCurrent->getIS_ARRAY() == 0){
                    out << "  DATATYPE_IS_ARRAY: no" << std::endl;
                    out << "DATATYPE_ARRAY_SIZE: " << paramCurrent->getARRAY_SIZE() + 1 << std::endl;
                }
                else{
                    out << "  DATATYPE_IS_ARRAY: yes" << std::endl;
                    out << "DATATYPE_ARRAY_SIZE: " << paramCurrent->getARRAY_SIZE() << std::endl;
                }
                out << "              SCOPE: " << paramCurrent->getSCOPE() << std::endl;
                out << std::endl;
                paramCurrent = paramCurrent->getNext();
            }
        }
    }
}


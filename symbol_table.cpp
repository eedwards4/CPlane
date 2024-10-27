//
// Created by Ethan Edwards on 10/24/2024.
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

void symbol_table::addSymbols(exec_path &path) {
    exec_node* current = path.get_head(); // Get the head of the execution path
    int scope = 0; // Initialize the scope level at global (0)
    int function_scope = 0; // Track the function scope level (i.e how many functions/procedures we have)

    while (current != nullptr) {
        if (current->get_type() == tokens::TOKEN_AS_STRING) {
            // Handle symbol declaration
            if (current->get_value() == "function"){ // Check if the value is a function
                auto newSymbol = new symbol_node();
                function_scope++;
                newSymbol->setIDENT_TYPE(symbols::identifiers::FUNCTION);

                current = current->get_next();
                newSymbol->setDATATYPE(symbols::data_types::check_type(current->get_value())); // Get the return type

                current = current->get_next();
                newSymbol->setIDENT_NAME(current->get_value()); // Get the function name
                // Add line/col info
                newSymbol->set_line(current->get_line());
                newSymbol->set_column(current->get_column());
                newSymbol->setSCOPE(function_scope); // Set the scope
                symbol_table::addSymbol(newSymbol); // Add the new symbol to the symbol table
                // Consume parameters
                current = addParameters(newSymbol, current);
            }
            else if (current->get_value() == "procedure"){ // Check if the value is a procedure
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
                while (current->get_type() != ';'){
                    auto newSymbol = new symbol_node();
                    newSymbol->setIDENT_TYPE(symbols::identifiers::DATATYPE);
                    newSymbol->setDATATYPE(dataType); // Get the datatype
                    current = current->get_next();
                    newSymbol->setIDENT_NAME(current->get_value()); // Get the variable name
                    // Add line/col info
                    newSymbol->set_line(current->get_line());
                    newSymbol->set_column(current->get_column());
                    newSymbol->setSCOPE(function_scope); // Set the scope
                    if (current->get_next()->get_type() == tokens::OPEN_BRACKET){
                        newSymbol->setIS_ARRAY(true); // Set as an array
                        current = current->get_next()->get_next(); // Move to the token containing the size
                        newSymbol->setARRAY_SIZE(std::stoi(current->get_value())); // Get the array size
                        current = current->get_next(); // Consume the rest of the declaration
                    }

                    // Add the new symbol to the symbol table
                    symbol_table::addSymbol(newSymbol);
                    current = current->get_next(); // Move to the next token (either a comma or a semicolon)
                }
            }
        }
        // Navigation
        if (current->get_next() == nullptr && current->get_fold() != nullptr) {
            if (current->get_fold()->get_type() == tokens::OPEN_BRACE) {
                scope++;
            } else if (current->get_fold()->get_type() == tokens::CLOSE_BRACE) {
                scope--;
            }
            current = current->get_fold();
        } else{
            current = current->get_next();
        }
    }
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

void symbol_table::addSymbol(symbol_node *newSymbol) {
    if (newSymbol->getSCOPE() == 0){
        global_scope = true; // We have a globally scoped variable
    }
    if (scopes.empty()){ // Fist symbol in the table
        scopes.push_back(newSymbol);
    } else {
        if (newSymbol->getSCOPE() > scopes.size() - 1) { // New scope
            if (newSymbol->getIDENT_TYPE() == symbols::identifiers::DATATYPE) {
                // Non-functions/Non-procedures cannot be declared as a unique scope
                //TODO: ERROR
            }
            scopes.push_back(newSymbol);
        } else {
            symbol_node* current = scopes[newSymbol->getSCOPE()];
            int ident = current->getIDENT_TYPE(); // Get the identifier type for the first symbol in the current scope
            while (current != nullptr) { // Find the end of the current scope, checking for duplicates/errors
                if (current->getIDENT_NAME() == newSymbol->getIDENT_NAME()) {
                    // Handle duplicate symbol error
                    // TODO: MOVE TO ERRORS.CPP
                    errors::E_ALREADY_DEFINED_VARIABLE_LOCAL(newSymbol->get_line(), newSymbol->get_column(), newSymbol->getIDENT_NAME());
                }
                if (newSymbol->getIDENT_TYPE() == ident){
                    // We have a function in a function/procedure in a procedure
                    // NESTING FUNCTIONS ARE NOT ALLOWED UNDER ISO C
                    // TODO: ERROR
                }
                if (current->getNext() == nullptr) break; // If we are at the end of the list
                current = current->getNext();
            }

            if (scopes[newSymbol->getSCOPE()]->get_params() != nullptr){ // If we have parameters, we need to check for duplicates
                symbol_node* paramCurrent = scopes[newSymbol->getSCOPE()]->get_params();
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

void symbol_table::printSymbols(const std::string& filename) const {
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

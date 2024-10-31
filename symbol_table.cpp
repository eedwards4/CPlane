//
// Created by Ethan Edwards on 10/24/2024.
//

#include "symbol_table.h"

// Destructor
symbol_table::~symbol_table() {
    for (symbol_node* s : scopes) {
        // Delete parameters
        if (s != nullptr && s->get_params() != nullptr) {
            symbol_node* p = s->get_params();
            while (p != nullptr) {
                symbol_node* temp = p;
                p = p->get_next();
                delete temp;
            }
        }
        while (s != nullptr) {
            symbol_node* temp = s;
            s = s->get_next();
            delete temp;
        }
    }
}

// Driver function for building the table
void symbol_table::build_table(exec_path &path) {
    exec_node* current = path.get_head();
    int curr_scope = 0; // The current pseudo-scope level
    int glob_scope = 0; // The current global scope level

    while(current != nullptr){
        // Handlers
        if (current->get_type() == tokens::TOKEN_AS_STRING){
            if (current->get_value() == "function"){
                if (curr_scope != 0){
                    errors::E_NESTED_FUNCTION_NOT_ALLOWED(current->get_line(), current->get_column(), current->get_next()->get_next()->get_value());
                }
                auto new_symbol = new symbol_node();
                // Set identifier type
                new_symbol->IDENT_TYPE = symbols::identifiers::FUNCTION;

                // Set datatype
                current = current->get_next();
                new_symbol->DATATYPE = symbols::data_types::check_type(current->get_value());

                // Set name
                current = current->get_next();
                new_symbol->IDENT_NAME = current->get_value();

                // Error tracking
                new_symbol->set_line(current->get_line());
                new_symbol->set_column(current->get_column());

                // Set scope
                glob_scope++;
                new_symbol->SCOPE_GLOBAL = glob_scope;

                // Add to symbol table
                add_symbol(new_symbol);

                // Consume parameters
                current = param_handler(current, new_symbol);
            }
            else if (current->get_value() == "procedure"){
                if (curr_scope != 0){
                    errors::E_NESTED_FUNCTION_NOT_ALLOWED(current->get_line(), current->get_column(), current->get_next()->get_next()->get_value());
                }
                auto new_symbol = new symbol_node();
                // Set identifier type
                new_symbol->IDENT_TYPE = symbols::identifiers::PROCEDURE;

                // Set datatype
                new_symbol->DATATYPE = symbols::data_types::NA;

                // Set name
                current = current->get_next();
                new_symbol->IDENT_NAME = current->get_value();

                // Error tracking
                new_symbol->set_line(current->get_line());
                new_symbol->set_column(current->get_column());

                // Set scope
                glob_scope++;
                new_symbol->SCOPE_GLOBAL = glob_scope;

                // Add to symbol table
                add_symbol(new_symbol);

                // Consume parameters
                current = param_handler(current, new_symbol);
            }
            else if (symbols::data_types::check_type(current->get_value()) != -1){
                int dtype = symbols::data_types::check_type(current->get_value());
                if (curr_scope > 0){
                    current = var_handler(current, dtype, glob_scope, curr_scope-1);
                } else{
                    current = var_handler(current, dtype, curr_scope, curr_scope);
                }
            }
        }
        // Navigation
        if (current->get_next() == nullptr && current->get_fold() != nullptr) {
            if (current->get_type() == tokens::OPEN_BRACE || current->get_type() == '{') {
                curr_scope++;
            } else if (current->get_type() == tokens::CLOSE_BRACE || current->get_type() == '}') {
                curr_scope--;
            }
            current = current->get_fold();
        } else{
            current = current->get_next();
        }
    }
}

// Handles adding variables to the symbol table
exec_node* symbol_table::var_handler(exec_node* current, int dType, int scope, int local_scope) {
    auto new_symbol = new symbol_node();
    // Set identifier type
    new_symbol->IDENT_TYPE = symbols::identifiers::DATATYPE;

    // Set datatype
    new_symbol->DATATYPE = dType;

    // Set name
    current = current->get_next();
    new_symbol->IDENT_NAME = current->get_value();

    // Error tracking
    new_symbol->set_line(current->get_line());
    new_symbol->set_column(current->get_column());

    // Set scope
    new_symbol->SCOPE_GLOBAL = scope;

    // Handle arrays
    if (current->get_next()->get_type() == tokens::OPEN_BRACKET){
        new_symbol->IS_ARRAY = true;
        current = current->get_next()->get_next();
        new_symbol->ARRAY_SIZE = std::stoi(current->get_value());
        current = current->get_next();
    }

    // Add to symbol table
    add_symbol(new_symbol);

    // Iterate until we reach a comma or a semicolon
    while (current->get_type() != ',' && current->get_type() != ';'){
        current = current->get_next();
    }

    // Return if we are at the end of the definition or recurse
    if (current->get_type() == ','){
        return var_handler(current, dType, scope, local_scope);
    } else{
        return current;
    }
}

// Handles adding parameters to a function or procedure
exec_node* symbol_table::param_handler(exec_node* current, symbol_node* functionSymbol) {
    while (current->get_next() != nullptr){
        current = current->get_next();
        if (current->get_type() == tokens::TOKEN_AS_STRING && current->get_value() != "void"){
            auto new_symbol = new symbol_node();
            new_symbol->IS_PARAMETER = true;
            new_symbol->PARAMETER_FOR = functionSymbol->IDENT_NAME;
            new_symbol->IDENT_TYPE = symbols::identifiers::PARAMETER;

            // Get the datatype
            new_symbol->DATATYPE = symbols::data_types::check_type(current->get_value());

            // Get the variable name
            current = current->get_next();
            new_symbol->IDENT_NAME = current->get_value();

            // Set scope
            new_symbol->SCOPE_GLOBAL = functionSymbol->SCOPE_GLOBAL;

            // Handle arrays
            if (current->get_next()->get_type() == tokens::OPEN_BRACKET){
                new_symbol->IS_ARRAY = true;
                current = current->get_next()->get_next();
                new_symbol->ARRAY_SIZE = std::stoi(current->get_value());
                current = current->get_next();
            }

            // Error tracking
            new_symbol->set_line(current->get_line());
            new_symbol->set_column(current->get_column());

            // Add to symbol table
            if (functionSymbol->get_params() == nullptr){
                functionSymbol->set_params(new_symbol);
            } else{
                symbol_node* param_current = functionSymbol->get_params();
                while (param_current->get_next() != nullptr){
                    param_current = param_current->get_next();
                }
                param_current->set_next(new_symbol);
            }
        }
    }
    return current;
}

// Handles actually adding a single symbol to the table, including error checking, duplicate checking, etc.
void symbol_table::add_symbol(symbol_node* new_symbol) {
    if (scopes.empty()){ // First symbol in the table
        if (new_symbol->SCOPE_GLOBAL == 0){ // First symbol is global
            scopes.push_back(new_symbol);
        }
        else{ // First symbol is not global, so we need to define a dummy global scope
            scopes.push_back(nullptr);
            scopes.push_back(new_symbol);
        }
    }
    else{
        if (new_symbol->SCOPE_GLOBAL > scopes.size() - 1){ // New scope
            if (new_symbol->IDENT_TYPE == symbols::identifiers::DATATYPE){
                // Non functions/procedures cannot be declared in a unique scope (THIS SHOULD NEVER HAPPEN)
                errors::E_NON_FUNCTION_SCOPE_DECLARATION(new_symbol->get_line(), new_symbol->get_column(), new_symbol->IDENT_NAME);
            }
            scopes.push_back(new_symbol);
        }
        else{
            // Handle definition of the first global variable if it isn't the first symbol in the table
            if (new_symbol->SCOPE_GLOBAL == 0 && scopes[0] == nullptr){
                scopes[0] = new_symbol;
                return;
            }
            symbol_node* current = scopes[new_symbol->SCOPE_GLOBAL];

            // Find the end of the current scope, checking for duplicates/errors
            while (current != nullptr){
                if (current->IDENT_NAME == new_symbol->IDENT_NAME){
                    // Handle duplicate symbol error
                    errors::E_ALREADY_DEFINED_VARIABLE_LOCAL(new_symbol->get_line(), new_symbol->get_column(), new_symbol->IDENT_NAME);
                }
                if (current->get_next() == nullptr){ break; } // Ensure the last symbol is checked
                current = current->get_next();
            }

            // Check for duplicate parameters
            if (scopes[new_symbol->SCOPE_GLOBAL]->get_params() != nullptr){
                symbol_node* param_current = scopes[new_symbol->SCOPE_GLOBAL]->get_params();
                while (param_current != nullptr){
                    if (param_current->IDENT_NAME == new_symbol->IDENT_NAME){
                        // Handle duplicate symbol error
                        errors::E_ALREADY_DEFINED_VARIABLE_LOCAL(new_symbol->get_line(), new_symbol->get_column(), new_symbol->IDENT_NAME);
                    }
                    param_current = param_current->get_next();
                }
            }

            // Check for duplicate global variables
            if (scopes[0] != nullptr){
                symbol_node* g_current = scopes[0];
                while (g_current != nullptr){
                    if (g_current->IDENT_NAME == new_symbol->IDENT_NAME){
                        // Handle duplicate symbol error
                        errors::E_ALREADY_DEFINED_VARIABLE_GLOBAL(new_symbol->get_line(), new_symbol->get_column(), new_symbol->IDENT_NAME);
                    }
                    g_current = g_current->get_next();
                }
            }

            // Add the new symbol to the table
            current->set_next(new_symbol);
        }
    }
}

// Function to print all symbols in the table
void symbol_table::print_symbols(const std::string& filename) const {
    std::ofstream out(filename);
    if (!out) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    // Iterate through each scope, printing each symbol
    for (const auto& scope : scopes) {
        symbol_node* current = scope;
        while (current != nullptr){
            out << "    IDENTIFIER_NAME: " << current->IDENT_NAME << std::endl;
            out << "    IDENTIFIER_TYPE: " << symbols::identifiers::get_type(current->IDENT_TYPE) << std::endl;
            out << "           DATATYPE: " << symbols::data_types::get_type(current->DATATYPE) << std::endl;
            if (current->IS_ARRAY == 0){
                out << "  DATATYPE_IS_ARRAY: no" << std::endl;
                out << "DATATYPE_ARRAY_SIZE: " << current->ARRAY_SIZE + 1 << std::endl;
            }
            else{
                out << "  DATATYPE_IS_ARRAY: yes" << std::endl;
                out << "DATATYPE_ARRAY_SIZE: " << current->ARRAY_SIZE << std::endl;
            }
            out << "              SCOPE: " << current->SCOPE_GLOBAL << std::endl;
            out << std::endl;
            current = current->get_next();
        }
    }

    // Iterate through the top of each scope, printing parameter lists if they exist
    for (const auto& scope : scopes){
        if (scope != nullptr && scope->get_params() != nullptr){
            out << " PARAMETER LIST FOR: " << scope->IDENT_NAME << std::endl;
            symbol_node* current = scope->get_params();
            while (current != nullptr) {
                out << "    IDENTIFIER_NAME: " << current->IDENT_NAME << std::endl;
                out << "           DATATYPE: " << symbols::data_types::get_type(current->DATATYPE) << std::endl;
                if (current->IS_ARRAY == 0){
                    out << "  DATATYPE_IS_ARRAY: no" << std::endl;
                    out << "DATATYPE_ARRAY_SIZE: " << current->ARRAY_SIZE + 1 << std::endl;
                }
                else{
                    out << "  DATATYPE_IS_ARRAY: yes" << std::endl;
                    out << "DATATYPE_ARRAY_SIZE: " << current->ARRAY_SIZE << std::endl;
                }
                out << "              SCOPE: " << current->SCOPE_GLOBAL << std::endl;
                out << std::endl;
                current = current->get_next();
            }
        }
    }

    out.close();
}
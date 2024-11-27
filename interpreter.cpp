#include "interpreter.h"

// Default constructor
Interpreter::Interpreter() {
    scope_stack.push(0); // push global scope (should never be popped)
}

Interpreter::Interpreter(ast tree, symbol_table& table, ERRORS& in_errors) {
    s_table = table; // Symbol table
    as_tree = tree; // AST
    errors = in_errors;
    scope_stack.push(0); // push global scope (should never be popped)
}

// Puts functions in array, then starts processing main
void Interpreter::begin() {
    ast_node* current = as_tree.get_head();
    while (current != nullptr) {

        if (current->value == "function" || current->value == "procedure") {
            if (current->func_name == "main") {
                scope_stack.push(s_table.get_function_scope("main"));
                process_function(current);
                break;
            } else {
                functions.push_back(current);
            }
        }

        if (current->get_next() != nullptr) {
            current = current->get_next();
        } else {
            current = current->get_chld();
        }
    }

    std::cout << "Program End\n";
}

// This function simply returns if the givin string is a number.
bool Interpreter::isNumber(std::string str){
    int i = 0;
    bool is = true;
    while ( str[i] ){
        if ( ! isdigit(str[i]) ){
            is = false;
            break;
        }
        i++;
    }
    return is;
}

// Returns true if the givin string is an operator else returns false
bool Interpreter::isOperator(std::string str){
    // Numerical operators
    if ( str == "+" ){
        return true;
    }
    else if ( str== "-" ){
        return true;
    }
    else if ( str == "*" ){
        return true;
    }
    else if ( str == "/" ){
        return true;
    }
    else if ( str == "%" ){
        return true;
    }
    else if ( str == "^" ){
        return true;
    }
    else if ( str == "="){
        return true;
    }

    // Relational operators
    else if ( str == ">" ){
        return true;
    }
    else if ( str == ">=" ){
        return true;
    }
    else if ( str == "<" ){
        return true;
    }
    else if ( str == "<=" ){
        return true;
    }
    else if ( str == "==" ){
        return true;
    }
    else if ( str == "!=" ){
        return true;
    }

    // Boolean operators
    else if ( str == "&&" ) {
        return true;
    }
    else if ( str == "||" ) {
        return true;
    }

    else {
        return false;
    }
}

// Retuns the ast_node for a function with name. Returned ast_node is either 'process' or 'function'
ast_node* Interpreter::getFunction(std::string str) {
    for (auto f : functions) {
        if (f->func_name == str) {
            return f;
        }
    }
    return nullptr;
}

// Evaluate non assingment expressions
ast_node* Interpreter::eval_top_three(std::string one, std::string two, std::string three) {
    ast_node* sol = new ast_node();

    // Integer
    if (one == "+") {
        sol->value = std::to_string(std::stoi(three) + std::stoi(two));
    }
    else if (one == "-") {
        sol->value = std::to_string(std::stoi(three) - std::stoi(two));
    }
    else if (one == "*") {
        sol->value = std::to_string(std::stoi(three) * std::stoi(two));
    }
    else if (one == "/") {
        sol->value = std::to_string(std::stoi(three) / std::stoi(two));
    }
    else if (one == "%") {
        sol->value = std::to_string(std::stoi(three) % std::stoi(two));
    }
    else if (one == "^") {
        sol->value = std::to_string(std::pow(std::stoi(three), std::stoi(two)));
    }

    // Relational
    else if (one == ">") {
        if (std::stoi(three) > std::stoi(two)) {
            sol->value = "TRUE";
        } else {
            sol->value = "FALSE";
        }
    }
    else if (one == ">=") {
        if (std::stoi(three) >= std::stoi(two)) {
            sol->value = "TRUE";
        } else {
            sol->value = "FALSE";
        }
    }
    else if (one == "<") {
        if (std::stoi(three) < std::stoi(two)) {
            sol->value = "TRUE";
        } else {
            sol->value = "FALSE";
        }
    }
    else if (one == "<=") {
        if (std::stoi(three) <= std::stoi(two)) {
            sol->value = "TRUE";
        } else {
            sol->value = "FALSE";
        }
    }
    else if (one == "==") {
        if (std::stoi(three) == std::stoi(two)) {
            sol->value = "TRUE";
        } else {
            sol->value = "FALSE";
        }
    }
    else if (one == "!=") {
        if (std::stoi(three) != std::stoi(two)) {
            sol->value = "TRUE";
        } else {
            sol->value = "FALSE";
        }
    }

    // Boolean
    else if (one == "&&") {
        if (two == "TRUE" && three == "TRUE") {
            sol->value = "TRUE";
        } else {
            sol->value = "FALSE";
        }
    }
    else if (one == "||") {
        if (two == "TRUE" || three == "TRUE") {
            sol->value = "TRUE";
        } else {
            sol->value = "FALSE";
        }
    }

    return sol;
}

// Processes a function
// Returns an ast node with the value returned by the function (nullptr for no value returned)
ast_node* Interpreter::process_function(ast_node* function) {
    ast_node* current = function->get_chld();
    if (current == nullptr || ast_types::what_is(current->type) != "BEG_BLOCK") {
        if (current != nullptr)
            std::cerr << "Expected BEG_BLOCK, but got '"<< ast_types::what_is(current->type) <<"' instead.\n";
        return nullptr;
    }

    // process inside of function
    if (process_block(current)) {
        // process return statement
        current = current->get_next();
        symbol_node* cur_symbol = s_table.get_symbol(current->value, scope_stack.top());

        ast_node* ret_value = new ast_node();
        // figure out type and assign value. Just doing int for now
        ret_value->value = std::to_string(cur_symbol->get_val_int());

        return ret_value;
    }

    // Otherwise there was no return statement
    else {
        return nullptr;
    }
}

// Processes a 'block' which is a pair of open and closed curly braces -> {}
// Returnes true if it reached a return statement, false otherwise
bool Interpreter::process_block(ast_node*& current) {
    std::cout << "ENTERING BLOCK\n";
    current = current->get_chld(); // skip '{'

    while (current != nullptr && ast_types::what_is(current->type) != "END_BLOCK") {
        if (ast_types::what_is(current->type) == "BEG_BLOCK") {
            if (process_block(current)) {
                std::cout << "EXITING BLOCK\n";
                return true;
            }
        }

        else if (ast_types::what_is(current->type) == "ASSIGNMENT") {
            current = current->get_next(); // Skip 'ASSIGNMENT' node
            process_assignment(current);
        }

        else if (ast_types::what_is(current->type) == "STATEMENT_PRINTF") {
            current = current->get_next(); // Skip 'STATEMENT_PRINTF' node
            process_printf(current);
        }

        else if (ast_types::what_is(current->type) == "STATEMENT_IF") {
            current = current->get_next(); // Skip 'STATEMENT_IF' node
            process_if(current);
        }

        else if (ast_types::what_is(current->type) == "EXPRESSION_WHILE") {
            current = current->get_next(); // Skip 'EXPRESSION_WHILE' node
            process_while(current);
        }

        else if (ast_types::what_is(current->type) == "EXPRESSION_FOR") {
            current = current->get_next(); // Skip 'EXPRESSION_FOR' node
            process_for(current);
        }

        else if (ast_types::what_is(current->type) == "CALL") {
            current = current->get_next(); // Skip 'CALL' node
            process_call(current);
        }

        else if (ast_types::what_is(current->type) == "RETURN") {
            std::cout << "EXITING BLOCK\n";
            return true;
        }

        if (current->get_next() != nullptr) {
            current = current->get_next();
        } else {
            current = current->get_chld();
        }
    }

    std::cout << "EXITING BLOCK\n";
    return false;
}

// Processes a function call (to then call process_function)
// Returns an ast node with the value returned by the function (nullptr for no value returned)
ast_node* Interpreter::process_call(ast_node*& current) {
    std::string func_name = current->value; // get function name
    int func_scope = s_table.get_function_scope(current->value); // get function scope
    symbol_node* func_symbol = s_table.get_symbol(func_name, func_scope);

    symbol_node* cur_param = func_symbol->get_params();

    std::cout << "FUNCTION CALL FOR " << func_name << " AT SCOPE " << func_scope << " WITH PARAMETERS ";
    
    current = current->get_next()->get_next(); // skip function name and '('

    while (current->value != ")") {
        if (current->value != ",") {
            std::cout << current->value << " ";
            symbol_node *temp_symbol = s_table.get_symbol(current->value, scope_stack.top());
            cur_param->set_val_int(temp_symbol->get_val_int());
        }
        current = current->get_next();
        cur_param = cur_param->get_next();
    }
    std::cout << std::endl;

    std::cout << "ENTERING FUNCTION\n";
    scope_stack.push(func_scope);
    ast_node* ret_value = process_function(getFunction(func_name));
    scope_stack.pop();
    std::cout << "EXITING FUNCTION\n";

    return ret_value;
}

// Processes assignment expression
void Interpreter::process_assignment(ast_node*& current) {
    std::cout << "ASSIGNMENT BEGIN\n";
    std::stack<ast_node*> expression_stack;
    // current = current->get_next();

    while (current != nullptr) {
        // Handle function call in expression
        if (getFunction(current->value) != nullptr) {
            expression_stack.push(process_call(current));
        }
        else {
            expression_stack.push(current);
        }

        if (expression_stack.size() >= 3) {

            ast_node* one = expression_stack.top();
            expression_stack.pop();

            ast_node* two = expression_stack.top();
            expression_stack.pop();

            ast_node* three = expression_stack.top();
            expression_stack.pop();

            // Invalid order, must be operator operand operand
            if ( ! isOperator(one->value) || isOperator(two->value) || isOperator(three->value) ){
                // Resetting stack
                expression_stack.push(three);
                expression_stack.push(two);
                expression_stack.push(one);
            }

            // Handles assignment
            else if (one->value == "=") {
                std::cout << "process_assignment() Assign: " << one->value << " " << two->value << " " << three->value << std::endl;

                symbol_node* s_two = s_table.get_symbol(two->value, scope_stack.top());
                symbol_node* s_three = s_table.get_symbol(three->value, scope_stack.top());

                if (isNumber(two->value)) {
                    std::cout << "Setting " << three->value << " = " << two->value << " (numerical)\n";
                    s_three->set_val_int(std::stoi(two->value));
                }
                else if (two->value == "TRUE" || two->value == "FALSE") {
                    std::cout << "Setting " << three->value << " = " << two->value << " (boolean)\n";
                    s_three->set_val_bool(two->value == "TRUE");
                }
                else if (s_two != nullptr) {
                    std::cout << "Setting " << three->value << " = " << two->value << " from symbol table\n";
                    switch (s_two->DATATYPE) {
                        case symbols::data_types::INT: s_three->set_val_int(s_two->get_val_int());
                        case symbols::data_types::BOOL: s_three->set_val_bool(s_two->get_val_bool());
                        case symbols::data_types::CHAR: s_three->set_val_char(s_two->get_val_char());
                    }
                }
                else {
                    std::cout << "ERROR in process_assignment(): Assingnment values could not be assessed\n";
                    exit(1);
                }
            }

            // Handles operations
            else if (isOperator(one->value)) {
                std::string two_val = two->value;
                std::string three_val = three->value;

                symbol_node* s_two = s_table.get_symbol(two->value, scope_stack.top());
                symbol_node* s_three = s_table.get_symbol(three->value, scope_stack.top());

                // get value of two from symbol table
                if (s_two != nullptr) {
                    switch (s_two->DATATYPE) {
                        case symbols::data_types::INT: two_val = std::to_string(s_two->get_val_int()); break;
                        case symbols::data_types::BOOL: two_val = std::to_string(s_two->get_val_bool()); break;
                        case symbols::data_types::CHAR: two_val = std::to_string(s_two->get_val_char()); break;
                    }
                }

                // get value of three from symbol table
                if (s_three != nullptr) {
                    switch (s_three->DATATYPE) {
                        case symbols::data_types::INT: three_val = std::to_string(s_three->get_val_int()); break;
                        case symbols::data_types::BOOL: three_val = std::to_string(s_three->get_val_bool()); break;
                        case symbols::data_types::CHAR: three_val = std::to_string(s_three->get_val_char()); break;
                    }
                }

                std::cout << "TopThree() 123 Evaluate: " << one->value << " " << two_val << " " << three_val << std::endl;
                expression_stack.push(eval_top_three(one->value, two_val, three_val));
            }
        }

        if (current->get_next() == nullptr) { break; }
        current = current->get_next();
    }
    std::cout << "ASSIGNMENT END\n";
}

// Processes if statement
void Interpreter::process_if(ast_node*& current) {
    std::cout << "IF STATEMENT\n";
    // Evaluate boolean expression
    // If true:
    //     call process_block on first block
    //     if there's an 'else' then skip the else block
    // If false:
    //     skip first block
    //     if there's an 'else' call process_block on the else block
    //
    // current should end on an end_block ast_node
}

// Processes while loop
void Interpreter::process_while(ast_node*& current) {
    std::cout << "WHILE STATEMENT\n";
    // Evaluate boolean expression               <---
    // If true:                                     |
    //     call process_block on following block    |
    //     go to beginning of function (eval bool) --
    // If false:
    //     skip following block
    //
    // current should end on an end_block ast_node
}

// Processes for loop
void Interpreter::process_for(ast_node*& current) {
    std::cout << "FOR STATEMENT\n";
    // For Expression 1:
    //     process assignment
    // For Expression 2:                         <---
    //     evaluate boolean expression               |
    //     if true, run block                       |
    //     else skip block                          |
    // For Expression 3:                            |
    //     if block was run, process assignment     |
    //     start again from For Expression 2 --------
    //
    // current should end on an end_block ast_node
}

// processes printf statement
void Interpreter::process_printf(ast_node*& current) {
    std::cout << "printf: ";
    std::string to_print = current->value;

    while (current->get_next() != nullptr) {
        current = current->get_next();
        int var_pos = to_print.find_first_of('%');

        symbol_node *temp_symbol = s_table.get_symbol(current->value, scope_stack.top());

        if (var_pos != std::string::npos) {
            if (to_print[var_pos+1] == 'd') {
                to_print.replace(var_pos, 2, std::to_string(temp_symbol->get_val_int()));
            }
            else if (to_print[var_pos+1] == 's') {
                to_print.replace(var_pos, 2, current->value); // temporary
            }
        }
    }

    // replace \n with newline
    int new_line_pos = to_print.rfind("\\n");
    while (new_line_pos != std::string::npos) {
        to_print.replace(new_line_pos, 2, "\n");
        new_line_pos = to_print.rfind("\\n");
    }

    std::cout << to_print;
}

// Skips 'block' which is a pair of open and closed curly braces -> {}
void Interpreter::skip_block(ast_node*& current) {
    // current should be open block (open curly brace '{')
    // moves current to corresponding end block (closed curly brace '}')
    // current should end on an end_block ast_node
    int counter = 0;
    while (current != nullptr) {
        if (ast_types::what_is(current->type) == "BEG_BLOCK") {
            counter++;
        }
        if (ast_types::what_is(current->type) == "END_BLOCK") {
            counter--;

        }
        if (counter == 0){
            return;
        }

        if (current->get_next() != nullptr) {
            current = current->get_next();
        } else {
            current = current->get_chld();
        }

    }
}

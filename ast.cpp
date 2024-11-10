//
// Created by Ethan Edwards on 11/7/2024.
//

#include "ast.h"

ast_node* ast::get_head(){
    return head;
}
ast_node* ast::get_tail(){
    return tail;
}



void ast::build_tree(exec_node *cst_head, symbol_table& table) {

    while (cst_head != nullptr){
        if (cst_head->get_type() == tokens::OPEN_BRACE){
            auto new_node = new ast_node;
            new_node->type = ast_types::BEG_BLOCK;
            new_node->set_err(cst_head->get_line(), cst_head->get_column());
            add_node(new_node);
        }
        else if (cst_head->get_type() == tokens::CLOSE_BRACE){
            auto new_node = new ast_node;
            new_node->type = ast_types::END_BLOCK;
            new_node->set_err(cst_head->get_line(), cst_head->get_column());
            add_node(new_node);
        }
        else if (cst_head->get_type() == tokens::TOKEN_AS_STRING){
            auto new_node = new ast_node;
            if (cst_head->get_value() == "function" || cst_head->get_value() == "procedure"){
                new_node->type = ast_types::DECLARATION;
                new_node->set_err(cst_head->get_line(), cst_head->get_column());
                add_node(new_node);
                // Consume declaration
                while (cst_head->get_next()->get_type() != tokens::OPEN_BRACE){
                    cst_head = cst_head->get_next();
                }
            }
            else if (cst_head->get_value() == "printf"){
                new_node->type = ast_types::STATEMENT_PRINTF;
                new_node->set_err(cst_head->get_line(), cst_head->get_column());
                add_node(new_node);
                // Skip parens
                cst_head = cst_head->get_next()->get_next();
                while (cst_head->get_next()->get_type() != ';'){
                    if (cst_head->get_type() != '"' && cst_head->get_type() != ','){ // Ignore commas and quotes
                        new_node = new ast_node;
                        new_node->type = ast_types::TOKEN;
                        new_node->value = cst_head->get_value();
                        new_node->set_err(cst_head->get_line(), cst_head->get_column());
                        tail->set_next(new_node);
                        tail = new_node;
                    }
                    cst_head = cst_head->get_next();
                }
            }
            else if (cst_head->get_value() == "if"){
                new_node->type = ast_types::STATEMENT_IF;
                new_node->set_err(cst_head->get_line(), cst_head->get_column());
                add_node(new_node);
                // Skip to the statement
                cst_head = cst_head->get_next();
                // Shunting time!
                cst_head = shunting_yard_wrapper(cst_head, new_node, table);
            }
            else if (cst_head->get_value() == "else"){
                auto new_node = new ast_node;
                new_node->type = ast_types::ELSE;
                new_node->set_err(cst_head->get_line(), cst_head->get_column());
                add_node(new_node);
            }
            else if (cst_head->get_value() == "return"){
                new_node->type = ast_types::RETURN;
                new_node->set_err(cst_head->get_line(), cst_head->get_column());
                add_node(new_node);
                cst_head = shunting_yard_wrapper(cst_head->get_next(), new_node, table);
            }
            else if (cst_head->get_value() == "for"){
                // Skip to the statement
                cst_head = cst_head->get_next();
                int for_tracker = 1;
                while (cst_head->get_type() != tokens::OPEN_BRACE){
                    //std::cout << "Called on: " << cst_head->get_value() << std::endl;
                    new_node = new ast_node;
                    new_node->type = ast_types::EXPRESSION_FOR;
                    new_node->set_err(cst_head->get_line(), cst_head->get_column());
                    new_node->for_expr_num = for_tracker;
                    add_node(new_node);
                    cst_head = shunting_yard_wrapper(cst_head, new_node, table);
                    for_tracker++;
                }
            }
            else if (cst_head->get_value() == "while"){
                new_node->type = ast_types::EXPRESSION_WHILE;
                new_node->set_err(cst_head->get_line(), cst_head->get_column());
                add_node(new_node);
                // Skip to the statement
                cst_head = cst_head->get_next();
                cst_head = shunting_yard_wrapper(cst_head->get_next(), new_node, table);
            }
            else if (symbols::data_types::check_type(cst_head->get_value()) != -1 && symbols::data_types::check_type(cst_head->get_value()) != symbols::data_types::VOID){
                new_node->type = ast_types::DECLARATION;
                new_node->set_err(cst_head->get_line(), cst_head->get_column());
                add_node(new_node);

                // Handle inline assignment or multi-variable definition
                if (cst_head->get_next()->get_next()->get_value() == "="){ // Skip the name and check for assignment operator/semicolon
                    /*cst_head = cst_head->get_next()->get_next();
                    auto new_node_two = new ast_node;
                    new_node_two->type = ast_types::ASSIGNMENT;
                    new_node_two->set_err(cst_head->get_line(), cst_head->get_column());
                    add_node(new_node_two);
                    // Handle the rest of the assignment with shunting yard
                    cst_head = shunting_yard_wrapper(cst_head, new_node_two);*/
                    //std::cout << "Called on: " << cst_head->get_next()->get_next()->get_value() << std::endl;
                }
                else if (cst_head->get_next()->get_next()->get_value() == ","){
                    while (cst_head->get_next()->get_next()->get_value() == ","){
                        cst_head = cst_head->get_next()->get_next();
                        auto new_node_two = new ast_node;
                        new_node_two->type = ast_types::DECLARATION;
                        new_node_two->set_err(cst_head->get_line(), cst_head->get_column());
                        add_node(new_node_two);
                    }
                    cst_head = cst_head->get_next()->get_next();
                }
                else{ // Consume the declaration
                    while (cst_head->get_next()->get_type() != ';'){
                        cst_head = cst_head->get_next();
                    }
                }
            }
            else if (table.find_symbol(cst_head->get_value()) && (cst_head->get_next()->get_value() == "=" || cst_head->get_next()->get_value() == "[")){
                new_node->type = ast_types::ASSIGNMENT;
                new_node->set_err(cst_head->get_line(), cst_head->get_column());
                add_node(new_node);
                // Create a token node to hold the actual value being assigned to
                auto new_node_2 = new ast_node;
                new_node_2->type = ast_types::TOKEN;
                new_node_2->value = cst_head->get_value();
                new_node_2->set_err(cst_head->get_line(), cst_head->get_column());
                add_node(new_node_2);
                // Transition to the next token (should be = since this is an assignment)
                cst_head = cst_head->get_next();
                // Handle the rest of the assignment with shunting yard
                cst_head = shunting_yard_wrapper(cst_head, new_node, table);
            }
            else if (table.find_symbol(cst_head->get_value())){ // Not an assignment so it must be a call
                auto new_node = new ast_node;
                new_node->type = ast_types::CALL;
                new_node->set_err(cst_head->get_line(), cst_head->get_column());
                add_node(new_node);
                cst_head = shunting_yard_wrapper(cst_head, new_node, table);
            }
        }
        // Nav
        if (cst_head == nullptr){
            std::cout << "ERROR! CST head is null." << std::endl;
            std::cout << "Tail: " << tail->value << std::endl;
            break;
        }
        if (cst_head->get_next() == nullptr){
            if (cst_head->get_fold() == nullptr){
                break;
            }

            cst_head = cst_head->get_fold();
        }
        else{
            cst_head = cst_head->get_next();
        }
    }
}

exec_node* ast::shunting_yard_wrapper(exec_node* cst_head, ast_node* prev, symbol_table& table) {
    std::vector<exec_node*> tokens;
    if (prev->type == ast_types::ASSIGNMENT || prev->type == ast_types::RETURN || prev->type == ast_types::CALL){
        while (cst_head != nullptr && cst_head->get_type() != ';'){
            tokens.push_back(cst_head);
            cst_head = cst_head->get_next();
        }
    }
    else if (prev->type == ast_types::STATEMENT_IF || prev->type == ast_types::EXPRESSION_WHILE){
        while (cst_head != nullptr && cst_head->get_type() != tokens::OPEN_BRACE){
            tokens.push_back(cst_head);
            if (cst_head->get_next()->get_type() == tokens::OPEN_BRACE){
                break;
            }
            cst_head = cst_head->get_next();
        }
    }
    else if (prev->type == ast_types::EXPRESSION_FOR){
        if (cst_head->get_type() == tokens::OPEN_PAREN || cst_head->get_type() == ';'){
            cst_head = cst_head->get_next();
        }
        while (cst_head != nullptr && cst_head->get_type() != ';' && cst_head->get_type() != tokens::OPEN_BRACE){
            tokens.push_back(cst_head);
            cst_head = cst_head->get_next();
        }
        // Handle final paren
        int paren_count = 0;
        for (exec_node* token : tokens){
            if (token->get_type() == tokens::OPEN_PAREN || token->get_type() == tokens::CLOSE_PAREN){
                paren_count++;
            }
        }
        if (paren_count % 2 != 0){
            tokens.pop_back();
        }
    }

    auto mini_head = new ast_node;
    if (!tokens.empty()){
        mini_head = shunting_yard(tokens, table);
        tail->set_next(mini_head);
        while(mini_head->get_next() != nullptr){
            mini_head = mini_head->get_next();
        }
        tail = mini_head;
    }
    return cst_head;
}

int get_precedence(exec_node* node){
    switch (node->get_type()){
        case tokens::TOKEN_AS_STRING: case tokens::INT_AS_STRING: case tokens::FLOAT_AS_STRING:
            return 0;

        case tokens::PLUS_PLUS: case tokens::MINUS_MINUS: case tokens::RIGHT_SLIM_ARROW:
            return 2;

        case tokens::LEFT_SHIFT: case tokens::RIGHT_SHIFT:
            return 5;

        case tokens::LESS_EQUALS: case tokens::GREATER_EQUALS:
            return 6;

        case tokens::EQUALS_EQUALS: case tokens::NOT_EQUALS:
            return 7;

        case tokens::BOOLEAN_AND:
            return 11;

        case tokens::BOOLEAN_OR:
            return 12;

        case tokens::PLUS_EQUALS: case tokens::MINUS_EQUALS: case tokens::DIVIDE_EQUALS: case tokens::TIMES_EQUALS: case tokens::MOD_EQUALS:
        case tokens::LEFT_SHIFT_EQUALS: case tokens::RIGHT_SHIFT_EQUALS: case tokens::AND_EQUALS: case tokens::OR_EQUALS: case tokens::XOR_EQUALS:
            return 14;

        case tokens::OPEN_PAREN: case tokens::CLOSE_PAREN:
            return 16; // KISS MY ASS ISO C

        default:
            break;
    }

    switch (node->get_value()[0]){

        case '!': case '~':
            return 2;

        case '*': case '/': case '%':
            return 3;

        case '+': case '-':
            return 4;

        case '<': case '>':
            return 6;

        case '&':
            return 8;

        case '^':
            return 9;

        case '|':
            return 10;

        case '=':
            return 14;

        case ',':
            return 15;

        default:
            break;
    }
    return -1;
}

bool check_precedence(exec_node* a, exec_node* b){
    int precedence_a = 0;
    int precedence_b = 0;

    // Determine precedence
    precedence_a = get_precedence(a);
    precedence_b = get_precedence(b);

    // Compare precedence
    if (precedence_a <= precedence_b){
        return true;
    }
    return false;
}

ast_node* ast::shunting_yard(std::vector<exec_node *> tokens, symbol_table& table) {
    ast_node* mini_head = nullptr;
    ast_node* mini_tail = nullptr;

    std::stack<exec_node*> stack;
    std::vector<exec_node*> queue;

    for (int i = 0; i < tokens.size(); i++){
        if (table.is_function(tokens[i]->get_value())){
            queue.push_back(tokens[i]);
            i++;
            while (tokens[i]->get_type() != tokens::CLOSE_PAREN){
                queue.push_back(tokens[i]);
                i++;
            }
            queue.push_back(tokens[i]);
        }
        if (table.find_symbol(tokens[i]->get_value()) || tokens[i]->get_type() == tokens::INT_AS_STRING || tokens[i]->get_type() == tokens::FLOAT_AS_STRING ||
            tokens[i]->get_type() == tokens::STRING_LITERAL || tokens[i]->get_type() == tokens::CHAR_LITERAL || tokens[i]->get_type() == '\'' || tokens[i]->get_type() == '"' ||
            tokens[i]->get_value() == "TRUE" || tokens[i]->get_value() == "FALSE" || tokens[i]->get_value() == "[" || tokens[i]->get_value() == "]"){
            queue.push_back(tokens[i]);
        }
        else if (is_operator(tokens[i])){
            while (!stack.empty() && check_precedence(stack.top(), tokens[i])){
                queue.push_back(stack.top());
                stack.pop();
            }
            stack.push(tokens[i]);
        }
        else if (tokens[i]->get_type() == tokens::OPEN_PAREN){
            stack.push(tokens[i]);
        }
        else if (tokens[i]->get_type() == tokens::CLOSE_PAREN){
            while (!stack.empty() && stack.top()->get_type() != tokens::OPEN_PAREN){
                queue.push_back(stack.top());
                stack.pop();
            }
            if (!stack.empty() && stack.top()->get_type() == tokens::OPEN_PAREN){
                stack.pop();
            }
        }
    }

    while (!stack.empty()){
        queue.push_back(stack.top());
        stack.pop();
    }

    for (exec_node* token : queue){
        auto new_node = new ast_node;
        new_node->type = ast_types::TOKEN;
        new_node->value = token->get_value();
        new_node->set_err(token->get_line(), token->get_column());
        if (mini_head == nullptr){
            mini_head = new_node;
            mini_tail = new_node;
        }
        else{
            mini_tail->set_next(new_node);
            mini_tail = new_node;
        }
    }
    return mini_head;
}

bool is_num(char c){
    return c >= '0' && c <= '9';
}

bool ast::is_operator(exec_node *token) {
    int type = token->get_type();
    switch (type){
        case tokens::PLUS_PLUS: case tokens::MINUS_MINUS: case tokens::PLUS_EQUALS: case tokens::MINUS_EQUALS:
        case tokens::BOOLEAN_AND: case tokens::BOOLEAN_OR: case tokens::AND_EQUALS: case tokens::OR_EQUALS:
        case tokens::XOR_EQUALS: case tokens::GREATER_EQUALS: case tokens::LESS_EQUALS: case tokens::LEFT_SHIFT:
        case tokens::RIGHT_SHIFT: case tokens::LEFT_SHIFT_EQUALS: case tokens::RIGHT_SHIFT_EQUALS:
        case tokens::RIGHT_SLIM_ARROW: case tokens::TIMES_EQUALS: case tokens::DIVIDE_EQUALS:
        case tokens::NOT_EQUALS: case tokens::MOD_EQUALS: case tokens::EQUALS_EQUALS:
            return true;

        default:
            break;
    }

    if (token->get_value().length() == 1){
        char val = token->get_value()[0];
        switch (val){
        case '*': case '/': case '%': case '+': case '-': case '<': case '>': case '&': case '^': case '|': case '=': case ',': case '!': case '~':
                return true;

            default:
                break;
        }
    }

    return false;
}

void ast::print_tree(std::string &filename) {
    ast_node* current = head;
    int num_spaces_over = 0;

    std::ofstream out(filename);

    while (current != nullptr) {
        switch (current->type) {
            case ast_types::BEG_BLOCK:
                out << "BEGIN BLOCK";
                break;

            case ast_types::END_BLOCK:
                out << "END BLOCK";
                break;

            case ast_types::RETURN:
                out << "RETURN";
                num_spaces_over += 6;
                break;

            case ast_types::DECLARATION:
                out << "DECLARATION";
                break;

            case ast_types::ASSIGNMENT:
                out << "ASSIGNMENT";
                num_spaces_over += 11;
                break;

            case ast_types::STATEMENT_IF:
                out << "IF";
                num_spaces_over += 2;
                break;

            case ast_types::EXPRESSION_FOR:
                out << "FOR EXPRESSION " << current->for_expr_num;
                num_spaces_over += 3;
                break;

            case ast_types::EXPRESSION_WHILE:
                out << "WHILE";
                num_spaces_over += 5;
                break;

            case ast_types::STATEMENT_PRINTF:
                out << "PRINTF";
                num_spaces_over += 6;
                break;

            case ast_types::ELSE:
                out << "ELSE";
                break;

            case ast_types::CALL:
                out << "CALL";
                num_spaces_over += 4;
                break;

            case ast_types::TOKEN: case ast_types::OPERATOR:
                out << current->value;
                num_spaces_over += current->value.length();
                break;

            default:
                break;
        }

        out << " -> ";
        if (current->get_next() == nullptr){
            out << std::endl;
            for (int i = 0; i < num_spaces_over; i++){
                out << " ";
            }
        } else{
            num_spaces_over += 3;
        }

        if (current->get_next() == nullptr && current->get_chld() == nullptr){
            return;
        }
        else if (current->get_next() == nullptr && current->get_chld() != nullptr){
            current = current->get_chld();
        }
        else{
            current = current->get_next();
        }
    }
    out.close();
}

void ast::add_node(ast_node *n) {
    if (head == nullptr){
        head = n;
        tail = head;
    }
    else{
        if (n->type < 9000 || tail->type == ast_types::ASSIGNMENT || tail->type == ast_types::STATEMENT_PRINTF || tail->type == ast_types::CALL){
            tail->set_next(n);
            tail = n;
        }
        else{
            tail->set_chld(n);
            tail = n;
        }
    }
}

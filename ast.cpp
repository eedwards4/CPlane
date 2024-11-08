//
// Created by Ethan Edwards on 11/5/24.
//

#include "ast.h"
#include "symbol_table.h"

ast::~ast(){
    while (head != nullptr){
        if (head->get_next() == nullptr && head->get_chld() == nullptr){
            delete head;
            return;
        }
        if (head->get_next() == nullptr){
            ast_node* tmp = head;
            head = head->get_chld();
            delete tmp;
        }
        else{
            ast_node* tmp = head;
            head = head->get_next();
            delete tmp;
        }
    }
}

void ast::build_tree(exec_node* cst_head, symbol_table table){
    int for_depth_tracker = 0;
    int while_depth_tracker = 0;

    while (cst_head != nullptr){
        if (cst_head->get_type() == tokens::OPEN_BRACKET){
            auto new_node = new ast_node;
            new_node->set_type(ast_types::BEG_BLOCK);
            add_node(new_node);
        }
        else if (cst_head->get_type() == tokens::CLOSE_BRACKET){
            auto new_node = new ast_node;
            new_node->set_type(ast_types::END_BLOCK);
            add_node(new_node);
        }
        else if (cst_head->get_type() == tokens::TOKEN_AS_STRING){
            auto new_node = new ast_node;
            if (cst_head->get_value() == "function" || cst_head->get_value() == "procedure"){
                new_node->set_type(ast_types::DECLARATION);
                add_node(new_node);
                // Consume declaration
                while (cst_head->get_next() != nullptr){
                    cst_head = cst_head->get_next();
                }
            }
            else if (symbols::data_types::check_type(cst_head->get_value()) != -1 && symbols::data_types::check_type(cst_head->get_value()) != symbols::data_types::VOID){
                new_node->set_type(ast_types::DECLARATION);
                add_node(new_node);

                if (cst_head->get_next()->get_next()->get_value() == "="){
                    // We have a declaration and assignment in one line
                    new_node = new ast_node;
                    new_node->set_type(ast_types::ASSIGNMENT);
                    add_node(new_node);
                    // Handle the rest of the declaration with shunting yard and push tokens to the table
                    cst_head = shunting_yard_wrapper(cst_head, new_node);
                }
            }
            else if (table.find_symbol(cst_head->get_value())){
                new_node->set_type(ast_types::ASSIGNMENT);
                add_node(new_node);
                // Handle the rest of the declaration with shunting yard and push tokens to the table
                cst_head = shunting_yard_wrapper(cst_head, new_node);
            }
            else if (cst_head->get_value() == "printf"){
                new_node->set_type(ast_types::STATEMENT_PRINTF);
                add_node(new_node);
                // Handle the rest of the printf statement
                cst_head = shunting_yard_wrapper(cst_head, new_node);
            }
            else if (cst_head->get_value() == "if"){
                new_node->set_type(ast_types::STATEMENT_IF);
                add_node(new_node);
                // Handle the rest of the if statement
                cst_head = shunting_yard_wrapper(cst_head, new_node);
            }
            else if (cst_head->get_value() == "return"){
                new_node->set_type(ast_types::RETURN);
                add_node(new_node);
                // Handle the rest of the return
                cst_head = shunting_yard_wrapper(cst_head, new_node);
            }
            else if (cst_head->get_value() == "for"){
                new_node->set_type(ast_types::EXPRESSION_FOR);
                new_node->set_depth(for_depth_tracker);
                add_node(new_node);
                // Handle the for loop conditions with shunting yard
                cst_head = shunting_yard_wrapper(cst_head, new_node);

                for_depth_tracker++; // TODO: Properly implement this
            }
            else if (cst_head->get_value() == "while"){
                new_node->set_type(ast_types::EXPRESSION_WHILE);
                new_node->set_depth(while_depth_tracker);
                add_node(new_node);

                // Handle the for loop conditions with shunting yard
                cst_head = shunting_yard_wrapper(cst_head, new_node);

                while_depth_tracker++; // TODO: Properly implement this
            }
        }
        // Navigation
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

exec_node* ast::shunting_yard_wrapper(exec_node* cst_head, ast_node* prev){
    std::vector<exec_node*> tokens;
    if (prev->get_type() == ast_types::ASSIGNMENT){
        while (cst_head->get_next() != nullptr && cst_head->get_next()->get_value() != ";"){
            cst_head = cst_head->get_next();
            tokens.push_back(cst_head);
        }
    }
    else if (prev->get_type() == ast_types::STATEMENT_IF || prev->get_type() == ast_types::EXPRESSION_FOR || prev->get_type() == ast_types::EXPRESSION_WHILE){
        // Create a vector of all symbols between the first symbol and an open brace
        while (cst_head->get_next()->get_type() != tokens::OPEN_BRACE){
            cst_head = cst_head->get_next();
            tokens.push_back(cst_head);
        }
    }

    auto mini_head = new ast_node;
    if (tokens.size() > 1){
        mini_head = shunting_yard(tokens);
        if (mini_head != nullptr){
            tail->set_next(mini_head);
            while(mini_head->get_next() != nullptr){ // Move through the newly added nodes to find the new tail
                mini_head = mini_head->get_next();
            }
            tail = mini_head;
        }
    }
    else{ // Skip shunting yard on single-token inputs
        mini_head->set_type(ast_types::TOKEN);
        mini_head->set_err(cst_head->get_line(), cst_head->get_column());
        tail->set_next(mini_head);
        tail = mini_head;
    }

    return cst_head;
}

bool is_num(char c){
    return c >= '0' && c <= '9';
}

bool is_operator(exec_node* token){
    int type = token->get_type();
    switch (type){
        case tokens::PLUS_PLUS: case tokens::MINUS_MINUS: case tokens::PLUS_EQUALS: case tokens::MINUS_EQUALS:
        case tokens::BOOLEAN_AND: case tokens::BOOLEAN_OR: case tokens::AND_EQUALS: case tokens::OR_EQUALS:
        case tokens::XOR_EQUALS: case tokens::GREATER_EQUALS: case tokens::LESS_EQUALS: case tokens::LEFT_SHIFT:
        case tokens::RIGHT_SHIFT: case tokens::LEFT_SHIFT_EQUALS: case tokens::RIGHT_SHIFT_EQUALS:
        case tokens::RIGHT_SLIM_ARROW: case tokens::TIMES_EQUALS: case tokens::DIVIDE_EQUALS:
        case tokens::NOT_EQUALS: case tokens::MOD_EQUALS:
            return true;

        default:
            break;
    }

    if (token->get_value().size() == 1){
        std::string val = token->get_value();
        try{
            if (!std::isalpha(std::stoi(val)) && !is_num(val[0])){
                return true;
            }
        }
        catch (std::invalid_argument&){
            return false;
        }
    }

    return false;
}

int get_precedence(exec_node* node){
    switch (node->get_type()){
        case tokens::PLUS_PLUS: case tokens::MINUS_MINUS: case tokens::RIGHT_SLIM_ARROW:
            return 1;

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

        default:
            break;
    }

    switch (node->get_value()[0]){
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
    if (precedence_a >= precedence_b){
        return true;
    }
    return false;
}

ast_node* ast::shunting_yard(std::vector<exec_node*> tokens){
    ast_node* mini_head = nullptr;
    ast_node* mini_tail = nullptr;

    std::vector<exec_node*> stack;

    for (exec_node* token : tokens){
        if (token->get_type() == tokens::TOKEN_AS_STRING || token->get_type() == tokens::INT_AS_STRING || token->get_type() == tokens::FLOAT_AS_STRING){
            if (mini_head == nullptr){
                mini_head = new ast_node;
                mini_head->set_type(ast_types::TOKEN);
                mini_head->set_value(token->get_value());
                mini_head->set_err(token->get_line(), token->get_column());
                mini_tail = mini_head;
            }
            else{
                auto new_node = new ast_node;
                new_node->set_type(ast_types::TOKEN);
                new_node->set_value(token->get_value());
                new_node->set_err(token->get_line(), token->get_column());
                mini_tail->set_next(new_node);
                mini_tail = new_node;
            }
        }
        else if (is_operator(token)){
            while (!stack.size() > 0 && check_precedence(stack.back(), token)){
                auto new_node = new ast_node;
                new_node->set_type(ast_types::OPERATOR);
                new_node->set_value(token->get_value());
                new_node->set_err(stack.back()->get_line(), stack.back()->get_column());
                mini_tail->set_next(new_node);
                mini_tail = new_node;
                stack.pop_back();
            }
            stack.push_back(token);
        }
        else if (token->get_type() == tokens::OPEN_PAREN){
            stack.push_back(token);
        }
        else if (token->get_type() == tokens::CLOSE_PAREN){
            while(!stack.empty() && stack.back()->get_type() != tokens::OPEN_PAREN){
                auto new_node = new ast_node;
                new_node->set_type(ast_types::OPERATOR);
                new_node->set_value(token->get_value());
                new_node->set_err(stack.back()->get_line(), stack.back()->get_column());
                mini_tail->set_next(new_node);
                mini_tail = new_node;
                stack.pop_back();
            }
            stack.pop_back();
        }
    }
    while (!stack.empty()){
        auto new_node = new ast_node;
        new_node->set_type(ast_types::OPERATOR);
        new_node->set_value(stack.back()->get_value());
        new_node->set_err(stack.back()->get_line(), stack.back()->get_column());
        mini_tail->set_next(new_node);
        mini_tail = new_node;
        stack.pop_back();
    }
    return mini_head;
}

void ast::add_node(ast_node* n){
    if (head == nullptr){ // First node
        head = n;
        tail = head;
    }
    else{
        if (n->get_type() < 9000){
            tail->set_next(n);
            tail = n;
        }
        else{
            tail->set_chld(n);
            tail = n;
        }
    }
}

void ast::print_tree(){
    ast_node* current = head;
    ast_node* prev = head;
    int num_spaces_over = 0;

    while (current != nullptr){
        switch (current->get_type()){
        case ast_types::BEG_BLOCK:
            std::cout << "BEGIN BLOCK";
            num_spaces_over += 11;
            break;

        case ast_types::END_BLOCK:
            std::cout << "END BLOCK";
            num_spaces_over += 9;
            break;

        case ast_types::RETURN:
            std::cout << "RETURN";
            num_spaces_over += 6;
            break;

        case ast_types::DECLARATION:
            std::cout << "DECLARATION";
            num_spaces_over += 11;
            break;

        case ast_types::ASSIGNMENT:
            std::cout << "ASSIGNMENT";
            num_spaces_over += 10;
            break;

        case ast_types::STATEMENT_IF:
            std::cout << "IF";
            num_spaces_over += 2;
            break;

        case ast_types::EXPRESSION_FOR:
            std::cout << "FOR";
            num_spaces_over += 3;
            break;

        case ast_types::EXPRESSION_WHILE:
            std::cout << "WHILE";
            num_spaces_over += 5;
            break;

        case ast_types::STATEMENT_PRINTF:
            std::cout << "PRINTF";
            num_spaces_over += 6;
            break;

        case ast_types::TOKEN: case ast_types::OPERATOR:
            std::cout << current->get_value();
            num_spaces_over += current->get_value().length();
            break;

        default:
            break;
        }

        std::cout << " -> ";
        num_spaces_over += 3;

        if (current->get_next() == nullptr && current->get_chld() == nullptr){
            return;
        }
        if (current->get_next() == nullptr){
            current = current->get_chld();
        }
        else{
            current = current->get_next();
        }
    }
}


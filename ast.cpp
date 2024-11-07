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

        while (cst_head->get_next()->get_value() != ";"){
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
        // This should never be the head of the tree so we can skip nullptr checking
        tail->set_next(mini_head);
        while(mini_head->get_next() != nullptr){ // Move through the newly added nodes to find the new tail
            mini_head = mini_head->get_next();
        }
        tail = mini_head;
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

    std::string val = token->get_value();
    if (val.size() == 1 && !std::isalpha(std::stoi(val)) && !is_num(val[0])){
        return true;
    }

    return false;
}

ast_node* ast::shunting_yard(std::vector<exec_node*> tokens){
    ast_node* mini_head = nullptr;

    for (exec_node* token : tokens){
        if (token->get_type() == tokens::TOKEN_AS_STRING || token->get_type() == tokens::INT_AS_STRING){
            // TODO: READ INTO QUEUE
        }
        else if (is_operator(token)){

        }
        else if (token->get_type() == tokens::OPEN_PAREN){
            
        }
        else if (token->get_type() == tokens::CLOSE_PAREN){

        }
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

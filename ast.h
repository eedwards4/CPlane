//
// Created by Ethan Edwards on 11/7/2024.
//

#ifndef CPLANE_AST_H
#define CPLANE_AST_H

#include <string>
#include <stack>

#include "exec_path.h"
#include "symbol_table.h"

class ast_types{
public:
    static constexpr int BEG_BLOCK = 9999;
    static constexpr int END_BLOCK = 9998;
    static constexpr int RETURN = 9997;
    static constexpr int DECLARATION = 9996;
    static constexpr int ASSIGNMENT = 9995;
    static constexpr int STATEMENT_IF = 9994;
    static constexpr int EXPRESSION_FOR = 9993;
    static constexpr int EXPRESSION_WHILE = 9992;
    static constexpr int STATEMENT_PRINTF = 9991;
    static constexpr int TOKEN = 9990;
    static constexpr int OPERATOR = 9989;
    static constexpr int ELSE = 9988;
    static constexpr int CALL = 9987;
};

class ast_node{
public:
    ast_node() = default;
    ~ast_node() = default;

    int type = -1;
    int for_expr_num = 0;

    std::string value;

    void set_next(ast_node* n){ this->next = n; }
    void set_chld(ast_node* c){ this->chld = c; }
    void set_err(const int l, const int c){ this->line = l; this->col = c; }

    ast_node* get_next() const { return next; }
    ast_node* get_chld() const { return chld; }

private:
    ast_node* next = nullptr;
    ast_node* chld = nullptr;

    int line = -1;
    int col = -1;
};

class ast {
public:
    ast() = default;
    ~ast() = default;

    void build_tree(exec_node* cst_head, symbol_table& table);
    void print_tree(std::string &filename);

private:
    ast_node* head = nullptr;
    ast_node* tail = nullptr;

    exec_node* shunting_yard_wrapper(exec_node* cst_head, ast_node* prev, symbol_table& table);
    ast_node* shunting_yard(std::vector<exec_node*> tokens, symbol_table& table);
    static bool is_operator(exec_node* token);

    void add_node(ast_node* n);
};


#endif //CPLANE_AST_H

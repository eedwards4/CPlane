//
// Created by Ethan Edwards on 11/5/24.
//

#ifndef AST_H
#define AST_H

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
};

class ast_node{
public:
    ast_node() = default;
    ~ast_node() = default;

    void set_type(const int t){ this->type = t; }
    void set_depth(const int d){ this->depth = d; }
    void set_value(const std::string& v){ this->value = v; }
    void set_err(const int l, const int c){ this->line = l; this->col = c; }
    void set_next(ast_node* n){ this->next = n; }
    void set_chld(ast_node* c){ this->chld = c; }

    int get_type() const{ return type; }
    int get_depth() const{ return depth; }
    std::string get_value() const{ return value; }
    int get_line() const{ return line; }
    int get_col() const{ return col; }
    ast_node* get_next() const { return next; }
    ast_node* get_chld() const { return chld; }

private:
    int type = -1;
    int depth = 0;

    std::string value;

    int line = -1;
    int col = -1;

    ast_node* next;
    ast_node* chld;
};


class ast {
public:
    ast() = default;
    ~ast();

    // Driver for building the tree
    void build_tree(exec_node* cst_head, symbol_table table);

    // Handle intake of tokens to be shunted
    exec_node* shunting_yard_wrapper(exec_node* cst_head, ast_node* prev);

    // Shunting yard algorithm to convert from infix to postfix
    // Returns an ast node to the head of the postfix so that the wrapper can handle adding all tokens at once
    ast_node* shunting_yard(std::vector<exec_node*> tokens);

    // Add nodes to the tree
    void add_node(ast_node* n);

    // Out
    void print_tree();

private:
    ast_node* head = nullptr;
    ast_node* tail = nullptr;

};



#endif //AST_H

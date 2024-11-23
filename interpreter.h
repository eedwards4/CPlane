//
// Created by Logan Puntous on 11/8/2024.
//

// USAGE
// in main
// Interpreter p(ast, table, errors);
// p.Start();

#ifndef CPLANE_INTERPRETER_H
#define CPLANE_INTERPRETER_H

#include "symbol_table.h"
#include "ast.h"
#include "errors_new.h"

#include <stack>
#include <type_traits>
#include <cctype>
#include <cmath>

class Interpreter {
    public:
    Interpreter();
    Interpreter(ast tree, symbol_table& table, ERRORS& errors);

    // Runs the program internally
    void Begin();
    // Not sure yet but maybe step through the program incrementally with keyboard
    void BeginDebug();
    
private:
    // Moving variables
    ast_node* pc; // Program counter pointer
    int running_counter;
    int level;
    int exit_code;
    int type;
    std::string working_print_statement;

    // Bool triggers
    bool is_running;
    bool in_main;
    bool in_function;

    // Internal dataframes
    std::stack<ast_node*> expression_stack; // stack for holding single nodes
    std::vector<ast_node*> functions; // lists all function heads
    std::stack<int> scope_stack; // stack for the current scope. push on function enter, pop on function exit
    ast as_tree;
    symbol_table s_table; // For reading/writing into
    ERRORS errors;

    // Internal functions
    void ExitQuiet();
    void clearStack();
    void printStack(std::stack<ast_node*>& expression_stack);
    void TopThree(int code);
    void beginHelper(ast_node* &current);
    void EvalOperatorUpdate(ast_node* one, ast_node* two, ast_node*& three);
    ast_node* eval_top_three(std::string one, std::string two, std::string three);
    void CheckAddFunction(ast_node *current);
    void CheckCallFunction(ast_node *current);

    bool isNumber(std::string str);
    bool isOperator(std::string str);
};  


#endif //CPLANE_INTERPRETER_H

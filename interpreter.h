//
// Created by Logan Puntous on 11/8/2024.
//

// USAGE
// in main
// Interpreter p;
// p.Start();

#ifndef CPLANE_INTERPRETER_H
#define CPLANE_INTERPRETER_H

#include "symbol_table.h"
#include "ast.h"
#include "errors_new.h"

#include<stack>

class Interpreter {
    public:
    Interpreter();
    //Interpreter(ast_node *head, ast_node *tail, symbol_table& table);
    Interpreter(ast tree, symbol_table& table);

    // Runs the program internally
    void Begin();
    // Not sure yet but maybe step through the program incrementally with keyboard
    void BeginDebug();
    int Exit();
  
    private:
    void clearStack();
    void printStack(std::stack<ast_node*> runtime_stack);
    void EvaluateStack();

    bool is_running;
    int program_counter;
    int exit_code; 

    std::stack<ast_node*> runtime_stack;
    ast as_tree;
    ast_node *ast_head; // For reading from
    ast_node *ast_tail; // For reading from
    symbol_table s_table; // For reading/writing into
};  




#endif //CPLANE_INTERPRETER_H

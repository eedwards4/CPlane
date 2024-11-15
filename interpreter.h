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

#include <stack>
#include <type_traits>

class Interpreter {
    public:
    Interpreter();
    //Interpreter(ast_node *head, ast_node *tail, symbol_table& table);
    Interpreter(ast tree, symbol_table& table, ERRORS& errors);

    // Runs the program internally
    void Begin();

    void Start();
    // Not sure yet but maybe step through the program incrementally with keyboard
    void BeginDebug();
    int Exit();
  
private:
    // Variables
    bool is_building;
    bool is_running;
    int running_counter;
    int level;
    bool in_main;
    int exit_code;

    std::stack<ast_node*> expression_stack; // stack for holding single nodes
    std::stack<std::stack<ast_node*>> execution_stack; // stack for holding stacks of single nodes
    ast_node* pc; // Program counter pointer
    std::vector<ast_node*> functions; // lists all function heads

    ast as_tree;
    ast_node *ast_head; // For reading from
    ast_node *ast_tail; // For reading from
    symbol_table s_table; // For reading/writing into
    ERRORS errors;

    void clearStack();
    void printStack(std::stack<ast_node*>& expression_stack);
    void printEStack();
    void EvaluateStack();


    void CheckAddFunction(ast_node *current);
};  




#endif //CPLANE_INTERPRETER_H

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
//#include "errors_new.h"

#include<stack>

class Interpreter {
    public:
    Interpreter();
    Interpreter(ast AST, symbol_table ST);

    // Runs the program internally
    void Begin();
    // Not sure yet but maybe step through the program incrementally with keyboard
    void BeginDebug();

    private:
    std::stack<int> runtime_stack;
    int program_counter;
    bool is_running; 
    int exit_code; 

    // Not sure yet
    const ast AST; // For reading from
    symbol_table ST; // For reading/writing into
};  




#endif //CPLANE_INTERPRETER_H
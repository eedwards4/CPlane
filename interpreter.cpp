#include "interpreter.h"


// Default constructor
Interpreter::Interpreter(){
    program_counter = 0;
    is_running = false; // Sets running to true in Begin
    exit_code = 0; // By default program is successful
}
Interpreter::Interpreter(ast AST, symbol_table ST){
    program_counter = 0;
    is_running = false; // Sets running to true in Begin
    exit_code = 0; // By default program is successful
    // TODO
    // Set head n shit
}

int Interpreter::Exit(){
    std::cout << "Process finished with exit code " << exit_code << std::endl; // exit code 0 by default
    return exit_code;
}




// Not necessary but would be ez and cool, can step through program manually.
void Interpreter::BeginDebug(){
    std::cout << "DEBUG MODE: w -> forward one step (not implemented)" << std::endl;
    Begin(); 
}


// Maintain a stack data structure and a program counter to keep track of the flow-of-control as the function interprets and executes an input program.
// Needs to alter the symbol_table
void Interpreter::Begin(/*symbol_node?*/){
    // Running program here
    // TODO: some prelim tests prolly maybe setting up vars
    while ( is_running ) {
        // TODO BUNCH OF SHISH
        is_running = false;








        program_counter++;
    }   


}
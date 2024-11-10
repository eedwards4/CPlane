#include "interpreter.h"


// Default constructor
Interpreter::Interpreter(){
    program_counter = 0;
    is_running = false; // Sets running to true in Begin
    exit_code = 0; // By default program is successful
    ast_head = nullptr;
    ast_tail = nullptr;    
}
Interpreter::Interpreter(ast tree, symbol_table& table){
    program_counter = 0;
    is_running = false; // Sets running to true in Begin
    exit_code = 0; // By default program is successful
    // TODO
    as_tree = tree; // AST
    s_table = table; // Symbol table
    ast_head = tree.get_head();
    ast_tail = tree.get_tail();
}


// Helper functions for stack
void Interpreter::clearStack(){
    while (!runtime_stack.empty()) { 
        runtime_stack.pop(); 
    } 
}
void Interpreter::printStack(std::stack<ast_node*> stack){
    // If stack is empty then return
    if (stack.empty()) 
        return;
     
    ast_node* curr = stack.top();
    stack.pop();
 
    printStack(stack); // Recursive call
 
    std::cout << curr->value << " ";
    stack.push(curr);
}



// Exit function for finishing process
int Interpreter::Exit(){
    std::cout << "Process finished with exit code " << exit_code << std::endl; // exit code 0 by default
    return exit_code;
}


// Takes the current stack and evaluates it and updates symbol table
void Interpreter::EvaluateStack(){
    if ( runtime_stack.empty() ){
        return;
    }
    // While there are elements in the stack
    while ( ! runtime_stack.empty() ){
        //TODO: need to go through elements, order them correctly
        //TODO: need to parse ordered list of elements and "execute them"
        runtime_stack.pop(); 
    }
    //TODO: update symbol table

}


// Not necessary but would be ez and cool, can step through program manually.
void Interpreter::BeginDebug(){
    std::cout << "*DEBUG MODE* n -> forward one step (not implemented)" << std::endl;
    char a;
    while ( std::cout << ":", std::cin >> a, a == 'n' ){
        std::cout << "next step innit" << std::endl;
    }
    Begin(); // Only here cause shit aint work yet
}


// Maintain a stack data structure and a program counter to keep track of the flow-of-control as the function interprets and executes an input program.
// Needs to alter the symbol_table
void Interpreter::Begin(){
    // PRELIM tests and checks
    if ( ast_head == nullptr && ast_tail == nullptr ){
        std::cout << "The abstract syntax tree is currently empty!" << std::endl;
        exit(1);
    }

    // Setting up variables
    is_running = true;
    ast_node* current = ast_head;

    // Check if the ast is not uninitilized
    if ( &ast_head == &ast_tail ) {
        std::cout << "ERROR in Interpreter::Begin: the ast_head and ast_tail are the same!" << std::endl;
    }
    if ( ! runtime_stack.empty() ){
        std::cout << "ERROR in Interpreter::Begin: the runtime stack is not empty!" << std::endl;
    }

    // This loop doesnt stop until the program completes by failing or completing
    // As of now goes through all elements of the AST
    while ( is_running ) {
        // CHECKS AND BALANCES
        program_counter++; // Inc program counter prolly needs to change


        // TRAVERSAL
        // At end of abstract syntax tree
        if (current->get_next() == nullptr && current->get_chld() == nullptr){
            std::cout << "AST successfully traversed with program count " << program_counter << std::endl;
            is_running = false;
            continue;
        }
        // Grabbing next child of the AST
        else if (current->get_next() == nullptr && current->get_chld() != nullptr){
            //USE STACK AND CLEAR IT
            if ( ! runtime_stack.empty() ){
                std::cout << "Stack: ";
                printStack(runtime_stack);
                std::cout << std::endl;
            }

            EvaluateStack();
            clearStack();
            
            //std::cout << "getting child" << std::endl;
            current = current->get_chld();
        }
        // Grabbing the next node of AST
        else{
            //std::cout << "getting next..." << std::endl;
            current = current->get_next();
            //std::cout << "Pushing " << current->value << " onto stack!" << std::endl;
            runtime_stack.push(current);
        }

        
        // If we are stuck in an infinite loop
        if ( program_counter > 999 ){
            std::cout << "ERROR in Interpreter::Begin: Stuck in infinite loop! Exiting..." << std::endl;
            clearStack();
            exit_code = 1;
            is_running = false;
            break;
        }
    }   


}

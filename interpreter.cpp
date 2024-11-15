#include "interpreter.h"


// Default constructor
Interpreter::Interpreter(){
    running_counter = 0;
    level = 0;
    is_building = false;
    is_running = false; // Sets running to true in Begin
    exit_code = 0; // By default program is successful
    ast_head = nullptr;
    ast_tail = nullptr;    
}
Interpreter::Interpreter(ast tree, symbol_table& table){
    level = 0;
    running_counter = 0;
    is_building = false;
    is_running = false; // Sets running to true in Begin
    exit_code = 0; // By default program is successful
    // TODO
    s_table = table; // Symbol table
    as_tree = tree; // AST
    ast_head = tree.get_head();
    ast_tail = tree.get_tail();
}


// Helper functions for stack
void Interpreter::clearStack(){
    while (!expression_stack.empty()) { 
        expression_stack.pop(); 
    }
}
void Interpreter::printStack(std::stack<ast_node*>& stack){
    // If stack is empty then return
    if (stack.empty()) 
        return;
     
    ast_node* curr = stack.top();
    stack.pop();
 
    printStack(stack); // Recursive call
 
    std::cout << curr->value << " ";
    stack.push(curr);
}
void Interpreter::printEStack(){
    std::cout << "Expression stack: " << std::endl;
    int s = execution_stack.size();
    for ( int i = 0; i < s; i++ ){
        std::cout << "Stack: ";
        printStack(execution_stack.top());
        std::cout << std::endl;
        execution_stack.pop();
    }
}



// Exit function for finishing process
int Interpreter::Exit(){
    std::cout << "Process finished with exit code " << exit_code << std::endl; // exit code 0 by default
    return exit_code;
}








void Interpreter::HandleType(ast_node *current){
    if ( ast_types::what_is(current->type) == "DECLARATION"){
        if ( (current->value == "function" || current->value == "procedure") ){
            expression_stack.push(current); // Simply pushing "function" or "procedure into expression stack for later use"
            if ( current->func_name == "main" ){
                pc = current;
            }
        }
        // Start of function stuff
        if ( ast_types::what_is(current->get_chld()->type) == "BEG_BLOCK"){
            //balls
            level++;
        }
    }
    // End of function stuff
    if ( ast_types::what_is(current->type) == "END_BLOCK" ){
        level--;
    }

    if ( ast_types::what_is(current->type) == "ASSIGNMENT"){
    }

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
    running_counter++;
    // PRELIM tests and checks
    if ( ast_head == nullptr && ast_tail == nullptr ){
        std::cout << "The abstract syntax tree is currently empty!" << std::endl;
        exit(1);
    }
    // Setting up variables
    is_building = true;
    ast_node* current = ast_head;
    // Check if the ast is not initialized
    if ( &ast_head == &ast_tail ) {
        std::cout << "ERROR in Interpreter::Begin: the ast_head and ast_tail are the same!" << std::endl;
    }
    if ( ! expression_stack.empty() ){
        std::cout << "ERROR in Interpreter::Begin: the runtime stack is not empty!" << std::endl;
    }

    // Loops through all ast_nodes in the ast and builds everything
    while ( is_building ) {
        //std::cout << "type: " << ast_types::what_is(current->type) << std::endl;
        //std::cout << "value: " << (current->value) << std::endl;
        // TRAVERSAL
        // At end of abstract syntax tree
        if (current->get_next() == nullptr && current->get_chld() == nullptr){
            std::cout << "AST successfully traversed during building " << running_counter << std::endl;
            is_building = false;
            continue;
        }
        // End of expression
        else if (current->get_next() == nullptr && current->get_chld() != nullptr){
            // Pushing expression stack onto the execution stack
            if ( ! expression_stack.empty() ){
                execution_stack.push(expression_stack);
            }
            // Clearing the expression stack
            clearStack();
            // Setting pc to top of main.
            HandleType(current);
            //std::cout << "else if: " << ast_types::what_is(current->type)<< std::endl;
    
            // Moving forward
            current = current->get_chld();  
        } else{
            // Next line
            //std::cout << "else: " << ast_types::what_is(current->type)<< std::endl;
            HandleType(current);
            current = current->get_next();
            // Filling expression stack
            expression_stack.push(current);
        }
        
        // If we are stuck in an infinite loop
        if ( running_counter > 9999 ){
            std::cout << "ERROR in Interpreter::Begin: Stuck in infinite building loop! Exiting..." << std::endl;
            clearStack();
            exit_code = 1;
            is_building = false;
            break;
        }   
    } // FINISH SETUP

    
    

    



    printEStack();
}

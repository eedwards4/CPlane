#include "interpreter.h"


// Default constructor
Interpreter::Interpreter(){
    in_main = false;
    running_counter = 0;
    level = 0;
    is_building = false;
    is_running = false; // Sets running to true in Begin
    exit_code = 0; // By default program is successful
    ast_head = nullptr;
    ast_tail = nullptr;    
}
Interpreter::Interpreter(ast tree, symbol_table& table, ERRORS& in_errors){
    in_main = false;
    level = 0;
    running_counter = 0;
    is_building = false;
    is_running = false; // Sets running to true in Begin
    exit_code = 0; // By default program is successful
    // TODO
    s_table = table; // Symbol table
    as_tree = tree; // AST
    errors = in_errors;
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







// Checks for main and other functions to add the head of each function to functions vector
void Interpreter::CheckAddFunction(ast_node *current){
    // DECLARATION
    if ( ast_types::what_is(current->type) == "DECLARATION"){
        // FUNCTION OR PROCEDURE
        if ( (current->value == "function" || current->value == "procedure") ){
            // MAIN
            if ( current->func_name == "main" && ! in_main){
                // STARTING EXECUTION HERE
                in_main = true;
                // Setting PC to begin
                pc = current;
                // Putting main into functions
                std::cout << "adding main" << std::endl;
                functions.push_back(current);
            } 
            // Check symbol table for prior declaration
            else if ( s_table.find_symbol(current->func_name) && ! in_main ){
                // add function to function vector.
                std::cout << "adding function " << current->func_name << std::endl;
                functions.push_back(current);
            } else {
                //ERROR prior decleration of function
            } 

            // Simply pushing "function" or "procedure into expression stack for later use"
            expression_stack.push(current); 
        }
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
    is_running = true;
    ast_node* current = ast_head;
    

    // Use this as simply a machine that can go through the ast_nodes incrementally
    // First it looks for main and all other functions to add their head node locations to a vector
    // Then starts executing main sequentially, bouncing to the head of each function accordingly.
    // When the function finishes it should jump back to the PC so the program can continue.
    // Example: in main, goHere(); then goHere() calls goThere() the pc must be copied for each function entered in case 
    // function calls another function that will need a pc to return to. then the first function called can complete and
    // the original pc is called bringing the machine back into main right after the function call. when you return, pc.next()
    // find main->go through main-> finishes when mains final end block is called. (level = 0 && current = end_block)
    // otherwise finishes when the ast is fully traversed.
    while ( is_running ) {
        //std::cout << "type: " << ast_types::what_is(current->type) << std::endl;
        //std::cout << "value: " << (current->value) << std::endl;
        // After traversal below we are now in main.
    
        // TRAVERSAL
        // At end of abstract syntax tree
        if (current->get_next() == nullptr && current->get_chld() == nullptr){
            std::cout << "AST successfully traversed during pc " << &pc << std::endl;
            is_running = false;
            continue;
        }
        // End of expression
        else if (current->get_next() == nullptr && current->get_chld() != nullptr){
            // Pushing expression stack onto the execution stack
            if ( ! expression_stack.empty() ){
                execution_stack.push(expression_stack);
            }
            // If we are in main 
            if ( in_main ){
                
            }

            // Clearing the expression stack
            clearStack();
            // Setting pc to top of main.
            CheckAddFunction(current);
            std::cout << "else if: " << ast_types::what_is(current->type)<< std::endl;
    
            // Moving forward
            current = current->get_chld();  
        } else{
            // Next line
            std::cout << "else: " << ast_types::what_is(current->type)<< std::endl;
            CheckAddFunction(current);
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

    
    

    


    errors.STOP_SYNTAX_ERRORS();
    //printEStack();
}

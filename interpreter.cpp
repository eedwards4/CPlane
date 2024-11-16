#include "interpreter.h"


// Default constructor
Interpreter::Interpreter(){
    in_main = false;
    in_function = false;
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
    in_function = false;
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
            //expression_stack.push(current); 
        }
    }
}


// Get top three of stack and evaluates accordingly (this is the MEAT)
void Interpreter::TopThree(){
    if ( expression_stack.size() < 3 ){
        return;
    }
    ast_node* one = expression_stack.top();
    expression_stack.pop();
    ast_node* two = expression_stack.top();
    expression_stack.pop();
    ast_node* three = expression_stack.top();
    expression_stack.pop();

    std::cout << "TopThree(): " << one->value << two->value << three->value << std::endl;


    expression_stack.push(three);
    expression_stack.push(two);
    expression_stack.push(one);
}


// Checks and add function names, pushes tokens onto expression stack, moves pc to current line of execution, 
// calls TopThree on expression stack and keeps track where we are in program.
void Interpreter::beginHelper(ast_node* current){
    //std::cout << "enc: " << ast_types::what_is(current->type)<< std::endl;
    if ( ! in_main ){ CheckAddFunction(current); }
    // Pushing onto expression stack
    if ( ast_types::what_is(current->type) == "TOKEN" ){
        expression_stack.push(current);  
    } else {
        // Moving pc to next line
        pc = current;
    }

    if ( in_main && in_function ){
        // check function, run that sish 
    }
    else if ( in_main && ! in_function ){
        //TODO HERE IS WHERE WE START GRABBING NODES AND USING TOP3 ON THE STACK FOR EVERYTHING.
        std::cout << "here type: " << ast_types::what_is(current->type) << " value: " << current->value << std::endl;
        
        TopThree();
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
    // PRELIM tests and checks
    if ( ast_head == nullptr && ast_tail == nullptr ){
        std::cout << "The abstract syntax tree is currently empty!" << std::endl;
        exit(1);
    }
    // Setting up variables
    is_running = true;
    ast_node* current = ast_head; // not program counter? idk
    

    // Use this as simply a machine that can go through the ast_nodes incrementally
    // First it looks for main and all other functions to add their head node locations to a vector "functions"
    // Then starts executing main sequentially, bouncing to the head of each function when called.
    // When the function finishes it should jump back to the PC+ one node so the program can continue.
    // Example: in main, goHere(); then goHere() calls goThere() the pc must be copied for each function entered in case 
    // function calls another function that will need the original pc to return to. then the first function called can complete and
    // the copied original pc is called bringing the machine back into main right after the function call. when you return, inc pc
    // good(find main) -> WIP(go through main) -> todo(finishes when mains final end block is called. (level = 0 && current = end_block))
    // otherwise finishes when the ast is fully traversed.
    while ( is_running ) {
        running_counter++;
        // TRAVERSAL
        // At end of abstract syntax tree FINAL NODE
        if (current->get_next() == nullptr && current->get_chld() == nullptr){
            beginHelper(current); // FINAL NODE
            std::cout << "AST successfully traversed with final pc: " << &pc << std::endl;
            is_running = false;
            continue;
        }
        // End of expression
        else if (current->get_next() == nullptr && current->get_chld() != nullptr ){
            if ( ! in_main ){ CheckAddFunction(current); }
            
            // TODO STUFF
            // CHECK DECLARATION/BEG_BLOCK/END_BLOCK

            // Moving down on ast
            current = current->get_chld();  
        } else if (current->get_next() != nullptr && current->get_chld() == nullptr || current->get_next()->get_next() == nullptr && current->get_next()->get_chld() != nullptr ){
            // Moving right on ast ( will encounter last node down)
            // IF we are at the second to end of a left moving statement. (DO EVERYTHING TWICE)
            // FIRST NODE
            if ( current->get_next()->get_next() == nullptr && current->get_next()->get_chld() != nullptr ){

                beginHelper(current);
                current = current->get_next();
            }
            // LAST NODE IF ABOVE TRUE

            beginHelper(current);




            // Moving right now depending if we are on last or not
            if ( current->get_next() == nullptr && current->get_chld() != nullptr ) { 
                //Pushing expression stack onto the execution stack
                if ( ! expression_stack.empty() ){
                    execution_stack.push(expression_stack);
                    clearStack();
                }
                current = current->get_chld(); 
                continue;
            } else {
                current = current->get_next();
            }
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

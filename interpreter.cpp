#include "interpreter.h"


// Default constructor
Interpreter::Interpreter(){
    in_main = false;
    in_function = false;
    running_counter = 0;
    level = 0;
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
                //errors.ENC_ERROR()
            } 
        }
    }
}


// Get top three of stack and evaluates accordingly yet another dfa
void Interpreter::TopThree(int code){
    if ( expression_stack.size() < 3 ){
        return;
    }
    ast_node* one = expression_stack.top();
    expression_stack.pop();
    ast_node* two = expression_stack.top();
    expression_stack.pop();
    ast_node* three = expression_stack.top();
    expression_stack.pop();

    if ( code == ast_types::ASSIGNMENT && one->value == "="){

    }

    expression_stack.push(three);
    expression_stack.push(two);
    expression_stack.push(one);
}

/* JUST HERE FOR ME ILL DELETE
    static constexpr int BEG_BLOCK = 9999; x
    static constexpr int END_BLOCK = 9998; x
    static constexpr int RETURN = 9997;
    static constexpr int DECLARATION = 9996;
    static constexpr int ASSIGNMENT = 9995; o
    static constexpr int STATEMENT_IF = 9994;
    static constexpr int EXPRESSION_FOR = 9993;
    static constexpr int EXPRESSION_WHILE = 9992;
    static constexpr int STATEMENT_PRINTF = 9991;
    static constexpr int TOKEN = 9990;
    static constexpr int OPERATOR = 9989;
    static constexpr int ELSE = 9988;
    static constexpr int CALL = 9987;
    */
// Evaluates one node of the ast 
// Checks and adds function names, pushes tokens onto expression stack, moves pc to current line of execution, 
// calls TopThree on expression stack and keeps track where we are in program.
void Interpreter::beginHelper(ast_node* &current){
    
    // For levels? idk
    if ( ast_types::what_is(current->type) != "BEG_BLOCK" ){ level++; }
    if ( ast_types::what_is(current->type) != "END_BLOCK" ){ level--; }

    //HERE IS WHERE WE START GRABBING NODES AND USING TOP3 ON THE STACK FOR EVERYTHING.
    // MAIN 
    if ( in_main && ! in_function ){
        // TOKEN
        if ( ast_types::what_is(current->type) == "TOKEN" ){  
            expression_stack.push(current);
            std::cout << "TOKEN: " << current->value << std::endl;
            // EVAL STACK
            TopThree(type);
        } else { // NOT A TOKEN
            std::cout << "NON-TOKEN: " << ast_types::what_is(current->type) << std::endl;
            // IMPORTANT TYPE
            /*
            if ( current->type == 9997 || current->type == 9996 || current->type == 9995 || current->type == 9994 || current->type == 9993 || current->type == 9992 || 
            current->type == 9991 || current->type == 9989 || current->type == 9988 || current->type == 9997 ){  
            }
            */
            
            if ( ! expression_stack.empty() ){

            }
            // Moving pc
            pc = current; 
            // Setting type for next token block
            type = current->type;
        }
    }
    // FUNCTION
    else if ( in_main && in_function ){
        // What function we be in. 
        
    } else { // BEFORE MAIN
        CheckAddFunction(current);
    }
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
    
    // MAIN RUNTIME ENVIROMENT
    // Use this as simply a machine that can go through the ast_nodes incrementally
    // First it looks for start of main and all other functions along the way to add their head node locations to a vector "functions"
    // Then starts executing main sequentially, bouncing to the head of a selected cell/function when called such as for loops.
    // When the function finishes it should jump back to the (PC + one node) so the program can continue.
    // Example: in main, goHere(); then goHere() calls goThere() the pc must be copied for each function entered in case 
    // function calls another function that will need the original pc to return to. then the first function called can complete and
    // the copied original pc is called bringing the machine back into main right after the function call. when you return, inc pc
    // good(find all function heads) -> good(find main) -> good(go through main) -> not even close(execute statements)->
    // --> todo(finishes when mains final end block is called. (level = 0 && current = end_block))
    // otherwise finishes when the ast is fully traversed.
    // calls beginHelper in the areas it will be needed.
    while ( is_running ) {
        running_counter++;// for memory leaks
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
            beginHelper(current);
            // Moving down on ast
            current = current->get_chld();  
        } else if (current->get_next() != nullptr && current->get_chld() == nullptr || current->get_next()->get_next() == nullptr && current->get_next()->get_chld() != nullptr ){
            // Moving right on ast ( will not encounter last node down automatically)
            // IF we are at the second to end of a left to right moving statement. (DO EVERYTHING TWICE manually move to final righthandmost node)
            // 2nd to last node
            if ( current->get_next()->get_next() == nullptr && current->get_next()->get_chld() != nullptr ){
                beginHelper(current);
                // Move to last node
                current = current->get_next();
            }
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
            is_running = false;
            break;
        }   
    } 

    
    errors.STOP_SYNTAX_ERRORS();
    printEStack();
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

#include "interpreter_old.h"


// Default constructor
Interpreter::Interpreter(){
    in_main = false;
    in_function = false;
    running_counter = 0;
    level = 0;
    is_running = false; // Sets running to true in Begin
    exit_code = 0; // By default program is successful
    scope_stack.push(0); // push global scope (should never be popped)
}
Interpreter::Interpreter(ast tree, symbol_table& table, ERRORS& in_errors){
    in_main = false;
    in_function = false;
    level = 0;
    running_counter = 0;
    is_running = false; // Sets running to true in Begin
    exit_code = 0; // By default program is successful
    
    s_table = table; // Symbol table
    as_tree = tree; // AST
    errors = in_errors;
    working_print_statement = "default";
    scope_stack.push(0); // push global scope (should never be popped)
}

// This function simply returns if the givin string is a number.
bool Interpreter::isNumber(std::string str){
    int i = 0;
    bool is = true;
    while ( str[i] ){
        if ( ! isdigit(str[i]) ){
            is = false;
            break;
        }
        i++;
    }
    return is;
} 
// Returns true if the givin string is an operator else returns false
bool Interpreter::isOperator(std::string str){
    // Numerical operators
    if ( str == "+" ){
        return true;
    }
    else if ( str== "-" ){
        return true;
    }
    else if ( str == "*" ){
        return true;
    }
    else if ( str == "/" ){
        return true;
    }
    else if ( str == "%" ){
        return true;
    }
    else if ( str == "^" ){
        return true;
    }
    else if ( str == "="){
        return true;
    }

    // Relational operators
    else if ( str == ">" ){
        return true;
    }
    else if ( str == ">=" ){
        return true;
    }
    else if ( str == "<" ){
        return true;
    }
    else if ( str == "<=" ){
        return true;
    }
    else if ( str == "==" ){
        return true;
    }
    else if ( str == "!=" ){
        return true;
    }

    // Boolean operators
    else if ( str == "&&" ) {
        return true;
    }
    else if ( str == "||" ) {
        return true;
    }

    else {
        return false;
    }
}

// Clears the current expression_stack
void Interpreter::clearStack(){
    while (!expression_stack.empty()) { 
        expression_stack.pop(); 
    }
}
// Prints the current expression stack.
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






// Exit function for finishing process
void Interpreter::ExitQuiet(){
    std::cout << std::endl;
    exit(exit_code);
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
                //functions_pc.push_back(current);
                // Pushing main's scope onto scope stack
                scope_stack.push(s_table.get_function_scope(current->func_name));
            } 
            // Check symbol table for prior declaration
            else if ( s_table.find_symbol(current->func_name) && ! in_main ){
                // add function to function vector.
                std::cout << "adding function " << current->func_name << std::endl;
                functions_pc.push_back(current);
            } else {
                //ERROR prior decleration of function
                std::cout << "ERROR in CheckAddFunction: unknown function" << std::endl;
            } 
        }
    }
}


// Returns true if eval was succ
// three is pass by reference because it is updated 
void Interpreter::EvalOperatorUpdate(ast_node* one, ast_node* two, ast_node*& three){
    if ( one->value == "=" ){
        three->value = two->value;
    }
    else if ( one->value == "+" ){
        three->value = std::to_string(std::stoi(three->value) + std::stoi(two->value));
        return;
    }
    else if ( one->value == "-" ){
        three->value = std::to_string(std::stoi(three->value) - std::stoi(two->value));
        return;
    }
    else if ( one->value == "*" ){
        three->value = std::to_string(std::stoi(three->value) * std::stoi(two->value));
        return;
    }
    else if ( one->value == "/" ){
        three->value = std::to_string(std::stoi(three->value) / std::stoi(two->value));
        return;
    }
    else if ( one->value == "%" ){
        three->value = std::to_string(std::stoi(three->value) % std::stoi(two->value));
        return;
    }
    else if ( one->value == "^" ){
                                                 // note that ^ is the XOR operator in c++, not used to raise to a power
        three->value = std::to_string(std::stoi(three->value) ^ std::stoi(two->value));
        return;
    }//..
    else {
        return;
    }

}

// Evaluate non assingment expressions
ast_node* Interpreter::eval_top_three(std::string one, std::string two, std::string three) {
    ast_node* sol = new ast_node();

    // Integer
    if (one == "+") {
        sol->value = std::to_string(std::stoi(three) + std::stoi(two));
    }
    else if (one == "-") {
        sol->value = std::to_string(std::stoi(three) - std::stoi(two));
    }
    else if (one == "*") {
        sol->value = std::to_string(std::stoi(three) * std::stoi(two));
    }
    else if (one == "/") {
        sol->value = std::to_string(std::stoi(three) / std::stoi(two));
    }
    else if (one == "%") {
        sol->value = std::to_string(std::stoi(three) % std::stoi(two));
    }
    else if (one == "^") {
        sol->value = std::to_string(std::pow(std::stoi(three), std::stoi(two)));
    }

    // Relational
    else if (one == ">") {
        if (std::stoi(three) > std::stoi(two)) {
            sol->value = "TRUE";
        } else {
            sol->value = "FALSE";
        }
    }
    else if (one == ">=") {
        if (std::stoi(three) >= std::stoi(two)) {
            sol->value = "TRUE";
        } else {
            sol->value = "FALSE";
        }
    }
    else if (one == "<") {
        if (std::stoi(three) < std::stoi(two)) {
            sol->value = "TRUE";
        } else {
            sol->value = "FALSE";
        }
    }
    else if (one == "<=") {
        if (std::stoi(three) <= std::stoi(two)) {
            sol->value = "TRUE";
        } else {
            sol->value = "FALSE";
        }
    }
    else if (one == "==") {
        if (std::stoi(three) == std::stoi(two)) {
            sol->value = "TRUE";
        } else {
            sol->value = "FALSE";
        }
    }
    else if (one == "!=") {
        if (std::stoi(three) != std::stoi(two)) {
            sol->value = "TRUE";
        } else {
            sol->value = "FALSE";
        }
    }

    // Boolean
    else if (one == "&&") {
        if (two == "TRUE" && three == "TRUE") {
            sol->value = "TRUE";
        } else {
            sol->value = "FALSE";
        }
    }
    else if (one == "||") {
        if (two == "TRUE" || three == "TRUE") {
            sol->value = "TRUE";
        } else {
            sol->value = "FALSE";
        }
    }

    return sol;
}
/*
static constexpr int NA = 9999;
        static constexpr int INT = 9998;
        static constexpr int FLOAT = 9997;
        static constexpr int CHAR = 9996;
        static constexpr int BOOL = 9995;
        static constexpr int VOID = 9994;
        */
// Check if current node is a fucntion and executes said function returning the value that would be returned from said function
void Interpreter::CheckCallFunction(ast_node* &current){
    ast_node *to_be_pushed = current;
    

    // Check if token is function call
    for (auto f : functions_pc)
    {
        // If this node is calling a function
        if (f->func_name == current->value)
        {
            
            //function_type = current->type;
            in_function = true;
            int func_scope = s_table.get_function_scope(f->func_name);
            symbol_node *func_symbol = s_table.get_symbol(f->func_name, func_scope);
            ast_node *fcurrent = current;

            std::cout << "I AM A FUNCTION CALL: " << current->value << ", returns: " << symbols::data_types::get_type(func_symbol->DATATYPE) << ", WITH PARAMETERS: ";
            fcurrent = fcurrent->get_next()->get_next(); // skip open parenthesis

            // Get parameters and prints them
            while (fcurrent != nullptr && fcurrent->value != ")")
            {
                std::cout << fcurrent->value << " ";
                fcurrent = fcurrent->get_next();
            }
            std::cout << std::endl;

            //std::cout << fcurrent->value << std::endl;
            fcurrent = fcurrent->get_next(); // Moving past the final )
            //std::cout << fcurrent->value << std::endl;
            

            // Todo:
            //  set value for parameters for function in symbol table
            //  run function(wip)
            //  set to_be_pushed to returned value (if any)(wip)

            // Moving current to function here
            //TEMP JUST GRABBING LAST PUSHED FUNCTION HEAD NODE
            current = functions_pc[functions_pc.size()-1];
            //std::cout << "current set to " << current->value << std::endl;
            // This is the setting main pc back to end of parenthesis
            pc = fcurrent;
            //std::cout << "pc set to " << pc->value << std::endl;
            // Adding the function into the scope stack

            // TODO FIX WHY THIS IS SEGFAULTING WHEN TRYING TO GET TOKENS WHEN IN FUCNTION
            //scope_stack.push(func_scope);

            break;
        }
    }

    expression_stack.push(to_be_pushed);

}


// Called for each token, evaluates current stack if able
// EX:
// FULL EXPRESSION: counter counter 1 + =
// NON-TOKEN: ASSIGNMENT
// START
// counter
// counter
// 1
// (evaluating counter counter 1)
// (not valid 3 adding next)
// +
// (evaluating counter 1 +)
// fixing stack:
// counter
// updated_counter
// (adding final token)
// =
// (evaluating counter updated_counter =)
// (updating counter to updated_counter)
// (updating stack to empty)
// Finished ASSIGNMENT block
void Interpreter::TopThree(int code){
    // Check if stack is empty
    if ( expression_stack.empty() ){
        return;
    }   
    
    // Yea just a little off the top
    ast_node* one = expression_stack.top();
    expression_stack.pop();
       

    // Depending on the type of block we are in, we need to treat the stack accordingly.
    // This giant dfa asseses how the stack will be evaluated.
    // It might only use one node from the stack or it might need the entire stack

    // ASSIGNMENT
    if ( code == ast_types::ASSIGNMENT ){
        // Ensure there are enough tokens on the stack
        if ( expression_stack.size() < 2 ){
            expression_stack.push(one);
            return;
        }

        // Get tokens from stack
        ast_node* two = expression_stack.top();
        expression_stack.pop();
        ast_node* three = expression_stack.top();
        expression_stack.pop();

        std::cout << "Evaluating " << three->value << " " << two->value << " " << one->value << std::endl;

        // Invalid order, must be operator operand operand
        if ( ! isOperator(one->value) || isOperator(two->value) || isOperator(three->value) ){
            // Resetting stack
            expression_stack.push(three);
            expression_stack.push(two);
            expression_stack.push(one);
            return;
        }
        
        // Handles assignment
        if (one->value == "=") {
            std::cout << "TopThree() Assign: " << one->value << " " << two->value << " " << three->value << std::endl;

            symbol_node* s_two = s_table.get_symbol(two->value, scope_stack.top());
            symbol_node* s_three = s_table.get_symbol(three->value, scope_stack.top());

            if (isNumber(two->value)) {
                std::cout << "Setting " << three->value << " = " << two->value << " (numerical)\n";
                s_three->set_val_int(std::stoi(two->value));
            }
            else if (two->value == "TRUE" || two->value == "FALSE") {
                std::cout << "Setting " << three->value << " = " << two->value << " (boolean)\n";
                s_three->set_val_bool(two->value == "TRUE");
            }
            else if (s_two != nullptr) {
                std::cout << "Setting " << three->value << " = " << two->value << " from symbol table\n";
                switch (s_two->DATATYPE) {
                    case symbols::data_types::INT: s_three->set_val_int(s_two->get_val_int());
                    case symbols::data_types::BOOL: s_three->set_val_bool(s_two->get_val_bool());
                    case symbols::data_types::CHAR: s_three->set_val_char(s_two->get_val_char());
                }
            } else {
                std::cout << "ERROR in TopThree(): Assingnment values could not be assessed\n";
            }
            return;
        }

        // Handles operations
        else if (isOperator(one->value)) {
            std::string two_val = two->value;
            std::string three_val = three->value;

            
            symbol_node* s_two = s_table.get_symbol(two->value, scope_stack.top());
            symbol_node* s_three = s_table.get_symbol(three->value, scope_stack.top());

            // get value of two from symbol table
            if (s_two != nullptr) {
                switch (s_two->DATATYPE) {
                    case symbols::data_types::INT: two_val = std::to_string(s_two->get_val_int()); break;
                    case symbols::data_types::BOOL: two_val = std::to_string(s_two->get_val_bool()); break;
                    case symbols::data_types::CHAR: two_val = std::to_string(s_two->get_val_char()); break;
                }
            }

            // get value of three from symbol table
            if (s_three != nullptr) {
                switch (s_three->DATATYPE) {
                    case symbols::data_types::INT: three_val = std::to_string(s_three->get_val_int()); break;
                    case symbols::data_types::BOOL: three_val = std::to_string(s_three->get_val_bool()); break;
                    case symbols::data_types::CHAR: three_val = std::to_string(s_three->get_val_char()); break;
                }
            }

            std::cout << "TopThree() 123 Evaluate: " << one->value << " " << two_val << " " << three_val << std::endl;
            // TODO segfaulting when in function 
            expression_stack.push(eval_top_three(one->value, two_val, three_val));
            
            return;
        }

        std::cout << "ERROR in TopThree(): Assignment not handled\n";
        expression_stack.push(three);
        expression_stack.push(two);
        expression_stack.push(one);
        return;
        //ERROR
    } 
    // PRINTF
    else if ( code == ast_types::STATEMENT_PRINTF ){
        // First node so retrive print statement
        if ( expression_stack.size() == 0 && working_print_statement == "default"){
            working_print_statement = one->value;
        }

        // Final node of printf statement
        if ( one->get_next() == nullptr && one->get_chld() != nullptr ){
            ast_node* temp = one;
            // Need to pop and insert values into the back of the print statement.
            // (munching on the stack)
            int substringpos;
            while ( expression_stack.size() > 0 ){
                // TODO NEED TO EVALUATE NON LITERAL (NOT JUST HERE) function needed
                // 1. get datatype from st 2. set value to temp for required datatype 3. put that selected value into the replace statement.
                // Putting temp into the lefthandmost substring in the statement
                substringpos = working_print_statement.rfind("%d");
                // Replace substring with appropiate value from stack
                if ( substringpos != std::string::npos ) {
                    symbol_node *temp_symbol = s_table.get_symbol(temp->value, scope_stack.top());
                    if (temp_symbol != nullptr) { // if symbol is in symbol table
                        working_print_statement.replace(substringpos, 2, std::to_string(temp_symbol->get_val_int()));
                    } else {
                        // probably should error, but leaving it for now since it's not vital to program execution
                        working_print_statement.replace(substringpos, 2, temp->value);
                    }
                    //std::cout << "Replacing " << substringpos << " w/ " << temp->value << " temporarily..." << std::endl;
                }
                // Grabbing top of stack after removing previously evaluated item.
                temp = expression_stack.top();
                expression_stack.pop();
            }

            // replace \n with newline
            int new_line_pos = working_print_statement.rfind("\\n");
            while (new_line_pos != std::string::npos) {
                working_print_statement.replace(new_line_pos, 2, "\n");
                new_line_pos = working_print_statement.rfind("\\n");
            }

            // Actually printing what the program would print
            std::cout << "PROGRAM OUTPUT: " << working_print_statement;
            
            working_print_statement = "default";
            return;
        } 
    }
    // DECLARATION
    else if ( code == ast_types::DECLARATION ){
        //not sure yet
    }
    // RETURN
    else if ( code == ast_types::RETURN ){
        if ( in_function ){
            // TODO
            // Check how deep we are in functions
            // move pc to last location before this function was called
            // update function scope stack
            // If we are returning back to main set in_function to false
            // Check if we are returning a value and make sure to retreive that
            // Possibly make a function that will return the fucntions return value
        } else {
            // should have previously caught this in beginHelper
            std::cout << "ERROR in Interpreter::TopThree: returning from main " << std::endl;
        }
    }
    // CALL
    else if ( code == ast_types::CALL ){
        //TODO
        // not even sure brother
        // Call function?
    }
    // OPERATOR
    else if ( code == ast_types::OPERATOR ){
        //TODO
        // Not even the slightest clue what this is for
    }
    // IF STATEMENT
    else if ( code == ast_types::STATEMENT_IF ){
        //TODO
        // Eval top three if true continue onto next boolean top three, if false move to next block
        // if all true execute begin block to end block

    }
    // ELSE STATEMENT
    else if ( code == ast_types::ELSE ){
        //TODO
        // if past if or else if statement failed exeucte this code block
        // else move to next block
        // (might need to look for next STATEMENT_IF depending how the internal system is)
    }
    // FOR LOOP
    else if ( code == ast_types::EXPRESSION_FOR ){
        //TODO
        // Evaluate entire top of for loop
        // If true execute block of code and once finished go back to top
        // not sure why begin block isnt printing after the expression_for there is something weird with the order in which the tokens are being output
        // Inc variable and check again
        // Repeat until boolean expression fails
    }
    // WHILE LOOP
    else if ( code == ast_types::EXPRESSION_WHILE ){
        //TODO
        // Evaluate entire top of while loop
        // If true execute code block and once finished go back to top
        // Re evaluate top expression
        // Repeat until evaultating the top expression returns false
    }
    else {
        std::cout << "ERROR in Interpreter::TopThree: code not found" << std::endl;
    }
    
    expression_stack.push(one);
   
}


// Evaluates one node of the ast 
// Checks and adds function names, pushes tokens onto expression stack, moves pc to current line of execution, 
// calls TopThree on expression stack and keeps track where we are in program.
void Interpreter::beginHelper(ast_node* &current){
    // For levels? idk
    if ( ast_types::what_is(current->type) == "BEG_BLOCK" ) { level++; }
    if ( ast_types::what_is(current->type) == "END_BLOCK" ) { level--; }

    //HERE IS WHERE WE START GRABBING NODES AND USING TOP3 ON THE STACK FOR EVERYTHING.
    // MAIN 
    if ( in_main && ! in_function ){
        // TOKEN
        //std::cout << "here" << std::endl;
        
        if ( ast_types::what_is(current->type) == "TOKEN" ){
            std::cout << "TOKEN: " << current->value << std::endl;
            //ast_node* to_be_pushed = current;

            // Check if token is function call
            CheckCallFunction(current);

            //expression_stack.push(to_be_pushed);
            
            TopThree(type);
        } 
        // NOT A TOKEN
        else { 
            // PROGRAM EXECUTION FINISHED
            if ( ast_types::what_is(current->type) == "END_BLOCK" || ast_types::what_is(current->type) == "RETURN" && level == 0 && expression_stack.empty() && ! in_function){
                // Checking for errors reported
                errors.STOP_SYNTAX_ERRORS();
                // No errors
                ExitQuiet();
            }
            // If function returned
            if ( ast_types::what_is(pc->type) == "TOKEN" ){
                std::cout << "here" << std::endl;
                TopThree(function_type);
            }
            
            // If we are still executing stack
            std::cout << "Finished " << ast_types::what_is(pc->type) << " block" << std::endl;
            std::cout << "NON-TOKEN: " << ast_types::what_is(current->type) << std::endl;
            // Next line so clear the stack
            if ( level == 1 ){ clearStack(); }
            // Moving pc
            pc = current; 
            // Setting type for next token block
            type = current->type;
        }
    }
    // FUNCTION (WIP)
    else if ( in_main && in_function ){
        // TOKEN
        if ( ast_types::what_is(current->type) == "TOKEN" ){
            std::cout << "FUNCTION TOKEN: " << current->value << std::endl;

            // Check if token is function call
            CheckCallFunction(current);
            
            TopThree(function_type);
        } 
        // NOT A TOKEN
        else { 
            // Moving the function head along with the program.
            functions_pc[functions_pc.size()-1] = current; 
            // FUNCTION EXECUTION FINISHED
            if ( ast_types::what_is(current->type) == "END_BLOCK" || ast_types::what_is(current->type) == "RETURN" && function_expression_stack.empty() && in_function){
                // check function scope
                // RETURN BACK TO THE specific PCma
                //std::cout << scope_stack.size() << std::endl;
                //std::cout << functions_pc.size() << std::endl;
                //std::cout << function_expression_stack.size() << std::endl;
                //std::cout << ast_types::what_is(functions_pc[functions_pc.size()-1]->type) << std::endl;
                // TEMPORARY ONE FUNCTION ONLY
                // 2 becuase not pushing fuctions on the scope stack
                if ( scope_stack.size() == 2 ){
                    TopThree(function_type);
                    in_function = false;
                    // pushing the final value from function into the expression stack
                    //expression_stack.push(functions_pc[functions_pc.size()-1]); // needs to figure out what function first
                    expression_stack.push(pc);//temp
                    //Popping current exiting function from scope stack?
                    scope_stack.pop();
                    // moving back into where we were before.
                    current = pc;
                    std::cout << "about to return to " << current->value << " : " << ast_types::what_is(current->type) << std::endl;
                    return;
                } else {
                    std::cout << "NOT HANDLED exit in function in function " << std::endl;
                }
                
                
            }
            // If we are still executing stack
            std::cout << "Finished " << ast_types::what_is(current->type) << " block in function" << std::endl;
            std::cout << "FUNCTION NON-TOKEN: " << ast_types::what_is(current->type) << std::endl;

            function_type = current->type;
            // Next line so clear the stack
            //if ( level == 1 ){ clearStack(); }
            // Moving pc
            //functions_pc[functions_pc.size()-1] = current; 
            // Setting type for next token block
            //
        }
    } 
    // BEFORE MAIN
    else { 
        CheckAddFunction(current);
    }
}


// Maintain a stack data structure and a program counter to keep track of the flow-of-control as the function interprets and executes an input program.
// Needs to alter the symbol_table
void Interpreter::Begin(){
    // Setting up variables
    is_running = true;
    //ast_node* current = ast_head; // not program counter? idk
    ast_node* current = as_tree.get_head();
    // MAIN RUNTIME ENVIROMENT
    // Use this as simply a machine that can go through the ast_nodes incrementally but can change where in the program it incrememnts from
    // First it looks for start of main and all other functions along the way to add their head node locations to a vector "functions"
    // Then starts executing main sequentially, bouncing to the head of a selected cell/function when called such as for loops.
    // When the function finishes it should jump back to the (PC + one node) so the program can continue.
    // Example: in main, goHere(); then goHere() calls goThere() the pc must be copied for each function entered in case 
    // function calls another function that will need the original pc to return to. then the first function called can complete and
    // the copied original pc is called bringing the machine back into main right after the function call. when you return, inc pc
    // otherwise finishes when the ast is fully traversed.
    // calls beginHelper in the areas it will be needed.
    while ( is_running ) {
        running_counter++;// for memory leaks
        // TRAVERSAL
        // At end of abstract syntax tree FINAL NODE
        if (current->get_next() == nullptr && current->get_chld() == nullptr){
            beginHelper(current); // FINAL NODE
            std::cout << "AST successfully traversed before execution completed, with final pc: " << &pc << std::endl;
            is_running = false;
            break;
        }
        // End of expression
        else if (current->get_next() == nullptr && current->get_chld() != nullptr ){
            beginHelper(current);
            // Moving down on ast
            current = current->get_chld();  
        } 
        else if (current->get_next() != nullptr && current->get_chld() == nullptr || current->get_next()->get_next() == nullptr && current->get_next()->get_chld() != nullptr ){
            // Moving right on ast ( will not encounter last node down automatically)
            // IF we are at the second to end of a left to right moving statement. (DO EVERYTHING TWIC, manually move to final righthandmost node)
            // 2nd to last node
            if ( current->get_next()->get_next() == nullptr && current->get_next()->get_chld() != nullptr ){
                beginHelper(current);
                // Move to last node
                current = current->get_next();
            }
            beginHelper(current);
            // Moving right now depending if we are on last or not
            if ( current->get_next() == nullptr && current->get_chld() != nullptr ) { 
                current = current->get_chld(); 
                continue;
            } else {
                current = current->get_next();
            }
        }
        // If we are stuck in an infinite loop
        if ( running_counter > 9999 ){
            std::cout << "ERROR in Interpreter::Begin: Stuck in infinitely running loop! Exiting..." << std::endl;
            clearStack();
            exit_code = 1;
            is_running = false;
            break;
        }   
    }
    // printEStack();
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

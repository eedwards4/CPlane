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
    s_table = table; // Symbol table
    as_tree = tree; // AST
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
    ast_node* current = nullptr;
    if ( expression_stack.empty() ){
        return;
    }

    // Reverse the stack
    // TODO: CHANGE STACK TO ANOTHER DATATYPE SO WE AREN'T ACCESSING IN REVERSE, THIS IS GONNA CAUSE RUNTIME SLOWDOWN
    std::stack<ast_node*> eval_stack;
    while ( ! expression_stack.empty() ){
        eval_stack.push(expression_stack.top());
        expression_stack.pop();
    }
    expression_stack = eval_stack;
    eval_stack = std::stack<ast_node*>(); // Clearing eval stack

    // While there are elements in the stack
    while ( !expression_stack.empty() ){
        //TODO: need to go through elements, order them correctly
        //TODO: need to parse ordered list of elements and "execute them"
        // Get the first element of the stack
        current = expression_stack.top();
        expression_stack.pop();
        switch ( current->type ){
            case ast_types::ASSIGNMENT:
                break;
            case ast_types::DECLARATION:
                break;
            case ast_types::CALL:
                break;
        }
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

    // Check if the ast is not initialized
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
            if ( ! expression_stack.empty() ){
                std::cout << "Stack: ";
                printStack(expression_stack);
                std::cout << std::endl;
            }

            EvaluateStack();
            clearStack();
            
            current = current->get_chld();
        }
        // Grabbing the next node of AST
        else{
            std::cout << "type: " << ast_types::what_is(current->type) << std::endl;

            current = current->get_next();
            expression_stack.push(current);
        }

        
        // If we are stuck in an infinite loop
        if ( program_counter > 999 ){
            std::cout << "ERROR in Interpreter::Begin: Stuck in infinite loop! Exiting..." << std::endl;
            clearStack();
            exit_code = 1;
            is_running = false;
            break;
        }
        /* PLEASE DON'T DELETE, THIS CODE IS UNFORTUNATELY STILL IN USE, JUST COMMENTED SO IT DOESN'T BREAK SHIT
        std::string s1 = "1";
        std::string s2 = "2";
        char c1 = '1';
        char c2 = '2';
        int i1 = 1;
        int i2 = 2;
        i1 += i2; // Int and int ----------------------------
        i1 += s1; // Int and string
        i1 += c1; // Int and char
        s1 += s2; // String and string ----------------------------
        s1 += c1; // String and char
        s1 += i1; // String and int
        c1 += c2; // Char and char ----------------------------
        c1 += i1; // Char and int
        c1 += s1; // Char and string
        
         * Operators
         * Single operators
         * + - * / % !
         * Bitwise
         * & | ^ ~ << >>
         * Compound operators
         * ++ -- += -= *= /= %= &= |= ^= <<= >>=
         * Comparison
         * == != > < >= <=
         * Logical
         * && ||
         * Ternary
         * ? :
         * Assignment
         * PLEASE DON'T DELETE, THIS CODE IS UNFORTUNATELY STILL IN USE, JUST COMMENTED SO IT DOESN'T BREAK SHIT
         */
    }
}

// Functions for eval
template <typename T, typename U> // +
auto Interpreter::add(T a, U b) -> typename std::enable_if<(std::is_arithmetic<T>::value && std::is_arithmetic<U>::value), decltype(a + b)>::type {
    return a + b;
}
template <typename T, typename U> // -
auto Interpreter::sub(T a, U b) -> typename std::enable_if<(std::is_arithmetic<T>::value && std::is_arithmetic<U>::value), decltype(a - b)>::type {
    return a - b;
}
template <typename T, typename U> // *
auto Interpreter::mul(T a, U b) -> typename std::enable_if<(std::is_arithmetic<T>::value && std::is_arithmetic<U>::value), decltype(a * b)>::type {
    return a * b;
}
template <typename T, typename U> // /
auto Interpreter::div(T a, U b) -> typename std::enable_if<(std::is_arithmetic<T>::value && std::is_arithmetic<U>::value), decltype(a / b)>::type {
    return a / b;
}
template <typename T, typename U> // %
auto Interpreter::mod(T a, U b) -> typename std::enable_if<(std::is_arithmetic<T>::value && std::is_arithmetic<U>::value), decltype(a % b)>::type {
    return a % b;
}
template <typename T, typename U> // >
auto Interpreter::grt(T a, U b) -> typename std::enable_if<(std::is_arithmetic<T>::value && std::is_arithmetic<U>::value), decltype(a > b)>::type {
    return a > b;
}
template <typename T, typename U> // <
auto Interpreter::lst(T a, U b) -> typename std::enable_if<(std::is_arithmetic<T>::value && std::is_arithmetic<U>::value), decltype(a < b)>::type {
    return a < b;
}
template<typename T> // !
auto Interpreter::op_not(T a) -> typename std::enable_if<std::is_same<T, bool>::value, decltype(!a)>::type {
    return !a;
}
template<typename T> // ++
auto Interpreter::inc(T a) -> typename std::enable_if<std::is_arithmetic<T>::value, decltype(a + 1)>::type {
    return a + 1;
}
template<typename T> // --
auto Interpreter::dec(T a) -> typename std::enable_if<std::is_arithmetic<T>::value, decltype(a - 1)>::type {
    return a - 1;
}
template<typename T, typename U> // +=
auto Interpreter::add_equ(T a, U b) -> typename std::enable_if<std::is_arithmetic<T>::value && std::is_arithmetic<U>::value, decltype(a + b)>::type {
    return a + b;
}
template<typename T, typename U> // -=
auto Interpreter::sub_equ(T a, U b) -> typename std::enable_if<std::is_arithmetic<T>::value && std::is_arithmetic<U>::value, decltype(a - b)>::type {
    return a - b;
}
template<typename T, typename U> // *=
auto Interpreter::mul_equ(T a, U b) -> typename std::enable_if<std::is_arithmetic<T>::value && std::is_arithmetic<U>::value, decltype(a * b)>::type {
    return a * b;
}
template<typename T, typename U> // /=
auto Interpreter::div_equ(T a, U b) -> typename std::enable_if<std::is_arithmetic<T>::value && std::is_arithmetic<U>::value, decltype(a / b)>::type {
    return a / b;
}
template<typename T, typename U> // %=
auto Interpreter::mod_equ(T a, U b) -> typename std::enable_if<std::is_arithmetic<T>::value && std::is_arithmetic<U>::value, decltype(a % b)>::type {
    return a % b;
}
template<typename T, typename U> // ==
auto Interpreter::equ(T a, U b) -> typename std::enable_if<std::is_same<T, U>::value, decltype(a == b)>::type {
    return a == b;
}
template<typename T, typename U> // !=
auto Interpreter::not_equ(T a, U b) -> typename std::enable_if<std::is_same<T, U>::value, decltype(a != b)>::type {
    return a != b;
}
template<typename T, typename U> // >=
auto Interpreter::grt_equ(T a, U b) -> typename std::enable_if<std::is_arithmetic<T>::value && std::is_arithmetic<U>::value, decltype(a >= b)>::type {
    return a >= b;
}
template<typename T, typename U> // <=
auto Interpreter::lst_equ(T a, U b) -> typename std::enable_if<std::is_arithmetic<T>::value && std::is_arithmetic<U>::value, decltype(a <= b)>::type {
    return a <= b;
}
template<typename T, typename U> // &&
auto Interpreter::and_op(T a, U b) -> typename std::enable_if<(std::is_same<T, bool>::value && std::is_same<U, bool>::value), decltype(a && b)>::type {
    return a && b;
}
template<typename T, typename U> // ||
auto Interpreter::or_op(T a, U b) -> typename std::enable_if<(std::is_same<T, bool>::value && std::is_same<U, bool>::value), decltype(a || b)>::type {
    return a || b;
}

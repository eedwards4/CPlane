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
    Interpreter(ast tree, symbol_table& table);

    // Runs the program internally
    void Begin();
    // Not sure yet but maybe step through the program incrementally with keyboard
    void BeginDebug();
    int Exit();
  
private:
    // Variables
    bool is_running;
    int program_counter;
    int exit_code;

    std::stack<ast_node*> expression_stack; // stack for holding single nodes
    std::stack<std::stack<ast_node*>> runtime_stack; // stack for holding stacks of single nodes
    
    ast as_tree;
    ast_node *ast_head; // For reading from
    ast_node *ast_tail; // For reading from
    symbol_table s_table; // For reading/writing into

    void clearStack();
    void printStack(std::stack<ast_node*> runtime_stack);
    void EvaluateStack();

    // Functions for eval
    // TODO: GO THROUGH THIS PILE OF GARBAGE AND ADD ALL VALID DATATYPE COMBOS :vomit:
    template <typename T, typename U> // +
    auto add(T a, U b) -> typename std::enable_if<(std::is_arithmetic<T>::value && std::is_arithmetic<U>::value), decltype(a + b)>::type;
    template <typename T, typename U> // -
    auto sub(T a, U b) -> typename std::enable_if<(std::is_arithmetic<T>::value && std::is_arithmetic<U>::value), decltype(a - b)>::type;
    template <typename T, typename U> // *
    auto mul(T a, U b) -> typename std::enable_if<(std::is_arithmetic<T>::value && std::is_arithmetic<U>::value), decltype(a * b)>::type;
    template <typename T, typename U> // /
    auto div(T a, U b) -> typename std::enable_if<(std::is_arithmetic<T>::value && std::is_arithmetic<U>::value), decltype(a / b)>::type;
    template <typename T, typename U> // %
    auto mod(T a, U b) -> typename std::enable_if<(std::is_arithmetic<T>::value && std::is_arithmetic<U>::value), decltype(a % b)>::type;
    // = not handled here
    template <typename T, typename U> // >
    auto grt(T a, U b) -> typename std::enable_if<(std::is_arithmetic<T>::value && std::is_arithmetic<U>::value), decltype(a > b)>::type;
    template <typename T, typename U> // <
    auto lst(T a, U b) -> typename std::enable_if<(std::is_arithmetic<T>::value && std::is_arithmetic<U>::value), decltype(a < b)>::type;
    template<typename T> // !
    auto op_not(T a) -> typename std::enable_if<std::is_same<T, bool>::value, decltype(!a)>::type;
    // TODO: ADD BITWISE OPERATORS (AND [&], OR [|], XOR [^], NOT [~], SHIFT [<<, >>])
    template<typename T> // ++
    auto inc(T a) -> typename std::enable_if<std::is_arithmetic<T>::value, decltype(a + 1)>::type;
    template<typename T> // --
    auto dec(T a) -> typename std::enable_if<std::is_arithmetic<T>::value, decltype(a - 1)>::type;
    template<typename T, typename U> // +=
    auto add_equ(T a, U b) -> typename std::enable_if<std::is_arithmetic<T>::value && std::is_arithmetic<U>::value, decltype(a + b)>::type;
    template<typename T, typename U> // -=
    auto sub_equ(T a, U b) -> typename std::enable_if<std::is_arithmetic<T>::value && std::is_arithmetic<U>::value, decltype(a - b)>::type;
    template<typename T, typename U> // *=
    auto mul_equ(T a, U b) -> typename std::enable_if<std::is_arithmetic<T>::value && std::is_arithmetic<U>::value, decltype(a * b)>::type;
    template<typename T, typename U> // /=
    auto div_equ(T a, U b) -> typename std::enable_if<std::is_arithmetic<T>::value && std::is_arithmetic<U>::value, decltype(a / b)>::type;
    template<typename T, typename U> // %=
    auto mod_equ(T a, U b) -> typename std::enable_if<std::is_arithmetic<T>::value && std::is_arithmetic<U>::value, decltype(a % b)>::type;
    // TODO: ADD LSHIFT_EQUALS AND RSHIFT_EQUALS
    template<typename T, typename U> // ==
    auto equ(T a, U b) -> typename std::enable_if<std::is_same<T, U>::value, decltype(a == b)>::type;
    template<typename T, typename U> // !=
    auto not_equ(T a, U b) -> typename std::enable_if<std::is_same<T, U>::value, decltype(a != b)>::type;
    template<typename T, typename U> // >=
    auto grt_equ(T a, U b) -> typename std::enable_if<std::is_arithmetic<T>::value && std::is_arithmetic<U>::value, decltype(a >= b)>::type;
    template<typename T, typename U> // <=
    auto lst_equ(T a, U b) -> typename std::enable_if<std::is_arithmetic<T>::value && std::is_arithmetic<U>::value, decltype(a <= b)>::type;
    template<typename T, typename U> // &&
    auto and_op(T a, U b) -> typename std::enable_if<(std::is_same<T, bool>::value && std::is_same<U, bool>::value), decltype(a && b)>::type;
    template<typename T, typename U> // ||
    auto or_op(T a, U b) -> typename std::enable_if<(std::is_same<T, bool>::value && std::is_same<U, bool>::value), decltype(a || b)>::type;
    // Boy I hope I got all of them

};  




#endif //CPLANE_INTERPRETER_H

//
// Created by evane on 11/14/2024.
//

#ifndef CPLANE_EVAL_H
#define CPLANE_EVAL_H

#include <type_traits>

class eval {
public:
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
};


#endif //CPLANE_EVAL_H

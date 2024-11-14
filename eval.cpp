//
// Created by Ethan Edwards on 11/14/2024.
//

#include "eval.h"

template <typename T, typename U> // +
auto eval::add(T a, U b) -> typename std::enable_if<(std::is_arithmetic<T>::value && std::is_arithmetic<U>::value) ||
        (std::is_same<T, int>::value && std::is_same<U, char>::value) ||
        (std::is_same<T, std::string>::value && std::is_same<U, std::string>::value) ||
        (std::is_same<T, std::string>::value && std::is_same<U, char>::value) ||
        (std::is_same<T, char>::value && std::is_same<U, char>::value) ||
        (std::is_same<T, char>::value && std::is_same<U, int>::value) ||
        (std::is_same<T, char>::value && std::is_same<U, std::string>::value), decltype(a + b)>::type {
    return a + b;
}

template <typename T, typename U> // -
auto eval::sub(T a, U b) -> typename std::enable_if<(std::is_arithmetic<T>::value && std::is_arithmetic<U>::value) ||
        (std::is_same<T, int>::value && std::is_same<U, char>::value) ||
        (std::is_same<T, char>::value && std::is_same<U, char>::value) ||
        (std::is_same<T, char>::value && std::is_same<U, int>::value), decltype(a - b)>::type {
    return a - b;
}

template <typename T, typename U> // *
auto eval::mul(T a, U b) -> typename std::enable_if<(std::is_arithmetic<T>::value && std::is_arithmetic<U>::value) ||
        (std::is_same<T, int>::value && std::is_same<U, char>::value) ||
        (std::is_same<T, char>::value && std::is_same<U, char>::value) ||
        (std::is_same<T, char>::value && std::is_same<U, int>::value), decltype(a * b)>::type {
    return a * b;
}

template <typename T, typename U> // /
auto eval::div(T a, U b) -> typename std::enable_if<(std::is_arithmetic<T>::value && std::is_arithmetic<U>::value) ||
        (std::is_same<T, int>::value && std::is_same<U, char>::value) ||
        (std::is_same<T, char>::value && std::is_same<U, char>::value) ||
        (std::is_same<T, char>::value && std::is_same<U, int>::value), decltype(a / b)>::type {
    return a / b;
}

template <typename T, typename U> // %
auto eval::mod(T a, U b) -> typename std::enable_if<(std::is_arithmetic<T>::value && std::is_arithmetic<U>::value) ||
        (std::is_same<T, int>::value && std::is_same<U, char>::value) ||
        (std::is_same<T, char>::value && std::is_same<U, char>::value) ||
        (std::is_same<T, char>::value && std::is_same<U, int>::value), decltype(a % b)>::type {
    return a % b;
}

template <typename T, typename U> // >
auto eval::grt(T a, U b) -> typename std::enable_if<(std::is_arithmetic<T>::value && std::is_arithmetic<U>::value) ||
        (std::is_same<T, int>::value && std::is_same<U, char>::value) ||
        (std::is_same<T, std::string>::value && std::is_same<U, std::string>::value) ||
        (std::is_same<T, char>::value && std::is_same<U, char>::value) ||
        (std::is_same<T, char>::value && std::is_same<U, int>::value), decltype(a > b)>::type {
    return a > b;
}

template <typename T, typename U> // <
auto eval::lst(T a, U b) -> typename std::enable_if<(std::is_arithmetic<T>::value && std::is_arithmetic<U>::value) ||
        (std::is_same<T, int>::value && std::is_same<U, char>::value) ||
        (std::is_same<T, std::string>::value && std::is_same<U, std::string>::value) ||
        (std::is_same<T, char>::value && std::is_same<U, char>::value) ||
        (std::is_same<T, char>::value && std::is_same<U, int>::value), decltype(a < b)>::type {
    return a < b;
}

template<typename T> // !
auto eval::op_not(T a) -> typename std::enable_if<std::is_same<T, bool>::value, decltype(!a)>::type {
    return !a;
}

template<typename T> // ++
auto eval::inc(T a) -> typename std::enable_if<std::is_arithmetic<T>::value, decltype(a + 1)>::type {
    return a + 1;
}

template<typename T> // --
auto eval::dec(T a) -> typename std::enable_if<std::is_arithmetic<T>::value, decltype(a - 1)>::type {
    return a - 1;
}

template<typename T, typename U> // +=
auto eval::add_equ(T a, U b) -> typename std::enable_if<(std::is_arithmetic<T>::value && std::is_arithmetic<U>::value) ||
        (std::is_same<T, int>::value && std::is_same<U, char>::value) ||
        (std::is_same<T, std::string>::value && std::is_same<U, std::string>::value) ||
        (std::is_same<T, std::string>::value && std::is_same<U, char>::value) ||
        (std::is_same<T, std::string>::value && std::is_same<U, int>::value) ||
        (std::is_same<T, char>::value && std::is_same<U, char>::value) ||
        (std::is_same<T, char>::value && std::is_same<U, int>::value), decltype(a + b)>::type {
    return a + b;
}

template<typename T, typename U> // -=
auto eval::sub_equ(T a, U b) -> typename std::enable_if<(std::is_arithmetic<T>::value && std::is_arithmetic<U>::value) ||
        (std::is_same<T, int>::value && std::is_same<U, char>::value) ||
        (std::is_same<T, char>::value && std::is_same<U, char>::value) ||
        (std::is_same<T, char>::value && std::is_same<U, int>::value), decltype(a - b)>::type {
    return a - b;
}

template<typename T, typename U> // *=
auto eval::mul_equ(T a, U b) -> typename std::enable_if<(std::is_arithmetic<T>::value && std::is_arithmetic<U>::value) ||
        (std::is_same<T, int>::value && std::is_same<U, char>::value) ||
        (std::is_same<T, char>::value && std::is_same<U, char>::value) ||
        (std::is_same<T, char>::value && std::is_same<U, int>::value), decltype(a * b)>::type {
    return a * b;
}

template<typename T, typename U> // /=
auto eval::div_equ(T a, U b) -> typename std::enable_if<(std::is_arithmetic<T>::value && std::is_arithmetic<U>::value) ||
        (std::is_same<T, int>::value && std::is_same<U, char>::value) ||
        (std::is_same<T, char>::value && std::is_same<U, char>::value) ||
        (std::is_same<T, char>::value && std::is_same<U, int>::value), decltype(a / b)>::type {
    return a / b;
}

template<typename T, typename U> // %=
auto eval::mod_equ(T a, U b) -> typename std::enable_if<(std::is_arithmetic<T>::value && std::is_arithmetic<U>::value) ||
        (std::is_same<T, int>::value && std::is_same<U, char>::value) ||
        (std::is_same<T, char>::value && std::is_same<U, char>::value) ||
        (std::is_same<T, char>::value && std::is_same<U, int>::value), decltype(a % b)>::type {
    return a % b;
}

template<typename T, typename U> // ==
auto eval::equ(T a, U b) -> typename std::enable_if<std::is_same<T, U>::value, decltype(a == b)>::type {
    return a == b;
}

template<typename T, typename U> // !=
auto eval::not_equ(T a, U b) -> typename std::enable_if<std::is_same<T, U>::value, decltype(a != b)>::type {
    return a != b;
}

template<typename T, typename U> // >=
auto eval::grt_equ(T a, U b) -> typename std::enable_if<(std::is_arithmetic<T>::value && std::is_arithmetic<U>::value) ||
        (std::is_same<T, int>::value && std::is_same<U, char>::value) ||
        (std::is_same<T, std::string>::value && std::is_same<U, std::string>::value) ||
        (std::is_same<T, char>::value && std::is_same<U, char>::value) ||
        (std::is_same<T, char>::value && std::is_same<U, int>::value), decltype(a >= b)>::type {
    return a >= b;
}

template<typename T, typename U> // <=
auto eval::lst_equ(T a, U b) -> typename std::enable_if<(std::is_arithmetic<T>::value && std::is_arithmetic<U>::value) ||
        (std::is_same<T, int>::value && std::is_same<U, char>::value) ||
        (std::is_same<T, std::string>::value && std::is_same<U, std::string>::value) ||
        (std::is_same<T, char>::value && std::is_same<U, char>::value) ||
        (std::is_same<T, char>::value && std::is_same<U, int>::value), decltype(a <= b)>::type {
    return a <= b;
}

template<typename T, typename U> // &&
auto eval::and_op(T a, U b) -> typename std::enable_if<(std::is_same<T, bool>::value && std::is_same<U, bool>::value) ||
        (std::is_arithmetic<T>::value && std::is_arithmetic<U>::value) ||
        (std::is_same<T, int>::value && std::is_same<U, char>::value) ||
        (std::is_same<T, char>::value && std::is_same<U, char>::value) ||
        (std::is_same<T, char>::value && std::is_same<U, int>::value), decltype(a && b)>::type {
    return a && b;
}

template<typename T, typename U> // ||
auto eval::or_op(T a, U b) -> typename std::enable_if<(std::is_same<T, bool>::value && std::is_same<U, bool>::value) ||
        (std::is_arithmetic<T>::value && std::is_arithmetic<U>::value) ||
        (std::is_same<T, int>::value && std::is_same<U, char>::value) ||
        (std::is_same<T, char>::value && std::is_same<U, char>::value) ||
        (std::is_same<T, char>::value && std::is_same<U, int>::value), decltype(a || b)>::type {
    return a || b;
}


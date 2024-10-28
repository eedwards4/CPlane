//
// Created by Ethan Edwards on 9/8/2024.
// Edited by Logan Puntous on 10/13/2024.
//

#ifndef CPLANE_ERRORS_H
#define CPLANE_ERRORS_H

#include "exec_path.h"

#include <vector>
#include <iostream>

class errors {
public:
    errors() = default;
    ~errors() = default;

    void check_syntax(exec_path* path); // Checks for syntax errors
    // Syntax errors
    static void EXPECTED_EXPRESSION(int line, int c, std::string val = ""); // Expected an expressn
    static void EXPECTED_STATEMENT(int line, int c, std::string val = ""); // Expected a statement
    static void EXPECTED_IDENTIFIER(int line, int c, std::string val = ""); // Expected an identifier
    static void EXPECTED_OPERATOR(int line, int c, std::string val = ""); // Expected an operator
    static void EXPECTED_TYPE(int line, int c, std::string val = ""); // Expected a type
    static void EXPECTED_VALUE(int line, int c, std::string val = ""); // Expected a value
    static void EXPECTED_END(int line, int c, std::string val = ""); // Expected end of statement
    static void EXPECTED_END_OF_FILE(int line, int c, std::string val = ""); // Expected end of file
    static void UNEXPECTED_TOKEN(int line, int c, std::string val = ""); // Unexpected token
    static void UNEXPECTED_END_OF_FILE(int line, int c, std::string val = ""); // Unexpected end of file
    static void UNEXPECTED_END(int line, int c, std::string val = ""); // Unexpected end of statement
    static void UNEXPECTED_EXPRESSION(int line, int c, std::string val = ""); // Unexpected expression
    static void UNEXPECTED_STATEMENT(int line, int c, std::string val = ""); // Unexpected statement
    static void UNEXPECTED_IDENTIFIER(int line, int c, std::string val = ""); // Unexpected identifier
    static void UNEXPECTED_OPERATOR(int line, int c, std::string val = ""); // Unexpected operator
    static void UNEXPECTED_TYPE(int line, int c, std::string val = ""); // Unexpected type
    static void UNEXPECTED_VALUE(int line, int c, std::string val = ""); // Unexpected value
    static void UNTERM_STRING(int line, int c, std::string val = ""); // Unterminated string
    static void UNTERM_COMMENT(int line, int c, std::string val = ""); // Unterminated multiline comment
    static void UNTERM_CHAR(int line, int c, std::string val = ""); // Unterminated character
    static void UNKNOWN_TOKEN(int line, int c, std::string val = ""); // Unknown token
    static void UNKNOWN_ERROR(int line, int c, std::string val = ""); // Unknown error
    static void MISSING_CLOSING_QUOTE(int line); // Missing a closing quote
    static void ARR_SIZE_POS(int line);
    static void RESERVED_WORD_VAR(int line, std::string val); // Use of reserverd word as a variable
    static void RESERVED_WORD_FUNC(int line, int col, std::string val); // Use of reserved word as a function

    static void E_UNEXPECTED_USE_OF_CHAR(int line, int c, std::string val =""); // Unexpected use of char
    static void E_UNEXPECTED_USE_OF_VOID(int line, int c, std::string val =""); // Unexpected use of void
    static void E_UNTERM_QUOTE(int line, int c, std::string val =""); // Unterminated quote
    static void E_NEGATIVE_ARRAY_SIZE(int line, int c, std::string val =""); // Negative array size used


    static void E_ALREADY_DEFINED_VARIABLE_GLOBAL(int line, int col, std::string val); //error: already defined global variable
    static void E_ALREADY_DEFINED_VARIABLE_LOCAL(int line, int col, std::string val); //error: already defined local variable
    static void E_NON_FUNCTION_SCOPE_DECLARATION(int line, int col, std::string val); //error: Non-functions/Non-procedures cannot be declared as a unique scope
    static void E_DUPLICATE_SYMBOL(int line, int col, std::string val); //error: duplicated symbol
    static void E_NESTED_FUNCTION_NOT_ALLOWED(int line, int col, std::string val); //error: nested funciton


    static void E_EPERM(int line, int c, std::string val = ""); // Operation not permitted


    // Runtime errors
  

private:
    // Helpers
    bool is_num(char c); // Checks if a character is a number
    static bool is_alpha(char c); // Checks if a character is a letter
    static std::string print_custom(int chr, std::string val);
    static void string_error_checker(std::string val, int line);
    std::vector<std::string> reserved_word_list = {"auto", "break", "case", "char", "const", "continue", "default", "do",
                                                   "double", "else", "enum", "extern", "float", "for", "goto", "if", "int",
                                                   "long", "register", "return", "short", "signed", "sizeof", "static",
                                                   "struct", "switch", "typedef", "union", "unsigned", "void", "volatile",
                                                   "while", "asm", "bool", "catch", "class", "const_cast", "delete", "dynamic_cast",
                                                   "explicit", "export", "false", "friend", "inline", "mutable", "namespace",
                                                   "new", "operator", "private", "protected", "public", "reinterpret_cast",
                                                   "static_cast", "template", "this", "throw", "true", "try", "typeid", "typename",
                                                   "using", "virtual", "wchar_t", "printf"};


};


#endif //CPLANE_ERRORS_H

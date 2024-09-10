//
// Created by evane on 9/8/2024.
//

#ifndef CPLANE_ERRORS_H
#define CPLANE_ERRORS_H

#include "tokens.h"

#include <vector>
#include <iostream>

class errors {
public:
    errors() = default;
    ~errors() = default;

    void check_syntax(std::vector<token> tokens); // Checks for syntax errors

private:
    // Helpers
    bool is_num(char c); // Checks if a character is a number
    static bool is_alpha(char c); // Checks if a character is a letter
    // Error handlers
    static void EXPECTED_EXPRESSION(int line, char c); // Expected an expression
    static void EXPECTED_STATEMENT(int line, char c); // Expected a statement
    static void EXPECTED_IDENTIFIER(int line, char c); // Expected an identifier
    static void EXPECTED_OPERATOR(int line, char c); // Expected an operator
    static void EXPECTED_TYPE(int line, char c); // Expected a type
    static void EXPECTED_VALUE(int line, char c); // Expected a value
    static void EXPECTED_END(int line, char c); // Expected end of statement
    static void EXPECTED_END_OF_FILE(int line, char c); // Expected end of file
    static void UNEXPECTED_TOKEN(int line, char c); // Unexpected token
    static void UNEXPECTED_END_OF_FILE(int line, char c); // Unexpected end of file
    static void UNEXPECTED_END(int line, char c); // Unexpected end of statement
    static void UNEXPECTED_EXPRESSION(int line, char c); // Unexpected expression
    static void UNEXPECTED_STATEMENT(int line, char c); // Unexpected statement
    static void UNEXPECTED_IDENTIFIER(int line, char c); // Unexpected identifier
    static void UNEXPECTED_OPERATOR(int line, char c); // Unexpected operator
    static void UNEXPECTED_TYPE(int line, char c); // Unexpected type
    static void UNEXPECTED_VALUE(int line, char c); // Unexpected value
    static void UNTERM_STRING(int line, char c); // Unterminated string
    static void UNTERM_COMMENT(int line, char c); // Unterminated multiline comment
    static void UNTERM_CHAR(int line, char c); // Unterminated character
    static void UNKNOWN_TOKEN(int line, char c); // Unknown token

};


#endif //CPLANE_ERRORS_H

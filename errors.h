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
    static void EXPECTED_EXPRESSION(int line, int col); // Expected an expression
    static void EXPECTED_STATEMENT(int line, int col); // Expected a statement
    static void EXPECTED_IDENTIFIER(int line, int col); // Expected an identifier
    static void EXPECTED_OPERATOR(int line, int col); // Expected an operator
    static void EXPECTED_TYPE(int line, int col); // Expected a type
    static void EXPECTED_VALUE(int line, int col); // Expected a value
    static void EXPECTED_END(int line, int col); // Expected end of statement
    static void EXPECTED_END_OF_FILE(int line, int col); // Expected end of file
    static void UNEXPECTED_TOKEN(int line, int col); // Unexpected token
    static void UNEXPECTED_END_OF_FILE(int line, int col); // Unexpected end of file
    static void UNEXPECTED_END(int line, int col); // Unexpected end of statement
    static void UNEXPECTED_EXPRESSION(int line, int col); // Unexpected expression
    static void UNEXPECTED_STATEMENT(int line, int col); // Unexpected statement
    static void UNEXPECTED_IDENTIFIER(int line, int col); // Unexpected identifier
    static void UNEXPECTED_OPERATOR(int line, int col); // Unexpected operator
    static void UNEXPECTED_TYPE(int line, int col); // Unexpected type
    static void UNEXPECTED_VALUE(int line, int col); // Unexpected value
    static void UNTERM_STRING(int line, int col); // Unterminated string
    static void UNTERM_COMMENT(int line, int col); // Unterminated multiline comment
    static void UNTERM_CHAR(int line, int col); // Unterminated character

};


#endif //CPLANE_ERRORS_H

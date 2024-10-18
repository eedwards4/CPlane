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
    // Error handlers
    static void EXPECTED_EXPRESSION(int line, int c, std::string val = ""); // Expected an expression
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



  static void E_UNEXPECTED_USE_OF_CHAR(int line, int c, std::string val ="");
  static void E_UNEXPECTED_USE_OF_VOID(int line, int c, std::string val ="");
  static void E_UNTERM_QUOTE(int line, int c, std::string val ="");
  static void E_NEGATIVE_ARRAY_SIZE(int line, int c, std::string val ="");
  //oh no
  //

    
    static void E_EPERM(int line, int c, std::string val = ""); // Operation not permitted
    static void E_ENOENT(int line, int c, std::string val);

private:
    // Helpers
    bool is_num(char c); // Checks if a character is a number
    static bool is_alpha(char c); // Checks if a character is a letter
    static std::string print_custom(int chr, std::string val);

};


#endif //CPLANE_ERRORS_H

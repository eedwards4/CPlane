//
// Created by evane on 9/8/2024.
//

#include "errors.h"

void errors::check_syntax(std::vector<token> tokens) {
    bool in_string = false; // Tracks if we are currently inside a string literal
    bool in_char = false; // Tracks if we are currently inside a character literal
    bool in_comment = false; // This essentially only triggers on an error (comment not terminated)
    int num_deep = 0; // Tracks current depth in nested statements. MUST BE ZERO AT END OF FILE
    int line = 1; // Tracks current line number (resets on newline)
    int col = 1; // Tracks current column number (resets on newline)

    for (int i = 0; i < tokens.size(); i++){
        break;
    }
}

// Error handlers
void errors::EXPECTED_EXPRESSION(int line, int col) {
    std::cerr << "Error: Expected an expression at line " << line << ", column " << col << std::endl;
    exit(1);
}

void errors::EXPECTED_STATEMENT(int line, int col) {
    std::cerr << "Error: Expected a statement at line " << line << ", column " << col << std::endl;
    exit(2);
}

void errors::EXPECTED_IDENTIFIER(int line, int col) {
    std::cerr << "Error: Expected an identifier at line " << line << ", column " << col << std::endl;
    exit(3);
}

void errors::EXPECTED_OPERATOR(int line, int col) {
    std::cerr << "Error: Expected an operator at line " << line << ", column " << col << std::endl;
    exit(4);
}

void errors::EXPECTED_TYPE(int line, int col) {
    std::cerr << "Error: Expected a type at line " << line << ", column " << col << std::endl;
    exit(5);
}

void errors::EXPECTED_VALUE(int line, int col) {
    std::cerr << "Error: Expected a value at line " << line << ", column " << col << std::endl;
    exit(6);
}

void errors::EXPECTED_END(int line, int col) {
    std::cerr << "Error: Expected end of statement at line " << line << ", column " << col << std::endl;
    exit(7);
}

void errors::EXPECTED_END_OF_FILE(int line, int col) {
    std::cerr << "Error: Expected end of file at line " << line << ", column " << col << std::endl;
    exit(8);
}

void errors::UNEXPECTED_TOKEN(int line, int col) {
    std::cerr << "Error: Unexpected token at line " << line << ", column " << col << std::endl;
    exit(9);
}

void errors::UNEXPECTED_END_OF_FILE(int line, int col) {
    std::cerr << "Error: Unexpected end of file at line " << line << ", column " << col << std::endl;
    exit(10);
}

void errors::UNEXPECTED_END(int line, int col) {
    std::cerr << "Error: Unexpected end of statement at line " << line << ", column " << col << std::endl;
    exit(11);
}

void errors::UNEXPECTED_EXPRESSION(int line, int col) {
    std::cerr << "Error: Unexpected expression at line " << line << ", column " << col << std::endl;
    exit(12);
}

void errors::UNEXPECTED_STATEMENT(int line, int col) {
    std::cerr << "Error: Unexpected statement at line " << line << ", column " << col << std::endl;
    exit(13);
}

void errors::UNEXPECTED_IDENTIFIER(int line, int col) {
    std::cerr << "Error: Unexpected identifier at line " << line << ", column " << col << std::endl;
    exit(14);
}

void errors::UNEXPECTED_OPERATOR(int line, int col) {
    std::cerr << "Error: Unexpected operator at line " << line << ", column " << col << std::endl;
    exit(15);
}

void errors::UNEXPECTED_TYPE(int line, int col) {
    std::cerr << "Error: Unexpected type at line " << line << ", column " << col << std::endl;
    exit(16);
}

void errors::UNEXPECTED_VALUE(int line, int col) {
    std::cerr << "Error: Unexpected value at line " << line << ", column " << col << std::endl;
    exit(17);
}

void errors::UNTERM_STRING(int line, int col) {
    std::cerr << "Error: Unterminated string at line " << line << ", column " << col << std::endl;
    exit(18);
}

void errors::UNTERM_COMMENT(int line, int col) {
    std::cerr << "Error: Unterminated multiline comment at line " << line << ", column " << col << std::endl;
    exit(19);
}

void errors::UNTERM_CHAR(int line, int col) {
    std::cerr << "Error: Unterminated character at line " << line << ", column " << col << std::endl;
    exit(20);
}

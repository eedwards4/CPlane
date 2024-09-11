//
// Created by Ethan Edwards on 9/8/2024.
//

#include "errors.h"

void errors::check_syntax(std::vector<token> tokens) {
    bool in_string = false; // This essentially only triggers on an error (string not terminated)
    bool in_char = false; // Tracks if we are currently inside a character literal
    bool in_comment = false; // This essentially only triggers on an error (comment not terminated)
    int num_deep = 0; // Tracks current depth in nested statements. MUST BE ZERO AT END OF FILE
    std::vector<char> structure; // Tracks the structure of the current nest (e.g. {, [, (, etc). MUST BE EMPTY AT END OF FILE
    int line = 1; // Tracks current line number
    char c; // Tracks the current character

    for (int i = 0; i < tokens.size(); i++){
        if (tokens.at(i).get_type() == -1) {
            errors::UNKNOWN_TOKEN(line, c);
        }
        else if (tokens.at(i).get_type() == 0) {
            c = tokens.at(i).get_char_value();
            if (in_string){
                if (c == '"'){
                    in_string = false; // We should never encounter this condition if the tokenizer is working properly
                }
            }
            else if (in_comment){
                if (c == '*' && i + 1 < tokens.size()){
                    if (tokens.at(i + 1).get_type() == 0 && tokens.at(i + 1).get_char_value() == '/'){
                        in_comment = false;
                        i++;
                    }
                }
            }
            else if (in_char){
                if (c == '\''){
                    in_char = false; // We should never encounter this condition if the tokenizer is working properly
                }
            }
            else{
                switch(c){
                    case '{': case '[': case '(':
                        structure.push_back(c);
                        num_deep++;
                        break;
                    case '}': case ']': case ')':
                        if (c == '}' && structure.back() != '{'){
                            errors::UNEXPECTED_TOKEN(line, c);
                        } else if (c == ']' && structure.back() != '[') {
                            errors::UNEXPECTED_TOKEN(line, c);
                        } else if (c == ')' && structure.back() != '(') {
                            errors::UNEXPECTED_TOKEN(line, c);
                        }
                        structure.pop_back();
                        num_deep--;
                        break;
                    case '"':
                        in_string = true;
                        break;
                    case '\'':
                        in_char = true;
                        break;
                    case '/':
                        if (i + 1 < tokens.size()) {
                            if (tokens.at(i + 1).get_type() == 0) {
                                char c1 = tokens.at(i + 1).get_char_value();
                                if (c1 == '*') {
                                    in_comment = true;
                                } else if (c1 == '(') {
                                    break;
                                } else if (!is_num(c1) && !is_alpha(c1)) {
                                    errors::UNEXPECTED_TOKEN(line, c);
                                }
                            }
                        }
                        break;
                    case '\n':
                        if (!in_string && !in_char) { // Ignore newlines inside strings or characters
                            line++;
                            c = 1;
                        }
                        break;
                }
            }
        }
    }
    if (in_comment){
        errors::UNTERM_COMMENT(line, c);
    }
    if (in_string){
        errors::UNTERM_STRING(line, c);
    }
    if (in_char){
        errors::UNTERM_CHAR(line, c);
    }
    if (num_deep != 0 || !structure.empty()){
        errors::EXPECTED_END_OF_FILE(line, c);
    }
}

// Helper functions
bool errors::is_num(char c) {
    return c >= '0' && c <= '9';
}

bool errors::is_alpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

// Error handlers
void errors::EXPECTED_EXPRESSION(int line, char c) {
    std::cerr << "Error: Expected an expression at line " << line << ", character " << c << std::endl;
    exit(1);
}

void errors::EXPECTED_STATEMENT(int line, char c) {
    std::cerr << "Error: Expected a statement at line " << line << ", character " << c << std::endl;
    exit(2);
}

void errors::EXPECTED_IDENTIFIER(int line, char c) {
    std::cerr << "Error: Expected an identifier at line " << line << ", character " << c << std::endl;
    exit(3);
}

void errors::EXPECTED_OPERATOR(int line, char c) {
    std::cerr << "Error: Expected an operator at line " << line << ", character " << c << std::endl;
    exit(4);
}

void errors::EXPECTED_TYPE(int line, char c) {
    std::cerr << "Error: Expected a type at line " << line << ", character " << c << std::endl;
    exit(5);
}

void errors::EXPECTED_VALUE(int line, char c) {
    std::cerr << "Error: Expected a value at line " << line << ", character " << c << std::endl;
    exit(6);
}

void errors::EXPECTED_END(int line, char c) {
    std::cerr << "Error: Expected end of statement at line " << line << ", character " << c << std::endl;
    exit(7);
}

void errors::EXPECTED_END_OF_FILE(int line, char c) {
    std::cerr << "Error: Expected end of file at line " << line << ", character " << c << std::endl;
    exit(8);
}

void errors::UNEXPECTED_TOKEN(int line, char c) {
    std::cerr << "Error: Unexpected token at line " << line << ", character " << c << std::endl;
    exit(9);
}

void errors::UNEXPECTED_END_OF_FILE(int line, char c) {
    std::cerr << "Error: Unexpected end of file at line " << line << ", character " << c << std::endl;
    exit(10);
}

void errors::UNEXPECTED_END(int line, char c) {
    std::cerr << "Error: Unexpected end of statement at line " << line << ", character " << c << std::endl;
    exit(11);
}

void errors::UNEXPECTED_EXPRESSION(int line, char c) {
    std::cerr << "Error: Unexpected expression at line " << line << ", character " << c << std::endl;
    exit(12);
}

void errors::UNEXPECTED_STATEMENT(int line, char c) {
    std::cerr << "Error: Unexpected statement at line " << line << ", character " << c << std::endl;
    exit(13);
}

void errors::UNEXPECTED_IDENTIFIER(int line, char c) {
    std::cerr << "Error: Unexpected identifier at line " << line << ", character " << c << std::endl;
    exit(14);
}

void errors::UNEXPECTED_OPERATOR(int line, char c) {
    std::cerr << "Error: Unexpected operator at line " << line << ", character " << c << std::endl;
    exit(15);
}

void errors::UNEXPECTED_TYPE(int line, char c) {
    std::cerr << "Error: Unexpected type at line " << line << ", character " << c << std::endl;
    exit(16);
}

void errors::UNEXPECTED_VALUE(int line, char c) {
    std::cerr << "Error: Unexpected value at line " << line << ", character " << c << std::endl;
    exit(17);
}

void errors::UNTERM_STRING(int line, char c) {
    std::cerr << "Error: Unterminated string at line " << line << ", character " << c << std::endl;
    exit(18);
}

void errors::UNTERM_COMMENT(int line, char c) {
    std::cerr << "Error: Unterminated multiline comment at line " << line << ", character " << c << std::endl;
    exit(19);
}

void errors::UNTERM_CHAR(int line, char c) {
    std::cerr << "Error: Unterminated character at line " << line << ", character " << c << std::endl;
    exit(20);
}

void errors::UNKNOWN_TOKEN(int line, char c) {
    std::cerr << "Error: Unknown token at line " << line << ", character " << c << std::endl;
    exit(21);
}

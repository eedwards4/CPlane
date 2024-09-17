//
// Created by Ethan Edwards on 9/8/2024.
//

#include "errors.h"

void errors::check_syntax(exec_path *path) {
    exec_node* current = path->get_head();
    int num_deep = 0; // Tracks current depth in nested statements. MUST BE ZERO AT END OF FILE
    std::vector<exec_node*> structure; // Tracks the structure of the current nest (e.g. {, [, (, etc). MUST BE EMPTY AT END OF FILE
    int line = 1; // Tracks current line number
    int tok = 1; // Tracks current token number
    int entered_at = 0; // Tracks the line where we entered a char, string, or comment
    
    while (current != nullptr){
        if (current->get_type() == tokens::TOKEN_AS_STRING || current->get_type() == tokens::STRING_LITERAL || current->get_type() == tokens::CHAR_LITERAL ||
            current->get_type() == tokens::INT_AS_STRING || current->get_type() == tokens::FLOAT_AS_STRING){ // Non singletons
            current = current->get_next();
            tok++;
        } else{
            if (current->get_type() == tokens::NEWLINE){
                current = current->get_next();
                line++;
                tok = 1;
            } else{
                switch(current->get_type()){
                    // Nesting
                    case tokens::OPEN_BRACE: case tokens::OPEN_BRACKET: case tokens::OPEN_PAREN:
                        structure.push_back(current);
                        current = current->get_fold();
                        num_deep++;
                        break;

                    case tokens::CLOSE_BRACE: case tokens::CLOSE_BRACKET: case tokens::CLOSE_PAREN:
                        if (current->get_type() == tokens::CLOSE_BRACE && structure.back()->get_type() != tokens::OPEN_BRACE){
                            errors::UNEXPECTED_TOKEN(line, '}');
                        } else if (current->get_type() == tokens::CLOSE_BRACKET && structure.back()->get_type() != tokens::OPEN_BRACKET) {
                            errors::UNEXPECTED_TOKEN(line, ']');
                        } else if (current->get_type() == tokens::CLOSE_PAREN && structure.back()->get_type() != tokens::OPEN_PAREN) {
                            errors::UNEXPECTED_TOKEN(line, ')');
                        }
                        current = structure.back()->get_next(); // Return to outside of fold
                        structure.pop_back();
                        num_deep--;
                        break;

                    // Multi char operators
                    case tokens::PLUS_PLUS: case tokens::MINUS_MINUS: case tokens::PLUS_EQUALS:
                    case tokens::MINUS_EQUALS: case tokens::TIMES_EQUALS: case tokens::DIVIDE_EQUALS:
                    case tokens::MOD_EQUALS: case tokens::EQUALS_EQUALS: case tokens::NOT_EQUALS:
                    case tokens::LESS_EQUALS: case tokens::GREATER_EQUALS: case tokens::LEFT_SHIFT:
                    case tokens::RIGHT_SHIFT: case tokens::LEFT_SHIFT_EQUALS: case tokens::RIGHT_SHIFT_EQUALS:
                    case tokens::AND_EQUALS: case tokens::OR_EQUALS: case tokens::XOR_EQUALS:
                    case tokens::BOOLEAN_AND: case tokens::BOOLEAN_OR: case tokens::RIGHT_SLIM_ARROW:
                        if (current->get_next() == nullptr || (current->get_next()->get_type() != tokens::TOKEN_AS_STRING
                                                              && current->get_next()->get_type() != tokens::INT_AS_STRING
                                                              && current->get_next()->get_type() != tokens::FLOAT_AS_STRING)){
                            errors::EXPECTED_EXPRESSION(line, char(current->get_type()));
                        }
                        current = current->get_next();
                        break;

                    // Single char operators
                    case '+': case '-': case '*': case '%': case '=': case '<': case '>': case '!': case '/':
                        if (current->get_next() == nullptr || !is_num(char(current->get_next()->get_type())) || !is_alpha(char(current->get_next()->get_type()))){
                            errors::EXPECTED_EXPRESSION(line, char(current->get_type()));
                        }
                        current = current->get_next();
                        break;

                    default:
                        current = current->get_next();
                        break;
                }
                tok++;
            }
        }
    }
    if (num_deep != 0){
        errors::EXPECTED_END_OF_FILE(line, '\0');
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
    // std::cerr << "Error: Unterminated multiline comment at line " << line << ", character " << c << std::endl;
    std::cerr << "ERROR: Program contains C-style, unterminated comment on line " << line << std::endl;
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

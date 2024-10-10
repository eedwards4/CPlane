//
// Created by Ethan Edwards on 9/8/2024.
//

#include "errors.h"

void errors::check_syntax(exec_path *path) {
    exec_node* current = path->get_head();
    int num_deep = 0; // Tracks current depth in nested statements. MUST BE ZERO AT END OF FILE
    std::vector<exec_node*> structure; // Tracks the structure of the current nest (e.g. {, [, (, etc). MUST BE EMPTY AT END OF FILE
    int line = 1; // Tracks current line number
    int entered_at = 0; // Tracks the line where we entered a char, string, or comment
    bool in_string = false; // Tracks if we are in a string
    bool in_char = false; // Tracks if we are in a char

    while (current != nullptr){
        if (current->get_type() == tokens::NEWLINE){
            current = current->get_next();
            line++;
        }
        else if (in_string){
            if (current->get_type() == '"'){
                in_string = false;
            }
            current = current->get_next();
        }
        else if (in_char){
            if (current->get_type() == '\''){
                in_char = false;
            }
            current = current->get_next();
        }
        else{
            switch(current->get_type()){
                case tokens::OPEN_BRACE:
                    structure.push_back(current);
                    current = current->get_fold();
                    num_deep++;
                    break;

                case tokens::CLOSE_BRACE:
                    if (structure.back()->get_type() != tokens::OPEN_BRACE){
                        errors::UNEXPECTED_TOKEN(line, '}');
                    }
                    current = current->get_fold(); // Return to outside of fold
                    structure.pop_back();
                    num_deep--;
                    break;

                case tokens::OPEN_BRACKET: case tokens::OPEN_PAREN:
                    structure.push_back(current);
                    current = current->get_next();
                    num_deep++;
                    break;

                case tokens::CLOSE_BRACKET: case tokens::CLOSE_PAREN:
                    if (current->get_type() == tokens::CLOSE_BRACKET && structure.back()->get_type() != tokens::OPEN_BRACKET){
                        errors::UNEXPECTED_TOKEN(line, ']');
                    } else if (current->get_type() == tokens::CLOSE_PAREN && structure.back()->get_type() != tokens::OPEN_PAREN){
                        errors::UNEXPECTED_TOKEN(line, ')');
                    }
                    current = current->get_next(); // Return to outside of fold
                    structure.pop_back();
                    num_deep--;
                    break;

                // Multi character operators
                case tokens::PLUS_PLUS: case tokens::MINUS_MINUS: case tokens::PLUS_EQUALS:
                case tokens::MINUS_EQUALS: case tokens::TIMES_EQUALS: case tokens::DIVIDE_EQUALS:
                case tokens::MOD_EQUALS: case tokens::EQUALS_EQUALS: case tokens::NOT_EQUALS:
                case tokens::LESS_EQUALS: case tokens::GREATER_EQUALS: case tokens::LEFT_SHIFT:
                case tokens::RIGHT_SHIFT: case tokens::LEFT_SHIFT_EQUALS: case tokens::RIGHT_SHIFT_EQUALS:
                case tokens::AND_EQUALS: case tokens::OR_EQUALS: case tokens::XOR_EQUALS:
                case tokens::BOOLEAN_AND: case tokens::BOOLEAN_OR: case tokens::RIGHT_SLIM_ARROW:
                    if (current->get_next() == nullptr){
                        errors::EXPECTED_END(line, '\0');
                    } else{
                        switch(current->get_next()->get_type()){
                            case tokens::FLOAT_AS_STRING: case tokens::INT_AS_STRING: case tokens::STRING_LITERAL: case tokens::CHAR_LITERAL: case tokens::TOKEN_AS_STRING:
                                current = current->get_next();
                                break;

                            case '\'': case '"':
                                current = current->get_next();
                                break;

                            case tokens::OPEN_BRACE: case tokens::OPEN_BRACKET: case tokens::OPEN_PAREN:
                                current = current->get_next();
                                break;

                            default:
                                errors::EXPECTED_EXPRESSION(line, current->get_next()->get_type(), current->get_next()->get_value());
                                break;
                        }
                    }
                    break;

                // Single char operators
                case '+': case '-': case '*': case '%': case '=': case '<': case '>': case '!': case '/':
                    if (current->get_next() == nullptr){
                        errors::EXPECTED_EXPRESSION(line, '\0');
                        break;
                    } else{
                        switch (current->get_next()->get_type()){
                            case tokens::FLOAT_AS_STRING: case tokens::INT_AS_STRING: case tokens::STRING_LITERAL: case tokens::CHAR_LITERAL: case tokens::TOKEN_AS_STRING:
                                current = current->get_next();
                                break;

                            case '\'': case '"':
                                current = current->get_next();
                                break;

                            case tokens::OPEN_BRACE: case tokens::OPEN_BRACKET: case tokens::OPEN_PAREN:
                                current = current->get_next();
                                break;

                            default:
                                errors::EXPECTED_EXPRESSION(line, current->get_next()->get_type(), current->get_next()->get_value());
                                break;
                        }
                    }
                    break;

                // Variables/statics
                case tokens::INT_AS_STRING: case tokens::FLOAT_AS_STRING: case tokens::STRING_LITERAL: case tokens::CHAR_LITERAL:
                    if (current->get_next() == nullptr){
                        errors::EXPECTED_END(line, '\0');
                    } else{
                        switch(current->get_next()->get_type()){
                            case tokens::PLUS_PLUS: case tokens::MINUS_MINUS: case tokens::PLUS_EQUALS:
                            case tokens::MINUS_EQUALS: case tokens::TIMES_EQUALS: case tokens::DIVIDE_EQUALS:
                            case tokens::MOD_EQUALS: case tokens::EQUALS_EQUALS: case tokens::NOT_EQUALS:
                            case tokens::LESS_EQUALS: case tokens::GREATER_EQUALS: case tokens::LEFT_SHIFT:
                            case tokens::RIGHT_SHIFT: case tokens::LEFT_SHIFT_EQUALS: case tokens::RIGHT_SHIFT_EQUALS:
                            case tokens::AND_EQUALS: case tokens::OR_EQUALS: case tokens::XOR_EQUALS:
                            case tokens::BOOLEAN_AND: case tokens::BOOLEAN_OR: case tokens::RIGHT_SLIM_ARROW:
                            case '+': case '-': case '*': case '%': case '=': case '<': case '>': case '!': case '/':
                                current = current->get_next();
                                break;

                            case tokens::CLOSE_BRACE: case tokens::CLOSE_BRACKET: case tokens::CLOSE_PAREN: case ';': case '\'': case '"':
                                current = current->get_next();
                                break;

                            default:
                                errors::EXPECTED_EXPRESSION(line, current->get_next()->get_type(), current->get_next()->get_value());
                                break;
                        }
                    }
                    break;

                case '\'':
                    in_char = true;
                    entered_at = line;
                    break;

                case '"':
                    in_string = true;
                    entered_at = line;
                    break;

                default:
                    current = current->get_next();
                    break;
            }
        }
    }

    // Check for folding errors and unterminated strings/chars
    if (in_char){
        errors::UNTERM_CHAR(entered_at, '\0');
    }
    if (in_string){
        errors::UNTERM_STRING(entered_at, '\0');
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

std::string errors::print_custom(int chr, std::string val = ""){
    if (chr < 9000){
        return std::string(1, static_cast<char>(chr));
    } else{
        switch(chr){
            case 9999: case 9998: case 9997: case 9996: case 9995: // Literals/tokens
                return "'" + val + "'";
            // Nesting structures
            case 9994: return "(";
            case 9993: return ")";
            case 9992: return "{";
            case 9991: return "}";
            case 9990: return "[";
            case 9989: return "]";
            // Multi-character operators
            case 9988: return "++";
            case 9987: return "+=";
            case 9986: return "--";
            case 9985: return "-=";
            case 9984: return "->";
            case 9983: return "&&";
            case 9982: return "&=";
            case 9981: return "||";
            case 9980: return "|=";
            case 9979: return "!=";
            case 9978: return "%=";
            case 9977: return "*=";
            case 9976: return "==";
            case 9975: return "^=";
            case 9974: return "##";
            case 9973: return ">=";
            case 9972: return ">>=";
            case 9971: return ">>";
            case 9970: return "<=";
            case 9969: return "<<=";
            case 9968: return "<<";
            case 9967: return "/=";
            case 9966: return "\\n";
            case 9965: return "...";
            case 9964: return "::";
            case 9963: return "L";
            default: // Should never happen
                return std::to_string(char(chr));
        }
    }
}

// Error handlers
void errors::EXPECTED_EXPRESSION(int line, int c, std::string val) {
    std::cerr << "Expected an expression at line " << line << " but found " << errors::print_custom(c, val) << std::endl;
    exit(1);
}

void errors::EXPECTED_STATEMENT(int line, int c, std::string val) {
    std::cerr << "Expected a statement at line " << line << " but found " << errors::print_custom(c, val) << std::endl;
    exit(2);
}

void errors::EXPECTED_IDENTIFIER(int line, int c, std::string val) {
    std::cerr << "Expected an identifier at line " << line << " but found " << errors::print_custom(c, val) << std::endl;
    exit(3);
}

void errors::EXPECTED_OPERATOR(int line, int c, std::string val) {
    std::cerr << "Expected an operator at line " << line << " but found " << errors::print_custom(c, val) << std::endl;
    exit(4);
}

void errors::EXPECTED_TYPE(int line, int c, std::string val) {
    std::cerr << "Expected a type at line " << line << " but found " << errors::print_custom(c, val) << std::endl;
    exit(5);
}

void errors::EXPECTED_VALUE(int line, int c, std::string val) {
    std::cerr << "Expected a value at line " << line << " but found " << errors::print_custom(c, val) << std::endl;
    exit(6);
}

void errors::EXPECTED_END(int line, int c, std::string val) {
    std::cerr << "Expected end of statement at line " << line << " but found " << errors::print_custom(c, val) << std::endl;
    exit(7);
}

void errors::EXPECTED_END_OF_FILE(int line, int c, std::string val) {
    std::cerr << "Expected end of file at line " << line << " but found " << errors::print_custom(c, val) << std::endl;
    exit(8);
}

void errors::UNEXPECTED_TOKEN(int line, int c, std::string val) {
    std::cerr << "Unexpected token at line " << line << " but found " << errors::print_custom(c, val) << std::endl;
    exit(9);
}

void errors::UNEXPECTED_END_OF_FILE(int line, int c, std::string val) {
    std::cerr << "Unexpected end of file at line " << line << " but found " << errors::print_custom(c, val) << std::endl;
    exit(10);
}

void errors::UNEXPECTED_END(int line, int c, std::string val) {
    std::cerr << "Unexpected end of statement at line " << line << " but found " << errors::print_custom(c, val) << std::endl;
    exit(11);
}

void errors::UNEXPECTED_EXPRESSION(int line, int c, std::string val) {
    std::cerr << "Unexpected expression at line " << line << " but found " << errors::print_custom(c, val) << std::endl;
    exit(12);
}

void errors::UNEXPECTED_STATEMENT(int line, int c, std::string val) {
    std::cerr << "Unexpected statement at line " << line << " but found " << errors::print_custom(c, val) << std::endl;
    exit(13);
}

void errors::UNEXPECTED_IDENTIFIER(int line, int c, std::string val) {
    std::cerr << "Unexpected identifier at line " << line << " but found " << errors::print_custom(c, val) << std::endl;
    exit(14);
}

void errors::UNEXPECTED_OPERATOR(int line, int c, std::string val) {
    std::cerr << "Unexpected operator at line " << line << " but found " << errors::print_custom(c, val) << std::endl;
    exit(15);
}

void errors::UNEXPECTED_TYPE(int line, int c, std::string val) {
    std::cerr << "Unexpected type at line " << line << " but found " << errors::print_custom(c, val) << std::endl;
    exit(16);
}

void errors::UNEXPECTED_VALUE(int line, int c, std::string val) {
    std::cerr << "Unexpected value at line " << line << " but found " << errors::print_custom(c, val) << std::endl;
    exit(17);
}

void errors::UNTERM_STRING(int line, int c, std::string val) {
    std::cerr << "Unterminated string at line " << line << std::endl;
    exit(18);
}

void errors::UNTERM_COMMENT(int line, int c, std::string val) {
    std::cerr << "Unterminated multiline comment on line " << line << std::endl;
    exit(19);
}

void errors::UNTERM_CHAR(int line, int c, std::string val) {
    std::cerr << "Unterminated char literal at line " << line << std::endl;
    exit(20);
}

void errors::UNKNOWN_TOKEN(int line, int c, std::string val) {
    std::cerr << "Unknown token at line " << line << std::endl;
    exit(21);
}

void errors::UNKNOWN_ERROR(int line, int c, std::string val) {
    std::cerr << "Unknown error at line " << line << std::endl;
    exit(22);
}

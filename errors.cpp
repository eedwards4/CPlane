//
// Created by Ethan Edwards on 9/8/2024.
//

#include "errors.h"
#include <set>

void errors::check_syntax(exec_path *path) {
    exec_node* current = path->get_head();
    int num_deep = 0; // Tracks current depth in nested statements. MUST BE ZERO AT END OF FILE
    std::vector<exec_node*> structure; // Tracks the structure of the current nest (e.g. {, [, (, etc). MUST BE EMPTY AT END OF FILE
    int entered_at = 0; // Tracks the line where we entered a char, string, or comment
    bool in_string = false; // Tracks if we are in a string
    bool in_char = false; // Tracks if we are in a char


    std::set<std::string> global_variables; // Track global variable names
    std::set<std::string> local_variables;   // Track local variable names in the current function/procedure
    bool expecting_variable = false; // Flag to track when a variable name is expected
    bool in_local_scope = false;   //check for local vs global

    while (current != nullptr){
        if (current->get_type() == tokens::NEWLINE){
            current = current->get_next();
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
                        errors::UNEXPECTED_TOKEN(current->get_line(), '}');
                    }
                    current = current->get_fold(); // Return to outside of fold
                    structure.pop_back();
                    num_deep--;
                    break;

                case tokens::OPEN_BRACKET:
                    structure.push_back(current);
                    if (current->get_next()->get_type() == tokens::INT_AS_STRING){
                        if (std::stoi(current->get_next()->get_value()) < 0){
                            errors::ARR_SIZE_POS(current->get_line());
                        }
                    }
                    current = current->get_next();
                    num_deep++;
                    break;

                case tokens::OPEN_PAREN:
                    structure.push_back(current);
                    current = current->get_next();
                    num_deep++;
                    break;

                case tokens::CLOSE_BRACKET: case tokens::CLOSE_PAREN:
                    if (current->get_type() == tokens::CLOSE_BRACKET && structure.back()->get_type() != tokens::OPEN_BRACKET){
                        errors::UNEXPECTED_TOKEN(current->get_line(), ']');
                    } else if (current->get_type() == tokens::CLOSE_PAREN && structure.back()->get_type() != tokens::OPEN_PAREN){
                        errors::UNEXPECTED_TOKEN(current->get_line(), ')');
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
                        errors::EXPECTED_END(current->get_line(), '\0');
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
                                errors::EXPECTED_EXPRESSION(current->get_line(), current->get_next()->get_column(), current->get_next()->get_value());
                                break;
                        }
                    }
                    break;

                // Single char operators
                case '+': case '-': case '*': case '%': case '=': case '<': case '>': case '!': case '/':
                    if (current->get_next() == nullptr){
                        errors::EXPECTED_EXPRESSION(current->get_line(), '\0');
                        break;
                    } else{
                        switch (current->get_next()->get_type()){
                            case tokens::FLOAT_AS_STRING: case tokens::INT_AS_STRING: case tokens::STRING_LITERAL: case tokens::CHAR_LITERAL: case tokens::TOKEN_AS_STRING:
                                current = current->get_next();
				//
                                break;

                            case '\'': case '"':
                                current = current->get_next();
                                break;

                            case tokens::OPEN_BRACE: case tokens::OPEN_BRACKET: case tokens::OPEN_PAREN:
                                current = current->get_next();
                                break;

                            default:
                                errors::EXPECTED_EXPRESSION(current->get_line(), current->get_next()->get_column(), current->get_next()->get_value());
                                break;
                        }
                    }
                    break;

                // Variables/statics
                case tokens::INT_AS_STRING: case tokens::FLOAT_AS_STRING: case tokens::CHAR_LITERAL:
                    if (current->get_next() == nullptr){
                        errors::EXPECTED_END(current->get_line(), '\0');
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
                                errors::EXPECTED_EXPRESSION(current->get_line(), current->get_next()->get_column(), current->get_next()->get_value());
                                break;
                        }
                    }
                    break;

                case tokens::STRING_LITERAL:
                    string_error_checker(current->get_value(), current->get_line());
                    if (current->get_next() == nullptr){
                        errors::EXPECTED_END(current->get_line(), '\0');
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
                                errors::EXPECTED_EXPRESSION(current->get_line(), current->get_next()->get_column(), current->get_next()->get_value());
                                break;
                        }
                    }
                    break;

                case tokens::TOKEN_AS_STRING:
                    if (current->get_value() == "int" || current->get_value() == "float" || current->get_value() == "char"){
                        for (std::string word : reserved_word_list){
                            if (current->get_next()->get_value() == word){
                                errors::RESERVED_WORD_VAR(current->get_line(), current->get_next()->get_value());
                            }
                        }
                    }
                    if (current->get_value() == "function"){
                        for (std::string word : reserved_word_list){
                            if (current->get_next()->get_next()->get_value() == word){
                                errors::RESERVED_WORD_FUNC(current->get_line(), current->get_next()->get_next()->get_column(), current->get_next()->get_next()->get_value());
                            }
                        }
                    }
                    //start of already defined variable checks. Removed for now
                    current = current->get_next();
                    break;

                case '\'':
                    in_char = true;
                    entered_at = current->get_line();
                    break;

                case '"':
                    in_string = true;
                    entered_at = current->get_line();
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
        errors::EXPECTED_END_OF_FILE(0, '\0'); // TODO: Add line number
    }
}

// Helper functions
bool errors::is_num(char c) {
    return c >= '0' && c <= '9';
}

bool errors::is_alpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

void errors::string_error_checker(std::string val, int line) {
    for (char c : val){
        if (c == '\\' && c == val.back()){
            errors::MISSING_CLOSING_QUOTE(line);
        }
    }
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
    std::cerr << ":" << line << ":" << c << ": error: expected expression but found " << val << std::endl;
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

void errors::E_EPERM(int line, int c, std::string val) {
    std::cerr << "Operation not permitted at line " << line << std::endl;
    exit(23);
}

void errors::MISSING_CLOSING_QUOTE(int line) {
    std::cerr << "Syntax error on line " << line << ": unterminated string quote." << std::endl;
    exit(24);
}

void errors::ARR_SIZE_POS(int line) {
    std::cerr << "Syntax error on line " << line << ": array size must be a positive integer." << std::endl;
    exit(25);
}

void errors::RESERVED_WORD_VAR(int line, std::string val) {
    std::cerr << "Syntax error on line " << line << ": reserved word " << val << " cannot be used as a variable name." << std::endl;
    exit(26);
}

void errors::RESERVED_WORD_FUNC(int line, int col, std::string val) {
    std::cerr << ":" << line << ":" << col << ": error: reserved word " << val << " cannot be used as a function name." << std::endl;
    exit(27);
}
/*
void errors::E_ENOENT(int line, int c, std::string val) {
  std::cerr << "No such file or directory on line " << line << std::endl;
  exit(24);
}
*/

void errors::E_UNTERM_QUOTE(int line, int c, std::string val) {
  std::cerr << "syntax error on line " << line << ": closing quote is missing. " << std::endl;
  exit(27);
}
void errors::E_UNEXPECTED_USE_OF_CHAR(int line, int c, std::string val) {
  std::cerr << "syntax error on line: " << line << " reserved word char cannot be for the name of a variable" << line << std::endl;
  exit(25);
}
void errors::E_UNEXPECTED_USE_OF_VOID(int line, int c, std::string val) {
  std::cerr << "syntax error on line " << line << ": reserved word void cannot be for the name of a variable " << std::endl;
  exit(26);
}
void errors::E_NEGATIVE_ARRAY_SIZE(int line, int c, std::string val) {
  std::cerr << "syntax error on line " << line << ": array size must be declared as a positive integer" << std::endl;
  exit(28);
}

void errors::E_ALREADY_DEFINED_VARIABLE_GLOBAL(int line, int col, std::string val) {
    std::cerr << ":" << line << ":" << col << ": error: variable '" << val << "' is already defined globally." << std::endl;
    exit(29);
}

void errors::E_ALREADY_DEFINED_VARIABLE_LOCAL(int line, int col, std::string val) {
    std::cerr << ":" << line << ":" << col << ": error: variable '" << val << "' is already defined locally." << std::endl;
    exit(29);
}
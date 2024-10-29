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
    exit(30);
}

void errors::E_NON_FUNCTION_SCOPE_DECLARATION(int line, int col, std::string val) {
    std::cerr << ":" << line << ":" << col << ": error: symbol '" << val << "' cannot be declared as a unique scope." << std::endl;
    exit(31);
}

void errors::E_DUPLICATE_SYMBOL(int line, int col, std::string val) {
    std::cerr << ":" << line << ":" << col << ": error: duplicate symbol '" << val << "' is already defined in this scope." << std::endl;
    exit(32);
}

void errors::E_NESTED_FUNCTION_NOT_ALLOWED(int line, int col, std::string val) {
    std::cerr << ":" << line << ":" << col << ": error: nested function or procedure '" << val << "' is not allowed under ISO C." << std::endl;
    exit(33);
}





void errors::E_ENOMEM(int line, int col, std::string val) {
    std::cerr << ":" << line << ":" << col << ": error: Cannot allocate memory for '" << val << "'." << std::endl;
    exit(34);
}

void errors::E_EACCES(int line, int col, std::string val) {
    std::cerr << ":" << line << ":" << col << ": error: Permission denied for access to '" << val << "'." << std::endl;
    exit(35);
}

void errors::E_EFAULT(int line, int col, std::string val) {
    std::cerr << ":" << line << ":" << col << ": error: Bad address for '" << val << "'." << std::endl;
    exit(36);
}

void errors::E_ENOTBLK(int line, int col, std::string val) {
    std::cerr << ":" << line << ":" << col << ": error: Block device required for '" << val << "'." << std::endl;
    exit(37);
}

void errors::E_EBUSY(int line, int col, std::string val) {
    std::cerr << ":" << line << ":" << col << ": error: Device or resource busy while accessing '" << val << "'." << std::endl;
    exit(38);
}

void errors::E_EEXIST(int line, int col, std::string val) {
    std::cerr << ":" << line << ":" << col << ": error: File '" << val << "' already exists." << std::endl;
    exit(39);
}

void errors::E_EXDEV(int line, int col, std::string val) {
    std::cerr << ":" << line << ":" << col << ": error: Invalid cross-device link for '" << val << "'." << std::endl;
    exit(40);
}

void errors::E_ENODEV(int line, int col, std::string val) {
    std::cerr << ":" << line << ":" << col << ": error: No such device for '" << val << "'." << std::endl;
    exit(41);
}

void errors::E_ENOTDIR(int line, int col, std::string val) {
    std::cerr << ":" << line << ":" << col << ": error: Not a directory: '" << val << "'." << std::endl;
    exit(42);
}

void errors::E_EISDIR(int line, int col, std::string val) {
    std::cerr << ":" << line << ":" << col << ": error: '" << val << "' is a directory." << std::endl;
    exit(43);
}

void errors::E_EINVAL(int line, int col, std::string val) {
    std::cerr << ":" << line << ":" << col << ": error: Invalid argument: '" << val << "'." << std::endl;
    exit(44);
}

void errors::E_EMFILE(int line, int col, std::string val) {
    std::cerr << ":" << line << ":" << col << ": error: Too many open files for process accessing '" << val << "'." << std::endl;
    exit(45);
}

void errors::E_ENFILE(int line, int col, std::string val) {
    std::cerr << ":" << line << ":" << col << ": error: Too many open files in the system when accessing '" << val << "'." << std::endl;
    exit(46);
}

void errors::E_ENOTTY(int line, int col, std::string val) {
    std::cerr << ":" << line << ":" << col << ": error: Inappropriate ioctl for device '" << val << "'." << std::endl;
    exit(47);
}

void errors::E_ETXTBSY(int line, int col, std::string val) {
    std::cerr << ":" << line << ":" << col << ": error: Text file '" << val << "' busy." << std::endl;
    exit(48);
}

void errors::E_EFBIG(int line, int col, std::string val) {
    std::cerr << ":" << line << ":" << col << ": error: File '" << val << "' too large." << std::endl;
    exit(49);
}

void errors::E_ENOSPC(int line, int col, std::string val) {
    std::cerr << ":" << line << ":" << col << ": error: No space left on device for '" << val << "'." << std::endl;
    exit(50);
}

void errors::E_ESPIPE(int line, int col, std::string val) {
    std::cerr << ":" << line << ":" << col << ": error: Illegal seek on '" << val << "'." << std::endl;
    exit(51);
}

void errors::E_EROFS(int line, int col, std::string val) {
    std::cerr << ":" << line << ":" << col << ": error: Read-only file system for '" << val << "'." << std::endl;
    exit(52);
}

void errors::E_EMLINK(int line, int col, std::string val) {
    std::cerr << ":" << line << ":" << col << ": error: Too many links for '" << val << "'." << std::endl;
    exit(53);
}

void errors::E_EPIPE(int line, int col, std::string val) {
    std::cerr << ":" << line << ":" << col << ": error: Broken pipe while accessing '" << val << "'." << std::endl;
    exit(54);
}

void errors::E_EDOM(int line, int col, std::string val) {
    std::cerr << ":" << line << ":" << col << ": error: Numerical argument out of domain for '" << val << "'." << std::endl;
    exit(55);
}

void errors::E_ERANGE(int line, int col, std::string val) {
    std::cerr << ":" << line << ":" << col << ": error: Numerical result out of range for '" << val << "'." << std::endl;
    exit(56);
}

void errors::E_EAGAIN(int line, int col, std::string val) {
    std::cerr << ":" << line << ":" << col << ": error: Resource temporarily unavailable for '" << val << "'." << std::endl;
    exit(57);
}

void errors::E_EWOULDBLOCK(int line, int col, std::string val) {
    std::cerr << ":" << line << ":" << col << ": error: Operation would block for '" << val << "'." << std::endl;
    exit(58);
}

void errors::E_EINPROGRESS(int line, int col, std::string val) {
    std::cerr << ":" << line << ":" << col << ": error: Operation now in progress for '" << val << "'." << std::endl;
    exit(59);
}

void errors::E_EALREADY(int line, int col, std::string val) {
    std::cerr << ":" << line << ":" << col << ": error: Operation already in progress for '" << val << "'." << std::endl;
    exit(60);
}

void errors::E_ENOTSOCK(int line, int col, std::string val) {
    std::cerr << ":" << line << ":" << col << ": error: Socket operation on non-socket: '" << val << "'." << std::endl;
    exit(61);
}

void errors::E_EMSGSIZE(int line, int col, std::string val) {
    std::cerr << ":" << line << ":" << col << ": error: Message too long for socket operation on '" << val << "'." << std::endl;
    exit(62);
}

void errors::E_EPROTOTYPE(int line, int col, std::string val) {
    std::cerr << ":" << line << ":" << col << ": error: Protocol wrong type for socket on '" << val << "'." << std::endl;
    exit(63);
}

void errors::E_ENOPROTOOPT(int line, int col, std::string val) {
    std::cerr << ":" << line << ":" << col << ": error: Protocol not available for socket option on '" << val << "'." << std::endl;
    exit(64);
}

void errors::E_EPROTONOSUPPORT(int line, int col, std::string val) {
    std::cerr << ":" << line << ":" << col << ": error: Protocol not supported on '" << val << "'." << std::endl;
    exit(65);
}

void errors::E_ESOCKTNOSUPPORT(int line, int col, std::string val) {
    std::cerr << ":" << line << ":" << col << ": error: Socket type not supported for '" << val << "'." << std::endl;
    exit(66);
}

void errors::E_EOPNOTSUPP(int line, int col, std::string val) {
    std::cerr << ":" << line << ":" << col << ": error: Operation not supported on '" << val << "'." << std::endl;
    exit(67);
}

void errors::E_EPFNOSUPPORT(int line, int col, std::string val) {
    std::cerr << ":" << line << ":" << col << ": error: Protocol family not supported for '" << val << "'." << std::endl;
    exit(68);
}

void errors::E_EAFNOSUPPORT(int line, int col, std::string val) {
    std::cerr << ":" << line << ":" << col << ": error: Address family not supported by protocol for '" << val << "'." << std::endl;
    exit(69);
}

void errors::E_EADDRINUSE(int line, int col, std::string val) {
    std::cerr << ":" << line << ":" << col << ": error: Address already in use: '" << val << "'." << std::endl;
    exit(70);
}

void errors::E_EADDRNOTAVAIL(int line, int col, std::string val) {
    std::cerr << ":" << line << ":" << col << ": error: Cannot assign requested address: '" << val << "'." << std::endl;
    exit(71);
}

// Continue in this pattern for the remaining error codes.

void errors::E_ENETDOWN(int line, int col, std::string val) {
    std::cerr << ":" << line << ":" << col << ": error: Network is down for '" << val << "'." << std::endl;
    exit(72);
}

void errors::E_ENETUNREACH(int line, int col, std::string val) {
    std::cerr << ":" << line << ":" << col << ": error: Network is unreachable for '" << val << "'." << std::endl;
    exit(73);
}

void errors::E_ENETRESET(int line, int col, std::string val) {
    std::cerr << ":" << line << ":" << col << ": error: Network dropped connection on reset for '" << val << "'." << std::endl;
    exit(74);
}

void errors::E_ECONNABORTED(int line, int col, std::string val) {
    std::cerr << ":" << line << ":" << col << ": error: Software caused connection abort on '" << val << "'." << std::endl;
    exit(75);
}

void errors::E_ECONNRESET(int line, int col, std::string val) {
    std::cerr << ":" << line << ":" << col << ": error: Connection reset by peer on '" << val << "'." << std::endl;
    exit(76);
}

void errors::E_ENOBUFS(int line, int col, std::string val) {
    std::cerr << ":" << line << ":" << col << ": error: No buffer space available for '" << val << "'." << std::endl;
    exit(77);
}

void errors::E_EISCONN(int line, int col, std::string val) {
    std::cerr << ":" << line << ":" << col << ": error: Transport endpoint is already connected for '" << val << "'." << std::endl;
    exit(78);
}

void errors::E_ENOTCONN(int line, int col, std::string val) {
    std::cerr << ":" << line << ":" << col << ": error: Transport endpoint is not connected for '" << val << "'." << std::endl;
    exit(79);
}

void errors::E_EDESTADDRREQ(int line, int col, std::string val) {
    std::cerr << ":" << line << ":" << col << ": error: Destination address required for '" << val << "'." << std::endl;
    exit(80);
}

void errors::E_ESHUTDOWN(int line, int col, std::string val) {
    std::cerr << ":" << line << ":" << col << ": error: Cannot send after transport endpoint shutdown for '" << val << "'." << std::endl;
    exit(81);
}

void errors::E_ETOOMANYREFS(int line, int col, std::string val) {
    std::cerr << ":" << line << ":" << col << ": error: Too many references: cannot splice for '" << val << "'." << std::endl;
    exit(82);
}

void errors::E_ETIMEDOUT(int line, int col, std::string val) {
    std::cerr << ":" << line << ":" << col << ": error: Connection timed out for '" << val << "'." << std::endl;
    exit(83);
}

void errors::E_ECONNREFUSED(int line, int col, std::string val) {
    std::cerr << ":" << line << ":" << col << ": error: Connection refused by '" << val << "'." << std::endl;
    exit(84);
}

void errors::E_ELOOP(int line, int col, std::string val) {
    std::cerr << ":" << line << ":" << col << ": error: Too many levels of symbolic links for '" << val << "'." << std::endl;
    exit(85);
}

void errors::E_ENAMETOOLONG(int line, int col, std::string val) {
    std::cerr << ":" << line << ":" << col << ": error: File name too long: '" << val << "'." << std::endl;
    exit(86);
}

void errors::E_EHOSTDOWN(int line, int col, std::string val) {
    std::cerr << ":" << line << ":" << col << ": error: Host is down for '" << val << "'." << std::endl;
    exit(87);
}

void errors::E_EHOSTUNREACH(int line, int col, std::string val) {
    std::cerr << ":" << line << ":" << col << ": error: No route to host for '" << val << "'." << std::endl;
    exit(88);
}

void errors::E_ENOTEMPTY(int line, int col, std::string val) {
    std::cerr << ":" << line << ":" << col << ": error: Directory not empty: '" << val << "'." << std::endl;
    exit(89);
}

void errors::E_EPROCLIM(int line, int col, std::string val) {
    std::cerr << ":" << line << ":" << col << ": error: Too many processes for user when accessing '" << val << "'." << std::endl;
    exit(90);
}

void errors::E_EUSERS(int line, int col, std::string val) {
    std::cerr << ":" << line << ":" << col << ": error: Too many users for quota system related to '" << val << "'." << std::endl;
    exit(91);
}

void errors::E_EDQUOT(int line, int col, std::string val) {
    std::cerr << ":" << line << ":" << col << ": error: Disk quota exceeded for '" << val << "'." << std::endl;
    exit(92);
}

void errors::E_ESTALE(int line, int col, std::string val) {
    std::cerr << ":" << line << ":" << col << ": error: Stale file handle for '" << val << "'." << std::endl;
    exit(93);
}

void errors::E_EREMOTE(int line, int col, std::string val) {
    std::cerr << ":" << line << ":" << col << ": error: Object is remote: '" << val << "'." << std::endl;
    exit(94);
}

void errors::E_ENOLCK(int line, int col, std::string val) {
    std::cerr << ":" << line << ":" << col << ": error: No locks available for '" << val << "'." << std::endl;
    exit(95);
}

void errors::E_ENOSYS(int line, int col, std::string val) {
    std::cerr << ":" << line << ":" << col << ": error: Function not implemented for '" << val << "'." << std::endl;
    exit(96);
}

void errors::E_ENOTSUP(int line, int col, std::string val) {
    std::cerr << ":" << line << ":" << col << ": error: Operation not supported for '" << val << "'." << std::endl;
    exit(97);
}

void errors::E_EILSEQ(int line, int col, std::string val) {
    std::cerr << ":" << line << ":" << col << ": error: Invalid or incomplete multibyte or wide character in '" << val << "'." << std::endl;
    exit(98);
}

void errors::E_EBADMSG(int line, int col, std::string val) {
    std::cerr << ":" << line << ":" << col << ": error: Bad message for '" << val << "'." << std::endl;
    exit(99);
}

void errors::E_ECANCELED(int line, int col, std::string val) {
    std::cerr << ":" << line << ":" << col << ": error: Operation canceled for '" << val << "'." << std::endl;
    exit(100);
}

void errors::E_ENOTRECOVERABLE(int line, int col, std::string val) {
    std::cerr << ":" << line << ":" << col << ": error: State not recoverable for '" << val << "'." << std::endl;
    exit(101);
}

void errors::E_EOWNERDEAD(int line, int col, std::string val) {
    std::cerr << ":" << line << ":" << col << ": error: Owner died for resource '" << val << "'." << std::endl;
    exit(102);
}

void errors::E_EMULTIHOP(int line, int col, std::string val) {
    std::cerr << ":" << line << ":" << col << ": error: Multihop attempted for '" << val << "'." << std::endl;
    exit(103);
}

void errors::E_ENOLINK(int line, int col, std::string val) {
    std::cerr << ":" << line << ":" << col << ": error: Link has been severed for '" << val << "'." << std::endl;
    exit(104);
}

void errors::E_ENOMEDIUM(int line, int col, std::string val) {
    std::cerr << ":" << line << ":" << col << ": error: No medium found for '" << val << "'." << std::endl;
    exit(105);
}

void errors::E_EMEDIUMTYPE(int line, int col, std::string val) {
    std::cerr << ":" << line << ":" << col << ": error: Wrong medium type for '" << val << "'." << std::endl;
    exit(106);
}

void errors::E_EKEYEXPIRED(int line, int col, std::string val) {
    std::cerr << ":" << line << ":" << col << ": error: Key expired for '" << val << "'." << std::endl;
    exit(107);
}

void errors::E_EKEYREVOKED(int line, int col, std::string val) {
    std::cerr << ":" << line << ":" << col << ": error: Key revoked for '" << val << "'." << std::endl;
    exit(108);
}

void errors::E_EKEYREJECTED(int line, int col, std::string val) {
    std::cerr << ":" << line << ":" << col << ": error: Key rejected by service for '" << val << "'." << std::endl;
    exit(109);
}

void errors::E_ERFKILL(int line, int col, std::string val) {
    std::cerr << ":" << line << ":" << col << ": error: Operation not possible due to RF-kill for '" << val << "'." << std::endl;
    exit(110);
}

void errors::E_EHWPOISON(int line, int col, std::string val) {
    std::cerr << ":" << line << ":" << col << ": error: Memory page has hardware error for '" << val << "'." << std::endl;
    exit(111);
}

void errors::E_EDEADLK(int line, int col, std::string val) {
    std::cerr << ":" << line << ":" << col << ": error: Resource deadlock avoided while accessing '" << val << "'." << std::endl;
    exit(112);
}
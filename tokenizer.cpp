//
// Created by Ethan Edwards on 9/12/2024.
//

#include "tokenizer.h"

void tokenizer::tokenize(std::string filename){
    // Input
    std::ifstream in(filename, std::ios::binary);
    std::string str;
    char curr;
    // Tracking
    int line = 1;
    int col = 1;
    int entered_at = 0;
    bool handle_str = false; // Handle string literals
    bool handle_chr = false; // Handle character literals
    bool ignore_slc = false; // Single line comment
    bool ignore_mlc = false; // Multi line comment
    bool is_float = false; // Floats
    std::vector<char> structure; // Nesting structures

    while (!in.eof()){
        in.get(curr);
        if (ignore_slc){
            if (curr == '\n'){
                ignore_slc = false;
                this->out << "\n"; // OUTPUT TAG
                this->path->add_node(tokens::NEWLINE);
                line++;
            }
            else{
                this->out << " "; // OUTPUT TAG
            }
        }
        else if (ignore_mlc){
            if (curr == '*' && in.peek() == '/'){
                ignore_mlc = false;
                in.get(curr); // Consume '/'
                this->out << "  "; // OUTPUT TAG
            }
            else if (curr == '\n'){
                line++;
                this->out << "\n"; // OUTPUT TAG
                this->path->add_node(tokens::NEWLINE);
            }
            else{
                this->out << " "; // OUTPUT TAG
            }
        }
        else if (handle_str){
            if (curr == '"'){
                handle_str = false;
                this->out << curr; // OUTPUT TAG
                this->path->add_node(tokens::STRING_LITERAL, str);
                this->path->add_node(tokens::single_char(curr));
                str = "";
            }
            else{
                this->out << curr; // OUTPUT TAG
                str += curr;
            }
        }
        else if (handle_chr){
            if (curr == '\''){
                handle_chr = false;
                this->out << curr; // OUTPUT TAG
                this->path->add_node(tokens::CHAR_LITERAL, str);
                this->path->add_node(tokens::single_char(curr));
                str = "";
            }
            else{
                this->out << curr; // OUTPUT TAG
                str += curr;
            }
        }
        else{
            switch(curr){
                // String literals
                case '"':
                    this->path->add_node(tokens::single_char(curr));
                    handle_str = true;
                    entered_at = line;
                    this->out << curr; // OUTPUT TAG
                    break;

                // Character literals
                case '\'':
                    this->path->add_node(tokens::single_char(curr));
                    handle_chr = true;
                    entered_at = line;
                    this->out << curr; // OUTPUT TAG
                    break;

                // Ignored characters
                case '\n': // Newline
                    this->out << "\n"; // OUTPUT TAG
                    this->path->add_node(tokens::NEWLINE);
                    line++;
                    col = 0;
                    break;

                    case ' ': case '\t': case '\f': case '\r': case '\v': // Whitespace
                    this->out << curr; // OUTPUT TAG
                    break;

                // Nesting structures
                case '{':
                    this->out << curr; // OUTPUT TAG
                    this->path->add_node(tokens::OPEN_BRACE);
                    structure.push_back(curr);
                    break;

                case '[':
                    this->out << curr; // OUTPUT TAG
                    this->path->add_node(tokens::OPEN_BRACKET);
                    structure.push_back(curr);
                    break;

                case '(':
                    this->out << curr; // OUTPUT TAG
                    this->path->add_node(tokens::OPEN_PAREN);
                    structure.push_back(curr);
                    break;

                case '}':
                    if (structure.back() != '{'){
                        errors::EXPECTED_END(line, curr);
                    }
                    structure.pop_back();
                    this->out << curr; // OUTPUT TAG
                    this->path->add_node(tokens::CLOSE_BRACE);
                    break;

                case ']':
                    if (structure.back() != '['){
                        errors::EXPECTED_END(line, curr);
                    }
                    structure.pop_back();
                    this->out << curr; // OUTPUT TAG
                    this->path->add_node(tokens::CLOSE_BRACKET);
                    break;

                case ')':
                    if (structure.back() != '('){
                        errors::EXPECTED_END(line, curr);
                    }
                    structure.pop_back();
                    this->out << curr; // OUTPUT TAG
                    this->path->add_node(tokens::CLOSE_PAREN);
                    break;

                // Characters with 2+ secondaries
                case '+':
                    if (in.peek() == '+'){
                        str = curr;
                        in.get(curr); // Consume secondary
                        str += curr;
                        this->out << str; // OUTPUT TAG
                        this->path->add_node(tokens::PLUS_PLUS);
                        str = "";
                    } else if (in.peek() == '='){
                        str = curr;
                        in.get(curr); // Consume secondary
                        str += curr;
                        this->out << str; // OUTPUT TAG
                        this->path->add_node(tokens::PLUS_EQUALS);
                        str = "";
                    } else{
                        this->out << curr; // OUTPUT TAG
                        this->path->add_node(tokens::single_char(curr));
                    }
                    break;

                case '-':
                    if (in.peek() == '-'){
                        str = curr;
                        in.get(curr); // Consume secondary
                        str += curr;
                        this->out << str; // OUTPUT TAG
                        this->path->add_node(tokens::MINUS_MINUS);
                        str = "";
                    } else if (in.peek() == '='){
                        str = curr;
                        in.get(curr); // Consume secondary
                        str += curr;
                        this->out << str; // OUTPUT TAG
                        this->path->add_node(tokens::MINUS_EQUALS);
                        str = "";
                    } else if (in.peek() == '>'){
                        str = curr;
                        in.get(curr); // Consume secondary
                        str += curr;
                        this->out << str; // OUTPUT TAG
                        this->path->add_node(tokens::RIGHT_SLIM_ARROW);
                        str = "";
                    } else if (is_num(in.peek())){
                        str = curr;
                        bool dot = false;
                        while (is_num(in.peek()) || in.peek() == '.'){
                            in.get(curr);
                            if (curr == '.'){
                                dot = true;
                            }
                            if ((curr == '.' && !is_num(in.peek())) || (curr == '.' && dot)){
                                errors::EXPECTED_VALUE(line, curr);
                            }
                            str += curr;
                        }
                        this->out << str; // OUTPUT TAG
                        if (dot){
                            this->path->add_node(tokens::FLOAT_AS_STRING, str);
                        }
                        else {
                            this->path->add_node(tokens::INT_AS_STRING, str);
                        }
                    }
                    else{
                        this->out << curr; // OUTPUT TAG
                        this->path->add_node(tokens::single_char(curr));
                    }
                    break;

                case '&':
                    if (in.peek() == '&'){
                        str = curr;
                        in.get(curr); // Consume secondary
                        str += curr;
                        this->out << str; // OUTPUT TAG
                        this->path->add_node(tokens::BOOLEAN_AND);
                        str = "";
                    } else if (in.peek() == '='){
                        str = curr;
                        in.get(curr); // Consume secondary
                        str += curr;
                        this->out << str; // OUTPUT TAG
                        this->path->add_node(tokens::AND_EQUALS);
                        str = "";
                    } else{
                        this->out << curr; // OUTPUT TAG
                        this->path->add_node(tokens::single_char(curr));
                    }
                    break;

                case '|':
                    if (in.peek() == '|'){
                        str = curr;
                        in.get(curr); // Consume secondary
                        str += curr;
                        this->out << str; // OUTPUT TAG
                        this->path->add_node(tokens::BOOLEAN_OR);
                        str = "";
                    } else if (in.peek() == '='){
                        str = curr;
                        in.get(curr); // Consume secondary
                        str += curr;
                        this->out << str; // OUTPUT TAG
                        this->path->add_node(tokens::OR_EQUALS);
                        str = "";
                    } else{
                        this->out << curr; // OUTPUT TAG
                        this->path->add_node(tokens::single_char(curr));
                    }
                    break;

                // Characters with 1 secondary
                case '!':
                    if (in.peek() == '='){
                        str = curr;
                        in.get(curr); // Consume secondary
                        str += curr;
                        this->out << str; // OUTPUT TAG
                        this->path->add_node(tokens::NOT_EQUALS);
                        str = "";
                    } else{
                        this->out << curr; // OUTPUT TAG
                        this->path->add_node(tokens::single_char(curr));
                    }
                    break;

                case '%':
                    if (in.peek() == '='){
                        str = curr;
                        in.get(curr); // Consume secondary
                        str += curr;
                        this->out << str; // OUTPUT TAG
                        this->path->add_node(tokens::MOD_EQUALS);
                        str = "";
                    } else{
                        this->out << curr; // OUTPUT TAG
                        this->path->add_node(tokens::single_char(curr));
                    }
                    break;

                case '*':
                    if (in.peek() == '='){
                        str = curr;
                        in.get(curr); // Consume secondary
                        str += curr;
                        this->out << str; // OUTPUT TAG
                        this->path->add_node(tokens::TIMES_EQUALS);
                        str = "";
                    } else{
                        this->out << curr; // OUTPUT TAG
                        this->path->add_node(tokens::single_char(curr));
                    }
                    break;

                case '=':
                    if (in.peek() == '='){
                        str = curr;
                        in.get(curr); // Consume secondary
                        str += curr;
                        this->out << str; // OUTPUT TAG
                        this->path->add_node(tokens::EQUALS_EQUALS);
                        str = "";
                    } else{
                        this->out << curr; // OUTPUT TAG
                        this->path->add_node(tokens::single_char(curr));
                    }
                    break;

                case '^':
                    if (in.peek() == '='){
                        str = curr;
                        in.get(curr); // Consume secondary
                        str += curr;
                        this->out << str; // OUTPUT TAG
                        this->path->add_node(tokens::XOR_EQUALS);
                        str = "";
                    } else{
                        this->out << curr; // OUTPUT TAG
                        this->path->add_node(tokens::single_char(curr));
                    }
                    break;

                case '#':
                    if (in.peek() == '#'){
                        str = curr;
                        in.get(curr); // Consume secondary
                        str += curr;
                        this->out << str; // OUTPUT TAG
                        this->path->add_node(tokens::TOKEN_PASTE);
                        str = "";
                    } else{
                        this->out << curr; // OUTPUT TAG
                        this->path->add_node(tokens::single_char(curr));
                    }
                    break;

                // Arrow operators
                case '>':
                    if (in.peek() == '=') {
                        str = curr;
                        in.get(curr); // Consume secondary
                        str += curr;
                        this->out << str; // OUTPUT TAG
                        this->path->add_node(tokens::GREATER_EQUALS);
                        str = "";
                    } else if (in.peek() == '>') {
                        str = curr;
                        in.get(curr); // Consume secondary
                        str += curr;
                        if (in.peek() == '=') {
                            in.get(curr); // Consume secondary
                            str += curr;
                            this->out << str; // OUTPUT TAG
                            this->path->add_node(tokens::RIGHT_SHIFT_EQUALS);
                            str = "";
                            break;
                        }
                        this->out << str; // OUTPUT TAG
                        this->path->add_node(tokens::RIGHT_SHIFT);
                        str = "";
                    } else {
                        this->out << curr; // OUTPUT TAG
                        this->path->add_node(tokens::single_char(curr));
                    }
                    break;

                case '<':
                    if (in.peek() == '=') {
                        str = curr;
                        in.get(curr); // Consume secondary
                        str += curr;
                        this->out << str; // OUTPUT TAG
                        this->path->add_node(tokens::LESS_EQUALS);
                        str = "";
                    } else if (in.peek() == '<') {
                        str = curr;
                        in.get(curr); // Consume secondary
                        str += curr;
                        if (in.peek() == '=') {
                            in.get(curr); // Consume secondary
                            str += curr;
                            this->out << str; // OUTPUT TAG
                            this->path->add_node(tokens::LEFT_SHIFT_EQUALS);
                            str = "";
                            break;
                        }
                        this->out << str; // OUTPUT TAG
                        this->path->add_node(tokens::LEFT_SHIFT);
                        str = "";
                    } else {
                        this->out << curr; // OUTPUT TAG
                        this->path->add_node(tokens::single_char(curr));
                    }
                    break;

                case '/':
                    if (in.peek() == '/') {
                        in.get(curr); // Consume secondary
                        ignore_slc = true;
                        entered_at = line;
                        this->out << "  "; // OUTPUT TAG
                    } else if (in.peek() == '*') {
                        in.get(curr); // Consume secondary
                        ignore_mlc = true;
                        entered_at = line;
                        this->out << "  "; // OUTPUT TAG
                    } else {
                        this->out << curr; // OUTPUT TAG
                        this->path->add_node(tokens::single_char(curr));
                    }
                    break;

                case '.':
                    if (is_num(in.peek())){
                        errors::UNEXPECTED_VALUE(line, curr);
                    }
                    else if (in.peek() == '.'){
                        str = curr;
                        in.get(curr); // Consume secondary
                        str += curr;
                        if (in.peek() == '.'){
                            in.get(curr); // Consume tertiary
                            str += curr;
                            if (in.peek() == '.'){
                                errors::UNEXPECTED_TOKEN(line, curr);
                            }
                            this->out << str; // OUTPUT TAG
                            this->path->add_node(tokens::ELLIPSIS);
                            str = "";
                            break;
                        } else{
                            errors::UNEXPECTED_TOKEN(line, curr);
                        }
                    }
                    else{
                        this->out << curr; // OUTPUT TAG
                        this->path->add_node(tokens::single_char(curr));
                    }
                    break;

                case ':':
                    if (in.peek() == ':'){
                        str = curr;
                        in.get(curr); // Consume secondary
                        str += curr;
                        this->out << str; // OUTPUT TAG
                        this->path->add_node(tokens::SCOPE);
                        str = "";
                    }
                    else{
                        this->out << curr; // OUTPUT TAG
                        this->path->add_node(tokens::single_char(curr));
                    }
                    break;

                case ',': case ';': case '?': case '~': case '`':
                    this->out << curr; // OUTPUT TAG
                    this->path->add_node(tokens::single_char(curr));
                    break;

                case 'L': // Can be used as an identifier or as a prefix for a string literal
                    this->out << curr; // OUTPUT TAG
                    this->path->add_node(tokens::LITERAL_DEF);
                    break;

                case '_': case 'a': case 'b': case 'c': case 'd': case 'e':
                case 'f': case 'g': case 'h': case 'i': case 'j': case 'k':
                case 'l': case 'm': case 'n': case 'o': case 'p': case 'q':
                case 'r': case 's': case 't': case 'u': case 'v': case 'w':
                case 'x': case 'y': case 'z': case 'A': case 'B': case 'C':
                case 'D': case 'E': case 'F': case 'G': case 'H': case 'I':
                case 'J': case 'K': case 'M': case 'N': case 'O': case 'P':
                case 'Q': case 'R': case 'S': case 'T': case 'U': case 'V':
                case 'W': case 'X': case 'Y': case 'Z':
                    str = curr;
                    while (is_alpha(in.peek()) || is_num(in.peek()) || in.peek() == '_'){
                        in.get(curr);
                        str += curr;
                    }
                    this->out << str; // OUTPUT TAG
                    this->path->add_node(tokens::TOKEN_AS_STRING, str);
                    str = "";
                    break;

                case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
                    str = curr;
                    is_float = false;
                    while (is_num(in.peek()) || in.peek() == '.'){
                        in.get(curr);
                        if (curr == '.'){
                            is_float = true;
                        }
                        if ((curr == '.' && !is_num(in.peek())) || (curr == '.' && is_float)){
                            errors::EXPECTED_VALUE(line, curr);
                        }
                        str += curr;
                    }
                    this->out << str; // OUTPUT TAG
                    if (is_float){
                        this->path->add_node(tokens::FLOAT_AS_STRING, str);
                    }
                    else {
                        this->path->add_node(tokens::INT_AS_STRING, str);
                    }
                    str = "";
                    is_float = false;
                    break;

                default:
                    break;
            }
        }
    }
    if (handle_str){
        errors::UNTERM_STRING(entered_at, curr);
    }
    if (handle_chr){
        errors::UNTERM_CHAR(entered_at, curr);
    }
    if (ignore_mlc){
        errors::UNTERM_COMMENT(entered_at, curr);
    }
    if (ignore_slc){
        errors::UNEXPECTED_END_OF_FILE(entered_at, curr);
    }
    in.close();
    out.close();
}

// Helpers
bool tokenizer::is_num(char c){
    return c >= '0' && c <= '9';
}

bool tokenizer::is_alpha(char c){
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

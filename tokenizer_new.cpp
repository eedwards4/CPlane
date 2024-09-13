//
// Created by Ethan Edwards on 9/12/2024.
//

#include "tokenizer_new.h"

void tokenizer_new::tokenize(std::string filename){
    // Input
    std::ifstream in(filename);
    std::string str;
    char curr;
    // Tracking
    int line = 1;
    int col = 1;
    int entered_at = 0;
    bool handle_str = false; // Handle string literals
    bool ignore_slc = false; // Single line comment
    bool ignore_mlc = false; // Multi line comment
    std::vector<char> structure; // Nesting structures

    while (!in.eof()){
        in.get(curr);
        if (ignore_slc){
            if (curr == '\n'){
                ignore_slc = false;
                this->out << "\n"; // OUTPUT TAG
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
            else{
                this->out << " "; // OUTPUT TAG
            }
        }
        else if (handle_str){
            if (curr == '"'){
                handle_str = false;
                this->out << curr; // OUTPUT TAG
                this->path->add_node(token(str, 2));
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
                    handle_str = true;
                    entered_at = line;
                    this->out << curr; // OUTPUT TAG
                    break;

                // Ignored characters
                case '\n':
                    this->out << "\n"; // OUTPUT TAG
                    this->path->add_node(token("\n", 0));
                    line++;
                    col = 0;
                    break;

                case ' ': case '\t': case '\r': case '\f': case '\v': // Whitespace
                    this->out << curr; // OUTPUT TAG
                    break;

                // Nesting structures
                case '{': case '[': case '(':
                    this->out << curr; // OUTPUT TAG
                    this->path->add_node(token(std::string(1, curr), 0));
                    structure.push_back(curr);
                    break;

                case '}':
                    if (structure.back() != '{'){
                        errors::EXPECTED_END(line, curr);
                    }
                    structure.pop_back();
                    this->out << curr; // OUTPUT TAG
                    this->path->add_node(token(std::string(1, curr), 0));
                    break;

                case ']':
                    if (structure.back() != '['){
                        errors::EXPECTED_END(line, curr);
                    }
                    structure.pop_back();
                    this->out << curr; // OUTPUT TAG
                    this->path->add_node(token(std::string(1, curr), 0));
                    break;

                case ')':
                    if (structure.back() != '('){
                        errors::EXPECTED_END(line, curr);
                    }
                    structure.pop_back();
                    this->out << curr; // OUTPUT TAG
                    this->path->add_node(token(std::string(1, curr), 0));
                    break;

                // Characters with 2+ secondaries
                case '+':
                    if (in.peek() == '+' || in.peek() == '='){
                        str = curr;
                        in.get(curr); // Consume secondary
                        str += curr;
                        this->out << str; // OUTPUT TAG
                        this->path->add_node(token(str, 1));
                        str = "";
                    } else{
                        this->out << curr; // OUTPUT TAG
                        this->path->add_node(token(std::string(1, curr), 0));
                    }
                    break;

                case '-':
                    if (in.peek() == '-' || in.peek() == '=' || in.peek() == '>'){
                        str = curr;
                        in.get(curr); // Consume secondary
                        str += curr;
                        this->out << str; // OUTPUT TAG
                        this->path->add_node(token(str, 1));
                        str = "";
                    } else{
                        this->out << curr; // OUTPUT TAG
                        this->path->add_node(token(std::string(1, curr), 0));
                    }
                    break;

                case '&':
                    if (in.peek() == '&' || in.peek() == '='){
                        str = curr;
                        in.get(curr); // Consume secondary
                        str += curr;
                        this->out << str; // OUTPUT TAG
                        this->path->add_node(token(str, 1));
                        str = "";
                    } else{
                        this->out << curr; // OUTPUT TAG
                        this->path->add_node(token(std::string(1, curr), 0));
                    }
                    break;

                case '|':
                    if (in.peek() == '|' || in.peek() == '='){
                        str = curr;
                        in.get(curr); // Consume secondary
                        str += curr;
                        this->out << str; // OUTPUT TAG
                        this->path->add_node(token(str, 1));
                        str = "";
                    } else{
                        this->out << curr; // OUTPUT TAG
                        this->path->add_node(token(std::string(1, curr), 0));
                    }
                    break;

                // Characters with 1 secondary
                case '!':
                    if (in.peek() == '='){
                        str = curr;
                        in.get(curr); // Consume secondary
                        str += curr;
                        this->out << str; // OUTPUT TAG
                        this->path->add_node(token(str, 1));
                        str = "";
                    } else{
                        this->out << curr; // OUTPUT TAG
                        this->path->add_node(token(std::string(1, curr), 0));
                    }
                    break;

                case '%':
                    if (in.peek() == '='){
                        str = curr;
                        in.get(curr); // Consume secondary
                        str += curr;
                        this->out << str; // OUTPUT TAG
                        this->path->add_node(token(str, 1));
                        str = "";
                    } else{
                        this->out << curr; // OUTPUT TAG
                        this->path->add_node(token(std::string(1, curr), 0));
                    }
                    break;

                case '*':
                    if (in.peek() == '='){
                        str = curr;
                        in.get(curr); // Consume secondary
                        str += curr;
                        this->out << str; // OUTPUT TAG
                        this->path->add_node(token(str, 1));
                        str = "";
                    } else{
                        this->out << curr; // OUTPUT TAG
                        this->path->add_node(token(std::string(1, curr), 0));
                    }
                    break;

                case '=':
                    if (in.peek() == '='){
                        str = curr;
                        in.get(curr); // Consume secondary
                        str += curr;
                        this->out << str; // OUTPUT TAG
                        this->path->add_node(token(str, 1));
                        str = "";
                    } else{
                        this->out << curr; // OUTPUT TAG
                        this->path->add_node(token(std::string(1, curr), 0));
                    }
                    break;

                case '^':
                    if (in.peek() == '='){
                        str = curr;
                        in.get(curr); // Consume secondary
                        str += curr;
                        this->out << str; // OUTPUT TAG
                        this->path->add_node(token(str, 1));
                        str = "";
                    } else{
                        this->out << curr; // OUTPUT TAG
                        this->path->add_node(token(std::string(1, curr), 0));
                    }
                    break;

                case '#':
                    if (in.peek() == '#'){
                        str = curr;
                        in.get(curr); // Consume secondary
                        str += curr;
                        this->out << str; // OUTPUT TAG
                        this->path->add_node(token(str, 1));
                        str = "";
                    } else{
                        this->out << curr; // OUTPUT TAG
                        this->path->add_node(token(std::string(1, curr), 0));
                    }
                    break;

                // Arrow operators
                case '>':
                    if (in.peek() == '=') {
                        str = curr;
                        in.get(curr); // Consume secondary
                        str += curr;
                        this->out << str; // OUTPUT TAG
                        this->path->add_node(token(str, 1));
                        str = "";
                    } else if (in.peek() == '>') {
                        str = curr;
                        in.get(curr); // Consume secondary
                        str += curr;
                        if (in.peek() == '=') {
                            in.get(curr); // Consume secondary
                            str += curr;
                        }
                        this->out << str; // OUTPUT TAG
                        this->path->add_node(token(str, 1));
                        str = "";
                    } else {
                        this->out << curr; // OUTPUT TAG
                        this->path->add_node(token(std::string(1, curr), 0));
                    }

                case '<':
                    if (in.peek() == '=') {
                        str = curr;
                        in.get(curr); // Consume secondary
                        str += curr;
                        this->out << str; // OUTPUT TAG
                        this->path->add_node(token(str, 1));
                        str = "";
                    } else if (in.peek() == '<') {
                        str = curr;
                        in.get(curr); // Consume secondary
                        str += curr;
                        if (in.peek() == '=') {
                            in.get(curr); // Consume secondary
                            str += curr;
                        }
                        this->out << str; // OUTPUT TAG
                        this->path->add_node(token(str, 1));
                        str = "";
                    } else {
                        this->out << curr; // OUTPUT TAG
                        this->path->add_node(token(std::string(1, curr), 0));
                    }

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
                        this->path->add_node(token(std::string(1, curr), 0));
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
                        }
                        this->out << str; // OUTPUT TAG
                        this->path->add_node(token(str, 1));
                        str = "";
                    }
                    else{
                        this->out << curr; // OUTPUT TAG
                        this->path->add_node(token(std::string(1, curr), 0));
                    }
                    break;

                case ':':
                    if (in.peek() == ':'){
                        str = curr;
                        in.get(curr); // Consume secondary
                        str += curr;
                        this->out << str; // OUTPUT TAG
                        this->path->add_node(token(str, 1));
                        str = "";
                    }
                    else{
                        this->out << curr; // OUTPUT TAG
                        this->path->add_node(token(std::string(1, curr), 0));
                    }
                    break;

                case ',': case ';': case '?': case '~': case '`':
                    this->out << curr; // OUTPUT TAG
                    this->path->add_node(token(std::string(1, curr), 0));
                    break;

                case 'L': // Can be used as an identifier or as a prefix for a string literal
                    this->out << curr; // OUTPUT TAG
                    this->path->add_node(token(std::string(1, curr), 0));
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
                    this->path->add_node(token(str, 1));
                    str = "";
                    break;

                case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
                    str = curr;
                    while (is_num(in.peek()) || in.peek() == '.'){
                        in.get(curr);
                        if (curr == '.' && !is_num(in.peek())){
                            errors::EXPECTED_VALUE(line, curr);
                        }
                        str += curr;
                    }
                    this->out << str; // OUTPUT TAG
                    this->path->add_node(token(str, 1));
                    str = "";
                    break;

                default:
                    break;
            }
        }
    }
    if (handle_str){
        errors::UNTERM_STRING(line, curr);
    }
    if (ignore_mlc){
        errors::UNTERM_COMMENT(line, curr);
    }
    if (ignore_slc){
        errors::UNEXPECTED_END_OF_FILE(line, curr);
    }
    in.close();
    out.close();
}

// Helpers
bool tokenizer_new::is_num(char c){
    return c >= '0' && c <= '9';
}

bool tokenizer_new::is_alpha(char c){
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

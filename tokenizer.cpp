//
// Created by Ethan Edwards on 9/4/24.
//

#include "tokenizer.h"

std::vector<token> tokenizer::tokenize(std::string filename) {
    std::vector<token> tokens;
    std::ifstream in(filename);
    std::string str;
    char curr;
    int line = 0;
    bool ignore_line = false;
    bool string_handling = false;
    bool ignore_multiline = false;

    while (!in.eof()) {
        in.get(curr);
        if (ignore_line){
            if (curr == '\n'){
                ignore_line = false;
                this->out << "\n"; // TEMPORARY FOR ASSIGNMENT ONE
                line++;
            }
            else{
                this->out << " "; // TEMPORARY FOR ASSIGNMENT ONE. Output a space to preserve original file formatting
            }
        } else if (ignore_multiline){
            if (curr == '*' && in.peek() == '/'){
                in >> curr; // Remove the next character from the stream
                this->out << " "; // TEMPORARY FOR ASSIGNMENT ONE. Output an extra space
                ignore_multiline = false;
            }
            else if (curr == '\n'){
                tokens.push_back(token(std::string(1, curr)));
                this->out << "\n"; // TEMPORARY FOR ASSIGNMENT ONE
                line++;
            }
            else{
                this->out << " "; // TEMPORARY FOR ASSIGNMENT ONE. Output a space to preserve original file formatting
            }
        } else if (string_handling) {
            if (curr == '"') {
                string_handling = false;
                tokens.push_back(token(str));
                str = "";
            } else {
                str += curr;
            }
            this->out << curr; // TEMPORARY FOR ASSIGNMENT ONE. Output the character to the output file
        } else{
            switch(curr){
                // String literals
                case '"':
                    string_handling = true;
                    this->out << curr; // TEMPORARY FOR ASSIGNMENT ONE
                    break;

                // BEGIN IGNORED CHARACTERS
                case '\n':
                    this->out << "\n"; // TEMPORARY FOR ASSIGNMENT ONE
                    line++;
                    break;

                case ' ': case '\t': case '\r': case '\f': case '\v': // Whitespace
                    this->out << curr; // TEMPORARY FOR ASSIGNMENT ONE
                    break;

                // END IGNORED CHARACTERS
                case '{': case '}': case '[': case ']': case '(': case ')': case ';':
                    tokens.push_back(token(std::string(1, curr)));
                    this->out << curr; // TEMPORARY FOR ASSIGNMENT ONE
                    break;

                // Characters with 2+ secondaries
                case '+':
                    if (in.peek() == '+' || in.peek() == '='){
                        str = curr;
                        in >> curr; // Remove the next character from the stream
                        str += curr;
                        tokens.push_back(token(str));
                        str = "";
                    }
                    else{
                        tokens.push_back(token(std::string(1, curr)));
                    }
                    this->out << curr; // TEMPORARY FOR ASSIGNMENT ONE
                    break;

                case '-':
                    if (in.peek() == '-' || in.peek() == '=' || in.peek() == '>'){
                        str = curr;
                        in >> curr; // Remove the next character from the stream
                        str += curr;
                        tokens.push_back(token(str));
                        str = "";
                    }
                    else{
                        tokens.push_back(token(std::string(1, curr)));
                    }
                    this->out << curr; // TEMPORARY FOR ASSIGNMENT ONE
                    break;

                case '&':
                    if (in.peek() == '&' || in.peek() == '='){
                        str = curr;
                        in >> curr; // Remove the next character from the stream
                        str += curr;
                        tokens.push_back(token(str));
                        str = "";
                    }
                    else{
                        tokens.push_back(token(std::string(1, curr)));
                    }
                    this->out << curr; // TEMPORARY FOR ASSIGNMENT ONE
                    break;

                case '|':
                    if (in.peek() == '|' || in.peek() == '='){
                        str = curr;
                        in >> curr; // Remove the next character from the stream
                        str += curr;
                        tokens.push_back(token(str));
                        str = "";
                    }
                    else{
                        tokens.push_back(token(std::string(1, curr)));
                    }
                    this->out << curr; // TEMPORARY FOR ASSIGNMENT ONE
                    break;

                // Characters with 1 secondary
                case '!':
                    if (in.peek() == '='){
                        str = curr;
                        in >> curr; // Remove the next character from the stream
                        str += curr;
                        tokens.push_back(token(str));
                        str = "";
                    }
                    else{
                        tokens.push_back(token(std::string(1, curr)));
                    }
                    this->out << curr; // TEMPORARY FOR ASSIGNMENT ONE
                    break;

                case '%':
                    if (in.peek() == '='){
                        str = curr;
                        in >> curr; // Remove the next character from the stream
                        str += curr;
                        tokens.push_back(token(str));
                        str = "";
                    }
                    else{
                        tokens.push_back(token(std::string(1, curr)));
                    }
                    this->out << curr; // TEMPORARY FOR ASSIGNMENT ONE
                    break;

                case '*':
                    if (in.peek() == '='){
                        str = curr;
                        in >> curr; // Remove the next character from the stream
                        str += curr;
                        tokens.push_back(token(str));
                        str = "";
                    }
                    else{
                        tokens.push_back(token(std::string(1, curr)));
                    }
                    this->out << curr; // TEMPORARY FOR ASSIGNMENT ONE
                    break;

                case '=':
                    if (in.peek() == '='){
                        str = curr;
                        in >> curr; // Remove the next character from the stream
                        str += curr;
                        tokens.push_back(token(str));
                        str = "";
                    }
                    else{
                        tokens.push_back(token(std::string(1, curr)));
                    }
                    this->out << curr; // TEMPORARY FOR ASSIGNMENT ONE
                    break;

                case '^':
                    if (in.peek() == '='){
                        str = curr;
                        in >> curr; // Remove the next character from the stream
                        str += curr;
                        tokens.push_back(token(str));
                        str = "";
                    }
                    else{
                        tokens.push_back(token(std::string(1, curr)));
                    }
                    this->out << curr; // TEMPORARY FOR ASSIGNMENT ONE
                    break;

                case '#':
                    if (in.peek() == '#'){
                        str = curr;
                        in >> curr; // Remove the next character from the stream
                        str += curr;
                        tokens.push_back(token(str));
                        str = "";
                    }
                    else{
                        tokens.push_back(token(std::string(1, curr)));
                    }
                    this->out << curr; // TEMPORARY FOR ASSIGNMENT ONE
                    break;

                // Arrow operators
                case '>':
                    if (in.peek() == '='){
                        str = curr;
                        in >> curr; // Remove the next character from the stream
                        str += curr;
                        tokens.push_back(token(str));
                        str = "";
                    } else if (in.peek() == '>'){
                        str = curr;
                        in >> curr; // Remove the next character from the stream
                        str += curr;
                        if (in.peek() == '='){
                            in >> curr; // Remove the next character from the stream
                            str += curr;
                        }
                        tokens.push_back(token(str));
                        str = "";
                    }
                    else{
                        tokens.push_back(token(std::string(1, curr)));
                    }
                    this->out << curr; // TEMPORARY FOR ASSIGNMENT ONE
                    break;

                case '<':
                    if (in.peek() == '='){
                        str = curr;
                        in >> curr; // Remove the next character from the stream
                        str += curr;
                        tokens.push_back(token(str));
                        str = "";
                    } else if (in.peek() == '<'){
                        str = curr;
                        in >> curr; // Remove the next character from the stream
                        str += curr;
                        if (in.peek() == '='){
                            in >> curr; // Remove the next character from the stream
                            str += curr;
                        }
                        tokens.push_back(token(str));
                        str = "";
                    }
                    else{
                        tokens.push_back(token(std::string(1, curr)));
                    }
                    this->out << curr; // TEMPORARY FOR ASSIGNMENT ONE
                    break;

                case '/':
                    // Check for comments
                    if (in.peek() == '/'){
                        in >> curr; // Remove the next character from the stream
                        ignore_line = true;
                    }
                    else if (in.peek() == '*'){
                        in >> curr; // Remove the next character from the stream
                        ignore_multiline = true;
                    }
                    else if (in.peek() == '='){
                        str = curr;
                        in >> curr; // Remove the next character from the stream
                        str += curr;
                        tokens.push_back(token(str));
                        str = "";
                    }
                    else{
                        tokens.push_back(token(std::string(1, curr)));
                    }
                    break;

                case '.':
                    if (in.peek() == '.'){
                        str = curr;
                        in >> curr; // Remove the next character from the stream
                        str += curr;
                        if (in.peek() == '.'){
                            in >> curr; // Remove the next character from the stream
                            str += curr;
                            tokens.push_back(token(str));
                            str = "";
                        }
                    }
                    else{
                        tokens.push_back(token(std::string(1, curr)));
                    }
                    this->out << curr; // TEMPORARY FOR ASSIGNMENT ONE
                    break;

                case 'L':
                    if (in.peek() == '\'' || in.peek() == '"'){
                        str = curr;
                        in >> curr; // Remove the next character from the stream
                        str += curr;
                        tokens.push_back(token(str));
                        str = "";
                    }
                    else{
                        tokens.push_back(token(std::string(1, curr)));
                    }
                    this->out << curr; // TEMPORARY FOR ASSIGNMENT ONE
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
                    while (in.peek() == '_' || in.peek() == 'a' || in.peek() == 'b' || in.peek() == 'c' || in.peek() == 'd' || in.peek() == 'e'
                           || in.peek() == 'f' || in.peek() == 'g' || in.peek() == 'h' || in.peek() == 'i' || in.peek() == 'j' || in.peek() == 'k'
                           || in.peek() == 'l' || in.peek() == 'm' || in.peek() == 'n' || in.peek() == 'o' || in.peek() == 'p' || in.peek() == 'q'
                           || in.peek() == 'r' || in.peek() == 's' || in.peek() == 't' || in.peek() == 'u' || in.peek() == 'v' || in.peek() == 'w'
                           || in.peek() == 'x' || in.peek() == 'y' || in.peek() == 'z' || in.peek() == 'A' || in.peek() == 'B' || in.peek() == 'C'
                           || in.peek() == 'D' || in.peek() == 'E' || in.peek() == 'F' || in.peek() == 'G' || in.peek() == 'H' || in.peek() == 'I'
                           || in.peek() == 'J' || in.peek() == 'K' || in.peek() == 'M' || in.peek() == 'N' || in.peek() == 'O' || in.peek() == 'P'
                           || in.peek() == 'Q' || in.peek() == 'R' || in.peek() == 'S' || in.peek() == 'T' || in.peek() == 'U' || in.peek() == 'V'
                           || in.peek() == 'W' || in.peek() == 'X' || in.peek() == 'Y' || in.peek() == 'Z' || in.peek() == '0' || in.peek() == '1'
                           || in.peek() == '2' || in.peek() == '3' || in.peek() == '4' || in.peek() == '5' || in.peek() == '6' || in.peek() == '7'
                           || in.peek() == '8' || in.peek() == '9'){ // Yucky
                        in >> curr; // Remove the next character from the stream
                        str += curr;
                    }
                    tokens.push_back(token(str));
                    this->out << str; // TEMPORARY FOR ASSIGNMENT ONE
                    str = "";
                    break;

                case '\'':
                    str = curr;
                    while (in.peek() != '\''){
                        in >> curr; // Remove the next character from the stream
                        str += curr;
                    }
                    in >> curr; // Remove the next character from the stream
                    str += curr;
                    tokens.push_back(token(str));
                    this->out << str; // TEMPORARY FOR ASSIGNMENT ONE
                    str = "";
                    break;

                case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
                    str = curr;
                    while (in.peek() == '0' || in.peek() == '1' || in.peek() == '2' || in.peek() == '3' || in.peek() == '4' || in.peek() == '5'
                           || in.peek() == '6' || in.peek() == '7' || in.peek() == '8' || in.peek() == '9'){
                        in >> curr; // Remove the next character from the stream
                        str += curr;
                    }
                    tokens.push_back(token(str));
                    str = "";
                    this->out << str; // TEMPORARY FOR ASSIGNMENT ONE
                    break;

                default:
                    tokens.push_back(token(std::string(1, curr)));
                    this->out << curr; // TEMPORARY FOR ASSIGNMENT ONE
                    break;
            }
        }
    }
    in.close();
    return tokens;
}
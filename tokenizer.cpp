//
// Created by Ethan Edwards on 9/4/24.
//

#include "tokenizer.h"

void tokenizer::tokenize(std::ifstream& in, std::vector<std::string>& tokens){
    char curr;
    bool ignore_line = false;
    bool string_handling = false;
    bool ignore_multiline = false;
    std::string str;

    while (!in.eof()){
        in.get(curr);
        if (ignore_line){ // Ignore characters until the end of the line (for single line comments)
            if (curr == '\n'){
                ignore_line = false;
            }
        } else if (ignore_multiline){ // Ignore characters until the end of the comment (for multi-line comments)
            if (curr == '*' && in.peek() == '/'){
                in >> curr; // Remove the next character from the stream
                ignore_multiline = false;
            }
            // TODO: Handle reaching EOF without finding the end of the comment
        } else if (string_handling){ // Write all characters from a string to a single token. String parsing will be handled later
            if (curr == '"'){
                string_handling = false;
                tokens.push_back(str);
                str = "";
            } else{
                str += curr;
            }
            // TODO: Handle reaching EOF without finding the end of the string
        } else{
            if (curr == '/' && in.peek() == '/'){ // Single line comment
                in >> curr; // Remove the next character from the stream
                ignore_line = true;
            }
            else if (curr == '/' && in.peek() == '*'){ // Multi-line comment
                in >> curr; // Remove the next character from the stream
                ignore_multiline = true;
            }
            else if (curr == '"'){ // String
                string_handling = true;
            }
            else { // Everything else
                if (curr == '(' || curr == ')' || curr == '{' || curr == '}' || curr == '[' || curr == ']' || curr == ';' || curr == '%') {
                    tokens.push_back(std::string(1, curr));
                }
                else if (curr == '=' || curr == '<' || curr == '>') {
                    if (in.peek() == '=') {
                        str = curr;
                        in >> curr; // Remove the next character from the stream
                        str += curr;
                        tokens.push_back(str);
                        str = "";
                    } else {
                        tokens.push_back(std::string(1, curr));
                    }
                }
                else if (curr == '*') {
                    if (in.peek() == '=') {
                        str = curr;
                        in >> curr; // Remove the next character from the stream
                        str += curr;
                        tokens.push_back(str);
                        str = "";
                    } else {
                        tokens.push_back(std::string(1, curr));
                    }
                }
                else if (curr == '+') {
                    if (in.peek() == '+' || in.peek() == '=') {
                        str = curr;
                        in >> curr; // Remove the next character from the stream
                        str += curr;
                        tokens.push_back(str);
                        str = "";
                    } else {
                        tokens.push_back(std::string(1, curr));
                    }
                }
                else if (curr == '-') {
                    if (in.peek() == '-' || in.peek() == '=') {
                        str = curr;
                        in >> curr; // Remove the next character from the stream
                        str += curr;
                        tokens.push_back(str);
                        str = "";
                    } else {
                        tokens.push_back(std::string(1, curr));
                    }
                }
                else if (curr == '!') {
                    if (in.peek() == '=') {
                        str = curr;
                        in >> curr; // Remove the next character from the stream
                        str += curr;
                        tokens.push_back(str);
                        str = "";
                    } else {
                        tokens.push_back(std::string(1, curr));
                    }
                }
                else if (curr == '&') {
                    if (in.peek() == '&') {
                        str = curr;
                        in >> curr; // Remove the next character from the stream
                        str += curr;
                        tokens.push_back(str);
                        str = "";
                    } else {
                        tokens.push_back(std::string(1, curr));
                    }
                }
                else if (curr == '|') {
                    if (in.peek() == '|') {
                        str = curr;
                        in >> curr; // Remove the next character from the stream
                        str += curr;
                        tokens.push_back(str);
                        str = "";
                    } else {
                        tokens.push_back(std::string(1, curr));
                    }
                }
                else if (curr != ' ' && curr != '\n' && curr != '\t') {
                    str += curr;
                    if (in.peek() == ' ' || in.peek() == '\n' || in.peek() == '\t' || in.peek() == '('
                        || in.peek() == ')' || in.peek() == '{' || in.peek() == '}' || in.peek() == '['
                        || in.peek() == ']' || in.peek() == ';' || in.peek() == '%' || in.peek() == '='
                        || in.peek() == '<' || in.peek() == '>' || in.peek() == '*' || in.peek() == '+'
                        || in.peek() == '-' || in.peek() == '!' || in.peek() == '&' || in.peek() == '|') {
                        tokens.push_back(str); // Add the token to the vector if we've reached it's end
                        str = "";
                    }
                }
            }
        }
    }
    // Error checking
    if (ignore_line){
        std::cerr << "Error: Reached EOF while ignoring a line comment" << std::endl;
    }
    if (ignore_multiline){
        std::cerr << "Error: Reached EOF while ignoring a multi-line comment" << std::endl;
    }
    if (string_handling){
        std::cerr << "Error: Reached EOF while parsing a string" << std::endl;
    }
}


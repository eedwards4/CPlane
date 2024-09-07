//
// Created by Ethan Edwards on 9/4/24.
//

#include "ctokenizer.h"

std::vector<ctoken> ctokenizer::tokenize(std::string filename) {
    std::vector<ctoken> tokens;
    std::ifstream in(filename);
    std::ofstream out("output_" + filename + ".txt"); // TEMPORARY FOR ASSIGNMENT ONE
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
                out << "\n"; // TEMPORARY FOR ASSIGNMENT ONE
                line++;
            }
            else{
                out << " "; // TEMPORARY FOR ASSIGNMENT ONE. Output a space to preserve original file formatting
            }
        } else if (ignore_multiline){
            if (curr == '*' && in.peek() == '/'){
                in >> curr; // Remove the next character from the stream
                out << " "; // TEMPORARY FOR ASSIGNMENT ONE. Output an extra space
                ignore_multiline = false;
            }
            else if (curr == '\n'){
                out << "\n"; // TEMPORARY FOR ASSIGNMENT ONE
                line++;
            }
            else{
                out << " "; // TEMPORARY FOR ASSIGNMENT ONE. Output a space to preserve original file formatting
            }
        } else if (string_handling) {
            if (curr == '"') {
                string_handling = false;
                tokens.push_back(ctoken(str));
                str = "";
            } else {
                str += curr;
            }
            out << curr; // TEMPORARY FOR ASSIGNMENT ONE. Output the character to the output file
        } else{
            switch(curr){
                case '/':
                    if (in.peek() == '/'){
                        in >> curr; // Remove the next character from the stream
                        ignore_line = true;
                    }
                    else if (in.peek() == '*'){
                        in >> curr; // Remove the next character from the stream
                        ignore_multiline = true;
                    }
                    else{ // ERROR: Expected character
                        out << curr; // TEMPORARY FOR ASSIGNMENT ONE
                        std::cerr << "ERROR: Incomplete comment, expected '/' or '*' " << line << std::endl;
                        exit(1);
                    }
                    break;

                case '"':
                    string_handling = true;
                    out << curr; // TEMPORARY FOR ASSIGNMENT ONE
                    break;

                case '\n':
                    out << "\n"; // TEMPORARY FOR ASSIGNMENT ONE
                    break;
                case '\t':
                    out << "\t"; // TEMPORARY FOR ASSIGNMENT ONE
                    break;
            }
        }
    }
}
#ifndef REMOVE_COMMENTS_H
#define REMOVE_COMMENTS_H

#include <iostream>
#include <fstream>
#include <sstream>

// Takes an input file and removes all c style comments, replacing the
// removed characters with white space. This is all written into the
// output file. Returns false if there is an error and true if none.
bool removeComments(std::string infile, std::string outfile) {
    std::ifstream in(infile, std::ios::in);

	std::stringstream ss;
	ss >> std::noskipws;
	ss << in.rdbuf();
	in.close();

    std::ofstream out(outfile, std::ios::out);
    char c;

    bool in_comment_single = false; // Bool if in single line comment
    bool in_comment_multi = false; // Bool if in multi line comment
    
    int line_number = 1; // Current line number
    int multi_line_start; // Line number multi line comment started on

    bool in_string_single = false; // Bool if in single quote string
    bool in_string_double = false; // Bool if in double quote string
    
    while (ss >> c) {
        // Determine if in string
        if (c == '\'' && !in_string_double) in_string_single = !in_string_single;
        if (c == '"' && !in_string_single) in_string_double = !in_string_double;

        // Start comment
        if (c == '/' && !in_string_single && !in_string_double && !in_comment_single && !in_comment_multi) {
            if (ss.peek() == '/') in_comment_single = true;
            else if (ss.peek() == '*') {
                in_comment_multi = true;
                multi_line_start = line_number;
            }
        }

        // End comment
        if (in_comment_single && c == '\n') in_comment_single = false;
        if (in_comment_multi && c == '*' && ss.peek() == '/') {
            in_comment_multi = false;
            ss >> c;
            ss >> c;
            out << "  ";
        }

        // Track line number
        if (c == '\n') line_number += 1;

        // Output
        if (in_comment_single || in_comment_multi) {
            if (iswspace(c)) {
                out << c;
            } else {
                out << " ";
            }
        } else {
            out << c;
        }
    }

    if (in_comment_multi) { // Prints error if multi line comment doesn't end
        std::cerr << "ERROR: Program contains C-style, unterminated comment on line " << multi_line_start << std::endl;
        return false;
    }
    return true;
}

#endif
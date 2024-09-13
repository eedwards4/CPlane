//
// Created by Ethan Edwards on 9/12/2024.
//

#ifndef CPLANE_TOKENIZER_NEW_H
#define CPLANE_TOKENIZER_NEW_H

// Internal
#include "errors.h"
#include "tokens.h"
#include "exec_path.h"
// Standard
#include <string>
#include <vector>
#include <fstream>
#include <iostream>

class tokenizer_new {
public:
    tokenizer_new(exec_path* path){
        this->out.open("output.txt"); // TEMPORARY FOR ASSIGNMENT ONE
        this->path = path;
    };
    ~tokenizer_new() = default;

    void tokenize(std::string filename);

private:
    std::ofstream out; // Output file
    exec_path* path; // Execution path

    // Helpers
    static bool is_num(char c); // Checks if a character is a number
    static bool is_alpha(char c); // Checks if a character is a letter
};


#endif //CPLANE_TOKENIZER_NEW_H

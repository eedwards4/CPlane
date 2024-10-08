//
// Created by Ethan Edwards on 9/12/2024.
//

#ifndef CPLANE_TOKENIZER_H
#define CPLANE_TOKENIZER_H

// Internal
#include "errors.h"
#include "exec_path.h"
#include "tokens.h"
// Standard
#include <string>
#include <vector>
#include <fstream>
#include <iostream>

class tokenizer {
public:
    tokenizer(exec_path* path){
        this->out.open("output_assignment_1.txt"); // TEMPORARY FOR ASSIGNMENT ONE
        this->path = path;
    };
    ~tokenizer() = default;

    void tokenize(std::string filename);

private:
    std::ofstream out; // Output file
    exec_path* path; // Execution path

    // Helpers
    static bool is_num(char c); // Checks if a character is a number
    static bool is_alpha(char c); // Checks if a character is a letter
};


#endif //CPLANE_TOKENIZER_H

//
// Created by Ethan Edwards on 9/4/24.
//

#ifndef CPLANE_TOKENIZER_H
#define CPLANE_TOKENIZER_H

#include "tokens.h"

#include <vector>
#include <fstream>
#include <iostream>

class tokenizer {
public:
    tokenizer() {
        this->out.open("output.txt"); // TEMPORARY FOR ASSIGNMENT ONE
    };
    ~tokenizer() = default;

    std::vector<token> tokenize(std::string filename);

private:
    std::ofstream out; // TEMPORARY FOR ASSIGNMENT ONE
};


#endif //CPLANE_TOKENIZER_H

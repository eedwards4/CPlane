//
// Created by Ethan Edwards on 9/4/24.
//

#ifndef CPLANE_CTOKENIZER_H
#define CPLANE_CTOKENIZER_H

#include "ctokens.h"

#include <vector>
#include <fstream>
#include <iostream>

class ctokenizer {
public:
    ctokenizer() {
        this->out.open("output.txt"); // TEMPORARY FOR ASSIGNMENT ONE
    };
    ~ctokenizer() = default;

    std::vector<ctoken> tokenize(std::string filename);

private:
    std::ofstream out; // TEMPORARY FOR ASSIGNMENT ONE
};


#endif //CPLANE_CTOKENIZER_H

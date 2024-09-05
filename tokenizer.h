//
// Created by Ethan Edwards on 9/4/24.
//

#ifndef CPLANE_TOKENIZER_H
#define CPLANE_TOKENIZER_H

#include <iostream>
#include <fstream>
#include <vector>

class tokenizer {
public:
    void tokenize(std::ifstream& in, std::vector<std::string>& tokens);
};


#endif //CPLANE_TOKENIZER_H

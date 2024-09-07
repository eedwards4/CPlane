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
    ctokenizer() = default;
    ~ctokenizer() = default;

    std::vector<ctoken> tokenize(std::string filename);

private:

};


#endif //CPLANE_CTOKENIZER_H

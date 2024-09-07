//
// Created by Ethan Edwards on 9/4/24.
//

#ifndef CPLANE_CTOKENIZER_H
#define CPLANE_CTOKENIZER_H

#include "ctoken.h"

#include <vector>
#include <fstream>

class ctokenizer {
public:
    std::vector<ctoken> tokenize(std::ifstream& in);
};


#endif //CPLANE_CTOKENIZER_H

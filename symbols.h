//
// Created by Ethan Edwards on 10/11/2024.
//

#ifndef CPLANE_SYMBOLS_H
#define CPLANE_SYMBOLS_H

#include <string>

class symbol {
public:
    symbol() = default;
    ~symbol() = default;

private:
    std::string IDENT_NAME;
    int IDENT_TYPE;
    int DATATYPE = -1;
    bool IS_ARRAY = false;
    int ARRAY_SIZE = 0;
    int SCOPE = 1;

};

class symbols {

};


#endif //CPLANE_SYMBOLS_H

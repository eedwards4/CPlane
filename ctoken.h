//
// Created by Ethan Edwards on 9/4/24.
//

#ifndef CPLANE_CTOKEN_H
#define CPLANE_CTOKEN_H

#include <string>

class ctoken {
public:
    ctoken(int type, char cval = '\0', std::string sval = "");
    const int type() { return val_type; }

private:
    int val_type = -1;
    char char_value;
    std::string string_value;
};


#endif //CPLANE_CTOKEN_H

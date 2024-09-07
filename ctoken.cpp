//
// Created by Ethan Edwards on 9/4/24.
//

#include "ctoken.h"

ctoken::ctoken(int type, char cval, std::string sval) {
    this->val_type = type;
    this->char_value = cval;
    this->string_value = sval;
}



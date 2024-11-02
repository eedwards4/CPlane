#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>

#include "token.h"

class Tokenizer {
    public:
        Tokenizer(std::string file_name);
        Token* getToken();
        void ungetToken() { hasUngottenToken = true; }
        std::string readString(char end);

    private:
        std::stringstream ss;
        Token* lastToken;
        bool hasUngottenToken = false;
        bool inString = false;
        int lineNumber = 1;
};

#endif
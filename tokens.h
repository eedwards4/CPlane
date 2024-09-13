//
// Created by Ethan Edwards on 9/4/24.
//

#ifndef CPLANE_TOKENS_H
#define CPLANE_TOKENS_H

#include <string>

class token {
public:
    token() = default;
    explicit token(std::string value){
        if (value.size() == 1){
            this->type = 0;
            this->char_value = value[0];
        }
        else { // String operators will be handled down the line by the parser (skips unnecessary tokenization for potentially unused tokens)
            this->type = 1;
            this->str_value = value;
        }
    }
    token(std::string value, int type){
        this->type = type;
        if (type == 0){
            this->char_value = value[0];
        }
        else {
            this->str_value = value;
        }
    }
    ~token() = default;

    int get_type() {return this->type;}
    char get_char_value() {return this->char_value;}
    std::string get_str_value() {return this->str_value;}

private:
    int type = -1;
    /* Type States
     * -1: Error/Unknown
     * 0: Character
     * 1: String
     * 2: String literal
     * TODO: TOKENIZE INTS, FLOATS, ETC AS THEIR OWN TYPES INSTEAD OF STRINGS
    */
    char char_value;
    std::string str_value;
};

#endif //CPLANE_TOKENS_H

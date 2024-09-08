//
// Created by Ethan Edwards on 8/20/24.
//

#include <iostream>
#include <string>
#include <vector>

#include "ctokens.h"
#include "ctokenizer.h"

using namespace std;

int main(){
    string filename = "C:\\Users\\evane\\Documents\\CPPprojects\\CPlane\\tests\\test.txt";
    ctokenizer t;
    vector<ctoken> tokens;
    tokens = t.tokenize(filename);
    for (auto & token : tokens){
        if (token.get_type() == 0){
            cout << "Type: Character, Value: " << token.get_char_value() << endl;
        }
        else if (token.get_type() == 1){
            cout << "Type: String, Value: " << token.get_str_value() << endl;
        }
    }
    return 0;
}
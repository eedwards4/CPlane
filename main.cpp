//
// Created by Ethan Edwards on 8/20/24.
//

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "tokenizer.h"

using namespace std;

int main(){
    ifstream file;
    ofstream out;
    file.open("/Users/ethan/CLionProjects/CPlane/tests/test.txt");
    // out.open("/Users/ethan/CLionProjects/CPlane/output.txt");
    vector <string> tokens;
    vector <tuple<string, int>> tokens2;
    tokenizer t;

    t.tokenize(file, tokens);
    for (int i = 0; i < tokens.size(); i++){
        cout << tokens[i] << endl;
    }
    for (int i = 0; i < tokens.size(); i++){
        bool found = false;
        for (int j = 0; j < tokens2.size(); j++){
            if (get<0>(tokens2[j]) == tokens[i]){
                found = true;
                get<1>(tokens2[j])++;
                break;
            }
        }
        if (!found){
            tokens2.push_back(make_tuple(tokens[i], 1));
        }
    }

    for (int i = 0; i < tokens2.size(); i++){
        cout << get<0>(tokens2[i]) << " " << get<1>(tokens2[i]) << endl;
    }
    return 0;
}
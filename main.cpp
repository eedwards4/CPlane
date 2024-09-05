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
    file.open("/Users/ethan/CLionProjects/CPlane/test.txt");
    // out.open("/Users/ethan/CLionProjects/CPlane/output.txt");
    vector <string> tokens;
    tokenizer t;

    t.tokenize(file, tokens);
    for (int i = 0; i < tokens.size(); i++){
        cout << tokens[i] << endl;
    }
    return 0;
}
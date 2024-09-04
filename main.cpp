//
// Created by Ethan Edwards on 8/20/24.
//

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

vector<string> tokenizer(ifstream& file){
    vector<string> tokens;
    while (!file.eof()){
        string line;
        getline(file, line);
        string token = "";
        for (int i = 0; i < line.size(); i++){
            if (line[i] == '(' || line[i] == ')' || line[i] == '{' ||
                line[i] == '}' || line[i] == '[' || line[i] == ']' ||
                line[i] == ';' || line[i] == '%') {
                tokens.emplace_back(1, line[i]);
            }
            else if (line[i] == '=' || line[i] =='<' || line[i] == '>' || line[i] == '*'){
                if (line[i+1] == '='){
                    token += line[i];
                    token += line[i+1];
                    tokens.push_back(token);
                    token = "";
                    i++; // Iterate i twice to skip the next character
                }
                else{
                    tokens.emplace_back(1, line[i]);
                }
            }
            else if (line[i] == '+'){
                if (line[i+1] == '+' || line[i+1] == '='){
                    token += line[i];
                    token += line[i+1];
                    tokens.push_back(token);
                    token = "";
                    i++;
                }
                else{
                    tokens.emplace_back(1, line[i]);
                }
            }
            else if (line[i] == '-'){
                if (line[i+1] == '-' || line[i+1] == '='){
                    token += line[i];
                    token += line[i+1];
                    tokens.push_back(token);
                    token = "";
                    i++;
                }
                else{
                    tokens.emplace_back(1, line[i]);
                }
            }
            else if (line[i] == '!'){
                if (line[i+1] == '='){
                    token += line[i];
                    token += line[i+1];
                    tokens.push_back(token);
                    token = "";
                    i++;
                }
                else{
                    tokens.emplace_back(1, line[i]);
                }
            }
            else if (line[i] == '&'){
                if (line[i+1] == '&'){
                    token += line[i];
                    token += line[i+1];
                    tokens.push_back(token);
                    token = "";
                    i++;
                }
                else{
                    tokens.emplace_back(1, line[i]);
                }
            }
            else if (line[i] == '|'){
                if (line[i+1] == '|'){
                    token += line[i];
                    token += line[i+1];
                    tokens.push_back(token);
                    token = "";
                    i++;
                }
                else{
                    tokens.emplace_back(1, line[i]);
                }
            }
            else if (line[i] != ' '){
                token += line[i];
                if (token.size() > 0 && (line[i+1] == ' ' || line[i+1] == '(' || line[i+1] == ')' ||
                                              line[i+1] == '{' || line[i+1] == '}' || line[i+1] == '[' ||
                                              line[i+1] == ']' || line[i+1] == ';' || line[i+1] == '%' ||
                                              line[i+1] == '=' || line[i+1] == '<' || line[i+1] == '>' ||
                                              line[i+1] == '*' || line[i+1] == '+' || line[i+1] == '-' ||
                                              line[i+1] == '!' || line[i+1] == '&' || line[i+1] == '|')){
                    tokens.push_back(token);
                    token = "";
                }
            }
        }
    }
    return tokens;
};

int main(){
    ifstream file;
    file.open("/Users/ethan/CLionProjects/CPlane/test.txt");
    vector<string> tokens = tokenizer(file);
    for (int i = 0; i < tokens.size(); i++){
        cout << tokens[i] << endl;
    }
    return 0;
}
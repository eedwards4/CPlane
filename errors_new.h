//
// Created by Logan Puntous on 11/8/2024.
//

#ifndef CPLANE_ERRORS_Z
#define CPLANE_ERRORS_Z

// No further need
//#include "exec_path.h"

#include <vector>
#include <iostream>
#include <string>

// USAGE
// When checking for errors, call in RST, errors ERRORS e;
// When encounter error in file e.ENC_ERROR(Relevant info);
// Then at end of file you need to call:
// if ( e.EXISTS ) { e.STOP_SYNTAX_ERRORS };
// This will stop the program compilation and print all the error statements.
/*
struct INFO
{   
    int code;
    int line;
    int c;
    std::string filename;
    std::string print_statement;
    
};
*/

class ERROR {
    public:
    ERROR();
    int code; // Exit code
    int line; // line
    int column; // column
    std::string filename;
    std::string print_statement;
    //std::string print_statement; // Message

    private:
    //INFO rel_info;
};


class ERRORS {
    public:
    ERRORS();
    bool exists; // Does an error exist?
    void ENC_ERROR(std::string filename, int code, int line, int column);
    void STOP_SYNTAX_ERRORS();

    private:
    std::vector<ERROR> VERRORS; // vector of errors for printing once STOP_SYNTAX_ERRORS added from ENC_ERROR
    void printHelper(ERROR error);
};





#endif //CPLANE_ERRORZ_H

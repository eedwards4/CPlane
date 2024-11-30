//
// Created by Logan Puntous on 11/8/2024.
//

#ifndef CPLANE_ERRORZ_H
#define CPLANE_ERRORZ_H

#include "exec_path.h"
#include <vector>
#include <iostream>
#include <string>

// USAGE
// When checking for errors, call in RST, errors ERRORS e;
// When encounter error in file e.ENC_ERROR(Relevant info);
// Then at end of file you need to call:
// if ( e.EXISTS ) { e.STOP_SYNTAX_ERRORS };
// This will stop the program compilation and print all the error statements if there are any errors



class ERROR {
    public:
    ERROR();
    int code; // Exit code
    int line; // line
    int column; // column
    std::string filename;
    std::string print_statement;
};


class ERRORS {
    public:
    ERRORS();
    bool exists; // Does an error exist?
    void ENC_ERROR(std::string filename, int code, int line, int column, std::string error_msg);
    void STOP_SYNTAX_ERRORS();
    void RDP_SYNTAX_ERRORS(); // FOR RDP

    private:
    bool FOR_RST;
    std::vector<ERROR> VERRORS; // vector of errors for printing once STOP_SYNTAX_ERRORS added from ENC_ERROR
    void printHelper(ERROR error);
};





#endif //CPLANE_ERRORZ_H

//
// Created by Ethan Edwards on 10/24/2024.
//

#ifndef CPLANE_SYMBOLS_H
#define CPLANE_SYMBOLS_H

#include <string>

class symbols {
public:
    class identifiers {
    public:
        static constexpr int FUNCTION = 9999;
        static constexpr int PROCEDURE = 9998;
        static constexpr int DATATYPE = 9997;
        static constexpr int PARAMETER = 9996;

        static std::string get_type(int type){
            switch (type) {
                case FUNCTION: return "function";
                case PROCEDURE: return "procedure";
                case DATATYPE: return "datatype";
                default: return "unknown";
            }
        }
    };
    class data_types {
    public:
        static constexpr int NA = 9999;
        static constexpr int INT = 9998;
        static constexpr int FLOAT = 9997;
        static constexpr int CHAR = 9996;
        static constexpr int BOOL = 9995;
        static constexpr int VOID = 9994;

        static int check_type(const std::string& type){
            if (type == "int") return INT;
            if (type == "float") return FLOAT;
            if (type == "char") return CHAR;
            if (type == "bool") return BOOL;
            if (type == "void") return VOID;
            return -1; // Invalid type
        }

        static std::string get_type(int type){
            switch (type) {
                case INT: return "int";
                case FLOAT: return "float";
                case CHAR: return "char";
                case BOOL: return "bool";
                case VOID: return "void";
                case NA: return "NA";
                default: return "unknown";
            }
        }
    };
};


#endif //CPLANE_SYMBOLS_H

//
// Created by evane on 9/13/2024.
//

#ifndef CPLANE_TOKENS_H
#define CPLANE_TOKENS_H
#include <iostream>

class tokens {
public:
    // Literals
    static constexpr int STRING_LITERAL = 9999;
    static constexpr int CHAR_LITERAL = 9998;
    static constexpr int INT_AS_STRING = 9997;
    static constexpr int FLOAT_AS_STRING = 9996;
    static constexpr int TOKEN_AS_STRING = 9995;
    // Nesting structures
    static constexpr int OPEN_PAREN = 9994; // (
    static constexpr int CLOSE_PAREN = 9993; // )
    static constexpr int OPEN_BRACE = 9992; // {
    static constexpr int CLOSE_BRACE = 9991; // }
    static constexpr int OPEN_BRACKET = 9990; // [
    static constexpr int CLOSE_BRACKET = 9989; // ]
    // Multi-character operators
    static constexpr int PLUS_PLUS = 9988; // ++
    static constexpr int PLUS_EQUALS = 9987; // +=
    static constexpr int MINUS_MINUS = 9986; // --
    static constexpr int MINUS_EQUALS = 9985; // -=
    static constexpr int RIGHT_SLIM_ARROW = 9984; // ->
    static constexpr int BOOLEAN_AND = 9983; // &&
    static constexpr int AND_EQUALS = 9982; // &=
    static constexpr int BOOLEAN_OR = 9981; // ||
    static constexpr int OR_EQUALS = 9980; // |=
    static constexpr int NOT_EQUALS = 9979; // !=
    static constexpr int MOD_EQUALS = 9978; // %=
    static constexpr int TIMES_EQUALS = 9977; // *=
    static constexpr int EQUALS_EQUALS = 9976; // ==
    static constexpr int XOR_EQUALS = 9975; // ^=
    static constexpr int TOKEN_PASTE = 9974; // ##
    static constexpr int GREATER_EQUALS = 9973; // >=
    static constexpr int RIGHT_SHIFT_EQUALS = 9972; // >>=
    static constexpr int RIGHT_SHIFT = 9971; // >>
    static constexpr int LESS_EQUALS = 9970; // <=
    static constexpr int LEFT_SHIFT_EQUALS = 9969; // <<= NICE
    static constexpr int LEFT_SHIFT = 9968; // <<
    static constexpr int DIVIDE_EQUALS = 9967; // /=
    static constexpr int NEWLINE = 9966; // \n NOTE: THIS IS ONLY USED FOR LINE TRACING
    static constexpr int ELLIPSIS = 9965; // ...
    static constexpr int SCOPE = 9964; // ::
    // Weird stuff
    static constexpr int LITERAL_DEF = 9963; // L"" or L''
    // Single-character operators are defined by their ASCII values
    static int single_char(int c){
        return c;
    }

    static std::string what_is(int id){
        switch (id){
            case STRING_LITERAL:
                return {"STRING_LITERAL"};
            case CHAR_LITERAL:
                return {"CHAR_LITERAL"};
            case INT_AS_STRING:
                return {"INT_AS_STRING"};
            case FLOAT_AS_STRING:
                return {"FLOAT_AS_STRING"};
            case TOKEN_AS_STRING:
                return {"TOKEN_AS_STRING"};
            case OPEN_PAREN:
                return {"OPEN_PAREN"};
            case CLOSE_PAREN:
                return {"CLOSE_PAREN"};
            case OPEN_BRACE:
                return {"OPEN_BRACE"};
            case CLOSE_BRACE:
                return {"CLOSE_BRACE"};
            case OPEN_BRACKET:
                return {"OPEN_BRACKET"};
            case CLOSE_BRACKET:
                return {"CLOSE_BRACKET"};
            case PLUS_PLUS:
                return {"PLUS_PLUS"};
            case PLUS_EQUALS:
                return {"PLUS_EQUALS"};
            case MINUS_MINUS:
                return {"MINUS_MINUS"};
            case MINUS_EQUALS:
                return {"MINUS_EQUALS"};
            case RIGHT_SLIM_ARROW:
                return {"RIGHT_SLIM_ARROW"};
            case BOOLEAN_AND:
                return {"BOOLEAN_AND"};
            case AND_EQUALS:
                return {"AND_EQUALS"};
            case BOOLEAN_OR:
                return {"BOOLEAN_OR"};
            case OR_EQUALS:
                return {"OR_EQUALS"};
            case NOT_EQUALS:
                return {"NOT_EQUALS"};
            case MOD_EQUALS:
                return {"MOD_EQUALS"};
            case TIMES_EQUALS:
                return {"TIMES_EQUALS"};
            case EQUALS_EQUALS:
                return {"EQUALS_EQUALS"};
            case XOR_EQUALS:
                return {"XOR_EQUALS"};
            case TOKEN_PASTE:
                return {"TOKEN_PASTE"};
            case GREATER_EQUALS:
                return {"GREATER_EQUALS"};
            case RIGHT_SHIFT_EQUALS:
                return {"RIGHT_SHIFT_EQUALS"};
            case RIGHT_SHIFT:
                return {"RIGHT_SHIFT"};
            case LESS_EQUALS:
                return {"LESS_EQUALS"};
            case LEFT_SHIFT_EQUALS:
                return {"LEFT_SHIFT_EQUALS"};
            case LEFT_SHIFT:
                return {"LEFT_SHIFT"};
            case DIVIDE_EQUALS:
                return {"DIVIDE_EQUALS"};
            case NEWLINE:
                return {"NEWLINE"};
            case ELLIPSIS:
                return {"ELLIPSIS"};
            case SCOPE:
                return {"SCOPE"};
            case LITERAL_DEF:
                return {"LITERAL_DEF"};
            default:
                return {"UNKNOWN TOKEN"};
        }
    }



};


#endif //CPLANE_TOKENS_H

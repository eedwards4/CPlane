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
    static std::string what_is(int id) {
    switch (id) {
        case STRING_LITERAL:
            return {"string literal"};
        case CHAR_LITERAL:
            return {"char literal"};
        case INT_AS_STRING:
            return {"int as string"};
        case FLOAT_AS_STRING:
            return {"float as string"};
        case TOKEN_AS_STRING:
            return {"token as string"};
        case OPEN_PAREN:
            return {"("};
        case CLOSE_PAREN:
            return {")"};
        case OPEN_BRACE:
            return {"{"};
        case CLOSE_BRACE:
            return {"}"};
        case OPEN_BRACKET:
            return {"["};
        case CLOSE_BRACKET:
            return {"]"};
        case PLUS_PLUS:
            return {"++"};
        case PLUS_EQUALS:
            return {"+="};
        case MINUS_MINUS:
            return {"--"};
        case MINUS_EQUALS:
            return {"-="};
        case RIGHT_SLIM_ARROW:
            return {"->"};
        case BOOLEAN_AND:
            return {"&&"};
        case AND_EQUALS:
            return {"&="};
        case BOOLEAN_OR:
            return {"||"};
        case OR_EQUALS:
            return {"|="};
        case NOT_EQUALS:
            return {"!="};
        case MOD_EQUALS:
            return {"%="};
        case TIMES_EQUALS:
            return {"*="};
        case EQUALS_EQUALS:
            return {"=="};
        case XOR_EQUALS:
            return {"^="};
        case TOKEN_PASTE:
            return {"##"};
        case GREATER_EQUALS:
            return {">="};
        case RIGHT_SHIFT_EQUALS:
            return {">>="};
        case RIGHT_SHIFT:
            return {">>"};
        case LESS_EQUALS:
            return {"<="};
        case LEFT_SHIFT_EQUALS:
            return {"<<="};
        case LEFT_SHIFT:
            return {"<<"};
        case DIVIDE_EQUALS:
            return {"/="};
        case NEWLINE:
            return {"\n"};
        case ELLIPSIS:
            return {"..."};
        case SCOPE:
            return {"::"};
        case LITERAL_DEF:
            return {"literal def"};
        default:
            return {"unknown token"};
    }
}


};


#endif //CPLANE_TOKENS_H

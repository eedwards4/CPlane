#ifndef TOKEN_H
#define TOKEN_H

#include <string>

class Token {
    public:
        Token() {};
        Token(char c, std::string t, int ln) { _value += c; _type = t; _lineNumber = ln; }
        Token(std::string s, std::string t, int ln) { _value += s; _type = t; _lineNumber = ln; }

        std::string& value() { return _value; }
        std::string& type() { return _type; }

        bool& eof() { return _eof; }
        bool& isIdentifier() { return _isId; }
        bool& isInteger() { return _isInt; }
        bool& isString() { return _isStr; }

		int& lineNumber() { return _lineNumber; }

		// Token*& sibling() { return _sibling; }
		// Token*& child() { return _child; }
		Token*& next() { return _next; }

    private:
        std::string _value = "";
        std::string _type = "";

        bool _eof = false;
        bool _isId = false;
        bool _isInt = false;
        bool _isStr = false;

		int _lineNumber;

		// Token* _sibling = nullptr;
		// Token* _child = nullptr;
		Token* _next = nullptr;
};

#endif
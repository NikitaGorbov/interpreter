#ifndef _CONST_H
#define _CONST_H

enum OPERATOR {
    IF, ELSE, ENDIF, WHILE, ENDWHILE, GOTO, COLON,
    PRINT,
    ASSIGN,
    LBRACKET, RBRACKET,
    OR,
    AND,
    BITOR,
    XOR,
    BITAND,
    EQ, NEQ,
    LEQ, LT, GEQ, GT,
    SHL, SHR,
    PLUS, MINUS,
    MULTIPLY, DIV, MOD,
};

extern int PRIORITY[];
extern std::string OPERTEXT[];
extern int OP_NUM;

#endif
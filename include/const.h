#ifndef _CONST_H
#define _CONST_H

enum OPERATOR {
    IF, ELSE, ENDIF, WHILE, ENDWHILE, GOTO, COLON,
    RETURN,
    PRINT,
    SIZE,
    ASSIGN,
    LVALUEARRAY,
    LBRACKET, RBRACKET, LSQRBRACKET, RSQRBRACKET,
    RVALUEARRAY,
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
#include <string>


#ifndef _CONST_H
#define _CONST_H

int PRIORITY[] = {
    -1, -1, -1, -1, -1, -1, -1,
    -1,
    -1,
    -1,
    -1,
    0, 0, 0, 0,
    1,
    1,
    2,
    3,
    4,
    5,
    6, 6,
    7, 7, 7, 7,
    8, 8,
    9, 9,
    10, 10, 10,
};

std::string OPERTEXT [] = {
    "if", "else", "endif", "while", "endwhile", "goto", ":",
    "print",
    "size",
    " := " ,
    "lvalue",
    "(", ")", "[", "]",
    "rvalue",
    " or " ,
    " and " ,
    "|",
    "^",
    "&",
    " == " , " != " ,
    " <= " , " <" , " >= " , " >" ,
    " shl" , " shr" ,
    "+" , " -" ,
    "*", "/", "%"
};

int OP_NUM = sizeof(OPERTEXT) / sizeof(std::string);

#endif
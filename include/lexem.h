#include "const.h"
#include <map>

#ifndef _LEXEM_H
#define _LEXEM_H

class Lexem {
public:
    Lexem();
    virtual int getPriority();
    virtual int getValue();
    virtual void setValue(int);
    virtual ::OPERATOR getType();
    virtual std::string getName();
    virtual ~Lexem();
};

class Number : public Lexem {
    int value;
public:
    Number(int);
    virtual int getValue();
};

class Oper : public Lexem {
    OPERATOR opertype;
public:
    Oper(OPERATOR);
    OPERATOR getType();
    int getPriority();
    int getValue(const Number& left, const Number& right);
    int getResultOne(Lexem*, bool*, int);
    int getResultTwo(Lexem*, Lexem*);
};

class Variable : public Lexem {
    std::string name;
    int value;
public:
    Variable(std::string, int = 0);
    int getValue();
    void setValue(int value);
    std::string getName();
};

class Array : public Lexem {
    int *data;
    int size;
    std::string name;
public:
    Array(std::string, int);
    ~Array();
    int getData(int);
    void setData(int, int);
};

#endif
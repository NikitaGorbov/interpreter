#include "const.h"
#include <map>

#ifndef _LEXEM_H
#define _LEXEM_H

class Lexem {
public:
    Lexem();
    virtual void debugPoliz();
    virtual int getPriority();
    virtual int getValue();
    virtual void setValue(int);
    virtual ::OPERATOR getType();
    virtual int getResultOne(Lexem*, bool*, int);
    virtual int getResultTwo(Lexem*, Lexem*);
    virtual std::string getName();
    virtual ~Lexem();
};

class Number : public Lexem {
    int value;
public:
    virtual void debugPoliz();
    Number(int);
    virtual int getValue();
};

class Oper : public Lexem {
    OPERATOR opertype;
public:
    Oper(OPERATOR);
    virtual void debugPoliz();
    OPERATOR getType();
    virtual int getPriority();
    int getValue(const Number& left, const Number& right);
    virtual int getResultOne(Lexem*, bool*, int);
    virtual int getResultTwo(Lexem*, Lexem*);
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

#endif
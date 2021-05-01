#include "const.h"
#include <map>

#ifndef _LEXEM_H
#define _LEXEM_H

struct Space {
    std::map<std::string, int> variablesMap;
};

class Lexem {
public:
    Lexem() {}
    virtual int getPriority();
    virtual int getValue();
    virtual void setValue(int);
    virtual OPERATOR getType();
    virtual const std::string getName();
    virtual ~Lexem() {}
};

class Function {
    int line;
    int argsNumber;
    std::vector<std::string> argsNames;
public:
    Function(int, int, const std::vector<std::string> &);
    int getLine() {return line;}
    int getArgsNumber();
    const std::vector<std::string> &getArgsNames();
};

class Number : public Lexem {
    int value;
public:
    Number(int number) : value(number) {}
    int getValue();
};

class Oper : public Lexem {
    OPERATOR opertype;
public:
    Oper(OPERATOR opertype) : opertype(opertype) {}
    OPERATOR getType();
    int getPriority();
    int getValue(const Number& left, const Number& right);
    int getResultOne(Lexem*, bool*, int);
    int getResultTwo(Lexem*, Lexem*);
};

class Variable : public Lexem {
    std::string name;
public:
    Variable(std::string &);
    int getValue();
    void setValue(int value);
    const std::string getName();
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
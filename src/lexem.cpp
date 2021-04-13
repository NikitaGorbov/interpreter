#include <string>
#include <iostream>
#include "const.h"
#include "lexem.h"

int Lexem::getPriority() {
    return -1;
}

void Lexem::debugPoliz() {}

int Lexem::getValue() {
    return 0;
}

void Lexem::setValue(int) {
}

OPERATOR Lexem::getType() {
    return ASSIGN;
}

int Lexem::getResult(Lexem*, Lexem*) {return 0;}

Lexem::Lexem() {}

Lexem::~Lexem() {}

void Number::debugPoliz() {
    std::cout << value << " ";
}

Number::Number(int number) : value(number) {}

int Number::getValue() {
    return value;
}

std::string Lexem::getName() {
    return "ERROR";
}

void Oper::debugPoliz() {
    std::cout << "op" <<  opertype << " ";
}

Oper::Oper(OPERATOR opertype) : opertype(opertype) {}

OPERATOR Oper::getType() {
    return opertype;
}

int Oper::getResult(Lexem* left, Lexem* right) {
    int ans = 0;
    switch (opertype) {

        case ASSIGN:
        left -> setValue(right -> getValue());
        ans = left -> getValue();
        break;

        case OR:
        ans = (left -> getValue() || right -> getValue());
        break;

        case AND:
        ans = (left -> getValue() && right -> getValue());
        break;

        case BITOR:
        ans = (left -> getValue() | right -> getValue());
        break;

        case XOR:
        ans = (left -> getValue() ^ right -> getValue());
        break;

        case BITAND:
        ans = (left -> getValue() & right -> getValue());
        break;

        case EQ:
        ans = (left -> getValue() == right -> getValue());
        break;

        case NEQ:
        ans = (left -> getValue() != right -> getValue());
        break;

        case LEQ:
        ans = left -> getValue() <= right -> getValue();
        break;

        case LT:
        ans = left -> getValue() < right -> getValue();
        break;

        case GEQ:
        ans = left -> getValue() >= right -> getValue();
        break;

        case GT:
        ans = left -> getValue() > right -> getValue();
        break;

        case SHL:
        ans = left -> getValue() << right -> getValue();
        break;

        case SHR:
        ans = left -> getValue() >> right -> getValue();
        break;

        case PLUS:
        ans = left -> getValue() + right -> getValue();
        break;

        case MINUS:
        ans = left -> getValue() - right -> getValue();
        break;

        case MULTIPLY:
        ans = left -> getValue() * right -> getValue();
        break;

        case DIV:
        if (right -> getValue() != 0) {
            ans = left -> getValue() / right -> getValue();
        } else {
            std::cout << "Devision by zero!" << std::endl;
            ans = 0;
        }
        break;

        case MOD:
        if (right -> getValue() != 0) {
            ans = left -> getValue() % right -> getValue();
        } else {
            std::cout << "Devision by zero!" << std::endl;
            ans = 0;
        }
        break;

        default:
        std::cout << "Unknown operator" << std::endl;
    }
    return ans;
}

int Oper::getPriority() {
    return PRIORITY[opertype];
}

Variable::Variable(std::string name, int value) : name(name), value(value) {}

int Variable::getValue() {
    return value;
}

void Variable::setValue(int newValue) {
    value = newValue;
}

std::string Variable::getName() {
    return name;
}
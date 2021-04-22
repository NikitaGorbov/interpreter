#include <string>
#include <iostream>
#include "const.h"
#include "lexem.h"

extern std::map<std::string, int> labelsMap;
extern std::map<int, int> conditionJumpLines;
extern std::map<std::string, Array*> arraysMap;

int Lexem::getPriority() {
    return -1;
}

int Lexem::getValue() {
    return 0;
}

void Lexem::setValue(int) {
}

OPERATOR Lexem::getType() {
    //return ASSIGN;
}

Lexem::Lexem() {}

Lexem::~Lexem() {}

Number::Number(int number) : value(number) {}

int Number::getValue() {
    return value;
}

std::string Lexem::getName() {
    return "ERROR";
}

Oper::Oper(OPERATOR opertype) : opertype(opertype) {}

OPERATOR Oper::getType() {
    return opertype;
}

int Oper::getResultOne(Lexem *operand, bool *jumpFlag, int row) {
    int ans = 0;
    switch (opertype) {

        case GOTO:
        ans = labelsMap[operand -> getName()];
        *jumpFlag = true;
        break;

        case IF:
        if (operand -> getValue() == 0) {
            ans = conditionJumpLines[row];
            *jumpFlag = true;
        } else {
            *jumpFlag = false;
        }
        break;

        case WHILE:
        if (operand -> getValue() == 0) {
            ans = conditionJumpLines[row];
            *jumpFlag = true;
        } else {
            *jumpFlag = false;
        }
        break;

        case PRINT:
        std::cout << operand -> getValue() << std::endl;
        break;

        default:
        std::cout << "Unknown operator" << std::endl;
    }

    return ans;
}

int Oper::getResultTwo(Lexem* left, Lexem* right) {
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

        case SIZE:
        {
            Array *newArray = new Array(left -> getName(), right -> getValue());
            arraysMap[left -> getName()] = newArray;
            std::cout << "Created array " << left -> getName() << std::endl;
        }
        break;

        case RVALUEARRAY:
        if (arraysMap.find(left -> getName()) == arraysMap.end()) {
            std::cout << left -> getName() << " was not allocated" << std::endl;
            break;
        }
        ans = arraysMap[left -> getName()] -> getData(right -> getValue());
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

Array::Array(std::string name, int newSize) : name(name) {
    if (data) {
        delete[] data;
    }
    data = new int(newSize);
    size = newSize;
}

Array::~Array() {
    delete[] data;
}

int Array::getData(int position) {
    if (position < size) {
        return data[position];
    }
    std::cout << "out of range" << std::endl;
    return 0;
}

void Array::setData(int position, int value) {
    if (position < size) {
        data[position] = value;
    } else {
        std::cout << position << " is out of range" << std::endl;
    }
}
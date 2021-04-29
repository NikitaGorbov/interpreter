#include <string>
#include <stack>
#include <vector>
#include <iostream>
#include "const.h"
#include "lexem.h"

extern std::map<std::string, int> labelsMap;
extern std::map<int, int> conditionJumpLines;
extern std::map<std::string, Array*> arraysMap;

extern std::stack<Space> SpacesStack;

int Lexem::getPriority() {
    return -1;
}

int Lexem::getValue() {
    return 0;
}

void Lexem::setValue(int) {
}

Function::Function(std::string &name, int line, int argsNumber, const std::vector<std::string> &argsNames)
    : name(name), line(line), argsNumber(argsNumber), argsNames(argsNames) {}

int Function::getLine() {return line;}
int Function::getArgsNumber() {return argsNumber;}
const std::vector<std::string> &Function::getArgsNames() {return argsNames;}

OPERATOR Lexem::getType() {
    // return ASSIGN;
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
        *jumpFlag = true;
        return labelsMap[operand -> getName()];

        case IF:
        if (operand -> getValue() == 0) {
            *jumpFlag = true;
            return conditionJumpLines[row];
        } else {
            *jumpFlag = false;
        }
        break;

        case WHILE:
        if (operand -> getValue() == 0) {
            *jumpFlag = true;
            return conditionJumpLines[row];
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
    switch (opertype) {
        case ASSIGN:
        left -> setValue(right -> getValue());
        return left -> getValue();

        case OR:
        return (left -> getValue() || right -> getValue());

        case AND:
        return (left -> getValue() && right -> getValue());

        case BITOR:
        return (left -> getValue() | right -> getValue());

        case XOR:
        return (left -> getValue() ^ right -> getValue());

        case BITAND:
        return (left -> getValue() & right -> getValue());

        case EQ:
        return (left -> getValue() == right -> getValue());

        case NEQ:
        return (left -> getValue() != right -> getValue());

        case LEQ:
        return left -> getValue() <= right -> getValue();

        case LT:
        return left -> getValue() < right -> getValue();

        case GEQ:
        return left -> getValue() >= right -> getValue();

        case GT:
        return left -> getValue() > right -> getValue();

        case SHL:
        return left -> getValue() << right -> getValue();

        case SHR:
        return left -> getValue() >> right -> getValue();

        case PLUS:
        return left -> getValue() + right -> getValue();

        case MINUS:
        return left -> getValue() - right -> getValue();

        case MULTIPLY:
        return left -> getValue() * right -> getValue();

        case DIV:
        if (right -> getValue() != 0) {
            return left -> getValue() / right -> getValue();
        } else {
            std::cout << "Devision by zero!" << std::endl;
            return 0;
        }

        case MOD:
        if (right -> getValue() != 0) {
            return left -> getValue() % right -> getValue();
        } else {
            std::cout << "Devision by zero!" << std::endl;
            return 0;
        }

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
        return arraysMap[left -> getName()] -> getData(right -> getValue());

        default:
        std::cout << "Unknown operator" << std::endl;
    }
    return 0;
}

int Oper::getPriority() {
    return PRIORITY[opertype];
}

Variable::Variable(std::string &name, int value) : name(name), value(value) {}

int Variable::getValue() {
    return SpacesStack.top().variablesMap[name];
}

void Variable::setValue(int newValue) {
    SpacesStack.top().variablesMap[name] = newValue;
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
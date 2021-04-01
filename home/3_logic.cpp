#include <string>
#include <vector>
#include <stack>
#include <iostream>
#include <iomanip>
#include <map>

enum OPERATOR {
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

int PRIORITY[] = {
    -1,
    0, 0,
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
    " := " ,
    "(", ")",
    " or " ,
    " and " ,
    "|",
    "^",
    "&",
    " == " , " != " ,
    " <= " , " <" , " >= " , " >" ,
    " shl" , " shr" ,
    " + " , " -" ,
    "*", "/", "%"
};


class Lexem {
public:
    Lexem();
    virtual void debugPoliz();
    virtual int getPriority();
    virtual int getValue();
    virtual void setValue(int);
    virtual ::OPERATOR getType();
    virtual int getResult(Lexem*, Lexem*);
    virtual ~Lexem();
};

int Lexem::getPriority() {
    return -1;
}

void Lexem::debugPoliz() {}

int Lexem::getValue() {
    return 0;
}

void Lexem::setValue(int) {
}

::OPERATOR Lexem::getType() {
    return ASSIGN;
}

Lexem::Lexem() {}

Lexem::~Lexem() {}

class Number : public Lexem {
    int value;
public:
    virtual void debugPoliz();
    Number(int);
    virtual int getValue();
};

void Number::debugPoliz() {
    std::cout << value << " ";
}

Number::Number(int number) : value(number) {}

int Number::getValue() {
    return value;
}

int Lexem::getResult(Lexem*, Lexem*) {return 0;}

class Oper : public Lexem {
    OPERATOR opertype;
public:
    Oper(OPERATOR);
    virtual void debugPoliz();
    OPERATOR getType();
    virtual int getPriority();
    int getValue(const Number& left,
                 const Number& right);
    virtual int getResult(Lexem*, Lexem*);
};

void Oper::debugPoliz() {
    std::cout << "op" <<  opertype << " ";
}

Oper::Oper(OPERATOR opertype) : opertype(opertype) {}

::OPERATOR Oper::getType() {
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
        (left -> getValue() || right -> getValue()) ? ans = 1 : ans = 0;
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

class Variable : public Lexem {
    std::string name;
    int value;
public:
    Variable(std::string, int);
    int getValue();
    void setValue(int value);
    std::string getName();
};

Variable::Variable(std::string name, int value = 0) : name(name), value(value) {}

int Variable::getValue() {
    return value;
}

void Variable::setValue(int newValue) {
    value = newValue;
}

std::string Variable::getName() {
    return name;
}

std::map<std::string, Variable*> variablesMap;

std::vector<Lexem *> parseLexem(
    std::string codeline);

std::vector<Lexem *> buildPoliz(
    std::vector<Lexem *> infix);

int evaluatePoliz(std::vector<Lexem *> poliz);

Number *read_number(std::string codeline, int *pos) {
    int number = 0;

    if (codeline[*pos] >= '0' && codeline[*pos] <= '9') {
        for (int j = *pos; j <= codeline.size(); j++) {
            if (codeline[j] >= '0' && codeline[j] <= '9') {
                number = number * 10 + codeline[j] - '0';
            } else {
                // set pos to position after number
                *pos = j - 1;
                return new Number(number);
            }
        }
    }
    return nullptr;
}

Oper *read_operator(std::string codeline, int *pos) {
    int n = sizeof(OPERTEXT) / sizeof(std::string);

    for (int op = 0; op < n; op++) {
        std::string subcodeline = codeline.substr(*pos, OPERTEXT[op].size());
        if (!OPERTEXT[op].compare(subcodeline)) {
            *pos += subcodeline.size() - 1;
            return new Oper((OPERATOR)op);
        }
    }
    return nullptr;
}

Variable *read_variable(std::string codeline, int *pos) {
    Variable *tmp = nullptr;

    if ((codeline[*pos] >= 'a' && codeline[*pos] <= 'z') ||
        (codeline[*pos] >= 'A' && codeline[*pos] <= 'Z')) {
        // read the whole variable name
        for (int j = *pos; j <= codeline.size(); j++) {
            if ((codeline[j] >= 'a' && codeline[j] <= 'z') ||
                (codeline[j] >= 'A' && codeline[j] <= 'Z')) {
                // DO NOTHING

            } else {
                std::string varName = codeline.substr(*pos, j - *pos);

                if (variablesMap.find(varName) == variablesMap.end()) {
                    Variable *newVar = new Variable(varName);
                    variablesMap[varName] = newVar;
                }
                tmp = variablesMap[varName];
                // set i to position after variable name
                *pos = j - 1;
                break;
            }
        }
    }
    return tmp;
}

std::vector<Lexem *> parseLexem(std::string codeline) {
    std::vector<Lexem *> Lex;
    // int number = 0;

    for (int i = 0; i <= codeline.size(); i++) {
        Lexem *newLexem = nullptr;

        newLexem = read_number(codeline, &i);
        if (newLexem) {
            Lex.push_back(newLexem);
            continue;
        }

        newLexem = read_operator(codeline, &i);
        if (newLexem) {
            Lex.push_back(newLexem);
            continue;
        }

        newLexem = read_variable(codeline, &i);
        if (newLexem) {
            Lex.push_back(newLexem);
            continue;
        } 
    }
    return Lex;
}

std::vector<Lexem *> buildPoliz(std::vector<Lexem *> infix) {
    std::vector<Lexem *> poliz;
    std::stack<Lexem *> operators;

    for (Lexem* curLexem : infix) {
        // push numbers to poliz
        if ((typeid(*curLexem) != typeid(Oper))) {
            poliz.push_back(curLexem);
        } else if (operators.size()) {
            if (curLexem -> getPriority() > operators.top() -> getPriority() ||
                curLexem -> getPriority() == PRIORITY[LBRACKET]) {
                if (curLexem -> getType() != RBRACKET) {
                    operators.push(curLexem);

                } else {
                    while (operators.top() -> getType() != LBRACKET) {
                            poliz.push_back(operators.top());
                            operators.pop();
                    }
                    if (operators.top() -> getType() == LBRACKET) {
                        operators.pop();
                    }
                }

            } else {
                // move all operators to poliz
                while (operators.size()) {
                    poliz.push_back(operators.top());
                    operators.pop();
                }
                operators.push(curLexem);
            }

        } else {
            // operator stack is empty
            operators.push(curLexem);
        }
    }

    while (operators.size()) {
        poliz.push_back(operators.top());
        operators.pop();
    }

    return poliz;
}

int evaluatePoliz(std::vector<Lexem *> poliz) {
    std::stack<Lexem *> computing;
    int tempNum;
    OPERATOR type;
    Lexem *newTempResult = nullptr;

    for (auto i : poliz) {
        if ((typeid(*i) == typeid(Number)) ||
            (typeid(*i) == typeid(Variable))) {
            computing.push(i);
        } else if (computing.size() > 1) {
            Lexem *right = computing.top();
            computing.pop();
            Lexem *left = computing.top();
            computing.pop();

            // get result of operation
            tempNum = i -> getResult(left, right);
            if (newTempResult) {
                delete newTempResult;
            }
            Lexem *newTempResult = new Number(tempNum);
            computing.push(newTempResult);

        } else {
            std::cout << "Incorrect expression" << std::endl;
            break;
        }
    }

    if (computing.size() == 1) {
        tempNum = computing.top() -> getValue();
        if (newTempResult) {
            delete newTempResult;
        }
        return tempNum;
    }
    std::cout << "Something went wrong" << std::endl;
    return 0;
}

bool greeting(std::string *codeline) {
    std::cout << "> ";
    std::getline(std::cin, *codeline);
    if (std::cin.eof()) {
        return false;
    }
    return true;
}

int main() {
    std::string codeline;
    std::vector<Lexem *> infix;
    std::vector<Lexem *> postfix;
    int value;

    while (greeting(&codeline)) {
        infix = parseLexem(codeline);

        postfix = buildPoliz(infix);
        value = evaluatePoliz(postfix);
        for (Lexem* lexem : infix) {
            if (typeid(*lexem) != typeid(Variable)) {
                delete lexem;
            }
        }
        infix.clear();
        std::cout << value << std::endl;
    }
    return 0;
}
#include <string>
#include <vector>
#include <stack>
#include <iostream>
#include <iomanip>
#include <map>

enum OPERATOR {
    LBRACKET, RBRACKET,
    ASSIGN,
    PLUS, MINUS,
    MULTIPLY,
    NOT_OPERATOR
};

int PRIORITY[] = {
    -1, -1,
    0,
    1, 1,
    2,
    666
};

class Lexem {
public:
    Lexem();
    virtual void debugPoliz();
    virtual int getPriority();
    virtual int getValue();
    virtual void setValue(int);
    virtual ::OPERATOR getType();
    virtual ~Lexem();
};

int Lexem::getPriority() {
    return -1;
}

void Lexem::debugPoliz() {};

int Lexem::getValue() {
    return 0;
}

void Lexem::setValue(int) {
}

::OPERATOR Lexem::getType() {
    return NOT_OPERATOR;
}

Lexem::Lexem() {

}

Lexem::~Lexem() {

}

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

Number::Number(int number) : value(number) {};

int Number::getValue() {
    return value;
};

class Oper : public Lexem {
    OPERATOR opertype;
public:
    Oper(char);
    virtual void debugPoliz();
    OPERATOR getType();
    virtual int getPriority();
    int getValue(const Number& lefg,
                 const Number& right);
};

void Oper::debugPoliz() {
    std::cout << "op" <<  opertype << " ";
}

Oper::Oper(char op) {
    switch (op) {
        case '(':
        opertype = LBRACKET;
        break;

        case ')':
        opertype = RBRACKET;
        break;

        case '+':
        opertype = PLUS;
        break;

        case '-':
        opertype = MINUS;
        break;

        case '*':
        opertype = MULTIPLY;
        break;

        case '=':
        opertype = ASSIGN;
        break;
    }
}

::OPERATOR Oper::getType() {
    return opertype;
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
};

Variable::Variable(std::string name, int value = 0) : name(name), value(value) {}

int Variable::getValue() {
    return value;
}

void Variable::setValue(int newValue) {
    value = newValue;
}

std::map<std::string, Variable*> variablesMap;

std::vector<Lexem *> parseLexem(
    std::string codeline);
    
std::vector<Lexem *> buildPoliz(
    std::vector<Lexem *> infix);

int evaluatePoliz(
    std::vector<Lexem *> poliz);

    std::vector<Lexem *> parseLexem(std::string codeline) {
    std::vector<Lexem *> Lex;
    int number = 0;

    for (int i = 0; i <= codeline.size(); i++) {

        number = 0;
        // If it's a digit, read the whole number
        if (codeline[i] >= '0' && codeline[i] <= '9') {
            for (int j = i; j <= codeline.size(); j++) {
                if (codeline[j] >= '0' && codeline[j] <= '9') {
                    number = number * 10 + codeline[j] - '0';
                } else {
                    // set i to position after number
                    i = j - 1;
                    Number *num = new Number(number);
                    Lex.push_back(num);
                    break;
                }
            }
        } else if ((codeline[i] >= 'a' && codeline[i] <= 'z') ||
                    (codeline[i] >= 'A' && codeline[i] <= 'Z')) {
            // read the whole variable name
            for (int j = i; j <= codeline.size(); j++) {
                if ((codeline[j] >= 'a' && codeline[j] <= 'z') ||
                    (codeline[j] >= 'A' && codeline[j] <= 'Z')) {
                    // DO NOTHING

                } else {
                    std::string varName = codeline.substr(i, j-i);

                    if (variablesMap.find(varName) == variablesMap.end()) {
                        Variable *newVar = new Variable(varName);
                        variablesMap[varName] = newVar; 
                    }
                    Variable *tmp = variablesMap[varName];
                    Lex.push_back(tmp);
                    // set i to position after variable name
                    i = j - 1;
                    break;
                }
            }
        } else {
            Oper *op = NULL;
            switch (codeline[i]) {

                case '+':
                op = new Oper('+');
                break;

                case '-':
                op = new Oper('-');
                break;

                case '*':
                op = new Oper('*');
                break;

                case '(':
                op = new Oper('(');
                break;

                case ')':
                op = new Oper(')');
                break;

                case '=':
                op = new Oper('=');
                break;

                case ' ':
                break;
            }
            if (op) {
                Lex.push_back(op);
            }
        }
    }
    return Lex;
}

std::vector<Lexem *> buildPoliz(std::vector<Lexem *> infix) {
    std::vector<Lexem *> poliz;
    std::stack<Lexem *> operators;

    for (Lexem* i : infix) {

        // push numbers to poliz
        if ((typeid(*i) != typeid(Oper))) {
            poliz.push_back(i);
        } else if (operators.size()) {
            if (i -> getPriority() > operators.top() -> getPriority() ||
                i -> getPriority() == PRIORITY[LBRACKET]) {
                if (i -> getType() != RBRACKET) {
                    operators.push(i);

                } else {
                    while (1) {
                        if (operators.top() -> getType() == LBRACKET) {
                            operators.pop();
                            break;

                        } else {
                            poliz.push_back(operators.top());
                            operators.pop();
                        }
                    }
                }

            // move all operators to poliz
            } else {
                while (operators.size()) {
                    poliz.push_back(operators.top());
                    operators.pop();
                }
                operators.push(i);
            }

        // operator stack is empty
        } else {
            operators.push(i);
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
    Lexem *newTempResult = 0;
    
    for (auto i : poliz) {
        if ((typeid(*i) == typeid(Number)) ||
            (typeid(*i) == typeid(Variable))) {
            computing.push(i);
        } else if (computing.size() > 1) {
            Lexem *right = computing.top();
            computing.pop();
            Lexem *left = computing.top();
            computing.pop();
            type = i -> getType();
            switch (type) {
                case PLUS:
                tempNum = left -> getValue() + right -> getValue();
                break;

                case MINUS:
                tempNum = left -> getValue() - right -> getValue();
                break;

                case MULTIPLY:
                tempNum = left -> getValue() * right -> getValue();
                break;

                case ASSIGN:
                tempNum = right -> getValue();
                left -> setValue(tempNum);
                break;
            }
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
    
    if (computing.size() == 1){
        tempNum = computing.top() -> getValue();
        if (newTempResult) {
            delete newTempResult;
        }
        return tempNum;
    } else {
        std::cout << "Something went wrong" << std::endl;
        return 0;
    }
}

int main() {
    std::string codeline;
    std::vector<Lexem *> infix;
    std::vector<Lexem *> postfix;
    int value;

    while (1) {
        std::cout << "> ";
        std::getline(std::cin, codeline);
        if (std::cin.eof()) {
            break;
        }
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
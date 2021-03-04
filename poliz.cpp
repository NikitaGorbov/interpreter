#include <string>
#include <vector>
#include <stack>
#include <iostream>
#include <iomanip>

enum OPERATOR {
    LBRACKET, RBRACKET,
    PLUS, MINUS,
    MULTIPLY,
    NOT_OPERATOR
};

int PRIORITY[] = {
    -1, -1,
    0, 0,
    1
};

class Lexem {
public:
    Lexem();
    virtual void debugPoliz();
    virtual int getPriority();
    virtual int getValue();
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
    }
}

::OPERATOR Oper::getType() {
    return opertype;
}

int Oper::getPriority() {
    return PRIORITY[opertype];
}

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
        if (codeline[i] == ' ') {
        }
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
        } else {
            Oper *op = NULL;
            switch (codeline[i]) {

                case '+' :
                op = new Oper('+');
                break;

                case '-' :
                op = new Oper('-');
                break;

                case '*' :
                op = new Oper('*');
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
            if (i -> getPriority() > operators.top() -> getPriority()) {
                operators.push(i);
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
    std::stack<Lexem *> tempStack;
    int tempNum = 0;
    OPERATOR type;

    while (poliz.size() > 1) {
        for (int i = 0; i < poliz.size(); i++) {
            if ((typeid(*(poliz.at(i))) == typeid(Oper))) {
                if (!poliz.at(i-1) || !poliz.at(i-2)) {
                    std::cout << "Something's wrong" << std::endl;
                }
                if (i > 0) {
                    type = poliz.at(i) -> getType();
                    switch (type) {
                        case PLUS:
                        tempNum = poliz.at(i - 2) -> getValue() + poliz.at(i - 1) -> getValue();
                        break;

                        case MINUS:
                        tempNum = poliz.at(i - 2) -> getValue() - poliz.at(i - 1) -> getValue();
                        break;

                        case MULTIPLY:
                        tempNum = poliz.at(i - 2) -> getValue() * poliz.at(i - 1) -> getValue();
                        break;
                    }
                    Lexem* newTempNum = new Number(tempNum);
                    tempStack.push(newTempNum);
                    poliz.erase(poliz.begin() + i - 2, poliz.begin() + i + 1);

                    poliz.insert(poliz.begin() + i - 2, newTempNum);
                    break;
                } else {
                    std::cout << "Incorrect notation" << std::endl;
                    return 0;
                }
            }
        }
    }

    while (tempStack.size()) {
        delete tempStack.top();
        tempStack.pop();
    }
    return tempNum;
}

int main() {
    std::string codeline;

    std::vector<Lexem *> infix;
    std::vector<Lexem *> postfix;
    int value;

    while (1) {
        std::cout << "> ";
        std::getline(std::cin, codeline);
        infix = parseLexem(codeline);
        
        postfix = buildPoliz(infix);
        value = evaluatePoliz(postfix);
        for (Lexem* lexem : infix) {
            delete lexem;
        }
        infix.clear();
        std::cout << value << std::endl;        
    }
    return 0;
}
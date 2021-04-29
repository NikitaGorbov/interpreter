#include <vector>
#include <string>
#include <stack>
#include <iostream>
#include "lexem.h"

extern std::map<std::string, Function*> functionsMap;

std::vector<Lexem *> buildPoliz(const std::vector<Lexem *> &infix) {
    std::vector<Lexem *> poliz;
    std::stack<Lexem *> operators;
    std::stack<Lexem *> functions;

    for (Lexem* curLexem : infix) {
        if (!curLexem) {
            continue;
        }
        // push numbers to poliz
        if ((typeid(*curLexem) != typeid(Oper))) {
            if (functionsMap.find(curLexem -> getName()) == functionsMap.end()) {
                poliz.push_back(curLexem);
            } else {
                functions.push(curLexem);
                // std::cout << "function call found" << std::endl;
            }

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
                    if (functions.size()) {
                        poliz.push_back(functions.top());
                        functions.pop();
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
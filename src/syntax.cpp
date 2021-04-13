#include <vector>
#include <string>
#include <stack>
#include "lexem.h"

std::vector<Lexem *> buildPoliz(std::vector<Lexem *> infix) {
    std::vector<Lexem *> poliz;
    std::stack<Lexem *> operators;

    for (Lexem* curLexem : infix) {
        if (!curLexem) {
            continue;
        }
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
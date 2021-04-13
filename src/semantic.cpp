#include <vector>
#include <stack>
#include <string>
#include <iostream>
#include "lexem.h"
#include "lexical.h"

int evaluatePoliz(std::vector<Lexem *> poliz, int row, int *result) {
    std::stack<Lexem *> computing;
    int tempNum;
    Lexem *newTempResult = nullptr;

    for (auto lexemIter : poliz) {
        if (!lexemIter) {
            continue;
        }
        if ((typeid(*lexemIter) == typeid(Number)) ||
            (typeid(*lexemIter) == typeid(Variable))) {
            computing.push(lexemIter);
        } else if (computing.size() > 0) {
            if (lexemIter -> getType() == GOTO) {
                int newRow = labelsMap[computing.top() -> getName()];
                computing.pop();
                std::cout << "jumping to " << newRow << std::endl;
                return newRow;
            }
            Lexem *right = computing.top();
            computing.pop();
            Lexem *left = computing.top();
            computing.pop();

            // get result of operation
            tempNum = lexemIter -> getResult(left, right);
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
        *result = tempNum;
        return row + 1;
    }
    //std::cout << "Something went wrong" << std::endl;
    return row + 1;
}
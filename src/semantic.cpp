#include <vector>
#include <stack>
#include <string>
#include <iostream>
#include "lexem.h"
#include "lexical.h"
#include "semantic.h"

extern std::map<std::string, Array*> arraysMap;

void initLabels(std::vector<Lexem *> &infix, int row) {
    for (int i = 1; i < (int)infix.size(); i++) {
        Variable *varptr = dynamic_cast<Variable*>(infix[i - 1]);
        Oper *operptr = dynamic_cast<Oper*>(infix[i]);
        if (varptr && operptr) {
            if (operptr -> getType() == COLON) {
                labelsMap[varptr -> getName()] = row;
                //delete infix[i - 1];
                delete infix[i];
                infix[i - 1] = nullptr;
                infix[i] = nullptr;
                i++;
            }
        }
    }
}

void analizeArrayElements(std::vector<Lexem *> &infix) {
    int assignLocation = -1;
    std::stack<int> arrayElementsLocations;
    int currentArrPosition;

    if (!infix.size()) {
        return;
    }

    for (int i = 0; i < (int)infix.size() - 1; i++) {
        if (infix[i] && infix[i] -> getType() == ASSIGN) {
            assignLocation = i;
        } else {
            Variable *varptr = dynamic_cast<Variable*>(infix[i]);
            if ((varptr) && (infix[i+1] -> getType() == LSQRBRACKET)) {
                arrayElementsLocations.push(i);
            }
        }
    }

    while (arrayElementsLocations.size()) {
        currentArrPosition = arrayElementsLocations.top();
        if (assignLocation > currentArrPosition && arrayElementsLocations.size() == 1) {
            // lvalue found
            Oper *lvalue = new Oper(LVALUEARRAY);
            delete infix[currentArrPosition+1];
            delete infix[assignLocation];
            infix[currentArrPosition+1] = lvalue;
            infix[assignLocation] = nullptr;
        } else {
            // rvalue found
            Oper *rvalue = new Oper(RVALUEARRAY);
            delete infix[currentArrPosition+1];
            infix[currentArrPosition+1] = rvalue;
        }
        int i = currentArrPosition + 1;
        while (i < (int)infix.size()) {
            // remove corresponding right square bracket
            if (infix[i] && infix[i] -> getType() == RSQRBRACKET) {
                delete infix[i];
                infix[i] = nullptr;
                break;
            }
            i++;
        }
        arrayElementsLocations.pop();
    }
}

void initIfJumps(std::vector< std::vector<Lexem *> > &infixLines) {
    std::stack<int> ifElseLines;
    std::stack<int> types;     // 0 - if, 1 - else
    int curIF, curElse, curEndif, typeIf, typeElse;

    for (int i = 0; i < (int)infixLines.size(); i++) {
        if (infixLines[i].size()) {
            Oper *operptr = dynamic_cast<Oper*>(infixLines[i][0]);
            if (operptr) {
                if (operptr -> getType() == IF) {
                    ifElseLines.push(i);
                    types.push(0);
                } else if (operptr -> getType() == ELSE) {
                    ifElseLines.push(i);
                    types.push(1);
                } else if (operptr -> getType() == ENDIF) {
                    if (ifElseLines.size() > 1) {
                        curElse = ifElseLines.top();
                        ifElseLines.pop();
                        curIF = ifElseLines.top();
                        ifElseLines.pop();
                        curEndif = i;
                        typeElse = types.top();
                        types.pop();
                        typeIf = types.top();
                        types.pop();
                        if (typeIf == 0 && typeElse == 1) {
                            conditionJumpLines[curIF] = curElse + 1;
                            conditionJumpLines[curElse] = curEndif + 1;
                        } else {
                            std::cout << "if - else - endif don't match" << std::endl;
                        }
                    } else {
                        std::cout << "if - else - endif don't match" << std::endl;
                    }
                }
            }
        }
    }
}

void initWhileJumps(const std::vector <std::vector<Lexem*> > &infixLines) {
    std::stack<int> whileLines;
    std::vector<int> endwhileLines;

    for (int i = 0; i < (int)infixLines.size(); i++) {
        if (infixLines[i].size()) {
            Oper *operptr = dynamic_cast<Oper*>(infixLines[i][0]);
            if (operptr) {
                if (operptr -> getType() == WHILE) {
                    whileLines.push(i);
                } else if (operptr -> getType() == ENDWHILE) {
                    if (whileLines.size()) {
                        conditionJumpLines[whileLines.top()] = i + 1;
                        conditionJumpLines[i] = whileLines.top();
                        whileLines.pop();
                    } else {
                        std::cout << "endwhile on line " << i << " but no while found" << std::endl;
                    }
                }
            }
        }
    }
}

int evaluatePoliz(std::vector<Lexem *> poliz, int row, int *result) {
    std::stack<Lexem *> computing;
    int tempNum;
    Lexem *newTempResult = nullptr;
    bool jumpFlag = false;
    Oper *lexemOper = nullptr;

    for (auto lexemIter : poliz) {
        if (!lexemIter) {
            continue;
        }
        lexemOper = dynamic_cast<Oper*>(lexemIter);
        if (!lexemOper) {
            computing.push(lexemIter);
            // std::cout << "size of computing: " << computing.size() << std::endl;
        } else if (computing.size() > 0) {
            if (computing.size() == 1) {
                Lexem *operand = computing.top();
                computing.pop();
                tempNum = lexemOper -> getResultOne(operand, &jumpFlag, row);
                if (jumpFlag) {
                    return tempNum;
                }
            }

            if (lexemOper -> getType() == LVALUEARRAY && computing.size() > 2) {
                // std::cout << "here" << std::endl;
                Lexem *first = computing.top();
                computing.pop();
                Lexem *second = computing.top();
                computing.pop();
                Lexem *third = computing.top();
                computing.pop();

                arraysMap[third -> getName()] -> setData(second -> getValue(), first -> getValue());
            }

            if (computing.size() > 1) {
                Lexem *right = computing.top();
                computing.pop();
                Lexem *left = computing.top();
                computing.pop();
                tempNum = lexemOper -> getResultTwo(left, right);
            }
            // get result of operation
            if (newTempResult) {
                delete newTempResult;
            }
            Lexem *newTempResult = new Number(tempNum);
            computing.push(newTempResult);

        } else if (lexemIter -> getType() == ELSE || lexemIter -> getType() == ENDWHILE) {
            // skip else or jump to while condition
            return conditionJumpLines[row];
        } else if (lexemIter -> getType() == ENDIF) {
            continue;
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
    // std::cout << "Something went wrong" << std::endl;
    return row + 1;
}
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

std::vector<int> findElse(std::vector< std::vector<Lexem *> > &infixLines, int startRow) {
    std::vector<int> elses;

    for (int i = startRow + 1; i < (int)infixLines.size(); i++) {
        if (infixLines[i].size()) {
            Oper *operptr = dynamic_cast<Oper*>(infixLines[i][0]);
            if (operptr) {
                if (operptr -> getType() == ELSE) {
                    elses.push_back(i);
                }
            }
        }
    }
    return elses;
}

std::vector<int> findEndif(std::vector< std::vector<Lexem *> > &infixLines, int startRow) {
    std::vector<int> endifs;

    for (int i = startRow + 1; i < (int)infixLines.size(); i++) {
        if (infixLines[i].size()) {
            Oper *operptr = dynamic_cast<Oper*>(infixLines[i][0]);
            if (operptr) {
                if (operptr -> getType() == ENDIF) {
                    endifs.push_back(i);
                }
            }
        }
    }
    return endifs;
}

std::vector<int> findEndwhiles(std::vector< std::vector<Lexem *> > &infixLines, int startRow) {
    std::vector<int> endwhiles;

    for (int i = startRow + 1; i < (int)infixLines.size(); i++) {
        if (infixLines[i].size()) {
            Oper *operptr = dynamic_cast<Oper*>(infixLines[i][0]);
            if (operptr) {
                if (operptr -> getType() == ENDWHILE) {
                    endwhiles.push_back(i);
                }
            }
        }
    }
    return endwhiles;
}

void analizeArrayElements(std::vector<Lexem *> &infix) {
    int assignLocation = -1;
    std::stack<int> arrayElementsLocations;
    int currentArrPosition;

    if (!infix.size()) {
        return;
    }

    for (int i = 0; i < (int)infix.size() - 1; i++) {
        if (infix[i] -> getType() == ASSIGN) {
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
    std::stack<int> ifLines;
    std::vector<int> elseLines;
    std::vector<int> endifLines;
    int ifLinesSize;

    for (int i = 0; i < (int)infixLines.size(); i++) {
        if (infixLines[i].size()) {
            Oper *operptr = dynamic_cast<Oper*>(infixLines[i][0]);
            if (operptr) {
                if (operptr -> getType() == IF) {
                    ifLines.push(i);
                }
            }
        }
    }
    elseLines = findElse(infixLines, 0);
    endifLines = findEndif(infixLines, 0);

    ifLinesSize = ifLines.size();

    if (ifLinesSize == (int)elseLines.size() && ifLinesSize == (int)endifLines.size()) {
        for (int i = 0; i < ifLinesSize; i++) {
            int currentIf = ifLines.top();
            int currentElse = elseLines[i];
            int currentEndif = endifLines[i];
            conditionJumpLines[currentIf] = currentElse + 1;
            conditionJumpLines[currentElse] = currentEndif + 1;
            ifLines.pop();
        }
    } else {
        std::cout << "if - else - endif don't match!" << std::endl;
    }
}

void initWhileJumps(std::vector <std::vector<Lexem*> > &infixLines) {
    std::stack<int> whileLines;
    std::vector<int> endwhileLines;
    int whileLinesSize;

    for (int i = 0; i < (int)infixLines.size(); i++) {
        if (infixLines[i].size()) {
            Oper *operptr = dynamic_cast<Oper*>(infixLines[i][0]);
            if (operptr) {
                if (operptr -> getType() == WHILE) {
                    whileLines.push(i);
                }
            }
        }
    }
    endwhileLines = findEndwhiles(infixLines, 0);
    whileLinesSize = whileLines.size();

    if (whileLinesSize == (int)endwhileLines.size()) {
        for (int i = 0; i < whileLinesSize; i++) {
            int currentWhile = whileLines.top();
            int currentEndwhile = endwhileLines[i];

            conditionJumpLines[currentWhile] = currentEndwhile + 1;
            conditionJumpLines[currentEndwhile] = currentWhile;
            whileLines.pop();
        }
    } else {
        std::cout << "while - endwhile don't match!" << std::endl;
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

        } else 
            if (lexemIter -> getType() == ELSE || lexemIter -> getType() == ENDWHILE) {
                // skip else or jump to while condition
                return conditionJumpLines[row];
            } else if (lexemIter -> getType() == ENDIF) {
                continue;
            }
            else {
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
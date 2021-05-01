#include <vector>
#include <stack>
#include <string>
#include <iostream>
#include <map>
#include <cstddef>
#include "lexem.h"
#include "lexical.h"
#include "semantic.h"

#define MAX_CODE_SIZE 1000000

extern std::map<std::string, Array*> arraysMap;
extern std::map<std::string, Function*> functionsMap;
extern int StartLine;
extern int NumberOfLines;
extern std::stack<Space> SpacesStack;

void initLabels(std::vector<Lexem *> &infix, int row) {
    size_t i = 1;
    if (i < infix.size() && infix[i - 1]) {
        Variable *varptr = dynamic_cast<Variable*>(infix[i - 1]);
        Oper *operptr = dynamic_cast<Oper*>(infix[i]);
        if (varptr && operptr) {
            if (operptr -> getType() == COLON) {
                if (varptr -> getName() == "START") {
                    StartLine = row;
                }
                labelsMap[varptr -> getName()] = row + 1;
                delete infix[i - 1];
                delete infix[i];
                infix[i - 1] = nullptr;
                infix[i] = nullptr;
                i++;
            }
        }
    }
}

void findFunctions(std::vector<Lexem *> &infix, int row) {
    int argsNum = 0;
    int line;
    size_t i = 1;

    if (i < infix.size() && infix[i - 1]) {
        std::vector<std::string> varNames;
        std::string functionName;
        Variable *varptr1 = dynamic_cast<Variable*>(infix[i - 1]);
        Variable *varptr2 = dynamic_cast<Variable*>(infix[i]);
        if (varptr1 && varptr1 -> getName() == "function") {
            if (varptr2) {
                line = row;
                functionName = varptr2 -> getName();
                if (infix[i + 1] && infix[i + 1] -> getType() == LBRACKET) {
                    for (size_t j = i + 2; j < infix.size(); j++) {
                        if (infix[j] && infix[j] -> getType() == RBRACKET) {
                            break;
                        } else {
                            varNames.push_back(infix[j] -> getName());
                            argsNum++;
                        }
                    }
                }
                for (size_t i = 0; i < infix.size(); i++) {
                    delete infix[i];
                    infix[i] = nullptr;
                }
                Function *newFun = new Function(line, argsNum, varNames);
                functionsMap[functionName] = newFun;
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

    for (size_t i = 0; i < infix.size() - 1; i++) {
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
        size_t i = currentArrPosition + 1;
        while (i < infix.size()) {
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

    for (size_t i = 0; i < infixLines.size(); i++) {
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

    for (size_t i = 0; i < infixLines.size(); i++) {
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

int evaluatePoliz(int row, int *result, const std::vector< std::vector<Lexem *> > &wholeCode, bool *hasResult) {
    const std::vector<Lexem *> poliz = wholeCode[row];
    std::stack<Lexem *> computing;
    int tempNum = 0;
    bool jumpFlag = false;
    Lexem *newTempResult = nullptr;
    Oper *lexemOper = nullptr;

    for (auto lexemIter : poliz) {
        if (!lexemIter) {
            continue;
        }
        lexemOper = dynamic_cast<Oper*>(lexemIter);
        if (!lexemOper) {
            if (functionsMap.find(lexemIter -> getName()) == functionsMap.end()) {
                computing.push(lexemIter);
            } else {
                // call function
                Space newSpace;
                int line = functionsMap[lexemIter -> getName()] -> getLine();
                int argsNum = functionsMap[lexemIter -> getName()] -> getArgsNumber();
                const std::vector<std::string> &argsNames = functionsMap[lexemIter -> getName()] -> getArgsNames();
                // copy arguments to funciton
                if (computing.size() >= argsNum) {
                    for (auto argsIterator : argsNames) {
                        newSpace.variablesMap[argsIterator] = computing.top() -> getValue();
                        computing.pop();
                    }
                }

                SpacesStack.push(newSpace);
                int result = 0;
                bool hasResult = false;
                while (0 <= line && line < NumberOfLines) {
                    line = evaluatePoliz(line, &result, wholeCode, &hasResult);
                }
                SpacesStack.pop();
                if (hasResult) {
                    Number *newNum = new Number(result);
                    computing.push(newNum);
                }
            }
        } else if (computing.size() > 0) {
            if (lexemOper -> getType() == RETURN) {
                *result = computing.top() -> getValue();
                *hasResult = true;
                return MAX_CODE_SIZE;
            }

            if (computing.size() == 1) {
                Lexem *operand = computing.top();
                computing.pop();
                tempNum = lexemOper -> getResultOne(operand, &jumpFlag, row);
                if (jumpFlag) {
                    return tempNum;
                }
            }

            if (lexemOper -> getType() == LVALUEARRAY && computing.size() > 2) {
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

    if (newTempResult) {
        delete newTempResult;
    }
    return row + 1;
}
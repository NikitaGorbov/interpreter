#include <string>
#include <vector>
#include <map>
#include "lexem.h"
#include "lexical.h"

std::map<std::string, int> variablesMap;
std::map<std::string, int> labelsMap;
std::map<int, int> conditionJumpLines;
std::map<std::string, Array*> arraysMap;

Number *read_number(const std::string &codeline, int *pos) {
    int number = 0;

    if (codeline[*pos] >= '0' && codeline[*pos] <= '9') {
        for (int j = *pos; j <= (int)codeline.size(); j++) {
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

Oper *read_operator(const std::string &codeline, int *pos) {
    for (int op = 0; op < OP_NUM; op++) {
        std::string subcodeline = codeline.substr(*pos, OPERTEXT[op].size());
        if (!OPERTEXT[op].compare(subcodeline)) {
            *pos += subcodeline.size() - 1;
            return new Oper((OPERATOR)op);
        }
    }
    return nullptr;
}

Variable *read_variable(const std::string &codeline, int *pos) {
    Variable *tmp = nullptr;

    if ((codeline[*pos] >= 'a' && codeline[*pos] <= 'z') ||
        (codeline[*pos] >= 'A' && codeline[*pos] <= 'Z')) {
        // read the whole variable name
        for (int j = *pos; j <= (int)codeline.size(); j++) {
            if ((codeline[j] >= 'a' && codeline[j] <= 'z') ||
                (codeline[j] >= 'A' && codeline[j] <= 'Z')) {
                // DO NOTHING
            } else {
                std::string varName = codeline.substr(*pos, j - *pos);

                if (variablesMap.find(varName) == variablesMap.end()) {
                    variablesMap[varName] = 0;
                }
                Variable *newVar = new Variable(varName);
                tmp = newVar;
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

    for (int i = 0; i <= (int)codeline.size(); i++) {
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
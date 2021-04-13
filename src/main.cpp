#include <string>
#include <vector>
#include <iostream>

#include "const.h"
#include "lexem.h"
#include "lexical.h"
#include "syntax.h"
#include "semantic.h"

bool prompt(std::string *codeline) {
    
    std::getline(std::cin, *codeline);
    if (std::cin.eof()) {
        return false;
    }
    std::cout << "> ";
    return true;
}

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

int main() {
    std::string codeline;
    std::vector< std::vector<Lexem *> > infixLines, polizLines;

    std::cout << "Send EOF to execute (ctrl + D)\n> ";

    while (prompt(&codeline)) {
        infixLines.push_back(parseLexem(codeline));
    }

    std::cout << std::endl;

    for (int row = 0; row < (int)infixLines.size(); row++) {
        initLabels(infixLines[row], row);
    }

    for (const auto &infix: infixLines) {
        polizLines.push_back(buildPoliz(infix));
    }

    int row = 0;
    int result = 0;
    while (0 <= row && row < (int)polizLines.size()) {
        row = evaluatePoliz(polizLines[row], row, &result);
        std::cout << row << ": " << result << std::endl;
    }
    
    return 0;
}
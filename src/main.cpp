#include <string>
#include <vector>
#include <iostream>

#include "const.h"
#include "lexem.h"
#include "lexical.h"
#include "syntax.h"
#include "semantic.h"

extern std::map<std::string, Array*> arraysMap;

bool prompt(std::string *codeline) {
    std::getline(std::cin, *codeline);
    if (std::cin.eof()) {
        return false;
    }
    std::cout << "> ";
    return true;
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

    initIfJumps(infixLines);
    initWhileJumps(infixLines);

    for (int i = 0; i < (int)infixLines.size(); i++) {
        analizeArrayElements(infixLines[i]);
    }

    for (const auto &infix : infixLines) {
        polizLines.push_back(buildPoliz(infix));
    }

    int row = 0;
    int result = 0;
    while (0 <= row && row < (int)polizLines.size()) {
        row = evaluatePoliz(polizLines[row], row, &result);
    }
    return 0;
}
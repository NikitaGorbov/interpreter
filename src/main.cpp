#include <string>
#include <vector>
#include <stack>
#include <iostream>

#include "const.h"
#include "lexem.h"
#include "lexical.h"
#include "syntax.h"
#include "semantic.h"

std::map<std::string, Function*> functionsMap;
int StartLine = -1;
int NumberOfLines; // size of poliz
extern std::map<std::string, Array*> arraysMap;

Space GlobalSpace;
std::stack<Space> SpacesStack;

bool prompt(std::string *codeline) {
    std::getline(std::cin, *codeline);
    if (std::cin.eof()) {
        return false;
    }
    std::cout << "> ";
    return true;
}

int main() {
    SpacesStack.push(GlobalSpace);

    std::string codeline;
    std::vector< std::vector<Lexem *> > infixLines, polizLines;

    std::cout << "Send EOF to execute (ctrl + D)\n> ";

    while (prompt(&codeline)) {
        infixLines.push_back(parseLexem(codeline));
    }

    std::cout << std::endl;

    for (int row = 0; row < (int)infixLines.size(); row++) {
        initLabels(infixLines[row], row);
        findFunctions(infixLines[row], row);
        analizeArrayElements(infixLines[row]);
    }

    initIfJumps(infixLines);
    initWhileJumps(infixLines);

    for (const auto &infix : infixLines) {
        polizLines.push_back(buildPoliz(infix));
    }
    NumberOfLines = polizLines.size();

    if (StartLine == -1) {
        std::cout << "No 'START:' label found" << std::endl;
        return 1;
    }

    int row = StartLine;
    int result = 0;
    bool hasResult = false;
    while (0 <= row && row < NumberOfLines) {
        row = evaluatePoliz(row, &result, polizLines, &hasResult);
    }
    return 0;
}
Number *read_number(std::string, int);
Oper *read_operator(std::string, int);
Variable *read_variable(std::string, int);
std::vector<Lexem *> parseLexem(std::string codeline);

extern std::map<std::string, Variable*> variablesMap;
extern std::map<std::string, int> labelsMap;
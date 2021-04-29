void initLabels(std::vector<Lexem *> &, int);
void findFunctions(std::vector<Lexem *> &, int);
void initIfJumps(std::vector< std::vector<Lexem *> > &);
void initWhileJumps(const std::vector< std::vector<Lexem *> > &);
void analizeArrayElements(std::vector<Lexem *> &);
int evaluatePoliz(int, int*, const std::vector< std::vector<Lexem *> > &wholeCode, bool *hasResult);
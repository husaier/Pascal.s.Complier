//
// Created by 叶铭炜 on 2020/5/24.
//

#ifndef LR_1_LR1RUNNER_H
#define LR_1_LR1RUNNER_H

#include <queue>
#include <iomanip>
#include <stack>
#include "LR1Table.h"
#include "lexical_analyzer.h"
#include "SymbolBlock.h"

class LR1Runner {

public:
    void run(LR1Table &table);

    static void load(const vector<LexicalItem>& vector);

private:
    SymbolBlock *curBlock = NULL;

    static void outStackInt(stack<int> stack);

    static void outStackString(stack<string> stack);

    static void outStrInput(vector<string> vector, int ip);

    void switchTable(int, const string&);

    void declareID(string);

    void quoteID(string);

    void locate(string);

    void relocate();
};


#endif //LR_1_LR1RUNNER_H

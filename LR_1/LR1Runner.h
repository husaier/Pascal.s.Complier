//
// Created by 叶铭炜 on 2020/5/24.
//

#ifndef LR_1_LR1RUNNER_H
#define LR_1_LR1RUNNER_H

#include <queue>
#include <iomanip>
#include <stack>
#include "LR1TableMaker.h"
#include "lexical_analyzer.h"

class LR1Runner {

public:
    static void run(LR1Table table);

    static void load(const vector<LexicalItem>& vector);

private:


    static void outStackInt(stack<int> stack);

    static void outStackString(stack<string> stack);

    static void outStrInput(vector<string> vector, int ip);
};


#endif //LR_1_LR1RUNNER_H

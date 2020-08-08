//
// Created by husserl on 2020/8/8.
//

#ifndef LR_1_MIDCODE_H
#define LR_1_MIDCODE_H

#include <iostream>
#include <vector>
#include "SymbolBlock.h"

class QuaternionItem {
public:
    enum {
        ADD = 1, MINUS = 2, MULTIPLY = 3, DIVIDE = 4, ASSIGN = 5
    };

    int seq = 0;
    int op;
    std::string arg1, arg2, res;
};

class Quaternion {
public:
    explicit Quaternion();

    std::vector<QuaternionItem> list;

private:
    static int tempID;
};

#endif //LR_1_MIDCODE_H

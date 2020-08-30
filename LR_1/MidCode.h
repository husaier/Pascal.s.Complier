//
// Created by husserl on 2020/8/8.
//

#ifndef LR_1_MIDCODE_H
#define LR_1_MIDCODE_H

#include <iostream>
#include <utility>
#include <vector>
#include <iomanip>
#include "SymbolBlock.h"

class TempVar {
public:
    explicit TempVar(string i, bool f) {
        id = std::move(i);
        isImmediate = f;
    }

    string id;
    Type *type{nullptr};
    string value;
    bool isImmediate;
    SymbolTableLine *tableLineEntry = nullptr;//如果是用户变量,则这里存储的就是相应的符号表行的指针
    TempVar *offset = nullptr;
    TempVar *backOffset = nullptr;

    string toString() const;
};

class QuaternionItem {
public:
    enum {
        ADD = 1, MINUS = 2, MULTIPLY = 3, DIVIDE = 4, ASSIGN = 5,
        EQUAL = 6, OR = 7, FLOOR_DIVIDE = 8, MOD = 9, AND = 10,
        UNEQUAL = 11, LESS = 12, LESS_EQUAL = 13, MORE = 14,
        MORE_EQUAL = 15, NOT = 16, ASSIGNOFFSET = 22, OFFSETASSIGN = 23,
        GOTO = 27, IF = 28, PARAM = 29, CALL = 30, RETURN = 31,
    };

    QuaternionItem(int seq, int op, std::string arg1, std::string arg2, std::string res) {
        this->seq = seq;
        this->op = op;
        this->arg1 = std::move(arg1);
        this->arg2 = std::move(arg2);
        this->res = std::move(res);
        if (op <= 16)
            this->type = 1;
        else {
            this->type = op - 20;
        }
    }

    int seq = 0;
    int op;
    std::string arg1, arg2, res;
    int type;
};

class Quaternion {
public:
    std::vector<QuaternionItem> codeList;

    std::vector<TempVar *> tempVarList;

    QuaternionItem *outCode(int op, const std::string &arg1, const std::string &arg2, const std::string &res);

    void print();

    TempVar *newTemp(SymbolBlock *block = nullptr, bool flag = false);

    TempVar *getTempById(string);

    void backPatch(const vector<int> &list, int quad);

private:
    int printWidth = 20;

    int tempSeq = 0;

    static string op2string(int);

    void argPrint(string arg);
};

#endif //LR_1_MIDCODE_H

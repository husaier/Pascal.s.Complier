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

class vectorAttributeItem {
public:
    vectorAttributeItem(string InAttribute, int InType, int InWidth) {
        attribute = InAttribute;
        type = InType;
        width = InWidth;
    }

    string attribute;
    int type;
    int width;
    int num;
    int dimension;
    vector<SymbolTableLine*> IDlist;
    SymbolBlock *tableEntry = nullptr;
    ArrayInfo *arrayInfo = nullptr; //数组信息链表
};


class LR1Runner {

public:
    void run(LR1Table &table);

    static void load(const vector<LexicalItem> &vector);

private:
    SymbolBlock *symbolTable = nullptr;
    SymbolBlock *curBlock = nullptr;
    int *offset = nullptr;
    stack<SymbolBlock*> tablePointers; //栈顶指针指向的是本块
    stack<int*> offSetStack;

    static void outStackInt(stack<int> stack);

    static void outStackString(stack<string> stack);

    static void outStrInput(vector<LexicalItem> vector, int ip);

    static void outVectorAttribute(vector<vectorAttributeItem> vector);

    void switchTable(vectorAttributeItem*, int);

    void declareID(string);

    void quoteID(string);

    void locate();

    void relocate();

    void initial();
};


#endif //LR_1_LR1RUNNER_H

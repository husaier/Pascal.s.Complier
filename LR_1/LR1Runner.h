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
    vectorAttributeItem(string InAttribute, int InType, int InWidth, int Inline) {
        attribute = InAttribute;
        type222 = InType;
        width = InWidth;
        line = Inline;
    }

    ~vectorAttributeItem();

    string attribute;
    int type222;

    Type *type{nullptr};

    int specialType;
    int width;
    int num;
    int dimension;
    int line = 0;
    string value;
    vector<SymbolTableLine *> IDlist;

    vector<int> expressionTypeList;
    vector<Type*> typeList;

    SymbolTableLine *entry;
    SymbolBlock *tableEntry = nullptr;
};


class LR1Runner {

public:
    int debugInfoLevel = 0;
    // 0 表示无输出信息，1 只展示分析结果，2 展示分析的详细过程，不展示符号表的内容，3 全部展示

    void run(LR1Table &table);

    void load(const vector<LexicalItem> &vector);

    void printSemanticError();

    vector<string> semanticError;
private:
    SymbolBlock *symbolTable = nullptr;
    SymbolBlock *curBlock = nullptr;
    int *offset = nullptr;
    stack<SymbolBlock *> tablePointers; //栈顶指针指向的是本块
    stack<int *> offSetStack;

    static void outStackInt(stack<int> stack);

    static void outStackString(stack<string> stack);

    static void outStrInput(vector<LexicalItem> vector, int ip);

    static void outVectorAttribute(vector<vectorAttributeItem> vector);

    void switchTable(vectorAttributeItem *, int);

    void quoteID(int, string);

    void locate();

    void relocate();

    void initial();

    void recordSemanticError(int line, const string &); // 记录语义错误

    static bool isBasicType(int);
};


#endif //LR_1_LR1RUNNER_H

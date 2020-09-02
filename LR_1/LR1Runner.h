//
// Created by 叶铭炜 on 2020/5/24.
//

#ifndef LR_1_LR1RUNNER_H
#define LR_1_LR1RUNNER_H

#include <queue>
#include <stack>
#include "LR1Table.h"
#include "lexical_analyzer.h"
#include "MidCode.h"

class vectorAttributeItem {
public:
    vectorAttributeItem(string InAttribute, int InWidth, int Inline) {
        attribute = InAttribute;
        width = InWidth;
        line = Inline;
    }

    ~vectorAttributeItem();

    string attribute; //词法分析得到的属性
    string variableName;
    Type *type{nullptr};
    int width;
    int num;
    int dimension;
    int line = 0;
    string value;
    vector<SymbolTableLine *> IDlist;
    vector<Type *> typeList;
    SymbolTableLine *tableLineEntry = nullptr; // 符号表中的入口地址
    TempVar *entry{nullptr}; // 临时变量入口
    SymbolBlock *tableEntry{nullptr};
    int startQuad = -1;
    vector<int> nextList, trueList, falseList, testList, caseList;
    vector<string> varList;
    vector<vector<string>> varList_list;
    int quad = -1;
    vector<string> queue;
};


class LR1Runner {
public:
    explicit LR1Runner();

    int debugInfoLevel = 0;
    // 0 表示无输出信息，1 只展示分析结果，2 展示分析的详细过程，不展示符号表的内容，3 全部展示

    void run(LR1Table &table);

    void load(const vector<LexicalItem> &vector);

    void printSemanticError(); // 打印语义错误信息

    void printMidCode(); // 打印中间代码
    vector<SymbolTableLine *> generateProFunVector(SymbolBlock *);

    SymbolBlock *startBlock = nullptr;

    Quaternion midCode; // 产生的中间代码
    vector<string> semanticError; // 语义错误信息


private:
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

    static Type *getType(SymbolTableLine *);
};


#endif //LR_1_LR1RUNNER_H

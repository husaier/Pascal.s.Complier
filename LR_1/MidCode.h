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

class TempVar { //临时变量类
public:
    explicit TempVar(string i, bool f) {
        id = std::move(i);
        isImmediate = f;
    }

    string id;  //临时变量唯一标识符
    Type *type{nullptr};    //临时变量的类型
    string value;           //存储的值
    bool isImmediate;       //是否是立即数
    bool charAdd= false;
    SymbolTableLine *tableLineEntry = nullptr;  //如果是用户变量,则这里存储的就是相应的符号表行的指针
    TempVar *offset = nullptr;  //地址偏移量,用于数组和结构体
    TempVar *backOffset = nullptr;  //指向偏移量的父临时变量
    string toString();    //转为输出的格式
};

class QuaternionItem { //四元式项
public:
    enum { // 各种op对应的整数
        ADD = 1, MINUS = 2, MULTIPLY = 3, DIVIDE = 4, ASSIGN = 5,
        EQUAL = 6, OR = 7, FLOOR_DIVIDE = 8, MOD = 9, AND = 10,
        UNEQUAL = 11, LESS = 12, LESS_EQUAL = 13, MORE = 14,
        MORE_EQUAL = 15, NOT = 16, ASSIGNOFFSET = 22, OFFSETASSIGN = 23,
        GOTO = 27, IF = 28, PARAM = 29, CALL = 30, RETURN = 31,NONE = 32,
        READ = 33, WRITE = 34
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

    int seq = 0;    //序列号
    int op;         //操作
    std::string arg1, arg2, res;    //参数arg1,arg2,res
    int type;       //用于目标代码生成的类型
};

class Quaternion { //四元式类
public:
    std::vector<QuaternionItem> codeList;   //四元式代码数组

    std::vector<TempVar *> tempVarList;     //临时变量数组

    QuaternionItem *outCode(int op, const std::string &arg1, const std::string &arg2, const std::string &res);

    void print();

    TempVar *newTemp(SymbolBlock *block = nullptr, bool flag = false); //flag用来标识是不是立即数

    TempVar *getTempById(string);

    void backPatch(const vector<int> &list, int quad);

private:
    int printWidth = 20;    //输出宽度

    int tempSeq = 0;

    static string op2string(int);

    void argPrint(string arg);  //参数打印
};

#endif //LR_1_MIDCODE_H

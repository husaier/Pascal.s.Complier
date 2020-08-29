//
// Created by 刘畅 on 2020/6/27.
//
#include <iostream>
#include <string>
#include <vector>
#include "TransformPcode.h"
#include "any"

#ifndef PASCAL_S_COMPLIER1_PCODE_INTERPRETER_H
#define PASCAL_S_COMPLIER1_PCODE_INTERPRETER_H
using namespace std;

class Pcode_Interpreter {
private:
    any dataStack[1000] = {0};//数据栈
    vector<Pcode> allPcode;

public:
    vector<Pcode> readFile(string filename);//从文件里面读取p-code码
    void interpreter(vector<Pcode> pcode);//解释器
    int getBase(int nowBp, int lev);//获取基地址
//    下列代码可以从文件里面读出p-code并执行
//    Pcode_Interpreter interpreter;
//    interpreter.interpreter(interpreter.readFile("test.txt"));
};


#endif //PASCAL_S_COMPLIER1_PCODE_INTERPRETER_H

//
// Created by husserl on 2020/5/22.
//
#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include <vector>
#include <sstream>
#include "CJsonObject.hpp"
#include "Production.h"
#ifndef LR_1_FIRSTSETCALCULATOR_H
#define LR_1_FIRSTSETCALCULATOR_H
using namespace std;

class FirstSetCalculator {
public:
    void demo_first();

    //非终结符、终结符、产生式
    void initial(vector<string> &Vn, vector<string> &Vt, vector<Production> &p);

    //计算单个符号的First集
    vector<string> parse(string);
private:
    vector<string> Vn;           //非终结符
    vector<string> Vt;           //终结符
    vector<vector<string>> first;  //存放每个终结符的first集
    vector<vector<string>> First;  //存放每个非终结符的first集
    vector<string> Vn_recursive;           //带左递归的非终结符
    int N;               //产生式个数
    vector<Production> p; //产生式

    //获取字符串在向量里面的位置，不存在则返回 -1
    int get_Index(string str, vector<string> vec);

    //识别终结符和非终结符
    void recognize();

    //将产生式的左右部分别放入left，right
    void getlr(string str);

    //计算first集
    void Letter_First(string str);

    //判断是不是非终结符
    bool isVn(string s);

    //判断是不是终结符
    bool isVt(string s);
};


#endif //LR_1_FIRSTSETCALCULATOR_H

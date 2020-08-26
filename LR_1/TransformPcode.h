//
// Created by 刘畅 on 2020/8/25.
//
#include <string>
#include <vector>
#include <map>
#include "MidCode.h"
#include "SymbolBlock.h"
#ifndef LR_1_TRANSFORMPCODE_H
#define LR_1_TRANSFORMPCODE_H

using namespace std;
enum Operator {
    LIT, LOD, STO, CAL, INT, JMP, JPC, OPR
};

struct Pcode {
    Operator OP;
    int L;
    int D;
};

class TransformPcode {
private:
    vector<Pcode> allPcode;;

public:
    vector<Pcode> transformPcode(Quaternion midCode); //把codelist里面的四元式转变成pcode
    void singlePcode(Quaternion midCode,int index); //把单个四元式转变成pcode
    int exist(vector<int> list, int num); //判断数字是否在数组里面,如果在，返回序号，否则返回-1
    int getProcedureIndex(int index); //根据index找到所在的过程序号
};


#endif //LR_1_TRANSFORMPCODE_H

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
    LIT, LOD, STO, CAL, INT, JMP, JPC, OPR, LOA, LAO
};

struct Pcode {
    Operator OP;
    int L;
    int D;
};

class TransformPcode {
private:
    vector<Pcode> allPcode;//生成的pcode集合
    vector<int> pcodeIndex;//每个过程的pcode码开始地址

public:
    vector<Pcode> transformPcode(Quaternion midCode); //把codelist里面的四元式转变成pcode
    void singlePcode(Quaternion midCode,int index); //把单个四元式转变成pcode
    int existInt(vector<int> list, int num); //判断数字是否在数组里面,如果在，返回序号，否则返回-1
    int existBlock(vector<SymbolBlock *> procedure,SymbolBlock *block);//判断block是否在数组里面,如果在，返回序号，否则返回-1
    int getProcedureIndex(int index); //根据index找到所在的过程序号
    int getAddress(int blockIndex, string value); //根据变量名和符号表序号得到该变量在该符号表里面的位置
};


#endif //LR_1_TRANSFORMPCODE_H

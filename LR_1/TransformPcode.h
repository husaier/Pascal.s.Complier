//
// Created by 刘畅 on 2020/8/25.
//
#include <string>
#include <vector>
#include <map>
#include "MidCode.h"
#include <cstdio>
#include "SymbolBlock.h"
#include "MidCode.h"
#include "any"

#ifndef LR_1_TRANSFORMPCODE_H
#define LR_1_TRANSFORMPCODE_H

using namespace std;
enum Operator {
    LIT, LOD, STO, CAL, INT, JMP, JPC, OPR, LIP, LOP, SRO
};

struct Pcode {
    Operator OP;
    int L;
    any D;
};
struct backItem {
    int size;
    int addr;
};
class TransformPcode {
private:
    vector<int> pcodeIndex;//每个过程的pcode码开始地址
    vector<int> pcodeStart;//每个四元式的pcode码开始地址
    vector<backItem> pcodeExit;//记录每条需要回填所在过程结束地址的pcode码的地址和其所在的过程
    vector<backItem> pcodeWait;//记录每条需要回填的pcode码的地址和其跳转的四元式位置
    int fillRecord(vector<string> &list, Record* record);

    vector<int> startCodeIndex;//[0,15,19,19,23,27,28]  前面是每个过程的四元式开始地址，按从小到大排序,最后一个值是程序的结束地址+1

    //符号表里面定义的变量的个数（数组的大小和结构的大小）+过程的个数+临时变量的个数（遍历一遍四元式列表，得到每个过程临时变量的个数）+参数的个数
    vector<int> valueNum;//[2,3,0,5,2,1]  每个过程定义的过程数加上变量数

    vector<SymbolBlock *> procedure;//每个过程的符号表，与startCodeIndex相对应

    //每个符号表存的变量数组的数组，包括每个过程的变量（数组和结构），过程，临时变量，参数
    vector<vector<string>> valueData;

    vector<string> para;//记录传入的参数

public:
    vector<Pcode> allPcode;//生成的pcode集合

    vector<Pcode> transformPcode(Quaternion midCode); //把codelist里面的四元式转变成pcode
    void singlePcode(Quaternion midCode, int index); //把单个四元式转变成pcode
    int existInt(vector<int> list, int num); //判断数字是否在数组里面,如果在，返回序号，否则返回-1
    int existBlock(vector<SymbolBlock *> procedure, SymbolBlock *block);//判断block是否在数组里面,如果在，返回序号，否则返回-1
    int getProcedureIndex(int index); //根据index找到所在的过程序号
    int getAddress(int blockIndex, string value); //根据变量名和符号表序号得到该变量在该符号表里面的位置
    void simple(Quaternion midCode, QuaternionItem code, int cal, int procedureIndex); //生成简单赋值语句的pcode码
    void init(vector<SymbolTableLine*> proFunVector, Quaternion midCode); //初始化startCodeIndex和procedure数组
    void initialValueData();
};


#endif //LR_1_TRANSFORMPCODE_H

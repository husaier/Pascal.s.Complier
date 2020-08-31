//
// Created by 叶铭炜 on 2020/5/30.
//
#include <string>
#include <vector>
#include <map>
#include "TypeSystem.h"

#ifndef LR_1_SYMBOLTABLE_H
#define LR_1_SYMBOLTABLE_H
using namespace std;

class SymbolBlock;

class SymbolTableLine;

class TempVar;

class SymbolTableLine {
public:
    SymbolTableLine(int InId, string InName, Type *InType, int InOffset, int InDimension, int InDeclarationLine,SymbolBlock* InCurrentBlock) {
        id = InId;//序号
        name = InName;//名字
        type = InType;//类型
        offset = InOffset;//存储地址,这里不确定是否用int
        dimension = InDimension;//维数,0,1,2,3,...
        declarationLine = InDeclarationLine;//声明行
        vector<int> referenceLineVector;//引用行 创建时为空
        point = nullptr; //指向存储位置的指针
        blockPoint = nullptr;//指指向下一个符号块
        currentBlock = InCurrentBlock;
    }

    SymbolTableLine(int InId, TempVar* var, string n, SymbolBlock* InCurrentBlock){
        id = InId;//序号
        name = n;
        type = nullptr;
        offset = 0;
        dimension = 0;
        declarationLine = 0;//声明行
        point = nullptr; //指向存储位置的指针
        blockPoint = nullptr;//指指向下一个符号块
        currentBlock = InCurrentBlock;
        tempVarPoint = var;
    }

    void printLine(SymbolTableLine *InLinePoint);

    int id;//序号
    string name;//名字
    Type *type{nullptr};
    bool isConst{false};
    int offset;//存储地址,这里不确定是否用int
    int dimension;//维数,0,1,2,3,...
    int declarationLine;//声明行
    vector<int> referenceLineVector;//引用行
    SymbolTableLine *point; //自己指针
    SymbolBlock *blockPoint;//指向下一个表的指针
    SymbolBlock *currentBlock; //指向自己所在表的指针
    string value; //值
    int startQuad = -1;
    TempVar* tempVarPoint{nullptr}; // 如果是临时变量会有这一项，注意对于临时变量type是无效的
};


class SymbolBlock {
public:
    SymbolBlock *previous = NULL;//用于指向外围的符号表
    //符号表
    vector<SymbolTableLine *> symbolBlock;
    //反映射表，建立string -> index的映射,name -> id
    map<string, int> invMap;
    int rowNum = 0;
    int level = 0; //符号表的深度
    //int colNum = 8;

    SymbolTableLine *query(string name);

    bool insert(string name, Type *type, int offset, int dimension, int declarationLine);

    SymbolTableLine *insert2(string name, Type *type, int offset, int dimension, int declarationLine);

    bool insertTmpVar(TempVar*, string);

    static SymbolBlock *makeBlock(SymbolBlock *InPrevious);

    void deleteBlock(SymbolBlock *InPoint);

    void printBlock();

    SymbolTableLine *blockQuery(string name);

    SymbolTableLine* findFunc_Proc(SymbolBlock*);
};

#endif //LR_1_SYMBOLTABLE_H

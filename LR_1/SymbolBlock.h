//
// Created by 叶铭炜 on 2020/5/30.
//
#include <string>
#include <vector>
#include <map>

#ifndef LR_1_SYMBOLTABLE_H
#define LR_1_SYMBOLTABLE_H
using namespace std;

class SymbolBlock;

class SymbolTableLine {
public:
    SymbolTableLine(int InId, string InName, int InType, int InOffset, int InDimension, int InDeclarationLine
    ) {
        id = InId;//序号
        name = InName;//名字
        type = InType;//类型
        offset = InOffset;//存储地址,这里不确定是否用int
        dimension = InDimension;//维数,0,1,2,3,...
        declarationLine = InDeclarationLine;//声明行
        vector<int> referenceLineVector;//引用行 创建时为空
        point = nullptr; //指向存储位置的指针
        blockPoint = nullptr;
    }


//      类型
//    static const int  = 0;
    static const int INTEGER = 1;
    static const int REAL = 2;
    static const int BOOLEAN = 3;
    static const int CHAR = 4;
    static const int ARRAY = 5;
    static const int RECORD = 6;
    static const int FUNCTION = 7;


    int id;//序号
    string name;//名字
    int type;//类型
    int offset;//存储地址,这里不确定是否用int
    int dimension;//维数,0,1,2,3,...
    int declarationLine;//声明行
    vector<int> referenceLineVector;//引用行
    SymbolTableLine *point; //自己指针
    SymbolBlock *blockPoint;//指向下一个表的指针

    void printLine(SymbolTableLine *InLinePoint);
};


class SymbolBlock {
public:
    SymbolBlock *previous = NULL;//用于指向外围的符号表
    //符号表
    vector<SymbolTableLine*> symbolBlock;
    //反映射表，建立string -> index的映射,name -> id
    map<string, int> invMap;
    int rowNum = 0;
    //int colNum = 8;

    SymbolTableLine *query(string name);

    bool insert(string name, int type, int offset, int dimension, int declarationLine);//新增表的一行

    SymbolTableLine* insert2(string name, int type, int offset, int dimension, int declarationLine);

    static SymbolBlock *makeBlock(SymbolBlock *InPrevious);

    void deleteBlock(SymbolBlock *InPoint);

    void printBlock();

    SymbolTableLine* blockQuery(string name);
};

class BlockIndexTable {
public:
    vector<SymbolBlock*> blockIndexTable;
};

#endif //LR_1_SYMBOLTABLE_H

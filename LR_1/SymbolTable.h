//
// Created by 叶铭炜 on 2020/5/30.
//
#include <string>
#include <vector>
#include <map>

#ifndef LR_1_SYMBOLTABLE_H
#define LR_1_SYMBOLTABLE_H
using namespace std;

class SymbolTable;

class SymbolTableLine {
public:
    SymbolTableLine(int InId, string InName, int InType, int InOffset, int InDimension, int InDeclarationLine,
                    SymbolTableLine *InPoint) {
        id = InId;//序号
        name = InName;//名字
        type = InType;//类型
        offset = InOffset;//存储地址,这里不确定是否用int
        dimension = InDimension;//维数,0,1,2,3,...
        declarationLine = InDeclarationLine;//声明行
        vector<int> referenceLineVector;//引用行 创建时为空
        point = InPoint; //指向自己的指针
        tablePoint = NULL;
    }


//      类型
//    static const int  = 0;
//    static const int  = 1;
//    static const int  = 2;
//    static const int  = 3;
//    static const int  = 4;
//
    int id;//序号
    string name;//名字
    int type;//类型
    int offset;//存储地址,这里不确定是否用int
    int dimension;//维数,0,1,2,3,...
    int declarationLine;//声明行
    vector<int> referenceLineVector;//引用行
    SymbolTableLine *point; //自己指针
    SymbolTable *tablePoint;//指向下一个表的指针
};


class SymbolTable {
public:
    SymbolTable *previous;//用于指向外围的符号表
    //符号表
    vector<SymbolTableLine> symbolTable;
    //反映射表，建立string -> index的映射,name -> id
    map<string, int> invMap;
    int rowNum = 0;
//    int colNum = 8;

    SymbolTableLine *query(string name);
    SymbolTableLine *insert(string name, int type, int offset, int dimension, int declarationLine);//新增表的一行
    SymbolTable *makeTable(SymbolTable *InPrevious);
    void deleteTable(SymbolTable *InPoint);
};


#endif //LR_1_SYMBOLTABLE_H

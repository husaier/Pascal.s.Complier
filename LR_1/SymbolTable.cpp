//
// Created by 叶铭炜 on 2020/5/30.
//



//
////文件未完工，注意第30行
//
#include "SymbolTable.h"


//查找操作：按给定的名字查找，若查找成功则返回该行的指针，否则返回空指针
SymbolTableLine *SymbolTable::query(string name) {
    const map<string, int>::iterator &iter = invMap.find(name);
    if (iter != invMap.end())
        return &symbolTable[iter->second];
    else
        return NULL;
}

//插入操作：按给定的名字查表，若查找失败，则在表中建立新的一行，并返回该行的指针，若查找成功则报错，注意作用域的范围
SymbolTableLine *SymbolTable::insert(string name, int type, int offset, int dimension, int declarationLine) {

    if (SymbolTable::query(name)) {
        printf("ERROR SymbolTable.cpp line 22\nIt is already in Table\n");
        return NULL;
    } else {
        auto *tempPoint = new SymbolTableLine(rowNum, name, type, offset, dimension, declarationLine,
                                              &symbolTable[rowNum]);
        rowNum++;
        if (type == 9999999) {//如果需要新建符号表
            ////高亮未完工
            SymbolTable::makeTable(this);
        }
        return tempPoint;
    }
}

//定位操作，为子过程或函数中声明的局部名字创建符号子表
SymbolTable *SymbolTable::makeTable(SymbolTable *InPrevious) {
    SymbolTable table;
    table.previous = InPrevious;
    return &table;
}

//重定位操作，从符号表中"删除"局部于给定函数或过程的所有名字
void SymbolTable::deleteTable(SymbolTable *InPoint) {
    for (int i = 0; i < InPoint->rowNum; ++i) {
        if (InPoint->symbolTable[i].tablePoint)
            SymbolTable::deleteTable(InPoint->symbolTable[i].tablePoint);
    }
    delete InPoint;
}


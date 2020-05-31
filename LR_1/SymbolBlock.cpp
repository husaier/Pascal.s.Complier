//
// Created by 叶铭炜 on 2020/5/30.
//



//
////文件未完工，注意第30行
//
#include "SymbolBlock.h"


//查找操作：按给定的名字查找，若查找成功则返回该行的指针，否则返回空指针
SymbolTableLine *SymbolBlock::query(string name) {
    const map<string, int>::iterator &iter = invMap.find(name);
    if (iter != invMap.end())
        return &symbolBlock[iter->second];
    else {//当前块没找到，则向外查询
        SymbolTableLine *tempPoint = previous->query(name);
        if (tempPoint == nullptr) {//如果返回结果为空，则返回空指针
            return nullptr;
        } else {//否则返回找到行的指针
            return tempPoint;
        }
    }
}

//插入操作：按给定的名字查表，若查找失败，则在表中建立新的一行，并返回该行的指针，若查找成功则报错，注意作用域的范围
SymbolTableLine *SymbolBlock::insert(string name, int type, int offset, int dimension, int declarationLine) {

    if (SymbolBlock::query(name)) {
        printf("ERROR SymbolBlock.cpp line 22\nIt is already in Block\n");
        return nullptr;
    } else {
        auto *tempPoint = new SymbolTableLine(rowNum, name, type, offset, dimension, declarationLine,
                                              &symbolBlock[rowNum]);
        rowNum++;
        if (type == 9999999) {//如果需要新建符号表
            ////高亮未完工
            SymbolBlock::makeBlock(this);
        }
        return tempPoint;
    }
}

//定位操作，为子过程或函数中声明的局部名字创建符号子表
SymbolBlock *SymbolBlock::makeBlock(SymbolBlock *InPrevious) {
    SymbolBlock block;
    block.previous = InPrevious;
    return &block;
}

//重定位操作，从符号表中"删除"局部于给定函数或过程的所有名字
void SymbolBlock::deleteBlock(SymbolBlock *InPoint) {
    for (int i = 0; i < InPoint->rowNum; ++i) {
        if (InPoint->symbolBlock[i].blockPoint)
            SymbolBlock::deleteBlock(InPoint->symbolBlock[i].blockPoint);
    }
    delete InPoint;
}


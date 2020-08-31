//
// Created by 叶铭炜 on 2020/5/30.
//
//
#include "SymbolBlock.h"
#include "MidCode.h"


//查找操作：按给定的名字查找，若查找成功则返回该行的指针，否则返回空指针
SymbolTableLine *SymbolBlock::query(string name) {
    map<string, int>::iterator iter;
    iter = invMap.find(name);
    if (iter != invMap.end())
        return symbolBlock[iter->second];
    else {//当前块没找到，则向外查询
        if (previous == nullptr)
            return nullptr;
        SymbolTableLine *tempPoint = previous->query(name);
        if (tempPoint == nullptr) {//如果返回结果为空，则返回空指针
            return nullptr;
        } else {//否则返回找到行的指针
            return tempPoint;
        }
    }
}


SymbolTableLine *SymbolBlock::insert2(string name, Type *type, int offset, int dimension, int declarationLine) {
    if (blockQuery(name)) {
        printf("ERROR SymbolBlock.cpp line 22\nIt is already in Block\n");
        return nullptr;
    } else {
        auto *tempPoint = new SymbolTableLine(rowNum, name, type, offset, dimension, declarationLine,this);
        invMap.insert(map<string, int>::value_type(name, rowNum));
        rowNum++;
        symbolBlock.push_back(tempPoint);
        return tempPoint;
    }
}

//插入操作：按给定的名字查表，若查找失败，则在表中建立新的一行，并返回该行的指针，若查找成功则报错，注意作用域的范围
bool SymbolBlock::insert(string name, Type *type, int offset, int dimension, int declarationLine) {
    if (blockQuery(name)) {
        printf("ERROR SymbolBlock.cpp line 22\nIt is already in Block\n");
        return false;
    } else {
        auto *tempPoint = new SymbolTableLine(rowNum, name, type, offset, dimension, declarationLine,this);
        invMap.insert(map<string, int>::value_type(name, rowNum));
        rowNum++;
        symbolBlock.push_back(tempPoint);
        return true;
    }
}

//定位操作，为子过程或函数中声明的局部名字创建符号子表
SymbolBlock *SymbolBlock::makeBlock(SymbolBlock *InPrevious) {
    SymbolBlock *block;
    block = new SymbolBlock();
    block->previous = InPrevious;
    return block;
}

//重定位操作，从符号表中"删除"局部于给定函数或过程的所有名字
void SymbolBlock::deleteBlock(SymbolBlock *InPoint) {
    for (int i = 0; i < InPoint->rowNum; ++i) {
        if (InPoint->symbolBlock[i]->blockPoint)
            SymbolBlock::deleteBlock(InPoint->symbolBlock[i]->blockPoint);
    }
    delete InPoint;
}

void SymbolBlock::printBlock() {
    printf("InPoint:%p\n", this);
    printf("perviousPoint:%p\n", this->previous);
    printf("rowNum:%d\n", this->rowNum);
    for (int i = 0; i < this->symbolBlock.size(); ++i) {
        this->symbolBlock[i]->printLine(this->symbolBlock[i]);//逐行输出
    }
}

SymbolTableLine *SymbolBlock::blockQuery(string name) {
    const map<string, int>::iterator &iter = invMap.find(name);
    if (iter != invMap.end())
        return symbolBlock[iter->second];
    else {//当前块没找到，则向返回空指针
        return nullptr;
    }
}

bool SymbolBlock::insertTmpVar(TempVar *tempVar, string name) {
    if (blockQuery(name)) {
        printf("ERROR SymbolBlock.cpp line 22\nIt is already in Block\n");
        return false;
    } else {
        auto *tempPoint = new SymbolTableLine(rowNum, tempVar, name, this);
        invMap.insert(map<string, int>::value_type(name, rowNum));
        rowNum++;
        symbolBlock.push_back(tempPoint);
        return true;
    }
}

void SymbolTableLine::printLine(SymbolTableLine *InLinePoint) {
    printf("id:%3d\tname:%8s\ttype:%d\toffset:%d\tdimension:%d\tdeclarationLine:%d\tpoint:%p\tblockPoint:%p\treferenceLineVector:",
           InLinePoint->id, InLinePoint->name.c_str(),
           InLinePoint->type->getType(), InLinePoint->offset, InLinePoint->dimension, InLinePoint->declarationLine,
           InLinePoint->point, InLinePoint->blockPoint);
    for (int i = 0; i < InLinePoint->referenceLineVector.size(); ++i) {
        printf(" %d", InLinePoint->referenceLineVector[i]);
    }
    printf("\n");
}

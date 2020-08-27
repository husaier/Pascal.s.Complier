//
// Created by 刘畅 on 2020/8/25.
//

#include "TransformPcode.h"

//todo:如果最后一个值等于倒数第二个值，那么在codelist里面加上一条无意义的语句（最后一个值加一），用于帮助空的主过程形成开栈和退栈的pcode语句
vector<int> startCodeIndex;//[0,15,19,19,23,27,28]  前面是每个过程的四元式开始地址，按从小到大排序,最后一个值是程序的结束地址+1

//todo:符号表里面定义的变量的个数（数组的大小和结构的大小）+过程的个数+临时变量的个数（遍历一遍四元式列表，得到每个过程临时变量的个数）+参数的个数
vector<int> valueNum;//[2,3,0,5,2,1]  每个过程定义的过程数加上变量数

vector<SymbolBlock *> procedure;//每个过程的符号表，与startCodeIndex相对应

//todo:每个符号表存的变量数组的数组，包括每个过程的变量（数组和结构），过程，临时变量，参数
vector<vector<string>> valueData;

vector<Boost::any> para;//记录传入的参数

//判断数字是否在数组里面,如果在，返回序号，否则返回-1
int TransformPcode::existInt(vector<int> list, int num) {
    for (int i = 0; i < list.size(); i++) {
        if (list[i] == num)
            return i;
    }
    return -1;
}

//判断block是否在数组里面,如果在，返回序号，否则返回-1
int TransformPcode::existBlock(vector<SymbolBlock *> procedureList, SymbolBlock *block) {
    for (int i = 0; i < procedureList.size(); i++) {
        if (procedureList[i] == block)
            return i;
    }
    return -1;
}

//根据index找到所在的过程序号,返回序号值，若没找到，则报错
int TransformPcode::getProcedureIndex(int index) {
    for (int i = 0; i < startCodeIndex.size() - 1; i++) {
        if (startCodeIndex[i] <= index && startCodeIndex[i + 1] > index)
            return i;
    }
    return -1;
}

//todo:根据变量名和符号表序号得到该变量在该符号表里面的位置, blockIndex必须大于等于0
int TransformPcode::getAddress(int blockIndex, string value) {
    return 0;
}


//把codelist里面的四元式转变成pcode
vector<Pcode> TransformPcode::transformPcode(Quaternion midCode) {

    //遍历所有的四元式
    for (int i = 0; i < midCode.codeList.size(); i++) {
        singlePcode(midCode, i);
    }
    //需要加上主过程的退栈pcode语句
    Pcode pcode;
    pcode.OP = OPR;
    pcode.L = 0;
    pcode.D = 0;
    allPcode.push_back(pcode);
    allPcode[0].D = pcodeIndex[pcodeIndex.size()];//回填主过程的pcode码
    return allPcode;
}

//把单个四元式转变成pcode
void TransformPcode::singlePcode(Quaternion midCode, int index) {
    QuaternionItem code = midCode.codeList[index];//获取当前四元式
    //如果四元式地址是某个过程的开始地址，形成开辟栈空间的pcode语句
    if (existInt(startCodeIndex, index) != -1) {
        int i = existInt(startCodeIndex, index);//当前地址为第i个过程的开始地址（i从0开始）
        do {
            if (i == 0) { //如果是第一个过程的开始语句
                //首条pcode指令需要跳转到主过程的开始的pcode码处
                Pcode pcode;
                pcode.OP = JMP;
                pcode.L = 0;
                pcode.D = -1;//等所有pcode码翻译结束后回填
                allPcode.push_back(pcode);
            } else { //不是第一个过程
                //需要加上上一个过程的退栈pcode语句
                Pcode pcode;
                pcode.OP = OPR;
                pcode.L = 0;
                pcode.D = 0;
                allPcode.push_back(pcode);
            }
            pcodeIndex.push_back(allPcode.size());//记录每个过程的开始地址

            Pcode pcode;
            pcode.OP = INT;
            pcode.L = 0;
            pcode.D = valueNum[i] + 3;
            allPcode.push_back(pcode);
        } while (startCodeIndex[++i] == index);

    }
    //开始对每一条四元式进行翻译
    int l, d = 0;
    int offset = 0;
    int procedureIndex = getProcedureIndex(index);//根据index找到所在的过程序号
    if (procedureIndex != -1) {
        switch (code.type) {
            case 1: //简单赋值语句
                switch (code.op) {
                    case QuaternionItem::ADD: // +
                        // 把第一个变量放到栈顶
                        if (code.arg1[0] == '$')
                            allPcode.push_back({LIT, 0, stoi(code.arg1.substr(1))});
                        else {
                            //找到变量arg1定义的位置，计算这个变量在其表中的序号，以及2个表的层次差
                            if (midCode.tempVarList[stoi(code.arg1.substr(1))]->tableLineEntry != nullptr) {  //不是临时变量
                                SymbolBlock *defineBlock = midCode.tempVarList[stoi(
                                        code.arg1.substr(1))]->tableLineEntry->currentBlock;
                                l = procedure[procedureIndex]->level - defineBlock->level;
                                d = getAddress(existBlock(procedure, defineBlock), code.arg1);
                            } else {  //是临时变量
                                l = 0;
                                d = getAddress(procedureIndex, code.arg1);
                            }
                            allPcode.push_back({LOD, l, d});
                        }
                        // 把第二个变量放到栈顶
                        if (code.arg2[0] == '$')
                            allPcode.push_back({LIT, 0, stoi(code.arg2.substr(1))});
                        else {
                            //找到变量arg2定义的位置，计算这个变量在其表中的序号，以及2个表的层次差
                            if (midCode.tempVarList[stoi(code.arg2.substr(1))]->tableLineEntry != nullptr) {  //不是临时变量
                                SymbolBlock *defineBlock = midCode.tempVarList[stoi(
                                        code.arg2.substr(1))]->tableLineEntry->currentBlock;
                                l = procedure[procedureIndex]->level - defineBlock->level;
                                d = getAddress(existBlock(procedure, defineBlock), code.arg2);
                            } else {  //是临时变量
                                l = 0;
                                d = getAddress(procedureIndex, code.arg2);
                            }
                            allPcode.push_back({LOD, l, d});
                        }
                        // 从栈顶取值存到结果中
                        allPcode.push_back({OPR, 0, 2});
                        //找到变量res定义的位置，计算这个变量在其表中的序号，以及2个表的层次差
                        if (midCode.tempVarList[stoi(code.res.substr(1))]->tableLineEntry != nullptr) {  //不是临时变量
                            SymbolBlock *defineBlock = midCode.tempVarList[stoi(
                                    code.res.substr(1))]->tableLineEntry->currentBlock;
                            l = procedure[procedureIndex]->level - defineBlock->level;
                            d = getAddress(existBlock(procedure, defineBlock), code.res);
                        } else {  //是临时变量
                            l = 0;
                            d = getAddress(procedureIndex, code.res);
                        }
                        allPcode.push_back({STO, l, d});
                        break;
                    case QuaternionItem::MINUS:// -
                        // 把第一个变量放到栈顶
                        if (code.arg1[0] == '$')
                            allPcode.push_back({LIT, 0, stoi(code.arg1.substr(1))});
                        else {
                            //找到变量arg1定义的位置，计算这个变量在其表中的序号，以及2个表的层次差
                            if (midCode.tempVarList[stoi(code.arg1.substr(1))]->tableLineEntry != nullptr) {  //不是临时变量
                                SymbolBlock *defineBlock = midCode.tempVarList[stoi(
                                        code.arg1.substr(1))]->tableLineEntry->currentBlock;
                                l = procedure[procedureIndex]->level - defineBlock->level;
                                d = getAddress(existBlock(procedure, defineBlock), code.arg1);
                            } else {  //是临时变量
                                l = 0;
                                d = getAddress(procedureIndex, code.arg1);
                            }
                            allPcode.push_back({LOD, l, d});
                        }
                        // 把第二个变量放到栈顶
                        if (code.arg2[0] == '$')
                            allPcode.push_back({LIT, 0, stoi(code.arg2.substr(1))});
                        else {
                            //找到变量arg2定义的位置，计算这个变量在其表中的序号，以及2个表的层次差
                            if (midCode.tempVarList[stoi(code.arg2.substr(1))]->tableLineEntry != nullptr) {  //不是临时变量
                                SymbolBlock *defineBlock = midCode.tempVarList[stoi(
                                        code.arg2.substr(1))]->tableLineEntry->currentBlock;
                                l = procedure[procedureIndex]->level - defineBlock->level;
                                d = getAddress(existBlock(procedure, defineBlock), code.arg2);
                            } else {  //是临时变量
                                l = 0;
                                d = getAddress(procedureIndex, code.arg2);
                            }
                            allPcode.push_back({LOD, l, d});
                        }
                        // 从栈顶取值存到结果中
                        allPcode.push_back({OPR, 0, 3});
                        //找到变量res定义的位置，计算这个变量在其表中的序号，以及2个表的层次差
                        if (midCode.tempVarList[stoi(code.res.substr(1))]->tableLineEntry != nullptr) {  //不是临时变量
                            SymbolBlock *defineBlock = midCode.tempVarList[stoi(
                                    code.res.substr(1))]->tableLineEntry->currentBlock;
                            l = procedure[procedureIndex]->level - defineBlock->level;
                            d = getAddress(existBlock(procedure, defineBlock), code.res);
                        } else {  //是临时变量
                            l = 0;
                            d = getAddress(procedureIndex, code.res);
                        }
                        allPcode.push_back({STO, l, d});
                        break;
                    case QuaternionItem::MULTIPLY:// *
                        // 把第一个变量放到栈顶
                        if (code.arg1[0] == '$')
                            allPcode.push_back({LIT, 0, stoi(code.arg1.substr(1))});
                        else {
                            //找到变量arg1定义的位置，计算这个变量在其表中的序号，以及2个表的层次差
                            if (midCode.tempVarList[stoi(code.arg1.substr(1))]->tableLineEntry != nullptr) {  //不是临时变量
                                SymbolBlock *defineBlock = midCode.tempVarList[stoi(
                                        code.arg1.substr(1))]->tableLineEntry->currentBlock;
                                l = procedure[procedureIndex]->level - defineBlock->level;
                                d = getAddress(existBlock(procedure, defineBlock), code.arg1);
                            } else {  //是临时变量
                                l = 0;
                                d = getAddress(procedureIndex, code.arg1);
                            }
                            allPcode.push_back({LOD, l, d});
                        }
                        // 把第二个变量放到栈顶
                        if (code.arg2[0] == '$')
                            allPcode.push_back({LIT, 0, stoi(code.arg2.substr(1))});
                        else {
                            //找到变量arg2定义的位置，计算这个变量在其表中的序号，以及2个表的层次差
                            if (midCode.tempVarList[stoi(code.arg2.substr(1))]->tableLineEntry != nullptr) {  //不是临时变量
                                SymbolBlock *defineBlock = midCode.tempVarList[stoi(
                                        code.arg2.substr(1))]->tableLineEntry->currentBlock;
                                l = procedure[procedureIndex]->level - defineBlock->level;
                                d = getAddress(existBlock(procedure, defineBlock), code.arg2);
                            } else {  //是临时变量
                                l = 0;
                                d = getAddress(procedureIndex, code.arg2);
                            }
                            allPcode.push_back({LOD, l, d});
                        }
                        // 从栈顶取值存到结果中
                        allPcode.push_back({OPR, 0, 4});
                        //找到变量res定义的位置，计算这个变量在其表中的序号，以及2个表的层次差
                        if (midCode.tempVarList[stoi(code.res.substr(1))]->tableLineEntry != nullptr) {  //不是临时变量
                            SymbolBlock *defineBlock = midCode.tempVarList[stoi(
                                    code.res.substr(1))]->tableLineEntry->currentBlock;
                            l = procedure[procedureIndex]->level - defineBlock->level;
                            d = getAddress(existBlock(procedure, defineBlock), code.res);
                        } else {  //是临时变量
                            l = 0;
                            d = getAddress(procedureIndex, code.res);
                        }
                        allPcode.push_back({STO, l, d});
                        break;
                    case QuaternionItem::DIVIDE:// /
                        // 把第一个变量放到栈顶
                        if (code.arg1[0] == '$')
                            allPcode.push_back({LIT, 0, stoi(code.arg1.substr(1))});
                        else {
                            //找到变量arg1定义的位置，计算这个变量在其表中的序号，以及2个表的层次差
                            if (midCode.tempVarList[stoi(code.arg1.substr(1))]->tableLineEntry != nullptr) {  //不是临时变量
                                SymbolBlock *defineBlock = midCode.tempVarList[stoi(
                                        code.arg1.substr(1))]->tableLineEntry->currentBlock;
                                l = procedure[procedureIndex]->level - defineBlock->level;
                                d = getAddress(existBlock(procedure, defineBlock), code.arg1);
                            } else {  //是临时变量
                                l = 0;
                                d = getAddress(procedureIndex, code.arg1);
                            }
                            allPcode.push_back({LOD, l, d});
                        }
                        // 把第二个变量放到栈顶
                        if (code.arg2[0] == '$')
                            allPcode.push_back({LIT, 0, stoi(code.arg2.substr(1))});
                        else {
                            //找到变量arg2定义的位置，计算这个变量在其表中的序号，以及2个表的层次差
                            if (midCode.tempVarList[stoi(code.arg2.substr(1))]->tableLineEntry != nullptr) {  //不是临时变量
                                SymbolBlock *defineBlock = midCode.tempVarList[stoi(
                                        code.arg2.substr(1))]->tableLineEntry->currentBlock;
                                l = procedure[procedureIndex]->level - defineBlock->level;
                                d = getAddress(existBlock(procedure, defineBlock), code.arg2);
                            } else {  //是临时变量
                                l = 0;
                                d = getAddress(procedureIndex, code.arg2);
                            }
                            allPcode.push_back({LOD, l, d});
                        }
                        // 从栈顶取值存到结果中
                        allPcode.push_back({OPR, 0, 5});
                        //找到变量res定义的位置，计算这个变量在其表中的序号，以及2个表的层次差
                        if (midCode.tempVarList[stoi(code.res.substr(1))]->tableLineEntry != nullptr) {  //不是临时变量
                            SymbolBlock *defineBlock = midCode.tempVarList[stoi(
                                    code.res.substr(1))]->tableLineEntry->currentBlock;
                            l = procedure[procedureIndex]->level - defineBlock->level;
                            d = getAddress(existBlock(procedure, defineBlock), code.res);
                        } else {  //是临时变量
                            l = 0;
                            d = getAddress(procedureIndex, code.res);
                        }
                        allPcode.push_back({STO, l, d});
                        break;
                    case QuaternionItem::ASSIGN:// 赋值
                        // 把第一个变量放到栈顶
                        if (code.arg1[0] == '$')
                            allPcode.push_back({LIT, 0, stoi(code.arg1.substr(1))});
                        else {
                            //找到变量arg1定义的位置，计算这个变量在其表中的序号，以及2个表的层次差
                            if (midCode.tempVarList[stoi(code.arg1.substr(1))]->tableLineEntry != nullptr) {  //不是临时变量
                                SymbolBlock *defineBlock = midCode.tempVarList[stoi(
                                        code.arg1.substr(1))]->tableLineEntry->currentBlock;
                                l = procedure[procedureIndex]->level - defineBlock->level;
                                d = getAddress(existBlock(procedure, defineBlock), code.arg1);
                            } else {  //是临时变量
                                l = 0;
                                d = getAddress(procedureIndex, code.arg1);
                            }
                            allPcode.push_back({LOD, l, d});
                        }

                        // 从栈顶取值存到结果中
                        //找到变量res定义的位置，计算这个变量在其表中的序号，以及2个表的层次差
                        if (midCode.tempVarList[stoi(code.res.substr(1))]->tableLineEntry != nullptr) {  //不是临时变量
                            SymbolBlock *defineBlock = midCode.tempVarList[stoi(
                                    code.res.substr(1))]->tableLineEntry->currentBlock;
                            l = procedure[procedureIndex]->level - defineBlock->level;
                            d = getAddress(existBlock(procedure, defineBlock), code.res);
                        } else {  //是临时变量
                            l = 0;
                            d = getAddress(procedureIndex, code.res);
                        }
                        allPcode.push_back({STO, l, d});
                        break;
                    case QuaternionItem::EQUAL:// ==
                        // 把第一个变量放到栈顶
                        if (code.arg1[0] == '$')
                            allPcode.push_back({LIT, 0, stoi(code.arg1.substr(1))});
                        else {
                            //找到变量arg1定义的位置，计算这个变量在其表中的序号，以及2个表的层次差
                            if (midCode.tempVarList[stoi(code.arg1.substr(1))]->tableLineEntry != nullptr) {  //不是临时变量
                                SymbolBlock *defineBlock = midCode.tempVarList[stoi(
                                        code.arg1.substr(1))]->tableLineEntry->currentBlock;
                                l = procedure[procedureIndex]->level - defineBlock->level;
                                d = getAddress(existBlock(procedure, defineBlock), code.arg1);
                            } else {  //是临时变量
                                l = 0;
                                d = getAddress(procedureIndex, code.arg1);
                            }
                            allPcode.push_back({LOD, l, d});
                        }
                        // 把第二个变量放到栈顶
                        if (code.arg2[0] == '$')
                            allPcode.push_back({LIT, 0, stoi(code.arg2.substr(1))});
                        else {
                            //找到变量arg2定义的位置，计算这个变量在其表中的序号，以及2个表的层次差
                            if (midCode.tempVarList[stoi(code.arg2.substr(1))]->tableLineEntry != nullptr) {  //不是临时变量
                                SymbolBlock *defineBlock = midCode.tempVarList[stoi(
                                        code.arg2.substr(1))]->tableLineEntry->currentBlock;
                                l = procedure[procedureIndex]->level - defineBlock->level;
                                d = getAddress(existBlock(procedure, defineBlock), code.arg2);
                            } else {  //是临时变量
                                l = 0;
                                d = getAddress(procedureIndex, code.arg2);
                            }
                            allPcode.push_back({LOD, l, d});
                        }
                        // 从栈顶取值存到结果中
                        allPcode.push_back({OPR, 0, 8});
                        //找到变量res定义的位置，计算这个变量在其表中的序号，以及2个表的层次差
                        if (midCode.tempVarList[stoi(code.res.substr(1))]->tableLineEntry != nullptr) {  //不是临时变量
                            SymbolBlock *defineBlock = midCode.tempVarList[stoi(
                                    code.res.substr(1))]->tableLineEntry->currentBlock;
                            l = procedure[procedureIndex]->level - defineBlock->level;
                            d = getAddress(existBlock(procedure, defineBlock), code.res);
                        } else {  //是临时变量
                            l = 0;
                            d = getAddress(procedureIndex, code.res);
                        }
                        allPcode.push_back({STO, l, d});
                        break;
                    case QuaternionItem::OR:// or
                        // 把第一个变量放到栈顶
                        if (code.arg1[0] == '$')
                            allPcode.push_back({LIT, 0, stoi(code.arg1.substr(1))});
                        else {
                            //找到变量arg1定义的位置，计算这个变量在其表中的序号，以及2个表的层次差
                            if (midCode.tempVarList[stoi(code.arg1.substr(1))]->tableLineEntry != nullptr) {  //不是临时变量
                                SymbolBlock *defineBlock = midCode.tempVarList[stoi(
                                        code.arg1.substr(1))]->tableLineEntry->currentBlock;
                                l = procedure[procedureIndex]->level - defineBlock->level;
                                d = getAddress(existBlock(procedure, defineBlock), code.arg1);
                            } else {  //是临时变量
                                l = 0;
                                d = getAddress(procedureIndex, code.arg1);
                            }
                            allPcode.push_back({LOD, l, d});
                        }
                        // 把第二个变量放到栈顶
                        if (code.arg2[0] == '$')
                            allPcode.push_back({LIT, 0, stoi(code.arg2.substr(1))});
                        else {
                            //找到变量arg2定义的位置，计算这个变量在其表中的序号，以及2个表的层次差
                            if (midCode.tempVarList[stoi(code.arg2.substr(1))]->tableLineEntry != nullptr) {  //不是临时变量
                                SymbolBlock *defineBlock = midCode.tempVarList[stoi(
                                        code.arg2.substr(1))]->tableLineEntry->currentBlock;
                                l = procedure[procedureIndex]->level - defineBlock->level;
                                d = getAddress(existBlock(procedure, defineBlock), code.arg2);
                            } else {  //是临时变量
                                l = 0;
                                d = getAddress(procedureIndex, code.arg2);
                            }
                            allPcode.push_back({LOD, l, d});
                        }
                        // 从栈顶取值存到结果中
                        allPcode.push_back({OPR, 0, 18});
                        //找到变量res定义的位置，计算这个变量在其表中的序号，以及2个表的层次差
                        if (midCode.tempVarList[stoi(code.res.substr(1))]->tableLineEntry != nullptr) {  //不是临时变量
                            SymbolBlock *defineBlock = midCode.tempVarList[stoi(
                                    code.res.substr(1))]->tableLineEntry->currentBlock;
                            l = procedure[procedureIndex]->level - defineBlock->level;
                            d = getAddress(existBlock(procedure, defineBlock), code.res);
                        } else {  //是临时变量
                            l = 0;
                            d = getAddress(procedureIndex, code.res);
                        }
                        allPcode.push_back({STO, l, d});
                        break;
                    case QuaternionItem::FLOOR_DIVIDE:// 整除
                        // 把第一个变量放到栈顶
                        if (code.arg1[0] == '$')
                            allPcode.push_back({LIT, 0, stoi(code.arg1.substr(1))});
                        else {
                            //找到变量arg1定义的位置，计算这个变量在其表中的序号，以及2个表的层次差
                            if (midCode.tempVarList[stoi(code.arg1.substr(1))]->tableLineEntry != nullptr) {  //不是临时变量
                                SymbolBlock *defineBlock = midCode.tempVarList[stoi(
                                        code.arg1.substr(1))]->tableLineEntry->currentBlock;
                                l = procedure[procedureIndex]->level - defineBlock->level;
                                d = getAddress(existBlock(procedure, defineBlock), code.arg1);
                            } else {  //是临时变量
                                l = 0;
                                d = getAddress(procedureIndex, code.arg1);
                            }
                            allPcode.push_back({LOD, l, d});
                        }
                        // 把第二个变量放到栈顶
                        if (code.arg2[0] == '$')
                            allPcode.push_back({LIT, 0, stoi(code.arg2.substr(1))});
                        else {
                            //找到变量arg2定义的位置，计算这个变量在其表中的序号，以及2个表的层次差
                            if (midCode.tempVarList[stoi(code.arg2.substr(1))]->tableLineEntry != nullptr) {  //不是临时变量
                                SymbolBlock *defineBlock = midCode.tempVarList[stoi(
                                        code.arg2.substr(1))]->tableLineEntry->currentBlock;
                                l = procedure[procedureIndex]->level - defineBlock->level;
                                d = getAddress(existBlock(procedure, defineBlock), code.arg2);
                            } else {  //是临时变量
                                l = 0;
                                d = getAddress(procedureIndex, code.arg2);
                            }
                            allPcode.push_back({LOD, l, d});
                        }
                        // 从栈顶取值存到结果中
                        allPcode.push_back({OPR, 0, 20});
                        //找到变量res定义的位置，计算这个变量在其表中的序号，以及2个表的层次差
                        if (midCode.tempVarList[stoi(code.res.substr(1))]->tableLineEntry != nullptr) {  //不是临时变量
                            SymbolBlock *defineBlock = midCode.tempVarList[stoi(
                                    code.res.substr(1))]->tableLineEntry->currentBlock;
                            l = procedure[procedureIndex]->level - defineBlock->level;
                            d = getAddress(existBlock(procedure, defineBlock), code.res);
                        } else {  //是临时变量
                            l = 0;
                            d = getAddress(procedureIndex, code.res);
                        }
                        allPcode.push_back({STO, l, d});
                        break;
                    case QuaternionItem::MOD:// 模除
                        // 把第一个变量放到栈顶
                        if (code.arg1[0] == '$')
                            allPcode.push_back({LIT, 0, stoi(code.arg1.substr(1))});
                        else {
                            //找到变量arg1定义的位置，计算这个变量在其表中的序号，以及2个表的层次差
                            if (midCode.tempVarList[stoi(code.arg1.substr(1))]->tableLineEntry != nullptr) {  //不是临时变量
                                SymbolBlock *defineBlock = midCode.tempVarList[stoi(
                                        code.arg1.substr(1))]->tableLineEntry->currentBlock;
                                l = procedure[procedureIndex]->level - defineBlock->level;
                                d = getAddress(existBlock(procedure, defineBlock), code.arg1);
                            } else {  //是临时变量
                                l = 0;
                                d = getAddress(procedureIndex, code.arg1);
                            }
                            allPcode.push_back({LOD, l, d});
                        }
                        // 把第二个变量放到栈顶
                        if (code.arg2[0] == '$')
                            allPcode.push_back({LIT, 0, stoi(code.arg2.substr(1))});
                        else {
                            //找到变量arg2定义的位置，计算这个变量在其表中的序号，以及2个表的层次差
                            if (midCode.tempVarList[stoi(code.arg2.substr(1))]->tableLineEntry != nullptr) {  //不是临时变量
                                SymbolBlock *defineBlock = midCode.tempVarList[stoi(
                                        code.arg2.substr(1))]->tableLineEntry->currentBlock;
                                l = procedure[procedureIndex]->level - defineBlock->level;
                                d = getAddress(existBlock(procedure, defineBlock), code.arg2);
                            } else {  //是临时变量
                                l = 0;
                                d = getAddress(procedureIndex, code.arg2);
                            }
                            allPcode.push_back({LOD, l, d});
                        }
                        // 从栈顶取值存到结果中
                        allPcode.push_back({OPR, 0, 7});
                        //找到变量res定义的位置，计算这个变量在其表中的序号，以及2个表的层次差
                        if (midCode.tempVarList[stoi(code.res.substr(1))]->tableLineEntry != nullptr) {  //不是临时变量
                            SymbolBlock *defineBlock = midCode.tempVarList[stoi(
                                    code.res.substr(1))]->tableLineEntry->currentBlock;
                            l = procedure[procedureIndex]->level - defineBlock->level;
                            d = getAddress(existBlock(procedure, defineBlock), code.res);
                        } else {  //是临时变量
                            l = 0;
                            d = getAddress(procedureIndex, code.res);
                        }
                        allPcode.push_back({STO, l, d});
                        break;
                    case QuaternionItem::AND:// and
                        // 把第一个变量放到栈顶
                        if (code.arg1[0] == '$')
                            allPcode.push_back({LIT, 0, stoi(code.arg1.substr(1))});
                        else {
                            //找到变量arg1定义的位置，计算这个变量在其表中的序号，以及2个表的层次差
                            if (midCode.tempVarList[stoi(code.arg1.substr(1))]->tableLineEntry != nullptr) {  //不是临时变量
                                SymbolBlock *defineBlock = midCode.tempVarList[stoi(
                                        code.arg1.substr(1))]->tableLineEntry->currentBlock;
                                l = procedure[procedureIndex]->level - defineBlock->level;
                                d = getAddress(existBlock(procedure, defineBlock), code.arg1);
                            } else {  //是临时变量
                                l = 0;
                                d = getAddress(procedureIndex, code.arg1);
                            }
                            allPcode.push_back({LOD, l, d});
                        }
                        // 把第二个变量放到栈顶
                        if (code.arg2[0] == '$')
                            allPcode.push_back({LIT, 0, stoi(code.arg2.substr(1))});
                        else {
                            //找到变量arg2定义的位置，计算这个变量在其表中的序号，以及2个表的层次差
                            if (midCode.tempVarList[stoi(code.arg2.substr(1))]->tableLineEntry != nullptr) {  //不是临时变量
                                SymbolBlock *defineBlock = midCode.tempVarList[stoi(
                                        code.arg2.substr(1))]->tableLineEntry->currentBlock;
                                l = procedure[procedureIndex]->level - defineBlock->level;
                                d = getAddress(existBlock(procedure, defineBlock), code.arg2);
                            } else {  //是临时变量
                                l = 0;
                                d = getAddress(procedureIndex, code.arg2);
                            }
                            allPcode.push_back({LOD, l, d});
                        }
                        // 从栈顶取值存到结果中
                        allPcode.push_back({OPR, 0, 17});
                        //找到变量res定义的位置，计算这个变量在其表中的序号，以及2个表的层次差
                        if (midCode.tempVarList[stoi(code.res.substr(1))]->tableLineEntry != nullptr) {  //不是临时变量
                            SymbolBlock *defineBlock = midCode.tempVarList[stoi(
                                    code.res.substr(1))]->tableLineEntry->currentBlock;
                            l = procedure[procedureIndex]->level - defineBlock->level;
                            d = getAddress(existBlock(procedure, defineBlock), code.res);
                        } else {  //是临时变量
                            l = 0;
                            d = getAddress(procedureIndex, code.res);
                        }
                        allPcode.push_back({STO, l, d});
                        break;
                    case QuaternionItem::UNEQUAL:// !=
                        // 把第一个变量放到栈顶
                        if (code.arg1[0] == '$')
                            allPcode.push_back({LIT, 0, stoi(code.arg1.substr(1))});
                        else {
                            //找到变量arg1定义的位置，计算这个变量在其表中的序号，以及2个表的层次差
                            if (midCode.tempVarList[stoi(code.arg1.substr(1))]->tableLineEntry != nullptr) {  //不是临时变量
                                SymbolBlock *defineBlock = midCode.tempVarList[stoi(
                                        code.arg1.substr(1))]->tableLineEntry->currentBlock;
                                l = procedure[procedureIndex]->level - defineBlock->level;
                                d = getAddress(existBlock(procedure, defineBlock), code.arg1);
                            } else {  //是临时变量
                                l = 0;
                                d = getAddress(procedureIndex, code.arg1);
                            }
                            allPcode.push_back({LOD, l, d});
                        }
                        // 把第二个变量放到栈顶
                        if (code.arg2[0] == '$')
                            allPcode.push_back({LIT, 0, stoi(code.arg2.substr(1))});
                        else {
                            //找到变量arg2定义的位置，计算这个变量在其表中的序号，以及2个表的层次差
                            if (midCode.tempVarList[stoi(code.arg2.substr(1))]->tableLineEntry != nullptr) {  //不是临时变量
                                SymbolBlock *defineBlock = midCode.tempVarList[stoi(
                                        code.arg2.substr(1))]->tableLineEntry->currentBlock;
                                l = procedure[procedureIndex]->level - defineBlock->level;
                                d = getAddress(existBlock(procedure, defineBlock), code.arg2);
                            } else {  //是临时变量
                                l = 0;
                                d = getAddress(procedureIndex, code.arg2);
                            }
                            allPcode.push_back({LOD, l, d});
                        }
                        // 从栈顶取值存到结果中
                        allPcode.push_back({OPR, 0, 9});
                        //找到变量res定义的位置，计算这个变量在其表中的序号，以及2个表的层次差
                        if (midCode.tempVarList[stoi(code.res.substr(1))]->tableLineEntry != nullptr) {  //不是临时变量
                            SymbolBlock *defineBlock = midCode.tempVarList[stoi(
                                    code.res.substr(1))]->tableLineEntry->currentBlock;
                            l = procedure[procedureIndex]->level - defineBlock->level;
                            d = getAddress(existBlock(procedure, defineBlock), code.res);
                        } else {  //是临时变量
                            l = 0;
                            d = getAddress(procedureIndex, code.res);
                        }
                        allPcode.push_back({STO, l, d});
                        break;
                    case QuaternionItem::LESS:// <
                        // 把第一个变量放到栈顶
                        if (code.arg1[0] == '$')
                            allPcode.push_back({LIT, 0, stoi(code.arg1.substr(1))});
                        else {
                            //找到变量arg1定义的位置，计算这个变量在其表中的序号，以及2个表的层次差
                            if (midCode.tempVarList[stoi(code.arg1.substr(1))]->tableLineEntry != nullptr) {  //不是临时变量
                                SymbolBlock *defineBlock = midCode.tempVarList[stoi(
                                        code.arg1.substr(1))]->tableLineEntry->currentBlock;
                                l = procedure[procedureIndex]->level - defineBlock->level;
                                d = getAddress(existBlock(procedure, defineBlock), code.arg1);
                            } else {  //是临时变量
                                l = 0;
                                d = getAddress(procedureIndex, code.arg1);
                            }
                            allPcode.push_back({LOD, l, d});
                        }
                        // 把第二个变量放到栈顶
                        if (code.arg2[0] == '$')
                            allPcode.push_back({LIT, 0, stoi(code.arg2.substr(1))});
                        else {
                            //找到变量arg2定义的位置，计算这个变量在其表中的序号，以及2个表的层次差
                            if (midCode.tempVarList[stoi(code.arg2.substr(1))]->tableLineEntry != nullptr) {  //不是临时变量
                                SymbolBlock *defineBlock = midCode.tempVarList[stoi(
                                        code.arg2.substr(1))]->tableLineEntry->currentBlock;
                                l = procedure[procedureIndex]->level - defineBlock->level;
                                d = getAddress(existBlock(procedure, defineBlock), code.arg2);
                            } else {  //是临时变量
                                l = 0;
                                d = getAddress(procedureIndex, code.arg2);
                            }
                            allPcode.push_back({LOD, l, d});
                        }
                        // 从栈顶取值存到结果中
                        allPcode.push_back({OPR, 0, 10});
                        //找到变量res定义的位置，计算这个变量在其表中的序号，以及2个表的层次差
                        if (midCode.tempVarList[stoi(code.res.substr(1))]->tableLineEntry != nullptr) {  //不是临时变量
                            SymbolBlock *defineBlock = midCode.tempVarList[stoi(
                                    code.res.substr(1))]->tableLineEntry->currentBlock;
                            l = procedure[procedureIndex]->level - defineBlock->level;
                            d = getAddress(existBlock(procedure, defineBlock), code.res);
                        } else {  //是临时变量
                            l = 0;
                            d = getAddress(procedureIndex, code.res);
                        }
                        allPcode.push_back({STO, l, d});
                        break;
                    case QuaternionItem::LESS_EQUAL:// <=
                        // 把第一个变量放到栈顶
                        if (code.arg1[0] == '$')
                            allPcode.push_back({LIT, 0, stoi(code.arg1.substr(1))});
                        else {
                            //找到变量arg1定义的位置，计算这个变量在其表中的序号，以及2个表的层次差
                            if (midCode.tempVarList[stoi(code.arg1.substr(1))]->tableLineEntry != nullptr) {  //不是临时变量
                                SymbolBlock *defineBlock = midCode.tempVarList[stoi(
                                        code.arg1.substr(1))]->tableLineEntry->currentBlock;
                                l = procedure[procedureIndex]->level - defineBlock->level;
                                d = getAddress(existBlock(procedure, defineBlock), code.arg1);
                            } else {  //是临时变量
                                l = 0;
                                d = getAddress(procedureIndex, code.arg1);
                            }
                            allPcode.push_back({LOD, l, d});
                        }
                        // 把第二个变量放到栈顶
                        if (code.arg2[0] == '$')
                            allPcode.push_back({LIT, 0, stoi(code.arg2.substr(1))});
                        else {
                            //找到变量arg2定义的位置，计算这个变量在其表中的序号，以及2个表的层次差
                            if (midCode.tempVarList[stoi(code.arg2.substr(1))]->tableLineEntry != nullptr) {  //不是临时变量
                                SymbolBlock *defineBlock = midCode.tempVarList[stoi(
                                        code.arg2.substr(1))]->tableLineEntry->currentBlock;
                                l = procedure[procedureIndex]->level - defineBlock->level;
                                d = getAddress(existBlock(procedure, defineBlock), code.arg2);
                            } else {  //是临时变量
                                l = 0;
                                d = getAddress(procedureIndex, code.arg2);
                            }
                            allPcode.push_back({LOD, l, d});
                        }
                        // 从栈顶取值存到结果中
                        allPcode.push_back({OPR, 0, 13});
                        //找到变量res定义的位置，计算这个变量在其表中的序号，以及2个表的层次差
                        if (midCode.tempVarList[stoi(code.res.substr(1))]->tableLineEntry != nullptr) {  //不是临时变量
                            SymbolBlock *defineBlock = midCode.tempVarList[stoi(
                                    code.res.substr(1))]->tableLineEntry->currentBlock;
                            l = procedure[procedureIndex]->level - defineBlock->level;
                            d = getAddress(existBlock(procedure, defineBlock), code.res);
                        } else {  //是临时变量
                            l = 0;
                            d = getAddress(procedureIndex, code.res);
                        }
                        allPcode.push_back({STO, l, d});
                        break;
                    case QuaternionItem::MORE:// >
                        // 把第一个变量放到栈顶
                        if (code.arg1[0] == '$')
                            allPcode.push_back({LIT, 0, stoi(code.arg1.substr(1))});
                        else {
                            //找到变量arg1定义的位置，计算这个变量在其表中的序号，以及2个表的层次差
                            if (midCode.tempVarList[stoi(code.arg1.substr(1))]->tableLineEntry != nullptr) {  //不是临时变量
                                SymbolBlock *defineBlock = midCode.tempVarList[stoi(
                                        code.arg1.substr(1))]->tableLineEntry->currentBlock;
                                l = procedure[procedureIndex]->level - defineBlock->level;
                                d = getAddress(existBlock(procedure, defineBlock), code.arg1);
                            } else {  //是临时变量
                                l = 0;
                                d = getAddress(procedureIndex, code.arg1);
                            }
                            allPcode.push_back({LOD, l, d});
                        }
                        // 把第二个变量放到栈顶
                        if (code.arg2[0] == '$')
                            allPcode.push_back({LIT, 0, stoi(code.arg2.substr(1))});
                        else {
                            //找到变量arg2定义的位置，计算这个变量在其表中的序号，以及2个表的层次差
                            if (midCode.tempVarList[stoi(code.arg2.substr(1))]->tableLineEntry != nullptr) {  //不是临时变量
                                SymbolBlock *defineBlock = midCode.tempVarList[stoi(
                                        code.arg2.substr(1))]->tableLineEntry->currentBlock;
                                l = procedure[procedureIndex]->level - defineBlock->level;
                                d = getAddress(existBlock(procedure, defineBlock), code.arg2);
                            } else {  //是临时变量
                                l = 0;
                                d = getAddress(procedureIndex, code.arg2);
                            }
                            allPcode.push_back({LOD, l, d});
                        }
                        // 从栈顶取值存到结果中
                        allPcode.push_back({OPR, 0, 12});
                        //找到变量res定义的位置，计算这个变量在其表中的序号，以及2个表的层次差
                        if (midCode.tempVarList[stoi(code.res.substr(1))]->tableLineEntry != nullptr) {  //不是临时变量
                            SymbolBlock *defineBlock = midCode.tempVarList[stoi(
                                    code.res.substr(1))]->tableLineEntry->currentBlock;
                            l = procedure[procedureIndex]->level - defineBlock->level;
                            d = getAddress(existBlock(procedure, defineBlock), code.res);
                        } else {  //是临时变量
                            l = 0;
                            d = getAddress(procedureIndex, code.res);
                        }
                        allPcode.push_back({STO, l, d});
                        break;
                    case QuaternionItem::MORE_EQUAL:// >=
                        // 把第一个变量放到栈顶
                        if (code.arg1[0] == '$')
                            allPcode.push_back({LIT, 0, stoi(code.arg1.substr(1))});
                        else {
                            //找到变量arg1定义的位置，计算这个变量在其表中的序号，以及2个表的层次差
                            if (midCode.tempVarList[stoi(code.arg1.substr(1))]->tableLineEntry != nullptr) {  //不是临时变量
                                SymbolBlock *defineBlock = midCode.tempVarList[stoi(
                                        code.arg1.substr(1))]->tableLineEntry->currentBlock;
                                l = procedure[procedureIndex]->level - defineBlock->level;
                                d = getAddress(existBlock(procedure, defineBlock), code.arg1);
                            } else {  //是临时变量
                                l = 0;
                                d = getAddress(procedureIndex, code.arg1);
                            }
                            allPcode.push_back({LOD, l, d});
                        }
                        // 把第二个变量放到栈顶
                        if (code.arg2[0] == '$')
                            allPcode.push_back({LIT, 0, stoi(code.arg2.substr(1))});
                        else {
                            //找到变量arg2定义的位置，计算这个变量在其表中的序号，以及2个表的层次差
                            if (midCode.tempVarList[stoi(code.arg2.substr(1))]->tableLineEntry != nullptr) {  //不是临时变量
                                SymbolBlock *defineBlock = midCode.tempVarList[stoi(
                                        code.arg2.substr(1))]->tableLineEntry->currentBlock;
                                l = procedure[procedureIndex]->level - defineBlock->level;
                                d = getAddress(existBlock(procedure, defineBlock), code.arg2);
                            } else {  //是临时变量
                                l = 0;
                                d = getAddress(procedureIndex, code.arg2);
                            }
                            allPcode.push_back({LOD, l, d});
                        }
                        // 从栈顶取值存到结果中
                        allPcode.push_back({OPR, 0, 11});
                        //找到变量res定义的位置，计算这个变量在其表中的序号，以及2个表的层次差
                        if (midCode.tempVarList[stoi(code.res.substr(1))]->tableLineEntry != nullptr) {  //不是临时变量
                            SymbolBlock *defineBlock = midCode.tempVarList[stoi(
                                    code.res.substr(1))]->tableLineEntry->currentBlock;
                            l = procedure[procedureIndex]->level - defineBlock->level;
                            d = getAddress(existBlock(procedure, defineBlock), code.res);
                        } else {  //是临时变量
                            l = 0;
                            d = getAddress(procedureIndex, code.res);
                        }
                        allPcode.push_back({STO, l, d});
                        break;
                    case QuaternionItem::NOT:// not
                        // 把第一个变量放到栈顶
                        if (code.arg1[0] == '$')
                            allPcode.push_back({LIT, 0, stoi(code.arg1.substr(1))});
                        else {
                            //找到变量arg1定义的位置，计算这个变量在其表中的序号，以及2个表的层次差
                            if (midCode.tempVarList[stoi(code.arg1.substr(1))]->tableLineEntry != nullptr) {  //不是临时变量
                                SymbolBlock *defineBlock = midCode.tempVarList[stoi(
                                        code.arg1.substr(1))]->tableLineEntry->currentBlock;
                                l = procedure[procedureIndex]->level - defineBlock->level;
                                d = getAddress(existBlock(procedure, defineBlock), code.arg1);
                            } else {  //是临时变量
                                l = 0;
                                d = getAddress(procedureIndex, code.arg1);
                            }
                            allPcode.push_back({LOD, l, d});
                        }

                        // 从栈顶取值存到结果中
                        allPcode.push_back({OPR, 0, 19});
                        //找到变量res定义的位置，计算这个变量在其表中的序号，以及2个表的层次差
                        if (midCode.tempVarList[stoi(code.res.substr(1))]->tableLineEntry != nullptr) {  //不是临时变量
                            SymbolBlock *defineBlock = midCode.tempVarList[stoi(
                                    code.res.substr(1))]->tableLineEntry->currentBlock;
                            l = procedure[procedureIndex]->level - defineBlock->level;
                            d = getAddress(existBlock(procedure, defineBlock), code.res);
                        } else {  //是临时变量
                            l = 0;
                            d = getAddress(procedureIndex, code.res);
                        }
                        allPcode.push_back({STO, l, d});
                        break;

                }
                break;
            case 2: //x := y[i]
                // 把y[i]放到栈顶
                //找到y定义的位置，计算这个变量在其表中的序号，以及2个表的层次差
                if (midCode.tempVarList[stoi(code.arg1.substr(1))]->tableLineEntry != nullptr) {  //不是临时变量
                    SymbolBlock *defineBlock = midCode.tempVarList[stoi(
                            code.arg1.substr(1))]->tableLineEntry->currentBlock;
                    l = procedure[procedureIndex]->level - defineBlock->level;
                    d = getAddress(existBlock(procedure, defineBlock), code.arg1);
                } else {  //是临时变量
                    printf("error：2，出现临时数组，转换失败");
                    //l = 0;
                    //d = getAddress(procedureIndex, code.arg1);
                }

                // 计算数组偏移量
                if (code.arg2[0] == '$') {  //i是立即数
                    offset = stoi(code.arg2.substr(1));
                    d += offset;
                    allPcode.push_back({LOD, l, d});
                } else {  //i是变量，则先把变量值和y的偏移量放到栈顶并计算和，再令下一条LOD 的d参数设为-1，意为等于栈顶值然后退一个栈元素
                    allPcode.push_back({LIT, 0, d});//把y的偏移量放在栈顶
                    int l1 = l;//记录y的层次
                    //找到变量arg2定义的位置，计算这个变量在其表中的序号，以及2个表的层次差
                    if (midCode.tempVarList[stoi(code.arg2.substr(1))]->tableLineEntry != nullptr) {  //不是临时变量
                        SymbolBlock *defineBlock = midCode.tempVarList[stoi(
                                code.arg2.substr(1))]->tableLineEntry->currentBlock;
                        l = procedure[procedureIndex]->level - defineBlock->level;
                        d = getAddress(existBlock(procedure, defineBlock), code.arg2);
                    } else {  //是临时变量
                        l = 0;
                        d = getAddress(procedureIndex, code.arg2);
                    }
                    allPcode.push_back({LOD, l, d});//把i的值放到栈顶
                    allPcode.push_back({OPR, 0, 2});//把i + y的偏移量 放在栈顶
                    allPcode.push_back({LOD, l1, -1});//偏移量等于等于栈顶值，取栈顶元素后需要退一个栈元素
                }

                // 从栈顶取值存到结果中
                //找到变量res定义的位置，计算这个变量在其表中的序号，以及2个表的层次差
                if (midCode.tempVarList[stoi(code.res.substr(1))]->tableLineEntry != nullptr) {  //不是临时变量
                    SymbolBlock *defineBlock = midCode.tempVarList[stoi(
                            code.res.substr(1))]->tableLineEntry->currentBlock;
                    l = procedure[procedureIndex]->level - defineBlock->level;
                    d = getAddress(existBlock(procedure, defineBlock), code.res);
                } else {  //是临时变量
                    l = 0;
                    d = getAddress(procedureIndex, code.res);
                }
                allPcode.push_back({STO, l, d});
                break;
            case 3: //x[i] := y
                // 把y放到栈顶
                // 找到y定义的位置，计算这个变量在其表中的序号，以及2个表的层次差
                if (code.arg1[0] == '$')
                    allPcode.push_back({LIT, 0, stoi(code.arg1.substr(1))});
                else {
                    //找到变量arg1定义的位置，计算这个变量在其表中的序号，以及2个表的层次差
                    if (midCode.tempVarList[stoi(code.arg1.substr(1))]->tableLineEntry != nullptr) {  //不是临时变量
                        SymbolBlock *defineBlock = midCode.tempVarList[stoi(
                                code.arg1.substr(1))]->tableLineEntry->currentBlock;
                        l = procedure[procedureIndex]->level - defineBlock->level;
                        d = getAddress(existBlock(procedure, defineBlock), code.arg1);
                    } else {  //是临时变量
                        l = 0;
                        d = getAddress(procedureIndex, code.arg1);
                    }
                    allPcode.push_back({LOD, l, d});
                }

                // 从栈顶取值存到结果中
                // 找到数组名x定义的位置，计算这个变量在其表中的序号，以及2个表的层次差
                if (midCode.tempVarList[stoi(code.res.substr(1))]->tableLineEntry != nullptr) {  //不是临时变量
                    SymbolBlock *defineBlock = midCode.tempVarList[stoi(
                            code.res.substr(1))]->tableLineEntry->currentBlock;
                    l = procedure[procedureIndex]->level - defineBlock->level;
                    d = getAddress(existBlock(procedure, defineBlock), code.res);
                } else {  //是临时变量
                    printf("error：3，出现临时数组，转换失败");
                    //l = 0;
                    //d = getAddress(procedureIndex, code.arg1);
                }

                // 计算数组偏移量
                if (code.arg2[0] == '$') {  //i是立即数
                    offset = stoi(code.arg2.substr(1));
                    d += offset;
                    allPcode.push_back({STO, l, d});
                } else {  //i是变量，则先把变量值和x的偏移量放到栈顶并计算和，再令下一条STO 的d参数设为-1，意为等于栈顶值然后退一个栈元素
                    allPcode.push_back({LIT, 0, d});//把x的偏移量放在栈顶
                    int l1 = l;//记录x的层次
                    //找到变量i定义的位置，计算这个变量在其表中的序号，以及2个表的层次差
                    if (midCode.tempVarList[stoi(code.arg2.substr(1))]->tableLineEntry != nullptr) {  //不是临时变量
                        SymbolBlock *defineBlock = midCode.tempVarList[stoi(
                                code.arg2.substr(1))]->tableLineEntry->currentBlock;
                        l = procedure[procedureIndex]->level - defineBlock->level;
                        d = getAddress(existBlock(procedure, defineBlock), code.arg2);
                    } else {  //是临时变量
                        l = 0;
                        d = getAddress(procedureIndex, code.arg2);
                    }
                    allPcode.push_back({LOD, l, d});//把i的值放到栈顶
                    allPcode.push_back({OPR, 0, 2});//把i + y的偏移量 放在栈顶
                    allPcode.push_back({STO, l1, -1});//偏移量等于等于栈顶值，取栈顶元素后需要退一个栈元素
                }
                break;
            case 4: //x := &y
                // 把第一个变量的地址放到栈顶
                //找到变量y定义的位置，计算这个变量在其表中的序号，以及2个表的层次差
                if (midCode.tempVarList[stoi(code.arg1.substr(1))]->tableLineEntry != nullptr) {  //不是临时变量
                    SymbolBlock *defineBlock = midCode.tempVarList[stoi(
                            code.arg1.substr(1))]->tableLineEntry->currentBlock;
                    l = procedure[procedureIndex]->level - defineBlock->level;
                    d = getAddress(existBlock(procedure, defineBlock), code.arg1);
                } else {  //是临时变量
                    l = 0;
                    d = getAddress(procedureIndex, code.arg1);
                }
                allPcode.push_back({LOA, l, d});

                // 从栈顶取值存到结果中
                //找到变量x定义的位置，计算这个变量在其表中的序号，以及2个表的层次差
                if (midCode.tempVarList[stoi(code.res.substr(1))]->tableLineEntry != nullptr) {  //不是临时变量
                    SymbolBlock *defineBlock = midCode.tempVarList[stoi(
                            code.res.substr(1))]->tableLineEntry->currentBlock;
                    l = procedure[procedureIndex]->level - defineBlock->level;
                    d = getAddress(existBlock(procedure, defineBlock), code.res);
                } else {  //是临时变量
                    l = 0;
                    d = getAddress(procedureIndex, code.res);
                }
                allPcode.push_back({STO, l, d});
                break;
            case 5: //x := *y
                // 把y放到栈顶
                // 找到变量y定义的位置，计算这个变量在其表中的序号，以及2个表的层次差
                if (midCode.tempVarList[stoi(code.arg1.substr(1))]->tableLineEntry != nullptr) {  //不是临时变量
                    SymbolBlock *defineBlock = midCode.tempVarList[stoi(
                            code.arg1.substr(1))]->tableLineEntry->currentBlock;
                    l = procedure[procedureIndex]->level - defineBlock->level;
                    d = getAddress(existBlock(procedure, defineBlock), code.arg1);
                } else {  //是临时变量
                    l = 0;
                    d = getAddress(procedureIndex, code.arg1);
                }
                allPcode.push_back({LOD, l, d});//y的值（地址）放在栈顶

                allPcode.push_back({LAO, 0, 0});//取栈顶的地址，用这个地址寻址找到值存在栈顶

                // 从栈顶取值存到结果中
                //找到变量x定义的位置，计算这个变量在其表中的序号，以及2个表的层次差
                if (midCode.tempVarList[stoi(code.res.substr(1))]->tableLineEntry != nullptr) {  //不是临时变量
                    SymbolBlock *defineBlock = midCode.tempVarList[stoi(
                            code.res.substr(1))]->tableLineEntry->currentBlock;
                    l = procedure[procedureIndex]->level - defineBlock->level;
                    d = getAddress(existBlock(procedure, defineBlock), code.res);
                } else {  //是临时变量
                    l = 0;
                    d = getAddress(procedureIndex, code.res);
                }
                allPcode.push_back({STO, l, d});
                break;
            case 6: //*x := y
                // 把y放到栈顶
                // 找到变量y定义的位置，计算这个变量在其表中的序号，以及2个表的层次差
                if (midCode.tempVarList[stoi(code.arg1.substr(1))]->tableLineEntry != nullptr) {  //不是临时变量
                    SymbolBlock *defineBlock = midCode.tempVarList[stoi(
                            code.arg1.substr(1))]->tableLineEntry->currentBlock;
                    l = procedure[procedureIndex]->level - defineBlock->level;
                    d = getAddress(existBlock(procedure, defineBlock), code.arg1);
                } else {  //是临时变量
                    l = 0;
                    d = getAddress(procedureIndex, code.arg1);
                }
                allPcode.push_back({LOD, l, d});//y的值放在栈顶

                //找到变量x定义的位置，计算这个变量在其表中的序号，以及2个表的层次差
                if (midCode.tempVarList[stoi(code.res.substr(1))]->tableLineEntry != nullptr) {  //不是临时变量
                    SymbolBlock *defineBlock = midCode.tempVarList[stoi(
                            code.res.substr(1))]->tableLineEntry->currentBlock;
                    l = procedure[procedureIndex]->level - defineBlock->level;
                    d = getAddress(existBlock(procedure, defineBlock), code.res);
                } else {  //是临时变量
                    l = 0;
                    d = getAddress(procedureIndex, code.res);
                }
                allPcode.push_back({LOD, l, d});//x的值(地址）放在栈顶
                allPcode.push_back({OPR, 0, 21});//把栈顶值作为地址，在此地址处的单元存入次栈顶的值，退2个栈元素
                break;
            case 7: // goto L

                break;
            case 8: // if x relop y goto L

                break;
            case 9: // param x
                para.push_back(code.arg1);
                break;
            case 10: // call p,n
                break;
            default:
                break;
        }
    } else
        printf("error:搜索过程序号时出现错误");


}




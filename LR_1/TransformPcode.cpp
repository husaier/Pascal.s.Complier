//
// Created by 刘畅 on 2020/8/25.
//

#include "TransformPcode.h"
#include "any"


vector<int> startCodeIndex;//[0,15,19,19,23,27,28]  前面是每个过程的四元式开始地址，按从小到大排序,最后一个值是程序的结束地址+1

//todo:符号表里面定义的变量的个数（数组的大小和结构的大小）+过程的个数+临时变量的个数（遍历一遍四元式列表，得到每个过程临时变量的个数）+参数的个数
vector<int> valueNum;//[2,3,0,5,2,1]  每个过程定义的过程数加上变量数

vector<SymbolBlock *> procedure;//每个过程的符号表，与startCodeIndex相对应

//todo:每个符号表存的变量数组的数组，包括每个过程的变量（数组和结构），过程，临时变量，参数
vector<vector<string>> valueData;

int TransformPcode::fillRecord(vector<string> &list, Record *record) {
    int num = 0;
    for (const auto &item : record->env) {
        auto name = item.id;
        auto type = item.type;
        if (type->getType() == Type::ARRAY) {
            auto array = (Array *) (type);
            int size = array->getSize();
            for (int i = 0; i < size; i++) {
                list.push_back(name);
                num++;
            }
        } else if (type->getType() == Type::RECORD) {
            auto r = (Record *) (type);
            list.push_back(name);
            num++;
            num += fillRecord(list, r);
        } else {
            list.push_back(name);
            num++;
        }
    }
    return num;
}

void TransformPcode::initialValueData() {
    for (auto block : procedure) {
        vector<string> t_varList;
        vector<string> t_tmpList;
        auto parent = block->previous;
        auto self = parent->findFunc_Proc(block);
        t_varList.push_back(self->name);
        int t_num = 0;
        for (auto item: block->symbolBlock) {
            string name = item->name;
            if (item->type == nullptr) { // 临时变量
                if (item->tempVarPoint != nullptr) {
                    auto tmpVar = item->tempVarPoint;
                    t_tmpList.push_back(tmpVar->id);
                    t_num++;
                }
                continue;
            }
            auto type = item->type;
            if (type->getType() == Type::FUNC) {
                continue;
            } else if (type->getType() == Type::PROC) {
                continue;
            } else if (type->getType() == Type::ARRAY) {
                auto array = (Array *) (type);
                int size = array->getSize();
                for (int i = 0; i < size; i++) {
                    t_varList.push_back(name);
                    t_num++;
                }
            } else if (type->getType() == Type::RECORD) {
                auto record = (Record *) (type);
                t_varList.push_back(name);
                t_num++;
                t_num += fillRecord(t_varList, record);
            } else {
                t_varList.push_back(name);
                t_num++;
            }
        }
        t_varList.insert(t_varList.end(), t_tmpList.begin(), t_tmpList.end());
        valueData.push_back(t_varList);
        valueNum.push_back(t_num);
    }
}

vector<string> para;//记录传入的参数

//由s中的字符串获取其中的值
any getValue(string s) {
    any value;
    if (s.at(0) == 'c') { //有标志位c，是字符
        value = s.at(1);
    } else {
        if (s.find(string(".")) != string::npos) {//有小数点，是double
            double d;
            d = atof(s.c_str());
            value = d;
        } else { //是整数
            int i;
            i = atoi(s.c_str());
            value = i;
        }
    }
    return value;
}

//初始化startCodeIndex和procedure数组
void TransformPcode::init(vector<SymbolTableLine *> proFunVector, Quaternion midCode) {
    midCodec = midCode;
    for (int i = 0; i < proFunVector.size(); i++) {
        procedure.push_back(proFunVector[i]->blockPoint);
        startCodeIndex.push_back(proFunVector[i]->startQuad);
    }
    //如果最后一个过程的开始地址处已经没有四元式，那么在codelist里面加上一条无意义的语句（最后一个值加一），用于帮助空的主过程形成开栈和退栈的pcode语句
    if (midCode.codeList.size() <= startCodeIndex[startCodeIndex.size() - 1]) {
        midCode.codeList.emplace_back(midCode.codeList.size(), QuaternionItem::NONE, "$0", "$0", "$0");
    }
    startCodeIndex.push_back(midCode.codeList.size());
}

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

//根据变量名和符号表序号得到该变量在该符号表里面的位置, blockIndex必须大于等于0
int TransformPcode::getAddress(int blockIndex, string value) {
    if (blockIndex < 0)
        return -1;
    int j = 0;
    while (j < valueData[blockIndex].size() && valueData[blockIndex][j].at(0) != '#') {
        if (midCodec.tempVarList[stoi(value.substr(1))]->tableLineEntry != nullptr &&
            midCodec.tempVarList[stoi(value.substr(1))]->tableLineEntry->name == valueData[blockIndex][j])
            return j;
        j++;
    }

    for (int i = 0; i < valueData[blockIndex].size(); i++) {
        if (valueData[blockIndex][i] == value)
            return i;
    }
    return -1;
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
    allPcode[0].D = pcodeIndex[pcodeIndex.size() - 1];//回填主过程的pcode码
    //回填pcodeExit的跳转语句
    for (int i = 0; i < pcodeExit.size(); i++) {
        if (pcodeExit[i].addr < pcodeIndex.size() - 1)
            allPcode[pcodeExit[i].size].D = pcodeIndex[pcodeExit[i].addr + 1] - 2;//回填所在过程的OPR 0 0的地址
        else
            allPcode[pcodeExit[i].size].D = allPcode.size() - 1;
    }
    //回填pcodeWait的跳转语句
    for (int i = 0; i < pcodeWait.size(); i++) {
        allPcode[pcodeWait[i].size].D = pcodeStart[pcodeWait[i].addr];//回填跳转的四元式的起始pcode码地址
    }

    return allPcode;
}

//生成简单赋值语句的pcode码
void TransformPcode::simple(Quaternion midCode, QuaternionItem code, int cal, int procedureIndex) {
    int l, d = 0;
    // 把第一个变量放到栈顶
    if (code.arg1[0] == '$')
        allPcode.push_back({LIT, 0, getValue(code.arg1.substr(1))});
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
        allPcode.push_back({LIT, 0, getValue(code.arg2.substr(1))});
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
    allPcode.push_back({OPR, 0, cal});
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
    SymbolBlock *callBlock;
    pcodeStart.push_back(allPcode.size());//记录每一条四元式的起始pcode地址
    if (procedureIndex != -1) {
        switch (code.type) {
            case 1: //简单赋值语句
                switch (code.op) {
                    case QuaternionItem::ADD: // +
                        simple(midCode, code, 2, procedureIndex);
                        break;
                    case QuaternionItem::MINUS:// -
                        simple(midCode, code, 3, procedureIndex);
                        break;
                    case QuaternionItem::MULTIPLY:// *
                        simple(midCode, code, 4, procedureIndex);
                        break;
                    case QuaternionItem::DIVIDE:// /
                        simple(midCode, code, 5, procedureIndex);
                        break;
                    case QuaternionItem::ASSIGN:// 赋值
                        // 把第一个变量放到栈顶
                        if (code.arg1[0] == '$')
                            allPcode.push_back({LIT, 0, getValue(code.arg1.substr(1))});
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
                        simple(midCode, code, 8, procedureIndex);
                        break;
                    case QuaternionItem::OR:// or
                        simple(midCode, code, 18, procedureIndex);
                        break;
                    case QuaternionItem::FLOOR_DIVIDE:// 整除
                        simple(midCode, code, 20, procedureIndex);
                        break;
                    case QuaternionItem::MOD:// 模除
                        simple(midCode, code, 7, procedureIndex);
                        break;
                    case QuaternionItem::AND:// and
                        simple(midCode, code, 17, procedureIndex);
                        break;
                    case QuaternionItem::UNEQUAL:// !=
                        simple(midCode, code, 9, procedureIndex);
                        break;
                    case QuaternionItem::LESS:// <
                        simple(midCode, code, 10, procedureIndex);
                        break;
                    case QuaternionItem::LESS_EQUAL:// <=
                        simple(midCode, code, 13, procedureIndex);
                        break;
                    case QuaternionItem::MORE:// >
                        simple(midCode, code, 12, procedureIndex);
                        break;
                    case QuaternionItem::MORE_EQUAL:// >=
                        simple(midCode, code, 11, procedureIndex);
                        break;
                    case QuaternionItem::NOT:// not
                        // 把第一个变量放到栈顶
                        if (code.arg1[0] == '$')
                            allPcode.push_back({LIT, 0, getValue(code.arg1.substr(1))});
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
                    allPcode.push_back({LIT, 0, getValue(code.arg1.substr(1))});
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
            case 7: // goto L
                if (code.arg1[0] == '$')
                    if (stoi(code.arg1.substr(1)) < index) {  //如果跳转地址小于当前地址，直接读出跳转的pcode地址
                        allPcode.push_back({JMP, 0, pcodeStart[stoi(code.arg1.substr(1))]});
                    } else if (stoi(code.arg1.substr(1)) == index) {
                        printf("goto语句跳入死循环");
                    } else {
                        if (index > startCodeIndex[existInt(startCodeIndex, index) + 1] - 1) { //如果跳转地址超过了其所在过程的四元式结束地址
                            //让他跳到当前过程pcode码的结束地址处，等回填，记录每条需要回填的pcode码的地址和其所在的过程，存入pcodeExit
                            pcodeExit.push_back(
                                    backItem{static_cast<int>(allPcode.size()), existInt(startCodeIndex, index)});
                            allPcode.push_back({JMP, 0, -1});
                        } else {
                            //否则等待回填,记录每条需要回填的pcode码的地址和其跳转的四元式位置，存入pcodeWait
                            pcodeWait.push_back(
                                    backItem{static_cast<int>(allPcode.size()), stoi(code.arg1.substr(1))});
                            allPcode.push_back({JMP, 0, -1});
                        }
                    }
                else
                    printf("goto语句跳转地址出错。\n");
                break;
            case 8: // if x goto L
                // 把x放到栈顶
                if (code.arg1[0] == '$')
                    allPcode.push_back({LIT, 0, getValue(code.arg1.substr(1))});
                else {
                    //找到变量x定义的位置，计算这个变量在其表中的序号，以及2个表的层次差
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
                if (code.arg2[0] == '$')
                    if (stoi(code.arg2.substr(1)) < index) {  //如果跳转地址小于当前地址，直接读出跳转的pcode地址
                        allPcode.push_back({JPC, 0, pcodeStart[stoi(code.arg2.substr(1))]});
                    } else if (stoi(code.arg2.substr(1)) == index) {
                        printf("goto语句跳入死循环");
                    } else {
                        if (index > startCodeIndex[existInt(startCodeIndex, index) + 1] - 1) { //如果跳转地址超过了其所在过程的四元式结束地址
                            //让他跳到当前过程pcode码的结束地址处，等回填，记录每条需要回填的pcode码的地址和其所在的过程，存入pcodeExit
                            pcodeExit.push_back(
                                    backItem{static_cast<int>(allPcode.size()), existInt(startCodeIndex, index)});
                            allPcode.push_back({JPC, 0, -1});
                        } else {
                            //否则等待回填,记录每条需要回填的pcode码的地址和其跳转的四元式位置，存入pcodeWait
                            pcodeWait.push_back(
                                    backItem{static_cast<int>(allPcode.size()), stoi(code.arg2.substr(1))});
                            allPcode.push_back({JPC, 0, -1});
                        }
                    }
                else
                    printf("goto语句跳转地址出错。\n");
                break;
            case 9: // param x
                para.emplace_back(code.arg1);
                break;
            case 10: // call p,n
                callBlock = procedure[procedureIndex]->query(
                        midCode.tempVarList[stoi(code.arg1.substr(1))]->value)->blockPoint;
                allPcode.push_back({CAL, procedure[procedureIndex]->level - callBlock->level,
                                    pcodeIndex[existBlock(procedure, callBlock)]});
                for (int i = 0; i < para.size(); i++) {
                    // 把参数放到栈顶
                    if (para[i][0] == '$')
                        allPcode.push_back({LIP, 0, getValue(para[i].substr(1))});
                    else {
                        //找到变量arg1定义的位置，计算这个变量在其表中的序号，以及2个表的层次差
                        if (midCode.tempVarList[stoi(para[i].substr(1))]->tableLineEntry != nullptr) {  //不是临时变量
                            SymbolBlock *defineBlock = midCode.tempVarList[stoi(
                                    para[i].substr(1))]->tableLineEntry->currentBlock;
                            l = procedure[procedureIndex]->level - defineBlock->level;
                            d = getAddress(existBlock(procedure, defineBlock), para[i]);
                        } else {  //是临时变量
                            l = 0;
                            d = getAddress(procedureIndex, para[i]);
                        }
                        allPcode.push_back({LOP, l, d});
                    }
                }
                if (midCode.codeList[index + 1].type == 11) {
                    code = midCode.codeList[index + 1];
                    //找到返回值定义的位置，计算这个变量在其表中的序号，以及2个表的层次差
                    if (midCode.tempVarList[stoi(code.arg1.substr(1))]->tableLineEntry != nullptr) {  //不是临时变量
                        SymbolBlock *defineBlock = midCode.tempVarList[stoi(
                                code.arg1.substr(1))]->tableLineEntry->currentBlock;
                        l = procedure[procedureIndex]->level - defineBlock->level;
                        d = getAddress(existBlock(procedure, defineBlock), code.arg1);
                    } else {  //是临时变量
                        l = 0;
                        d = getAddress(procedureIndex, code.arg1);
                    }
                    allPcode.push_back({SRO, l, d});
                }
                break;
            case 13://read
                if(midCode.tempVarList[stoi(code.arg1.substr(1))]->type->getType() == Type::INTEGER)
                    allPcode.push_back({OPR, 0, 161});
                if(midCode.tempVarList[stoi(code.arg1.substr(1))]->type->getType() == Type::CHAR)
                    allPcode.push_back({OPR, 0, 162});
                if(midCode.tempVarList[stoi(code.arg1.substr(1))]->type->getType() == Type::REAL)
                    allPcode.push_back({OPR, 0, 163});
                if(midCode.tempVarList[stoi(code.arg1.substr(1))]->type->getType() == Type::BOOLEAN)
                    allPcode.push_back({OPR, 0, 164});
                //找到变量res定义的位置，计算这个变量在其表中的序号，以及2个表的层次差
                if (midCode.tempVarList[stoi(code.arg1.substr(1))]->tableLineEntry != nullptr) {  //不是临时变量
                    SymbolBlock *defineBlock = midCode.tempVarList[stoi(
                            code.arg1.substr(1))]->tableLineEntry->currentBlock;
                    l = procedure[procedureIndex]->level - defineBlock->level;
                    d = getAddress(existBlock(procedure, defineBlock), code.arg1);
                } else {  //是临时变量
                    l = 0;
                    d = getAddress(procedureIndex, code.arg1);
                }
                allPcode.push_back({STO, l, d});
                break;
            case 14://write
                if (code.arg1 == "\n")
                    allPcode.push_back({OPR, 0, 15});
                else{
                    // 把第变量放到栈顶
                    if (code.arg1[0] == '$')
                        allPcode.push_back({LIT, 0, getValue(code.arg1.substr(1))});
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
                    allPcode.push_back({OPR, 0, 14});
                }
                break;
            default:
                break;
        }
    } else
        printf("error:搜索过程序号时出现错误");


}

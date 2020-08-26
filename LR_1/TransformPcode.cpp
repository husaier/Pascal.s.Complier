//
// Created by 刘畅 on 2020/8/25.
//

#include "TransformPcode.h"

vector<int> startCodeIndex;//[0,15,19,19,23,27,28]  前面是每个过程的四元式开始地址，按从小到大排序,最后一个值是程序的结束地址+1
//todo:如果最后一个值等于倒数第二个值，那么在codelist里面加上一条无意义的语句（最后一个值加一），用于帮助空的主过程形成开栈和退栈的pcode语句
vector<int> valueNum;//[2,3,0,5,2,1]  每个过程定义的过程数加上变量数
//todo:符号表里面定义的变量的个数（数组的大小和结构的大小）+结构的个数+临时变量的个数（遍历一遍四元式列表，得到每个过程临时变量的个数）
vector<SymbolBlock *> procedure;//每个过程的符号表，与startCodeIndex相对应
vector<int> pcodeIndex;//[0,20,34,36,48,65]  每个过程的pcode码开始地址

//判断数字是否在数组里面,如果在，返回序号，否则返回-1
int TransformPcode::exist(vector<int> list, int num) {
    for (int i = 0; i < list.size(); i++) {
        if (list[i] == num)
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
    if (exist(startCodeIndex, index) != -1) {
        int i = exist(startCodeIndex, index);//当前地址为第i个过程的开始地址（i从0开始）
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
    int procedureIndex = getProcedureIndex(index);//根据index找到所在的过程序号
    if (procedureIndex != -1) {
        switch (code.type) {
            case 1:
                switch (code.op) {
                    case QuaternionItem::ADD: // +
                        // 把第一个变量放到栈顶
                        if (code.arg1[0] == '$')
                            allPcode.push_back({LIT, 0, stoi(code.arg1.substr(1))});
                        else {
                            //todo:调用对应的block的query函数找到变量arg1定义的位置
                            midCode.tempVarList[index]->tableLineEntry->currentBlock
                            allPcode.push_back({LOD, l, d});
                        }
                        // 把第二个变量放到栈顶
                        if (code.arg2[0] == '$')
                            allPcode.push_back({LIT, 0, stoi(code.arg1.substr(1))});
                        else {
                            //todo:调用对应的block的query函数找到变量arg2定义的位置，计算这个变量在其表中的序号，以及2个表的层次差
                            allPcode.push_back({LOD, l, d});
                        }
                        // 从栈顶取值存到结果中
                        allPcode.push_back({OPR, 0, 2});
                        //todo:调用对应的block的query函数找到变量res定义的位置，计算这个变量在其表中的序号，以及2个表的层次差
                        allPcode.push_back({STO, l, d});
                        break;
                    case QuaternionItem::MINUS:// -
                        break;
                    case QuaternionItem::MULTIPLY:// *
                        break;
                    case QuaternionItem::DIVIDE:// /
                        break;
                    case QuaternionItem::ASSIGN:// 赋值
                        break;
                    case QuaternionItem::EQUAL:// ==
                        break;
                    case QuaternionItem::OR:// or
                        break;
                    case QuaternionItem::FLOOR_DIVIDE:// 整除
                        break;
                    case QuaternionItem::MOD:// 模除
                        break;
                    case QuaternionItem::AND:// and
                        break;
                    case QuaternionItem::UNEQUAL:// !=
                        break;
                    case QuaternionItem::LESS:// <
                        break;
                    case QuaternionItem::LESS_EQUAL:// <=
                        break;
                    case QuaternionItem::MORE:// >
                        break;
                    case QuaternionItem::MORE_EQUAL:// >=
                        break;
                    case QuaternionItem::NOT:// not
                        break;

                }
                break;
            case 2:
                break;
            case 3:
                break;
            case 4:
                break;
            case 5:
                break;
            case 6:
                break;
            case 7:
                break;
            case 8:
                break;
            case 9:
                break;
            case 10:
                break;
            default:
                break;
        }
    } else
        printf("error:搜索过程序号时出现错误");


}





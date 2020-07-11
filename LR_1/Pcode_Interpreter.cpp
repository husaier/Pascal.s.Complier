//
// Created by 刘畅 on 2020/6/27.
//

#include <fstream>
#include <sstream>
#include "Pcode_Interpreter.h"

//获取基地址
int Pcode_Interpreter::getBase(int nowBp, int lev) {
    int oldBp = nowBp;
    while (lev > 0) {
        oldBp = dataStack[oldBp + 1];
        lev--;
    }
    return oldBp;
}

//从文件里面读取p-code码
vector<Pcode> Pcode_Interpreter::readFile(string filename) {
    vector<Pcode> allPcode;
    Pcode_Interpreter interpreter;
    string line_info, input_result, read;
    Pcode a;

    ifstream file(filename);

    if (!file.is_open())
        cout << "can not open this file" << endl;
    else {
        while (getline(file, line_info)) // line中不包括每行的换行符
        {
            stringstream input(line_info);
            //依次输出到input_result中，并存入vectorString中
            input >> read;

            if (read == "LIT")
                a.OP = LIT;
            else if (read == "LOD")
                a.OP = LOD;
            else if (read == "STO")
                a.OP = STO;
            else if (read == "CAL")
                a.OP = CAL;
            else if (read == "INT")
                a.OP = INT;
            else if (read == "JMP")
                a.OP = JMP;
            else if (read == "JPC")
                a.OP = JPC;
            else if (read == "OPR")
                a.OP = OPR;
            else {
                printf("出错\n");
                return allPcode;
            }
            input >> read;
            a.L = stoi(read);
            input >> read;
            a.D = stoi(read);
            allPcode.push_back(a);
        }
        return allPcode;
    }
    return allPcode;
}


//解释器
void Pcode_Interpreter::interpreter(vector<Pcode> pcode) {
    int pc = 0; //程序计数器，指向下一条指令
    int base = 0; //当前基地址
    int top = 0; //程序运行栈栈顶
    int size = 0; //pcode码集合的大小
    allPcode.assign(pcode.begin(), pcode.end());//为allPcode赋值
    size = allPcode.size();
    do {
        Pcode currentPcode = allPcode[pc];
        pc++;
        switch (currentPcode.OP) {
            case OPR:
                //OPR：关系或算术运算，A段指明具体运算
                switch (currentPcode.D) {
                    case 0:
                        //OPR 0 0   过程调用结束后,返回调用点并退栈
                        top = base;
                        pc = dataStack[base + 2];
                        base = dataStack[base];
                        break;
                    case 1:
                        //OPR 0 1     栈顶元素取反
                        dataStack[top - 1] = -dataStack[top - 1];
                        break;
                    case 2:
                        //OPR 0 2   次栈顶与栈顶相加，退两个栈元素，结果值进栈
                        dataStack[top - 2] = dataStack[top - 1] + dataStack[top - 2];
                        top--;
                        break;
                    case 3:
                        //OPR 0 3   次栈顶减去栈顶，退两个栈元素，结果值进栈
                        dataStack[top - 2] = dataStack[top - 2] - dataStack[top - 1];
                        top--;
                        break;
                    case 4:
                        //OPR 0 4     次栈顶乘以栈顶，退两个栈元素，结果值进栈
                        dataStack[top - 2] = dataStack[top - 1] * dataStack[top - 2];
                        top--;
                        break;
                    case 5:
                        //OPR 0 5     次栈顶除以栈顶，退两个栈元素，结果值进栈
                        dataStack[top - 2] = dataStack[top - 2] / dataStack[top - 1];
                        top--;
                        break;
                    case 6:
                        //OPR 0 6   栈顶元素的奇偶判断，结果值在栈顶
                        dataStack[top - 1] = dataStack[top - 1] % 2;
                        break;
                    case 8:
                        //OPR 0 8   次栈顶与栈顶是否相等，退两个栈元素，结果值进栈
                        if (dataStack[top - 2] == dataStack[top - 1]) {
                            dataStack[top - 2] = 1;
                        } else {
                            dataStack[top - 2] = 0;
                        }
                        top--;
                        break;
                    case 9:
                        //OPR 0 9   次栈顶与栈顶是否不等，退两个栈元素，结果值进栈
                        if (dataStack[top - 2] != dataStack[top - 1]) {
                            dataStack[top - 2] = 1;
                        } else {
                            dataStack[top - 2] = 0;
                        }
                        top--;
                        break;
                    case 10:
                        //OPR 0 10  次栈顶是否小于栈顶，退两个栈元素，结果值进栈
                        if (dataStack[top - 2] < dataStack[top - 1]) {
                            dataStack[top - 2] = 1;
                        } else {
                            dataStack[top - 2] = 0;
                        }
                        top--;
                        break;
                    case 11:
                        //OPR 0 11    次栈顶是否大于等于栈顶，退两个栈元素，结果值进栈
                        if (dataStack[top - 2] >= dataStack[top - 1]) {
                            dataStack[top - 2] = 1;
                        } else {
                            dataStack[top - 2] = 0;
                        }
                        top--;
                        break;
                    case 12:
                        //OPR 0 12  次栈顶是否大于栈顶，退两个栈元素，结果值进栈
                        if (dataStack[top - 2] > dataStack[top - 1]) {
                            dataStack[top - 2] = 1;
                        } else {
                            dataStack[top - 2] = 0;
                        }
                        top--;
                        break;
                    case 13:
                        //OPR 0 13  次栈顶是否小于等于栈顶，退两个栈元素，结果值进栈
                        if (dataStack[top - 2] <= dataStack[top - 1]) {
                            dataStack[top - 2] = 1;
                        } else {
                            dataStack[top - 2] = 0;
                        }
                        top--;
                        break;
                    case 14:
                        //OPR 0 14  栈顶值输出至屏幕
                        cout << dataStack[top - 1] << " ";
                        break;
                    case 15:
                        //OPR 0 15  屏幕输出换行
                        cout << endl;
                        break;
                    case 16:
                        //OPR 0 16  从命令行读入一个输入置于栈顶
                        cout << "please input a number" << endl;
                        cin >> dataStack[top];
                        top++;
                        break;
                    default:
                        cout << "OPR指令出现错误" << currentPcode.D << " " << pc;
                        pc = 0;
                        break;
                }
                break;
            case LIT:
                //LIT：将常量送到运行栈S的栈顶，这时A段为常量值
                dataStack[top] = currentPcode.D;
                top++;
                break;
            case LOD:
                // LOD：将变量送到运行栈S的栈顶，这时A段为变量所在说明层中的相对位置。
                dataStack[top] = dataStack[currentPcode.D + getBase(base, currentPcode.L)];
                top++;
                break;
            case STO:
                //STO：将运行栈S的栈顶内容送入某个变量单元中，A段为变量所在说明层中的相对位置。
                dataStack[currentPcode.D + getBase(base, currentPcode.L)] = dataStack[top - 1];
                top--;
                break;
            case CAL:
                //CAL：调用过程，这时A段为被调用过程的过程体（过程体之前一条指令）在目标程序区的入口地址。
                //跳转时，将该层基地址，跳转层基地址，pc指针保存在栈中
                //基地址base变为此时栈顶top，pc指向要跳转的地方
                //不修改top，因为前面代码已经将address+3，生成Pcode后会产生INT语句，修改top值
                dataStack[top] = base;
                dataStack[top + 1] = getBase(base, currentPcode.L);
                dataStack[top + 2] = pc;
                base = top;
                pc = currentPcode.D;
                break;
            case INT:
                //INT：为被调用的过程（包括主过程）在运行栈S中开辟数据区，这时A段为所需数据单元个数（包括三个连接数据）；L段恒为0。
                top = top + currentPcode.D;
                break;
            case JMP:
                //JMP：无条件转移，这时A段为转向地址（目标程序）。
                pc = currentPcode.D;
                break;
            case JPC:
                //JPC：条件转移，当运行栈S的栈顶的布尔值为假（0）时，则转向A段所指目标程序地址；否则顺序执行。
                if (dataStack[top - 1] == 0) {
                    pc = currentPcode.D;
                }
                break;
        }
        if(pc == 0 || pc >= size)
            cout<< 1;
    } while (pc != 0 && pc < size);
}
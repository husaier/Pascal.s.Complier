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
        oldBp = std::any_cast<int>(dataStack[oldBp + 1]);//取上一层函数的基地址
        lev--;
    }
    return oldBp;
}

//从文件里面读取p-code码
vector<Pcode> Pcode_Interpreter::readFile(string filename) {
    vector<Pcode> allPcode;
    Pcode_Interpreter interpreter;
    string line_info, input_result, read;
    Pcode pcodeitem;

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
                pcodeitem.OP = LIT;
            else if (read == "LOD")
                pcodeitem.OP = LOD;
            else if (read == "STO")
                pcodeitem.OP = STO;
            else if (read == "CAL")
                pcodeitem.OP = CAL;
            else if (read == "INT")
                pcodeitem.OP = INT;
            else if (read == "JMP")
                pcodeitem.OP = JMP;
            else if (read == "JPC")
                pcodeitem.OP = JPC;
            else if (read == "OPR")
                pcodeitem.OP = OPR;
            else {
                printf("出错\n");
                return allPcode;
            }
            input >> read;
            pcodeitem.L = stoi(read);
            input >> read;
            pcodeitem.D = stoi(read);
            allPcode.push_back(pcodeitem);
        }
        return allPcode;
    }
    return allPcode;
}

//对2个any类型的变量进行运算，若出错返回空的res，否则返回结果
any Pcode_Interpreter::calculate(pcodeOP op, any arg1, any arg2) {
    any res;
    res.reset();
    // 没有第二个操作数，为单操作数op
    if (!arg2.has_value()) {
        switch (op) {
            case UMINUS:
                if (string(arg1.type().name()) == "i") {
                    int i = any_cast<int>(arg1);
                    i = -i;
                    res = i;
                } else if (string(arg1.type().name()) == "d") {
                    double d = any_cast<double>(arg1);
                    d = -d;
                    res = d;
                } else
                    printf("error:uminus 类型错误");
                break;
            case NOT:
                if (string(arg1.type().name()) == "b") {
                    bool b = any_cast<bool>(arg1);
                    b = !b;
                    res = b;
                } else
                    printf("error:not 类型错误");
                break;
            default:
                printf("error:双操作数op 参数只有一个");
                break;
        }
    }
        // 有第二个操作数，为双操作数op
    else {
        switch (op) {
            case ADD: {
                int flag = 0;
                if (string(arg1.type().name()) == "i") {
                    if (string(arg2.type().name()) == "i")
                        flag = 1;
                    else if (string(arg2.type().name()) == "d")
                        flag = 2;
                    else
                        flag = 0;

                } else if (string(arg1.type().name()) == "d") {
                    if (string(arg2.type().name()) == "i")
                        flag = 3;
                    else if (string(arg2.type().name()) == "d")
                        flag = 4;
                    else
                        flag = 0;
                }

                if (flag == 1) {
                    int i1 = any_cast<int>(arg1);
                    int i2 = any_cast<int>(arg2);
                    i1 = i1 + i2;
                    res = i1;
                } else if (flag > 1) {
                    double d1 = any_cast<double>(arg1);
                    double d2 = any_cast<double>(arg2);
                    d1 = d1 + d2;
                    res = d1;
                } else
                    printf("error:add 类型错误");
                break;
            }
            case MINUS: {
                int flag = 0;
                if (string(arg1.type().name()) == "i") {
                    if (string(arg2.type().name()) == "i")
                        flag = 1;
                    else if (string(arg2.type().name()) == "d")
                        flag = 2;
                    else
                        flag = 0;

                } else if (string(arg1.type().name()) == "d") {
                    if (string(arg2.type().name()) == "i")
                        flag = 3;
                    else if (string(arg2.type().name()) == "d")
                        flag = 4;
                    else
                        flag = 0;
                }

                if (flag == 1) {
                    int i1 = any_cast<int>(arg1);
                    int i2 = any_cast<int>(arg2);
                    i1 = i1 - i2;
                    res = i1;
                } else if (flag > 1) {
                    double d1 = any_cast<double>(arg1);
                    double d2 = any_cast<double>(arg2);
                    d1 = d1 - d2;
                    res = d1;
                } else
                    printf("error:minus 类型错误");
                break;
            }
            case MULTIPLY: {
                int flag = 0;
                if (string(arg1.type().name()) == "i") {
                    if (string(arg2.type().name()) == "i")
                        flag = 1;
                    else if (string(arg2.type().name()) == "d")
                        flag = 2;
                    else
                        flag = 0;

                } else if (string(arg1.type().name()) == "d") {
                    if (string(arg2.type().name()) == "i")
                        flag = 3;
                    else if (string(arg2.type().name()) == "d")
                        flag = 4;
                    else
                        flag = 0;
                }

                if (flag == 1) {
                    int i1 = any_cast<int>(arg1);
                    int i2 = any_cast<int>(arg2);
                    i1 = i1 * i2;
                    res = i1;
                } else if (flag > 1) {
                    double d1 = any_cast<double>(arg1);
                    double d2 = any_cast<double>(arg2);
                    d1 = d1 * d2;
                    res = d1;
                } else
                    printf("error:multiply 类型错误");
                break;
            }
            case DIVIDE: {
                if ((string(arg1.type().name()) == "i" || string(arg1.type().name()) == "d") &&
                    (string(arg2.type().name()) == "i") || string(arg2.type().name()) == "d") {
                    double d1 = any_cast<double>(arg1);
                    double d2 = any_cast<double>(arg2);
                    d1 = d1 / d2;
                    res = d1;
                } else
                    printf("error:divide 类型错误");
                break;
            }
            case MOD: {
                if (string(arg1.type().name()) == "i" && string(arg2.type().name()) == "i") {
                    int i1 = any_cast<int>(arg1);
                    int i2 = any_cast<int>(arg2);
                    i1 = i1 % i2;
                    res = i1;
                } else
                    printf("error:mod 类型错误");
                break;
            }
            case EQUAL: {
                if (string(arg1.type().name()) == string(arg2.type().name())) {
                    auto type = string(arg1.type().name());
                    if (type == "i") {
                        int t1 = any_cast<int>(arg1);
                        int t2 = any_cast<int>(arg1);
                        res = (t1 == t2);
                    } else if (type == "d") {
                        double t1 = any_cast<double>(arg1);
                        double t2 = any_cast<double>(arg1);
                        res = (t1 == t2);
                    } else if (type == "c") {
                        char t1 = any_cast<char>(arg1);
                        char t2 = any_cast<char>(arg1);
                        res = (t1 == t2);
                    } else if (type == "b") {
                        bool t1 = any_cast<bool>(arg1);
                        bool t2 = any_cast<bool>(arg1);
                        res = (t1 == t2);
                    }
                } else
                    printf("error:equal 类型错误");
                break;
            }
            case UNEQUAL: {
                if (string(arg1.type().name()) == string(arg2.type().name())) {
                    auto type = string(arg1.type().name());
                    if (type == "i") {
                        int t1 = any_cast<int>(arg1);
                        int t2 = any_cast<int>(arg1);
                        res = (t1 != t2);
                    } else if (type == "d") {
                        double t1 = any_cast<double>(arg1);
                        double t2 = any_cast<double>(arg1);
                        res = (t1 != t2);
                    } else if (type == "c") {
                        char t1 = any_cast<char>(arg1);
                        char t2 = any_cast<char>(arg1);
                        res = (t1 != t2);
                    } else if (type == "b") {
                        bool t1 = any_cast<bool>(arg1);
                        bool t2 = any_cast<bool>(arg1);
                        res = (t1 != t2);
                    }
                } else
                    printf("error:unequal 类型错误");
                break;
            }
            case LESS: {
                if (string(arg1.type().name()) == string(arg2.type().name())) {
                    auto type = string(arg1.type().name());
                    if (type == "i") {
                        int t1 = any_cast<int>(arg1);
                        int t2 = any_cast<int>(arg1);
                        res = (t1 < t2);
                    } else if (type == "d") {
                        double t1 = any_cast<double>(arg1);
                        double t2 = any_cast<double>(arg1);
                        res = (t1 < t2);
                    } else if (type == "c") {
                        char t1 = any_cast<char>(arg1);
                        char t2 = any_cast<char>(arg1);
                        res = (t1 < t2);
                    } else if (type == "b") {
                        bool t1 = any_cast<bool>(arg1);
                        bool t2 = any_cast<bool>(arg1);
                        res = (t1 < t2);
                    }
                } else
                    printf("error:less 类型错误");
                break;
            }
            case LESS_EQUAL: {
                if (string(arg1.type().name()) == string(arg2.type().name())) {
                    auto type = string(arg1.type().name());
                    if (type == "i") {
                        int t1 = any_cast<int>(arg1);
                        int t2 = any_cast<int>(arg1);
                        res = (t1 <= t2);
                    } else if (type == "d") {
                        double t1 = any_cast<double>(arg1);
                        double t2 = any_cast<double>(arg1);
                        res = (t1 <= t2);
                    } else if (type == "c") {
                        char t1 = any_cast<char>(arg1);
                        char t2 = any_cast<char>(arg1);
                        res = (t1 <= t2);
                    } else if (type == "b") {
                        bool t1 = any_cast<bool>(arg1);
                        bool t2 = any_cast<bool>(arg1);
                        res = (t1 <= t2);
                    }
                } else
                    printf("error:less_equal 类型错误");
                break;
            }
            case MORE: {// 大于
                if (string(arg1.type().name()) == string(arg2.type().name())) {
                    auto type = string(arg1.type().name());
                    if (type == "i") {
                        int t1 = any_cast<int>(arg1);
                        int t2 = any_cast<int>(arg1);
                        res = (t1 > t2);
                    } else if (type == "d") {
                        double t1 = any_cast<double>(arg1);
                        double t2 = any_cast<double>(arg1);
                        res = (t1 > t2);
                    } else if (type == "c") {
                        char t1 = any_cast<char>(arg1);
                        char t2 = any_cast<char>(arg1);
                        res = (t1 > t2);
                    } else if (type == "b") {
                        bool t1 = any_cast<bool>(arg1);
                        bool t2 = any_cast<bool>(arg1);
                        res = (t1 > t2);
                    }
                } else
                    printf("error:more 类型错误");
                break;
            }
            case MORE_EQUAL: {
                if (string(arg1.type().name()) == string(arg2.type().name())) {
                    auto type = string(arg1.type().name());
                    if (type == "i") {
                        int t1 = any_cast<int>(arg1);
                        int t2 = any_cast<int>(arg1);
                        res = (t1 >= t2);
                    } else if (type == "d") {
                        double t1 = any_cast<double>(arg1);
                        double t2 = any_cast<double>(arg1);
                        res = (t1 >= t2);
                    } else if (type == "c") {
                        char t1 = any_cast<char>(arg1);
                        char t2 = any_cast<char>(arg1);
                        res = (t1 >= t2);
                    } else if (type == "b") {
                        bool t1 = any_cast<bool>(arg1);
                        bool t2 = any_cast<bool>(arg1);
                        res = (t1 >= t2);
                    }
                } else
                    printf("error:more_equal 类型错误");
                break;
            }
            case AND: {
                if (string(arg1.type().name()) == "b" && string(arg2.type().name()) == "b") {
                    bool t1 = any_cast<bool>(arg1);
                    bool t2 = any_cast<bool>(arg1);
                    res = (t1 && t2);
                } else
                    printf("error:and 类型错误");
                break;
            }
            case OR: {
                if (string(arg1.type().name()) == "b" && string(arg2.type().name()) == "b") {
                    bool t1 = any_cast<bool>(arg1);
                    bool t2 = any_cast<bool>(arg1);
                    res = (t1 || t2);
                } else
                    printf("error:or 类型错误");
                break;
            }
            case FLOOR_DIVIDE: {
                if (string(arg1.type().name()) == "i" && string(arg2.type().name()) == "i") {
                    int i1 = any_cast<int>(arg1);
                    int i2 = any_cast<int>(arg2);
                    i1 = i1 / i2;
                    res = i1;
                } else
                    printf("error:floor_divide 类型错误");
                break;
            }
            default:
                printf("error:单操作数op 参数有两个");
                break;
        }
    }
    return res;
}

//解释器
void Pcode_Interpreter::interpreter(vector<Pcode> pcode) {
    int pc = 0; //程序计数器，指向下一条指令
    int base = 0; //当前基地址
    int top = 0; //程序运行栈栈顶
    int size = 0; //pcode码集合的大小
    int address = 0;
    int i;
    char c;
    double d;
    bool b;
    allPcode.assign(pcode.begin(), pcode.end());//为allPcode赋值
    size = allPcode.size();
    do {
        Pcode currentPcode = allPcode[pc];
        pc++;
        switch (currentPcode.OP) {
            case OPR:
                //OPR：关系或算术运算，D段指明具体运算
                switch (std::any_cast<int>(currentPcode.D)) {
                    case 0:
                        //OPR 0 0   过程调用结束后,返回调用点并退栈
                        top = base;
                        pc = std::any_cast<int>(dataStack[base + 2]);
                        base = std::any_cast<int>(dataStack[base]);
                        break;
                    case 1:
                        //OPR 0 1     栈顶元素取负
                        dataStack[top - 1] = calculate(UMINUS, dataStack[top - 1], any());
                        break;
                    case 2:
                        //OPR 0 2   次栈顶与栈顶相加，退两个栈元素，结果值进栈
                        dataStack[top - 2] = calculate(ADD, dataStack[top - 2], dataStack[top - 1]);
                        top--;
                        break;
                    case 3:
                        //OPR 0 3   次栈顶减去栈顶，退两个栈元素，结果值进栈
                        dataStack[top - 2] = calculate(MINUS, dataStack[top - 2], dataStack[top - 1]);
                        top--;
                        break;
                    case 4:
                        //OPR 0 4     次栈顶乘以栈顶，退两个栈元素，结果值进栈
                        dataStack[top - 2] = calculate(MULTIPLY, dataStack[top - 2], dataStack[top - 1]);
                        top--;
                        break;
                    case 5:
                        //OPR 0 5     次栈顶除以栈顶，退两个栈元素，结果值进栈
                        dataStack[top - 2] = calculate(DIVIDE, dataStack[top - 2], dataStack[top - 1]);
                        top--;
                        break;
                    case 6:
                        //OPR 0 6   栈顶元素的奇偶判断，结果值在栈顶
                        dataStack[top - 2] = calculate(MOD, dataStack[top - 2], any(2));
                        break;
                    case 7:
                        //OPR 0 7   次栈顶模除栈顶，退两个栈元素，结果值进栈
                        dataStack[top - 2] = calculate(MOD, dataStack[top - 2], dataStack[top - 1]);
                        top--;
                        break;
                    case 8:
                        //OPR 0 8   次栈顶与栈顶是否相等，退两个栈元素，结果值进栈
                        dataStack[top - 2] = calculate(EQUAL, dataStack[top - 2], dataStack[top - 1]);
                        top--;
                        break;
                    case 9:
                        //OPR 0 9   次栈顶与栈顶是否不等，退两个栈元素，结果值进栈
                        dataStack[top - 2] = calculate(UNEQUAL, dataStack[top - 2], dataStack[top - 1]);
                        top--;
                        break;
                    case 10:
                        //OPR 0 10  次栈顶是否小于栈顶，退两个栈元素，结果值进栈
                        dataStack[top - 2] = calculate(LESS, dataStack[top - 2], dataStack[top - 1]);
                        top--;
                        break;
                    case 11:
                        //OPR 0 11    次栈顶是否大于等于栈顶，退两个栈元素，结果值进栈
                        dataStack[top - 2] = calculate(MORE_EQUAL, dataStack[top - 2], dataStack[top - 1]);
                        top--;
                        break;
                    case 12:
                        //OPR 0 12  次栈顶是否大于栈顶，退两个栈元素，结果值进栈
                        dataStack[top - 2] = calculate(MORE, dataStack[top - 2], dataStack[top - 1]);
                        top--;
                        break;
                    case 13:
                        //OPR 0 13  次栈顶是否小于等于栈顶，退两个栈元素，结果值进栈
                        dataStack[top - 2] = calculate(LESS_EQUAL, dataStack[top - 2], dataStack[top - 1]);
                        top--;
                        break;
                    case 14:
                        //OPR 0 14  栈顶值输出至屏幕
                        if (string(dataStack[top - 1].type().name()) == "i") {
                            i = any_cast<int>(dataStack[top - 1]);
                            cout << i << " ";
                        } else if (string(dataStack[top - 1].type().name()) == "d") {
                            d = any_cast<double>(dataStack[top - 1]);
                            cout << d << " ";
                        } else if (string(dataStack[top - 1].type().name()) == "b") {
                            b = any_cast<bool>(dataStack[top - 1]);
                            cout << b << " ";
                        } else if (string(dataStack[top - 1].type().name()) == "c") {
                            c = any_cast<char>(dataStack[top - 1]);
                            cout << c << " ";
                        } else {
                            printf("error：栈顶值无法打印");
                        }
                        break;
                    case 15:
                        //OPR 0 15  屏幕输出换行
                        cout << endl;
                        break;
                    case 161:
                        //OPR 0 161  从命令行读入一个int输入置于栈顶
                        cout << "please input a number" << endl;
                        cin >> i;
                        dataStack[top] = i;
                        top++;
                        break;
                    case 162:
                        //OPR 0 162  从命令行读入一个char输入置于栈顶
                        cout << "please input a char" << endl;
                        cin >> c;
                        dataStack[top] = c;
                        top++;
                        break;
                    case 163:
                        //OPR 0 163  从命令行读入一个real输入置于栈顶
                        cout << "please input a real" << endl;
                        cin >> d;
                        dataStack[top] = d;
                        top++;
                        break;
                    case 164:
                        //OPR 0 164  从命令行读入一个bool输入置于栈顶
                        cout << "please input a bool" << endl;
                        cin >> b;
                        dataStack[top] = b;
                        top++;
                        break;
                    case 17:
                        //OPR 0 17     次栈顶与栈顶取and，退两个栈元素，结果值进栈
                        dataStack[top - 2] = calculate(AND, dataStack[top - 2], dataStack[top - 1]);
                        top--;
                        break;
                    case 18:
                        //OPR 0 18     次栈顶与栈顶取or，退两个栈元素，结果值进栈
                        dataStack[top - 2] = calculate(OR, dataStack[top - 2], dataStack[top - 1]);
                        top--;
                        break;
                    case 19:
                        //OPR 0 19     栈顶取not
                        dataStack[top - 1] = calculate(NOT, dataStack[top - 1], any());
                        break;
                    case 20:
                        //OPR 0 20     次栈顶整除栈顶，退两个栈元素，结果值进栈
                        dataStack[top - 2] = calculate(FLOOR_DIVIDE, dataStack[top - 2], dataStack[top - 1]);
                        top--;
                        break;
//                    case 21:
//                        //OPR 0 21     把栈顶值作为地址，在此地址处的单元存入次栈顶的值，退2个栈元素
//                        dataStack[dataStack[top - 1]] = dataStack[top - 2];
//                        top -= 2;
//                        break;
                    default:
                        cout << "OPR指令出现错误" << std::any_cast<int>(currentPcode.D) << " " << pc;
                        pc = 0;
                        break;
                }
                break;
            case LIT:
                //LIT：将常量送到运行栈S的栈顶，这时D段为常量值
                dataStack[top] = currentPcode.D;
                top++;
                break;
            case LOD:
                // LOD：将变量送到运行栈S的栈顶，这时D段为变量所在说明层中的相对位置。 D若为-1意为从栈顶取相对地址
                if (std::any_cast<int>(currentPcode.D) != -1) {
                    dataStack[top] = dataStack[std::any_cast<int>(
                            calculate(ADD, currentPcode.D, any(getBase(base, currentPcode.L))))];
                    top++;
                } else {
                    int offset = std::any_cast<int>(dataStack[top - 1]);//从栈顶取偏移量
                    top--;
                    dataStack[top] = dataStack[offset + getBase(base, currentPcode.L)];
                    top++;
                }

                break;
            case STO:
                //STO：将运行栈S的栈顶内容送入某个变量单元中，D段为变量所在说明层中的相对位置。  D若为-1意为从栈顶取相对地址
                if (std::any_cast<int>(currentPcode.D) != -1) {
                    dataStack[std::any_cast<int>(
                            calculate(ADD, currentPcode.D, any(getBase(base, currentPcode.L))))] = dataStack[top - 1];
                    top--;
                } else {
                    int offset = std::any_cast<int>(dataStack[top - 1]);//从栈顶取偏移量
                    top--;
                    dataStack[offset + getBase(base, currentPcode.L)] = dataStack[top - 1];
                    top--;
                }
                break;
//            case LOA:
//                //LOA：获取变量的地址放到数据栈栈顶(相对地址为d，层次差为l)
//                dataStack[top] = currentPcode.D + getBase(base, currentPcode.L);
//                top++;
//                break;
//            case LAO:
//                //LAO：获取地址为栈顶值的变量放到数据栈栈顶
//                address = dataStack[top - 1];//从栈顶取地址
//                top--;
//                dataStack[top] = dataStack[address];
//                top++;
//                break;
            case CAL:
                //CAL：调用过程，这时D段为被调用过程的过程体（过程体之前一条指令）在目标程序区的入口地址。
                //跳转时，将该层基地址，跳转层基地址，pc指针保存在栈中
                //基地址base变为此时栈顶top，pc指向要跳转的地方
                //不修改top，因为前面代码已经将address+3，生成Pcode后会产生INT语句，修改top值
                dataStack[top] = base;
                dataStack[top + 1] = getBase(base, currentPcode.L);
                dataStack[top + 2] = pc;
                i = 1;
                while (allPcode[pc + i].OP == LIP || allPcode[pc + i].OP == LOP) {
                    if (allPcode[pc + i].OP == LIP) {
                        //LIP：取常量d作为函数参数传入
                        dataStack[top + 2 + i] = currentPcode.D;
                    } else {
                        //LOP：取变量(相对地址为d,层次差为l)作为函数参数传入
                        dataStack[top + 2 + i] = dataStack[std::any_cast<int>(
                                calculate(ADD, currentPcode.D, any(getBase(base, currentPcode.L))))];
                    }
                }
                base = top;
                pc = std::any_cast<int>(currentPcode.D);
                break;
            case LIP:
                break;
            case LOP:
                break;
            case INT:
                //INT：为被调用的过程（包括主过程）在运行栈S中开辟数据区，这时D段为所需数据单元个数（包括三个连接数据）；L段恒为0。
                top = std::any_cast<int>(calculate(ADD, top, currentPcode.D));
                break;
            case JMP:
                //JMP：无条件转移，这时D段为转向地址（目标程序）。
                pc = std::any_cast<int>(currentPcode.D);
                break;
            case JPC:
                //JPC：条件转移，当运行栈S的栈顶的布尔值为真时，则转向D段所指目标程序地址；否则顺序执行。
                if (std::any_cast<bool>(dataStack[top - 1])) {
                    pc = std::any_cast<int>(currentPcode.D);
                }
                break;
        }
        if (pc == 0 || pc >= size)
            cout << "运行结束";
    } while (pc != 0 && pc < size);

}

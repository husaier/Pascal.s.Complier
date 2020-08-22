//
// Created by husserl on 2020/8/8.
//

#include <sstream>
#include "MidCode.h"

QuaternionItem *Quaternion::outCode
        (int op, const std::string &arg1, const std::string &arg2, const std::string &res) {
    int seq = codeList.size();
    codeList.emplace_back(seq, op, arg1, arg2, res);
    return &codeList[seq];
}

void Quaternion::argPrint(string arg) {//判断是用户变量还是临时变量
    TempVar *var;
    if (arg[0] == '#') {
        var = getTempById(arg);
        cout << setiosflags(ios::left) << setw(printWidth) << var->toString();
    }else{
        cout << setiosflags(ios::left) << setw(printWidth) << arg;
    }
}

void Quaternion::print() {
    cout << "\n";
    cout << "中间代码：" << endl;
    int i = 50;
    while (i--)
        cout << '=';
    cout << '\n';
    cout << setiosflags(ios::left) << setw(printWidth) << "seq";
    cout << setiosflags(ios::left) << setw(printWidth) << "op";
    cout << setiosflags(ios::left) << setw(printWidth) << "arg1";
    cout << setiosflags(ios::left) << setw(printWidth) << "arg2";
    cout << setiosflags(ios::left) << setw(printWidth) << "res";
    cout << '\n';
    i = 50;
    while (i--)
        cout << '=';
    cout << '\n';
    TempVar *var;
    for (const auto &item:codeList) {
        cout << setiosflags(ios::left) << setw(printWidth) << item.seq;
        cout << setiosflags(ios::left) << setw(printWidth) << op2string(item.op);
        argPrint(item.arg1);
//        var = getTempById(item.arg1);
//        cout << setiosflags(ios::left) << setw(printWidth) << var->toString();
        if (!item.arg2.empty()) {
            argPrint(item.arg2);
//            var = getTempById(item.arg2);
//            cout << setiosflags(ios::left) << setw(printWidth) << var->toString();
        } else
            cout << setiosflags(ios::left) << setw(printWidth) << "";
        argPrint(item.res);
//        var = getTempById(item.res);
//        cout << setiosflags(ios::left) << setw(printWidth) << var->toString();
        cout << '\n';
    }
}

string Quaternion::op2string(int op) {
    string output;
    switch (op) {
        case QuaternionItem::ADD:
            output = "+";
            break;
        case QuaternionItem::MINUS:
            output = "minus";
            break;
        case QuaternionItem::MULTIPLY:
            output = "*";
            break;
        case QuaternionItem::DIVIDE:
            output = "/";
            break;
        case QuaternionItem::ASSIGN:
            output = ":=";
            break;
        case QuaternionItem::EQUAL:
            output = "=";
            break;
        case QuaternionItem::OR:
            output = "or";
            break;
        case QuaternionItem::FLOOR_DIVIDE:
            output = "//";
            break;
        case QuaternionItem::MOD:
            output = "%";
            break;
        case QuaternionItem::AND:
            output = "and";
            break;
        case QuaternionItem::LESS:
            output = "<";
            break;
        case QuaternionItem::LESS_EQUAL:
            output = "<=";
            break;
        case QuaternionItem::MORE:
            output = ">";
            break;
        case QuaternionItem::MORE_EQUAL:
            output = ">=";
            break;
        case QuaternionItem::UNEQUAL:
            output = "<>";
            break;
        case QuaternionItem::NOT:
            output = "not";
            break;
        default:
            break;
    }
    return output;
}

TempVar *Quaternion::newTemp(bool flag) {
    char t[10];
    snprintf(t, sizeof(t), "#%d", tempSeq++);//这里的井号用于表示临时变量
    string name;
    name.append(t);
    auto var = new TempVar(name, flag);
    tempVarList.push_back(var);
    return var;
}

TempVar *Quaternion::getTempById(string id) {
    id.erase(id.begin());
    int i = stoi(id);
    return tempVarList[i];
}

string TempVar::toString() const {
    stringstream ss;
    if (isImmediate) {
        ss << "$" << value;
        return ss.str();
    } else if (id[0] == '#') {//如果是临时变量
        return id;
    }
}

//
// Created by 叶铭炜 on 2020/5/24.
//

#include "LR1Runner.h"


vector<string> vectorInput;
stack<int> stackState;
stack<string> stackSymbol;

const int width1 = 50, width2 = 50, width3 = 50;


void LR1Runner::run(LR1Table& table) {
    cout << "-------------------------------------------------------------------------------------------Runner::run"
         << "------------------------------------------------------------------------------------------------------"
         << endl;
    stackState.push(0);
    stackSymbol.push(" ");
    cout << setiosflags(ios::left) << setw(width1) << "Stack" << resetiosflags(ios::left) << setiosflags(ios::left)
         << setw(width2) << "Input" << setw(width3) << "Output" << resetiosflags(ios::left) << endl;
    cout << "------------------------------------------------------------------------------------------------------"
         << "------------------------------------------------------------------------------------------------------"
         << endl;

    int errorIpNumber = 0, ip = 0;
    while (true) {
        if (ip >= vectorInput.size()) {
            cout << "ERROR Break" << endl;
            break;
        }
        outStackInt(stackState);
        outStackString(stackSymbol);
        outStrInput(vectorInput, ip);
        int s = stackState.top();       //从栈顶取当前状态
        cout << endl << "OutPut:" << setw(width3) << setiosflags(ios::left);
        int tempCol = table.invMap[vectorInput.at(ip)];
        int currentType = table.table[s][tempCol].type;
        if (currentType == TableItem::SHIFT) {
            stackSymbol.push(vectorInput.at(ip));   //根据输入，压入当前遇到的输入
            stackState.push(table.table[s][tempCol].index);  //根据表格中S后的数字，压入状态
            ip++;
            cout << "S" + to_string(table.table[s][tempCol].index) << endl;//输出动作
        } else if (currentType == TableItem::REDUCTION) {
            int tempProd = table.table[s][tempCol].index;  //取出要按哪个产生式R
            Production tempProduction = table.productions.at(tempProd);
            for (int i = 0; i < tempProduction.right.size(); ++i) {    //按照产生式右侧数目来弹出
                if (tempProduction.right[i] != "#"){
                    stackState.pop();
                    stackSymbol.pop();
                }
            }
            stackSymbol.push(tempProduction.left);  //将产生式的左侧符号压入栈
            //根据当前状态栈和遇到的输入符号，确定下一次goto到几号状态
            stackState.push(table.table[stackState.top()][table.invMap[stackSymbol.top()]].index);
            string tempString;
            for (const auto &i : tempProduction.right) {
                tempString += i + " ";
            }
            cout << "R" + to_string(table.table[s][tempCol].index) + " by " + tempProduction.left + " -> " + tempString
                 << endl;
        } else if (currentType == TableItem::ACC) {
            cout << "ACC" << endl;
            break;
        } else {
            //没有操作的话报错，ip加一
            cout << "ERROR ip++" << endl;
            ip++;
            errorIpNumber++;
        }
        cout << resetiosflags(ios::left);
        cout << "------------------------------------------------------------------------------------------------------"
             << "------------------------------------------------------------------------------------------------------"
             << endl;
    }
    if (errorIpNumber == 0 && ip == vectorInput.size() - 1)    //输出统计信息
        cout << "分析结束，无错误" << endl;
    else {
        cout << "分析结束，出现错误" << endl << endl;
        cout << "分析过程中错误处理" << endl;
        cout << "指针后移次数:\t" << errorIpNumber << endl;
    }
}

void LR1Runner::outStackInt(stack<int> staTemp) {
    stack<int> staTempR;
    while (!staTemp.empty()) {
        staTempR.push(staTemp.top());
        staTemp.pop();
    }
    string strTemp = "State: ";
    while (!staTempR.empty()) {
        strTemp += to_string(staTempR.top()) + " ";
        staTempR.pop();
    }
    cout << setiosflags(ios::left) << setw(width1) << strTemp << resetiosflags(ios::left) << endl;
}

void LR1Runner::outStackString(stack<string> staTemp) {
    stack<string> staTempR;
    while (!staTemp.empty()) {
        staTempR.push(staTemp.top());
        staTemp.pop();
    }
    string strTemp = "Symbol:";
    while (!staTempR.empty()) {
        strTemp += staTempR.top() + " ";
        staTempR.pop();
    }
    cout << setiosflags(ios::left) << setw(width1) << strTemp << resetiosflags(ios::left)<<"\n";
}

void LR1Runner::outStrInput(vector<string> tempVector, int temp) {
    cout << setiosflags(ios::left) << setw(width2);
    string tempString;
    for (int i = temp; i < tempVector.size(); i++)
        tempString += tempVector.at(i) + " ";
    cout << tempString << resetiosflags(ios::left);
}

void LR1Runner::load(const vector<LexicalItem> &result) {
    string tempInputString, tempStackString;
    for (const auto &i : result) {
        vectorInput.push_back(i.symbol);
    }
    for (const auto &i : vectorInput) {
        cout << i + " ";
    }
    cout << endl;
    vectorInput.emplace_back("$");
}



//
// Created by 叶铭炜 on 2020/5/24.
//

#include "LR1Runner.h"
#include "lexical_analyzer.h"

vector<LexicalItem> vectorInput;
stack<int> stackState;
stack<string> stackSymbol;
vector<string> vectorAttribute;
const int width1 = 50, width2 = 50, width3 = 50;


void LR1Runner::run(LR1Table &table) {
    cout << "-------------------------------------------------------------------------------------------Runner::run"
         << "------------------------------------------------------------------------------------------------------"
         << endl;
    stackState.push(0);
    stackSymbol.push(" ");
    vectorAttribute.emplace_back(" ");
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
        outVectorAttribute(vectorAttribute);
        outStrInput(vectorInput, ip);

        int s = stackState.top();       //从栈顶取当前状态
        cout << endl << "OutPut:" << setw(width3) << setiosflags(ios::left);
        int tempCol = table.invMap[vectorInput.at(ip).symbol];
        int currentType = table.table[s][tempCol].type;
        if (currentType == TableItem::SHIFT) {
            stackSymbol.push(vectorInput.at(ip).symbol);   //根据输入，压入当前遇到的输入
            vectorAttribute.push_back(vectorInput.at(ip).attribute);///
            stackState.push(table.table[s][tempCol].index);  //根据表格中S后的数字，压入状态
            ip++;
            cout << "S" + to_string(table.table[s][tempCol].index) << endl;//输出动作
        } else if (currentType == TableItem::REDUCTION) {
            int tempProd = table.table[s][tempCol].index;  //取出要按哪个产生式R
            switchTable(tempProd);
            Production tempProduction = table.productions.at(tempProd);
            for (int i = 0; i < tempProduction.right.size(); ++i) {    //按照产生式右侧数目来弹出
                if (tempProduction.right[i] != "#") {
                    stackState.pop();
                    stackSymbol.pop();
                    vectorAttribute.pop_back();
                }
            }
            stackSymbol.push(tempProduction.left);  //将产生式的左侧符号压入栈
            vectorAttribute.push_back(tempProduction.left);////这里与符号相同
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
    cout << setiosflags(ios::left) << setw(width1) << strTemp << resetiosflags(ios::left) << "\n";
}

void LR1Runner::outVectorAttribute(vector<string> tempVector) {
    cout << setiosflags(ios::left) << setw(width2);
    string tempString;
    tempString += "VectorAttribute:";
    for (int i = 0; i < tempVector.size(); i++)
        tempString += tempVector.at(i) + " ";
    cout << tempString << resetiosflags(ios::left) << endl;
}


void LR1Runner::outStrInput(vector<LexicalItem> tempVector, int temp) {
    cout << setiosflags(ios::left) << setw(width2);
    string tempString;
    tempString += "InputSymbol:";
    for (int i = temp; i < tempVector.size(); i++)
        tempString += tempVector.at(i).symbol + " ";
    tempString += "\nInputAttribute:";
    for (int i = temp; i < tempVector.size(); i++)
        tempString += tempVector.at(i).attribute + " ";
    cout << tempString << resetiosflags(ios::left);
}

void LR1Runner::load(const vector<LexicalItem> &result) {
    string tempInputString, tempStackString;
    for (const auto &i : result) {
        vectorInput.push_back(i);
    }
    cout << "InputSymbol:";
    for (const auto &i : vectorInput) {
        cout << i.symbol + " ";
    }
    cout << endl << "InputAttribute:";
    for (const auto &i : vectorInput) {
        cout << i.attribute + " ";
    }
    cout << endl;
    LexicalItem temp = LexicalItem(999, "$");
    temp.symbol = "$";
    vectorInput.emplace_back(temp);
}

void LR1Runner::switchTable(int type) {
    int top = vectorAttribute.size() - 1;
    string id;
    switch(type) {
        case 109:
            locate();
            id = vectorAttribute[top];
            declareID(id);
            break;
        case 110:
        case 111:
            id = vectorAttribute[top];
            declareID(id);
            locate();
            break;
        case 4:
        case 5:
            id = vectorAttribute[top];
            declareID(id);
            break;
        case 8:
        case 9:
        case 19:
        case 20:
            id = vectorAttribute[top - 2];
            declareID(id);
            break;
        case 10:
        case 11:
        case 12:
        case 66:
        case 78:
        case 106:
            id = vectorAttribute[top];
            quoteID(id);
            break;
        case 62:
            id = vectorAttribute[top - 1];
            quoteID(id);
            break;
        case 79:
        case 103:
            id = vectorAttribute[top - 3];
            quoteID(id);
            break;
        case 50:
            relocate();
            break;
        default:
            break;
    }
}

void LR1Runner::declareID(string id) {
    if (curBlock->blockQuery(id) == nullptr) {
        curBlock->insert(id, 0, 0, 0, 0);
        cout << "声明"<<id<<endl;
        curBlock->printBlock();
    }
    else {
        cout<<"语义错误！在作用域内有重复定义的标识符"<<endl;
    }
}

void LR1Runner::quoteID(string id) {
    if (curBlock->query(id) == nullptr)
        cout<<"语义错误！引用了未定义的标识符"<<id<<endl;
    else{
        cout<<"找到引用"<<id<<endl;
        curBlock->printBlock();
    }
}

void LR1Runner::locate() {
    SymbolBlock* childBlock;
    childBlock = SymbolBlock::makeBlock(curBlock);
    curBlock = childBlock;
    cout<<"定位"<<endl;
}

void LR1Runner::relocate() {
    SymbolBlock* parentBlock = curBlock->previous;
    delete curBlock;
    curBlock = parentBlock;
    cout<<"重定位删除"<<endl;
}

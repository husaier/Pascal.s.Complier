//
// Created by 叶铭炜 on 2020/5/24.
//

#include "LR1Runner.h"
#include "lexical_analyzer.h"

vector<LexicalItem> vectorInput;
stack<int> stackState;
stack<string> stackSymbol;
vector<vectorAttributeItem> vectorAttribute;
const int width1 = 50, width2 = 50, width3 = 50;


void LR1Runner::run(LR1Table &table) {
    initial();

    cout << "-------------------------------------------------------------------------------------------Runner::run"
         << "------------------------------------------------------------------------------------------------------"
         << endl;
    stackState.push(0);
    stackSymbol.push(" ");
    vectorAttribute.emplace_back(vectorAttributeItem(" ", -1, -1, -1));    ///line
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
            vectorAttribute.emplace_back(vectorInput.at(ip).attribute, -1, -1, vectorInput.at(ip).line);    ///line
            stackState.push(table.table[s][tempCol].index);  //根据表格中S后的数字，压入状态
            ip++;
            cout << "S" + to_string(table.table[s][tempCol].index) << endl;//输出动作
        } else if (currentType == TableItem::REDUCTION) {
            int tempProd = table.table[s][tempCol].index;  //取出要按哪个产生式R
            Production tempProduction = table.productions.at(tempProd);
            vectorAttributeItem tempAttributeItem(tempProduction.left, -1, -1, vectorInput.at(ip).line);    ///line
            switchTable(&tempAttributeItem, tempProd);

            for (int i = 0; i < tempProduction.right.size(); ++i) {    //按照产生式右侧数目来弹出
                if (tempProduction.right[i] != "#") {
                    stackState.pop();
                    stackSymbol.pop();
                    vectorAttribute.pop_back();
                }
            }
            stackSymbol.push(tempProduction.left);  //将产生式的左侧符号压入栈
            vectorAttribute.push_back(tempAttributeItem);////这里与符号相同
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

void LR1Runner::outVectorAttribute(vector<vectorAttributeItem> tempVector) {
    cout << setiosflags(ios::left) << setw(width2);
    string tempString;
    tempString += "VectorAttribute:";
    for (auto &i : tempVector)
        tempString += i.attribute + " ";
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

void LR1Runner::switchTable(vectorAttributeItem *leftSymbol, int op_type) {
    int top = vectorAttribute.size() - 1;
    string id;
    switch (op_type) {
        case 4:
            id = vectorAttribute[top].attribute;
            for (auto item : vectorAttribute[top - 2].IDlist) {
                leftSymbol->IDlist.push_back(item);
            }
            if (curBlock->blockQuery(id) == nullptr) {
                SymbolTableLine *entry;
                entry = curBlock->insert2(id, 0, 0, 0, 0);
                cout << "声明" << id << endl;
                curBlock->printBlock();

                leftSymbol->IDlist.push_back(entry);
            } else {
                cout << "语义错误！在作用域内有重复定义的标识符" << endl;
            }
            break;
        case 5:
            id = vectorAttribute[top].attribute;
            if (curBlock->blockQuery(id) == nullptr) {
                SymbolTableLine *entry;
                entry = curBlock->insert2(id, 0, 0, 0, 0);
                cout << "声明" << id << endl;
                curBlock->printBlock();

                leftSymbol->IDlist.push_back(entry);
            } else {
                cout << "语义错误！在作用域内有重复定义的标识符" << endl;
            }
            break;
        case 8:
        case 9:
        case 19:
        case 20:
            id = vectorAttribute[top - 2].attribute;
            declareID(id);
            break;
        case 10:
        case 11:
        case 12:
        case 66:
        case 78:
        case 106:
            id = vectorAttribute[top].attribute;
            quoteID(id);
            break;
        case 15: {
            leftSymbol->num = stoi(vectorAttribute[top].attribute);
            break;
        }
        case 21:
            leftSymbol->type = vectorAttribute[top].type;
            leftSymbol->width = vectorAttribute[top].width;
            break;
        case 22: {
            auto Record_body = vectorAttribute[top - 1];
            leftSymbol->type = SymbolTableLine::RECORD;
            leftSymbol->tableEntry = curBlock;
            leftSymbol->width = *offset;
            leftSymbol->dimension = Record_body.dimension + 1;
            relocate();
            break;
        }
        case 23: {
            auto Type = vectorAttribute[top];
            auto Periods = vectorAttribute[top - 3];

            int baseType = Type.type;
            int elementWidth = Type.width;
            //把Type.arrayInfo链表插入到Periods.arrayInfo链表的尾部
            auto tail = Periods.arrayInfo;
            while (tail != nullptr) {
                if (tail->nextDemision == nullptr)
                    break;
                tail = tail->nextDemision;
            }
            int demendionTH = tail->deimensionTH;
            auto typeArrayInfoHead = Type.arrayInfo;
            tail->nextDemision = typeArrayInfoHead;
            while (typeArrayInfoHead != nullptr) {
                demendionTH++;
                typeArrayInfoHead->deimensionTH = demendionTH;
                typeArrayInfoHead = typeArrayInfoHead->nextDemision;
            }
            auto head = Periods.arrayInfo;
            int nums = 1;
            while (head != nullptr) {
                head->elementType = baseType;
                nums *= head->legnth;
                head = head->nextDemision;
            }
            leftSymbol->type = SymbolTableLine::ARRAY;
            leftSymbol->arrayInfo = Periods.arrayInfo;
            leftSymbol->width = Type.width * nums;
            leftSymbol->dimension = Periods.dimension + Type.dimension;
            break;
        }
        case 24: //integer
            leftSymbol->type = SymbolTableLine::INTEGER;
            leftSymbol->width = 4;
            leftSymbol->dimension = 0;
            break;
        case 25: //real
            leftSymbol->type = SymbolTableLine::REAL;
            leftSymbol->width = 4;
            leftSymbol->dimension = 0;
            break;
        case 26: //boolean
            leftSymbol->type = SymbolTableLine::BOOLEAN;
            leftSymbol->width = 1;
            leftSymbol->dimension = 0;
            break;
        case 27: //char
            leftSymbol->type = SymbolTableLine::CHAR;
            leftSymbol->width = 1;
            leftSymbol->dimension = 0;
            break;
        case 28: {
            auto Var_declaration = vectorAttribute[top];
            leftSymbol->dimension = Var_declaration.dimension;
            break;
        }
        case 30: {
            auto period = vectorAttribute[top];
            auto period1 = vectorAttribute[top - 2];
            auto &period0 = leftSymbol;
            period0->dimension = period1.dimension + 1;
            auto *t = period.arrayInfo;
            auto head = period1.arrayInfo;
            //插入到链表的尾部
            while (head != nullptr) {
                if (head->nextDemision == nullptr)
                    break;
                head = head->nextDemision;
            }
            t->deimensionTH = head->deimensionTH + 1;
            head->nextDemision = t;
            // 传给period0
            period0->arrayInfo = period1.arrayInfo;
            break;
        }
        case 31: {
            auto attr = vectorAttribute[top];
            leftSymbol->dimension = 1;
            leftSymbol->arrayInfo = attr.arrayInfo;
            break;
        }
        case 32: {
            int start = vectorAttribute[top - 2].num;
            int end = vectorAttribute[top].num;
            auto *t = new ArrayInfo();
            t->startIndex = start;
            t->endIndex = end;
            t->deimensionTH = 1;
            t->legnth = end - start + 1;
            t->nextDemision = nullptr;

            leftSymbol->arrayInfo = t;
            break;
        }
        case 35:
        case 36: {
            vectorAttributeItem &Type = vectorAttribute[top];
            for (auto &item : vectorAttribute[top - 2].IDlist) {
                item->type = Type.type;
                item->offset = *offset;
                item->blockPoint = Type.tableEntry;
                item->arrayInfo = Type.arrayInfo;
                item->dimension = Type.dimension;
                *offset += Type.width;
            }
            leftSymbol->dimension = Type.dimension;
            curBlock->printBlock();
            break;
        }
        case 49: {
            vectorAttributeItem tmp = vectorAttribute[top];
            for (auto &item : vectorAttribute[top - 2].IDlist) {
                item->type = tmp.type;
                item->offset = *offset;
                item->blockPoint = tmp.tableEntry;
                item->arrayInfo = tmp.arrayInfo;
                item->dimension = tmp.dimension;
                *offset += tmp.width;
            }
            curBlock->printBlock();
        }
            break;
        case 50:
            relocate();
            break;
        case 53: { // Statement -> Variable assignop Expression
            auto Expression = vectorAttribute[top];
            auto Variable = vectorAttribute[top - 2];
            if(Variable.type != Expression.type)
                recordSemanticError("赋值语句类型错误");
            break;
        }
        case 62: { // Variable -> id F62 Id_varparts
            auto Id_varparts  = vectorAttribute[top];
            leftSymbol->entry = Id_varparts.entry;
            int type = leftSymbol->entry->type;
            leftSymbol->type = type;
            break;
        }
        case 64: {
            leftSymbol->entry = vectorAttribute[top].entry;
            break;
        }
        case 79:
        case 103:
            id = vectorAttribute[top - 3].attribute;
            quoteID(id);
            break;
        case 88: { // Expression -> Simple_expression
            auto Simple_expression = vectorAttribute[top];
            leftSymbol->type = Simple_expression.type;
            leftSymbol->value = Simple_expression.value;
            break;
        }
        case 89: { // Simple_expression -> Term
            auto Term = vectorAttribute[top];
            leftSymbol->type = Term.type;
            leftSymbol->value = Term.value;
            break;
        }
        case 95: { //Term0 -> Term1 * Factor
            auto Term1 = vectorAttribute[top - 2];
            auto Factor = vectorAttribute[top];
            int resultType = SymbolTableLine::VOID;
            string value;
            int flag;
            if (Term1.type == SymbolTableLine::INTEGER) {
                if (Factor.type == SymbolTableLine::INTEGER)
                    flag = 1;
                else if (Factor.type == SymbolTableLine::REAL)
                    flag = 2;
                else
                    flag = 0;
            } else if (Term1.type == SymbolTableLine::REAL) {
                if (Factor.type == SymbolTableLine::INTEGER)
                    flag = 3;
                else if (Factor.type == SymbolTableLine::REAL)
                    flag = 4;
                else
                    flag = 0;
            } else
                flag = 0;

            switch (flag) {
                case 1: { // 都是INTEGER
                    resultType = SymbolTableLine::INTEGER;
                    int a = stoi(Term1.value);
                    int b = stoi(Factor.value);
                    value = to_string(a * b);
                    break;
                }
                case 2:
                case 3:
                case 4: {
                    resultType = SymbolTableLine::REAL;
                    float a = stof(Term1.value);
                    float b = stof(Factor.value);
                    value = to_string(a * b);
                    break;
                }
                default: {
                    recordSemanticError("运算符*：运算对象类型错误");
                    break;
                }
            }
            leftSymbol->type = resultType;
            leftSymbol->value = value;
            break;
        }
        case 100: { //Term -> Factor
            auto Factor = vectorAttribute[top];
            leftSymbol->type = Factor.type;
            leftSymbol->value = Factor.value;
            break;
        }
        case 101: { //Factor -> Unsign_const_variable
            auto Unsign_const_variable = vectorAttribute[top];
            leftSymbol->type = Unsign_const_variable.type;
            leftSymbol->value = Unsign_const_variable.value;
            break;
        }
        case 107: {
            auto num = vectorAttribute[top];
            bool isRealType = false;
            for (auto c : num.attribute) {
                if (c == '.') //如果含有小数点就说明是实数类型
                {
                    isRealType = true;
                    break;
                }
            }
            if (isRealType) {
                leftSymbol->type = SymbolTableLine::REAL;
                leftSymbol->value = num.attribute;
            } else {
                leftSymbol->type = SymbolTableLine::INTEGER;
                leftSymbol->value = num.attribute;
            }
            break;
        }
        case 109:
        case 110:
        case 111: {
            id = vectorAttribute[top].attribute;
            declareID(id);

            auto tableLine = curBlock->blockQuery(id);

            locate();

            tableLine->blockPoint = curBlock;
            break;
        }
        case 113:
            locate();
            break;
        case 114: { // F62 -> #
            auto t = vectorAttribute[top];
            auto entry = curBlock->query(t.attribute);
            if (entry == nullptr) {
                recordSemanticError("未定义的引用");
                break;
            }
            t.entry = entry;
            leftSymbol->entry = entry;
            break;
        }
        default:
            break;
    }
}

void LR1Runner::declareID(string id) {
    if (curBlock->blockQuery(id) == nullptr) {
        curBlock->insert(id, 0, 0, 0, 0);
        cout << "声明" << id << endl;
        curBlock->printBlock();
    } else {
        cout << "语义错误！在作用域内有重复定义的标识符" << endl;
    }
}

void LR1Runner::quoteID(string id) {
    if (curBlock->query(id) == nullptr)
        cout << "语义错误！引用了未定义的标识符" << id << endl;
    else {
        cout << "找到引用" << id << endl;
        curBlock->printBlock();
    }
}

void LR1Runner::locate() {
    SymbolBlock *childBlock;
    childBlock = SymbolBlock::makeBlock(curBlock);
    curBlock = childBlock;
    offset = new int(0);

    tablePointers.push(curBlock);
    offSetStack.push(offset);
    cout << "定位" << endl;
}

void LR1Runner::relocate() {
    tablePointers.pop();
    offSetStack.pop();
    // 恢复到父表
    if (!tablePointers.empty()) {
        curBlock = tablePointers.top();
        offset = offSetStack.top();
    }
    cout << "重定位" << endl;
}

void LR1Runner::initial() {
    SymbolBlock *childBlock;
    childBlock = SymbolBlock::makeBlock(curBlock);
    curBlock = childBlock;
    symbolTable = curBlock;
    offset = new int(0);

    tablePointers.push(curBlock);
    offSetStack.push(offset);
}

void LR1Runner::printSemanticError() {
    if (semanticError.empty()) {
        cout << "源代码中没有语义错误";
        return;
    }
    for (const auto &s : semanticError) {
        cout << s << endl;
    }
}

void LR1Runner::recordSemanticError(const string& e) {
    semanticError.push_back(e);
}

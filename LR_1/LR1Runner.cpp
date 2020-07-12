//
// Created by 叶铭炜 on 2020/5/24.
//

#include <sstream>
#include "LR1Runner.h"
#include "lexical_analyzer.h"

vector<LexicalItem> vectorInput;
stack<int> stackState;
stack<string> stackSymbol;
vector<vectorAttributeItem> vectorAttribute;
const int width1 = 50, width2 = 50, width3 = 50;


void LR1Runner::run(LR1Table &table) {
    initial();
    if (debugInfoLevel >= 2) {
        cout << "-------------------------------------------------------------------------------------------Runner::run"
             << "------------------------------------------------------------------------------------------------------"
             << endl;
    }

    stackState.push(0);
    stackSymbol.push(" ");
    vectorAttribute.emplace_back(vectorAttributeItem(" ", -1, -1, -1));    ///line
    if (debugInfoLevel >= 2) {
        cout << setiosflags(ios::left) << setw(width1) << "Stack" << resetiosflags(ios::left) << setiosflags(ios::left)
             << setw(width2) << "Input" << setw(width3) << "Output" << resetiosflags(ios::left) << endl;
        cout << "------------------------------------------------------------------------------------------------------"
             << "------------------------------------------------------------------------------------------------------"
             << endl;
    }

    int errorIpNumber = 0, ip = 0;
    while (true) {
        if (ip >= vectorInput.size()) {
            if (debugInfoLevel >= 2) {
                cout << "ERROR Break" << endl;
            }
            break;
        }
        if (debugInfoLevel >= 2) {
            outStackInt(stackState);
            outStackString(stackSymbol);
            outVectorAttribute(vectorAttribute);
            outStrInput(vectorInput, ip);
        }

        int s = stackState.top();       //从栈顶取当前状态
        if (debugInfoLevel >= 2) {
            cout << endl << "OutPut:" << setw(width3) << setiosflags(ios::left);
        }

        int tempCol = table.invMap[vectorInput.at(ip).symbol];
        int currentType = table.table[s][tempCol].type;
        if (currentType == TableItem::SHIFT) {
            stackSymbol.push(vectorInput.at(ip).symbol);   //根据输入，压入当前遇到的输入
            vectorAttribute.emplace_back(vectorInput.at(ip).attribute, -1, -1, vectorInput.at(ip).line);    ///line
            stackState.push(table.table[s][tempCol].index);  //根据表格中S后的数字，压入状态
            ip++;
            if (debugInfoLevel >= 2) {
                cout << "S" + to_string(table.table[s][tempCol].index) << endl;//输出动作
            }
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
            if (debugInfoLevel >= 2) {
                cout << "R" + to_string(table.table[s][tempCol].index) + " by " + tempProduction.left + " -> " +
                        tempString
                     << endl;
            }

        } else if (currentType == TableItem::ACC) {
            if (debugInfoLevel >= 2) {
                cout << "ACC" << endl;
            }
            break;
        } else {
            if (debugInfoLevel >= 2) {
                //没有操作的话报错，ip加一
                cout << "ERROR ip++" << endl;
            }
            ip++;
            errorIpNumber++;
        }
        if (debugInfoLevel >= 2) {
            cout << resetiosflags(ios::left);
            cout
                    << "------------------------------------------------------------------------------------------------------"
                    << "------------------------------------------------------------------------------------------------------"
                    << endl;
        }
    }
    if (debugInfoLevel >= 1) {
        cout
                << "=======================================================================================================\n";
        if (errorIpNumber == 0 && ip == vectorInput.size() - 1) { //输出统计信息
            cout << "语法分析结束，无错误" << endl;
        } else {
            cout << "分析结束，出现错误" << endl << endl;
            cout << "分析过程中错误处理" << endl;
            cout << "指针后移次数:\t" << errorIpNumber << endl;
        }
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
    for (const auto &i : result) { //这里的vectorInput其实就是result,最后多了一个$
        vectorInput.push_back(i);
    }
    if (debugInfoLevel >= 2) {
        cout << "InputSymbol:";
        for (const auto &i : vectorInput) {
            cout << i.symbol + " ";
        }
        cout << endl << "InputAttribute:";
        for (const auto &i : vectorInput) {
            cout << i.attribute + " ";
        }
        cout << endl;
    }
    LexicalItem temp = LexicalItem(999, "$");
    temp.symbol = "$";
    vectorInput.emplace_back(temp);
}

void LR1Runner::switchTable(vectorAttributeItem *leftSymbol, int op_type) {
    int top = vectorAttribute.size() - 1;
    string id;
    switch (op_type) {
        case 4: { // Identifier_list -> Identifier_list , id
            id = vectorAttribute[top].attribute;
            int line = vectorAttribute[top].line;
            for (auto item : vectorAttribute[top - 2].IDlist) {
                leftSymbol->IDlist.push_back(item);
            }
            if (curBlock->blockQuery(id) == nullptr) {
                SymbolTableLine *entry;
                entry = curBlock->insert2(id, 0, 0, 0, 0);
                if (debugInfoLevel >= 3) {
                    cout << "声明" << id << endl;
                    curBlock->printBlock();
                }

                leftSymbol->IDlist.push_back(entry);
            } else {
                recordSemanticError(line, "语义错误！在作用域内有重复定义的标识符");
            }
            break;
        }
        case 5: { // Identifier_list -> id
            id = vectorAttribute[top].attribute;
            int line = vectorAttribute[top].line;
            if (curBlock->blockQuery(id) == nullptr) {
                SymbolTableLine *entry;
                entry = curBlock->insert2(id, 0, 0, 0, 0);
                if (debugInfoLevel >= 3) {
                    cout << "声明" << id << endl;
                    curBlock->printBlock();
                }
                leftSymbol->IDlist.push_back(entry);
            } else {
                recordSemanticError(line, "语义错误！在作用域内有重复定义的标识符");
            }
            break;
        }
        case 8: // Const_declaration -> Const_declaration ; id = Const_variable
        case 9: {// Const_declaration -> id = Const_variable
            id = vectorAttribute[top - 2].attribute;
            int line = vectorAttribute[top].line;
            if (curBlock->blockQuery(id) == nullptr) {
                SymbolTableLine *tempPoint = curBlock->insert2(id, vectorAttribute[top].type, 0, 0, 0);
                tempPoint->specialType = SymbolTableLine::CONST;//将常量标志位置为1
                if (debugInfoLevel >= 3) {
                    cout << "声明" << id << endl;
                    curBlock->printBlock();
                }
            } else {
                recordSemanticError(line, "语义错误！在作用域内有重复定义的标识符");
            }
            break;
        }
        case 10:    //10. Const_variable -> + id
        case 11:    //11. Const_variable -> - id
        case 12:    //12. Const_variable -> id
        case 106: {  //106. Unsign_const_variable -> id
            id = vectorAttribute[top].attribute;
            int line = vectorAttribute[top].line;
            quoteID(line, id);  //查看是否存在这个id
            SymbolTableLine *tempLinePoint = curBlock->query(id);
            if (tempLinePoint != nullptr) {//如果存在
                if (tempLinePoint->specialType == SymbolTableLine::CONST) {
                    leftSymbol->type = tempLinePoint->type;
                } else {
                    recordSemanticError(line, "语义错误！此id不为常量");
                }
            }
            break;
        }
        case 13:    // 13. Const_variable -> + num
        case 14:    // 14. Const_variable -> - num
        case 15: { // 15. Const_variable -> num
            string tempString = vectorAttribute[top].attribute;
            leftSymbol->width = 4;
            leftSymbol->dimension = 0;
            leftSymbol->num = stoi(tempString);
            if (op_type == 14) {
                leftSymbol->value = "-" + tempString;
                leftSymbol->num *= -1;
            } else {
                leftSymbol->value = tempString;
            }
            if (tempString.find('.') == std::string::npos) {
                //如果没找到'.'说明是整形
                leftSymbol->type = SymbolTableLine::INTEGER;
            } else {//实数型常量
                leftSymbol->type = SymbolTableLine::REAL;
            }
            break;
        }
        case 16: {   //16. Const_variable -> letter
            leftSymbol->type = SymbolTableLine::CHAR;
            leftSymbol->width = 1;
            leftSymbol->dimension = 0;
            leftSymbol->value = vectorAttribute[top].attribute;
            break;
        }
        case 19:
        case 20: {
            id = vectorAttribute[top - 2].attribute;
            int line = vectorAttribute[top].line;
            if (curBlock->blockQuery(id) == nullptr) {
                curBlock->insert(id, 0, 0, 0, 0);
                if (debugInfoLevel >= 3) {
                    cout << "声明" << id << endl;
                    curBlock->printBlock();
                }
            } else {
                recordSemanticError(line, "语义错误！在作用域内有重复定义的标识符");
            }
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
            int elementWidth = Type.width;
            //把Type.arrayInfo链表插入到Periods.arrayInfo链表的尾部
            auto tail = Periods.arrayInfo;
            int baseType = tail->elementType;
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
        case 32: {// Const_declarations -> const Const_declaration ;
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
            if (debugInfoLevel >= 3)
                curBlock->printBlock();
            break;
        }
        case 40: {// Subprogram_head -> function id B Formal_parameter : Standard_type ;
            auto tempPoint = curBlock->query(vectorAttribute[top - 5].attribute);
            tempPoint->type = vectorAttribute[top - 1].type;
            tempPoint->specialType = SymbolTableLine::FUNCTION;
            // 参数的声明
            auto tempVector = vectorAttribute[top - 3].IDlist;
            FuncInfo tempFuncInfo;
            tempFuncInfo.parametersNum = tempVector.size();
            tempFuncInfo.IDlist = tempVector;
            for (int i = 0; i < tempVector.size(); ++i) {
                tempFuncInfo.paraTypeArray.push_back(tempVector[i]->type);
            }
            tempPoint->funcInfo = tempFuncInfo;

            break;
        }
        case 41: {// Subprogram_head -> procedure id C Formal_parameter ;
            auto tempPoint = curBlock->query(vectorAttribute[top - 3].attribute);
            tempPoint->type = -1;
            tempPoint->specialType = SymbolTableLine::PROCEDURE;
            //参数的声明
            auto tempVector = vectorAttribute[top - 1].IDlist;
            FuncInfo tempFuncInfo;
            tempFuncInfo.parametersNum = tempVector.size();
            tempFuncInfo.IDlist = tempVector;
            for (int i = 0; i < tempVector.size(); ++i) {
                tempFuncInfo.paraTypeArray.push_back(tempVector[i]->type);
            }
            tempPoint->funcInfo = tempFuncInfo;
            break;
        }
        case 42: {  //42. Formal_parameter -> ( Parameter_lists )
            leftSymbol->IDlist = vectorAttribute[top-1].IDlist; // 向上传递IDlist
            break;
        }
        case 44: {  //44. Parameter_lists -> Parameter_lists ; Parameter_list
            leftSymbol->IDlist.insert(leftSymbol->IDlist.end(),vectorAttribute[top-2].IDlist.begin(),vectorAttribute[top-2].IDlist.end());
            leftSymbol->IDlist.insert(leftSymbol->IDlist.end(),vectorAttribute[top].IDlist.begin(),vectorAttribute[top].IDlist.end());
            break;
        }
        case 45:    //45. Parameter_lists -> Parameter_list
        case 46:    //46. Parameter_list -> Var_parameter
        case 47:    //47. Parameter_list -> Value_parameter
        case 48: {  //48. Var_parameter -> var Value_parameter
            leftSymbol->IDlist = vectorAttribute[top].IDlist; // 向上传递IDlist
            break;
        }
        case 49: {  //49. Value_parameter -> Identifier_list : Standard_type
            vectorAttributeItem tmp = vectorAttribute[top];
            for (auto &item : vectorAttribute[top - 2].IDlist) {
                item->type = tmp.type;
                item->offset = *offset;
                item->blockPoint = tmp.tableEntry;
                item->arrayInfo = tmp.arrayInfo;
                item->dimension = tmp.dimension;
                *offset += tmp.width;
            }
            if (debugInfoLevel >= 3)
                curBlock->printBlock();
            leftSymbol->IDlist = vectorAttribute[top - 2].IDlist; // 向上传递IDlist
            break;
        }
        case 50:
            relocate();
            break;
        case 53: { // Statement -> Variable assignop Expression
            auto Expression = vectorAttribute[top];
            auto Variable = vectorAttribute[top - 2];
            int type = Variable.type;
            if (type != Expression.type)
                recordSemanticError(Variable.line, "赋值语句类型错误");
            break;
        }
        case 62: { // Variable -> id F62 Id_varparts
            auto Id_varparts = vectorAttribute[top];
//            leftSymbol->entry = Id_varparts.entry;
            int type;
            if (Id_varparts.entry != nullptr)
                type = Id_varparts.entry->type;
            else
                type = SymbolTableLine::VOID;

            // 数组做特殊处理
            if (type == SymbolTableLine::ARRAY) {
                // 拷贝构造
                leftSymbol->entry = new SymbolTableLine(*Id_varparts.entry);
                if (Id_varparts.curArrayInfo == nullptr) {
                    type = Id_varparts.entry->arrayInfo->elementType;
                }
                else {
                    recordSemanticError(Id_varparts.line, "不允许数组直接参加运算");
                }
            }
            leftSymbol->type = type;
            break;
        }
        case 63: { // Id_varparts0 -> Id_varparts1 Id_varpart
            auto Id_varpart = vectorAttribute[top];
            leftSymbol->entry = Id_varpart.entry;
            leftSymbol->curArrayInfo = Id_varpart.curArrayInfo;
            break;
        }
        case 64: { // Id_varparts -> #
            leftSymbol->entry = vectorAttribute[top].entry;
            leftSymbol->curArrayInfo = vectorAttribute[top].curArrayInfo;
            break;
        }
        case 65: {// Id_varpart -> [ Expression_list ]
            auto Expression_list = vectorAttribute[top - 1];
            auto i_entry = vectorAttribute[top - 3].entry;
            auto i_curArrayInfo = vectorAttribute[top - 3].curArrayInfo;
            int lines = vectorAttribute[top].line;
            int type = i_entry->type;
            if (type == SymbolTableLine::ARRAY) {
                int dimension = 0;
                auto ptr = i_curArrayInfo;
                while (ptr != nullptr) {
                    dimension++;
                    ptr = ptr->nextDemision;
                }
                auto expressionTypeList = Expression_list.expressionTypeList;
                if (dimension >= expressionTypeList.size()) {
                    bool isExpressionsTypeInteger = true;
                    for(int expressionType : expressionTypeList) {
                        if (expressionType != SymbolTableLine::INTEGER) {
                            isExpressionsTypeInteger = false;
                            break;
                        }
                        i_curArrayInfo = i_curArrayInfo->nextDemision;
                    }
                    if (isExpressionsTypeInteger) {
                        leftSymbol->entry = i_entry;
                        leftSymbol->curArrayInfo = i_curArrayInfo;
                    }
                    else {
                        leftSymbol->entry = nullptr;
                        recordSemanticError(lines, "运算符[]的参数不是INTEGER");
                    }
                } else {
                    leftSymbol->entry = nullptr;
                    recordSemanticError(lines, "类型错误，超过指定ARRAY的维度");
                }
            } else {
                leftSymbol->entry = nullptr;
                recordSemanticError(lines, "类型错误，不是ARRAY类型，不能使用[]运算符");
            }
            break;
        }
        case 66: { // Id_varpart -> . id
            auto id_local = vectorAttribute[top];
            auto i_entry = vectorAttribute[top - 2].entry;
            int type = i_entry->type;
            if (type == SymbolTableLine::RECORD) {
                auto recordBlock = i_entry->blockPoint;
                auto entry = recordBlock->blockQuery(id_local.attribute);
                if (entry != nullptr) {
                    leftSymbol->entry = entry;
                } else {
                    leftSymbol->entry = nullptr;
                    stringstream o;
                    o << "语义错误，record " << i_entry->name << "中没有" << id_local.attribute << "域";
                    recordSemanticError(id_local.line, o.str());
                }
            } else {
                leftSymbol->entry = nullptr;
                recordSemanticError(id_local.line, "类型错误，不是RECORD类型，不能使用.运算符");
            }
            break;
        }
        case 78:    // Call_procedure_statement -> id
        case 79: {  // Call_procedure_statement -> id ( Expression_list )
            id = vectorAttribute[top].attribute;
            int line = vectorAttribute[top].line;
            quoteID(line, id);  //查看是否存在这个id
            SymbolTableLine *tempLinePoint = curBlock->query(id);
            if (tempLinePoint != nullptr) {//如果存在
                if (tempLinePoint->specialType == SymbolTableLine::PROCEDURE) {
                    leftSymbol->type = tempLinePoint->type;
                } else {
                    recordSemanticError(line, "语义错误！此id不为Procesure");
                }
            }
            break;
        }
        case 103: {
            id = vectorAttribute[top - 3].attribute;
            int line = vectorAttribute[top - 3].line;
            quoteID(line, id);
            break;
        }
        case 80: { // Expression_list0 -> Expression_list1 , Expression
            auto Expression_list1 = vectorAttribute[top - 2];
            auto Expression = vectorAttribute[top];
            leftSymbol->expressionTypeList = Expression_list1.expressionTypeList;
            leftSymbol->expressionTypeList.push_back(Expression.type);
            break;
        }
        case 81: { // Expression_list -> Expression
            auto Expression = vectorAttribute[top];
            leftSymbol->expressionTypeList.push_back(Expression.type);
            break;
        }
        case 82: { // Expression -> Simple_expression0 = Simple_expression1
            auto Simple_expression0 = vectorAttribute[top - 2];
            auto Simple_expression1 = vectorAttribute[top];
            int resultType = SymbolTableLine::VOID;
            string value;
            if (Simple_expression0.type == Simple_expression1.type) {
                bool flag = isBasicType(Simple_expression0.type) &&
                            isBasicType(Simple_expression1.type);
                if (flag) {
                    resultType = SymbolTableLine::BOOLEAN;
                    if (!Simple_expression0.value.empty() && !Simple_expression1.value.empty()) {
                        if (Simple_expression0.value == Simple_expression1.value)
                            value = "1";
                        else
                            value = "0";
                    }
                } else {
                    recordSemanticError(Simple_expression0.line, "逻辑运算符=：操作数类型违法");
                }
            } else {
                recordSemanticError(Simple_expression0.line, "逻辑运算符=：左右操作数类型不一致");
            }
            leftSymbol->type = resultType;
            leftSymbol->value = value;
            break;
        }
        case 83: { // Expression -> Simple_expression0 <> Simple_expression1
            auto Simple_expression0 = vectorAttribute[top - 2];
            auto Simple_expression1 = vectorAttribute[top];
            int resultType = SymbolTableLine::VOID;
            string value;
            if (Simple_expression0.type == Simple_expression1.type) {
                bool flag = isBasicType(Simple_expression0.type) &&
                            isBasicType(Simple_expression1.type);
                if (flag) {
                    resultType = SymbolTableLine::BOOLEAN;
                    if (!Simple_expression0.value.empty() && !Simple_expression1.value.empty()) {
                        if (Simple_expression0.value == Simple_expression1.value)
                            value = "0"; // false
                        else
                            value = "1"; // true
                    }
                } else {
                    recordSemanticError(Simple_expression0.line, "逻辑运算符<>：操作数类型违法");
                }
            } else {
                recordSemanticError(Simple_expression0.line, "逻辑运算符<>：左右操作数类型不一致");
            }
            leftSymbol->type = resultType;
            leftSymbol->value = value;
            break;
        }
        case 84: { // Expression -> Simple_expression0 < Simple_expression1
            auto Simple_expression0 = vectorAttribute[top - 2];
            auto Simple_expression1 = vectorAttribute[top];
            int resultType = SymbolTableLine::VOID;
            string value;
            if (Simple_expression0.type == Simple_expression1.type) {
                if (Simple_expression0.type == SymbolTableLine::BOOLEAN ||
                    Simple_expression0.type == SymbolTableLine::INTEGER) {
                    resultType = SymbolTableLine::BOOLEAN;
                    if (Simple_expression0.value.empty() && Simple_expression1.value.empty())
                        value = "";
                    else {
                        int a = stoi(Simple_expression0.value);
                        int b = stoi(Simple_expression1.value);
                        if (a < b)
                            value = "1";
                        else
                            value = "0";
                    }
                } else if (Simple_expression0.type == SymbolTableLine::REAL) {
                    resultType = SymbolTableLine::BOOLEAN;
                    if (Simple_expression0.value.empty() && Simple_expression1.value.empty())
                        value = "";
                    else {
                        float a = stof(Simple_expression0.value);
                        float b = stof(Simple_expression1.value);
                        if (a < b)
                            value = "1";
                        else
                            value = "0";
                    }
                } else if (Simple_expression0.type == SymbolTableLine::CHAR) {
                    resultType = SymbolTableLine::BOOLEAN;
                    if (Simple_expression0.value.empty() && Simple_expression1.value.empty())
                        value = "";
                    else {
                        char a = Simple_expression0.value[0];
                        char b = Simple_expression1.value[0];
                        if (a < b)
                            value = "1";
                        else
                            value = "0";
                    }
                } else {
                    recordSemanticError(Simple_expression0.line, "逻辑运算符<：操作数类型违法");
                }
            } else {
                recordSemanticError(Simple_expression0.line, "逻辑运算符<：左右操作数类型不一致");
            }
            leftSymbol->type = resultType;
            leftSymbol->value = value;
            break;
        }
        case 85: { // Expression -> Simple_expression0 <= Simple_expression1
            auto Simple_expression0 = vectorAttribute[top - 2];
            auto Simple_expression1 = vectorAttribute[top];
            int resultType = SymbolTableLine::VOID;
            string value;
            if (Simple_expression0.type == Simple_expression1.type) {
                if (!Simple_expression0.value.empty() && !Simple_expression1.value.empty()) {
                    if (Simple_expression0.type == SymbolTableLine::BOOLEAN ||
                        Simple_expression0.type == SymbolTableLine::INTEGER) {
                        resultType = SymbolTableLine::BOOLEAN;
                        int a = stoi(Simple_expression0.value);
                        int b = stoi(Simple_expression1.value);
                        if (a <= b)
                            value = "1";
                        else
                            value = "0";
                    } else if (Simple_expression0.type == SymbolTableLine::REAL) {
                        resultType = SymbolTableLine::BOOLEAN;
                        float a = stof(Simple_expression0.value);
                        float b = stof(Simple_expression1.value);
                        if (a <= b)
                            value = "1";
                        else
                            value = "0";
                    } else if (Simple_expression0.type == SymbolTableLine::CHAR) {
                        resultType = SymbolTableLine::BOOLEAN;
                        char a = Simple_expression0.value[0];
                        char b = Simple_expression1.value[0];
                        if (a < b)
                            value = "1";
                        else
                            value = "0";
                    }
                } else {
                    recordSemanticError(Simple_expression0.line, "逻辑运算符<=：操作数类型违法");
                }
            } else {
                recordSemanticError(Simple_expression0.line, "逻辑运算符<=：左右操作数类型不一致");
            }
            leftSymbol->type = resultType;
            leftSymbol->value = value;
            break;
        }
        case 86: { // Expression -> Simple_expression0 > Simple_expression1
            auto Simple_expression0 = vectorAttribute[top - 2];
            auto Simple_expression1 = vectorAttribute[top];
            int resultType = SymbolTableLine::VOID;
            string value;
            if (Simple_expression0.type == Simple_expression1.type) {
                if (!Simple_expression0.value.empty() && !Simple_expression1.value.empty()) {
                    if (Simple_expression0.type == SymbolTableLine::BOOLEAN ||
                        Simple_expression0.type == SymbolTableLine::INTEGER) {
                        resultType = SymbolTableLine::BOOLEAN;
                        int a = stoi(Simple_expression0.value);
                        int b = stoi(Simple_expression1.value);
                        if (a > b)
                            value = "1";
                        else
                            value = "0";
                    } else if (Simple_expression0.type == SymbolTableLine::REAL) {
                        resultType = SymbolTableLine::BOOLEAN;
                        float a = stof(Simple_expression0.value);
                        float b = stof(Simple_expression1.value);
                        if (a > b)
                            value = "1";
                        else
                            value = "0";
                    } else if (Simple_expression0.type == SymbolTableLine::CHAR) {
                        resultType = SymbolTableLine::BOOLEAN;
                        char a = Simple_expression0.value[0];
                        char b = Simple_expression1.value[0];
                        if (a > b)
                            value = "1";
                        else
                            value = "0";
                    } else {
                        recordSemanticError(Simple_expression0.line, "逻辑运算符>：操作数类型违法");
                    }
                }
            } else {
                recordSemanticError(Simple_expression0.line, "逻辑运算符>：左右操作数类型不一致");
            }
            leftSymbol->type = resultType;
            leftSymbol->value = value;
            break;
        }
        case 87: { // Expression -> Simple_expression0 >= Simple_expression1
            auto Simple_expression0 = vectorAttribute[top - 2];
            auto Simple_expression1 = vectorAttribute[top];
            int resultType = SymbolTableLine::VOID;
            string value;
            if (Simple_expression0.type == Simple_expression1.type) {
                if (!Simple_expression0.value.empty() && !Simple_expression1.value.empty()) {
                    if (Simple_expression0.type == SymbolTableLine::BOOLEAN ||
                        Simple_expression0.type == SymbolTableLine::INTEGER) {
                        resultType = SymbolTableLine::BOOLEAN;
                        int a = stoi(Simple_expression0.value);
                        int b = stoi(Simple_expression1.value);
                        if (a >= b)
                            value = "1";
                        else
                            value = "0";
                    } else if (Simple_expression0.type == SymbolTableLine::REAL) {
                        resultType = SymbolTableLine::BOOLEAN;
                        float a = stof(Simple_expression0.value);
                        float b = stof(Simple_expression1.value);
                        if (a >= b)
                            value = "1";
                        else
                            value = "0";
                    } else if (Simple_expression0.type == SymbolTableLine::CHAR) {
                        resultType = SymbolTableLine::BOOLEAN;
                        char a = Simple_expression0.value[0];
                        char b = Simple_expression1.value[0];
                        if (a >= b)
                            value = "1";
                        else
                            value = "0";
                    } else {
                        recordSemanticError(Simple_expression0.line, "逻辑运算符>=：操作数类型违法");
                    }
                }
            } else {
                recordSemanticError(Simple_expression0.line, "逻辑运算符>=：左右操作数类型不一致");
            }
            leftSymbol->type = resultType;
            leftSymbol->value = value;
            break;
        }
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
        case 90: { // Simple_expression -> + Term
            auto Term = vectorAttribute[top];
            leftSymbol->type = Term.type;
            string value;
            if (Term.type == SymbolTableLine::INTEGER ||
                Term.type == SymbolTableLine::REAL) {
                if (!Term.value.empty()) {
                    float a = stof(Term.value);
                    value = to_string(a);
                }
            }
            leftSymbol->value = Term.value;
            break;
        }
        case 91: { // Simple_expression -> - Term
            auto Term = vectorAttribute[top];
            leftSymbol->type = Term.type;
            string value;
            if (Term.type == SymbolTableLine::INTEGER ||
                Term.type == SymbolTableLine::REAL) {
                if (!Term.value.empty()) {
                    float a = stof(Term.value);
                    value = to_string(0 - a);
                }
            }
            leftSymbol->value = Term.value;
            break;
        }
        case 92: { // Simple_expression0 -> Simple_expression1 + Term
            auto Simple_expression1 = vectorAttribute[top - 2];
            auto Term = vectorAttribute[top];
            int resultType = SymbolTableLine::VOID;
            string value;
            int flag;
            if (Simple_expression1.type == SymbolTableLine::INTEGER) {
                if (Term.type == SymbolTableLine::INTEGER)
                    flag = 1;
                else if (Term.type == SymbolTableLine::REAL)
                    flag = 2;
                else
                    flag = 0;
            } else if (Simple_expression1.type == SymbolTableLine::REAL) {
                if (Term.type == SymbolTableLine::INTEGER)
                    flag = 3;
                else if (Term.type == SymbolTableLine::REAL)
                    flag = 4;
                else
                    flag = 0;
            } else
                flag = 0;

            switch (flag) {
                case 1: { // 都是INTEGER
                    resultType = SymbolTableLine::INTEGER;
                    if (!Simple_expression1.value.empty() && !Term.value.empty()) {
                        int a = stoi(Simple_expression1.value);
                        int b = stoi(Term.value);
                        value = to_string(a + b);
                    }
                    break;
                }
                case 2:
                case 3:
                case 4: {
                    resultType = SymbolTableLine::REAL;
                    if (!Simple_expression1.value.empty() && !Term.value.empty()) {
                        float a = stof(Simple_expression1.value);
                        float b = stof(Term.value);
                        value = to_string(a + b);
                    }
                    break;
                }
                default: {
                    recordSemanticError(Simple_expression1.line, "运算符+：运算对象类型错误");
                    break;
                }
            }
            leftSymbol->type = resultType;
            leftSymbol->value = value;
            break;
        }
        case 93: { // Simple_expression0 -> Simple_expression1 - Term
            auto Simple_expression1 = vectorAttribute[top - 2];
            auto Term = vectorAttribute[top];
            int resultType = SymbolTableLine::VOID;
            string value;
            int flag;
            if (Simple_expression1.type == SymbolTableLine::INTEGER) {
                if (Term.type == SymbolTableLine::INTEGER)
                    flag = 1;
                else if (Term.type == SymbolTableLine::REAL)
                    flag = 2;
                else
                    flag = 0;
            } else if (Simple_expression1.type == SymbolTableLine::REAL) {
                if (Term.type == SymbolTableLine::INTEGER)
                    flag = 3;
                else if (Term.type == SymbolTableLine::REAL)
                    flag = 4;
                else
                    flag = 0;
            } else
                flag = 0;

            switch (flag) {
                case 1: { // 都是INTEGER
                    resultType = SymbolTableLine::INTEGER;
                    if (!Simple_expression1.value.empty() && !Term.value.empty()) {
                        int a = stoi(Simple_expression1.value);
                        int b = stoi(Term.value);
                        value = to_string(a - b);
                    }
                    break;
                }
                case 2:
                case 3:
                case 4: {
                    resultType = SymbolTableLine::REAL;
                    if (!Simple_expression1.value.empty() && !Term.value.empty()) {
                        float a = stof(Simple_expression1.value);
                        float b = stof(Term.value);
                        value = to_string(a - b);
                    }
                    break;
                }
                default: {
                    recordSemanticError(Simple_expression1.line, "运算符-：运算对象类型错误");
                    break;
                }
            }
            leftSymbol->type = resultType;
            leftSymbol->value = value;
            break;
        }
        case 94: { // Simple_expression0 -> Simple_expression1 or Term
            auto Simple_expression1 = vectorAttribute[top - 2];
            auto Term = vectorAttribute[top];
            int resultType = SymbolTableLine::VOID;
            string value;
            if (Simple_expression1.type == SymbolTableLine::BOOLEAN &&
                Term.type == SymbolTableLine::BOOLEAN) {
                resultType = SymbolTableLine::BOOLEAN;
                if (!Simple_expression1.value.empty() && !Term.value.empty()) {
                    int a = stoi(Simple_expression1.value);
                    int b = stoi(Term.value);
                    value = to_string(a || b);
                }
            } else {
                recordSemanticError(Simple_expression1.line, "运算符or：运算对象类型错误");
            }
            leftSymbol->type = resultType;
            leftSymbol->value = value;
            break;
        }
        case 95: { // Term0 -> Term1 * Factor
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
                    if (!Term1.value.empty() && !Factor.value.empty()) {
                        int a = stoi(Term1.value);
                        int b = stoi(Factor.value);
                        value = to_string(a * b);
                    }
                    break;
                }
                case 2:
                case 3:
                case 4: {
                    resultType = SymbolTableLine::REAL;
                    if (!Term1.value.empty() && !Factor.value.empty()) {
                        float a = stof(Term1.value);
                        float b = stof(Factor.value);
                        value = to_string(a * b);
                    }
                    break;
                }
                default: {
                    recordSemanticError(Term1.line, "运算符*：运算对象类型错误");
                    break;
                }
            }
            leftSymbol->type = resultType;
            leftSymbol->value = value;
            break;
        }
        case 96: { // Term0 -> Term1 / Factor
            auto Term1 = vectorAttribute[top - 2];
            auto Factor = vectorAttribute[top];
            int resultType = SymbolTableLine::VOID;
            string value;
            int flag;
            if ((Term1.type == SymbolTableLine::INTEGER || Term1.type == SymbolTableLine::REAL) &&
                (Factor.type == SymbolTableLine::INTEGER || Factor.type == SymbolTableLine::REAL)) {
                if (!Term1.value.empty() && !Factor.value.empty()) {
                    float a = stof(Term1.value);
                    float b = stof(Factor.value);
                    if (b != 0) {
                        resultType = SymbolTableLine::REAL;
                        value = to_string(a / b);
                    } else {
                        recordSemanticError(Term1.line, "除0错误");
                    }
                } else
                    resultType = SymbolTableLine::REAL;
            } else {
                recordSemanticError(Term1.line, "运算符/：运算对象类型错误");
            }
            leftSymbol->type = resultType;
            leftSymbol->value = value;
            break;
        }
        case 97: { // Term0 -> Term1 div Factor
            auto Term1 = vectorAttribute[top - 2];
            auto Factor = vectorAttribute[top];
            int resultType = SymbolTableLine::VOID;
            string value;
            if (Term1.type == SymbolTableLine::INTEGER &&
                Factor.type == SymbolTableLine::INTEGER) {
                if (!Term1.value.empty() && !Factor.value.empty()) {
                    int a = stoi(Term1.value);
                    int b = stoi(Factor.value);
                    if (b != 0) {
                        resultType = SymbolTableLine::INTEGER;
                        value = to_string(a / b);
                    } else {
                        recordSemanticError(Term1.line, "除0错误");
                    }
                } else
                    resultType = SymbolTableLine::REAL;
            } else {
                recordSemanticError(Term1.line, "运算符div：运算对象类型错误");
            }
            leftSymbol->type = resultType;
            leftSymbol->value = value;
            break;
        }
        case 98: { // Term0 -> Term1 mod Factor
            auto Term1 = vectorAttribute[top - 2];
            auto Factor = vectorAttribute[top];
            int resultType = SymbolTableLine::VOID;
            string value;
            if (Term1.type == SymbolTableLine::INTEGER &&
                Factor.type == SymbolTableLine::INTEGER) {
                if (!Term1.value.empty() && !Factor.value.empty()) {
                    int a = stoi(Term1.value);
                    int b = stoi(Factor.value);
                    if (b != 0) {
                        resultType = SymbolTableLine::INTEGER;
                        value = to_string(a % b);
                    } else {
                        recordSemanticError(Term1.line, "除0错误");
                    }
                } else
                    resultType = SymbolTableLine::REAL;
            } else {
                recordSemanticError(Term1.line, "运算符mod：运算对象类型错误");
            }
            leftSymbol->type = resultType;
            leftSymbol->value = value;
            break;
        }
        case 99: { // Term0 -> Term1 and Factor
            auto Term1 = vectorAttribute[top - 2];
            auto Factor = vectorAttribute[top];
            int resultType = SymbolTableLine::VOID;
            string value;
            if (Term1.type == SymbolTableLine::BOOLEAN &&
                Factor.type == SymbolTableLine::BOOLEAN) {
                resultType = SymbolTableLine::BOOLEAN;
                if (!Term1.value.empty() && !Factor.value.empty()) {
                    int a = stoi(Term1.value);
                    int b = stoi(Factor.value);
                    value = to_string(a && b);
                }
            } else {
                recordSemanticError(Term1.line, "运算符and：运算对象类型错误");
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
        case 102: { // Factor -> Variable
            auto Variable = vectorAttribute[top];
            auto entry = Variable.entry;
            int type;
            string value;
            if (entry != nullptr) {
                type = entry->type;
            } else {
                type = SymbolTableLine::VOID;
            }
            leftSymbol->type = type;
            leftSymbol->value = value;
            break;
        }
        case 104: { // Factor -> ( Expression )
            auto Expression = vectorAttribute[top - 1];
            leftSymbol->type = Expression.type;
            leftSymbol->value = Expression.value;
            break;
        }
        case 105: { // Factor0 -> not Factor1
            auto Factor1 = vectorAttribute[top];
            int resultType = SymbolTableLine::VOID;
            string value;
            if (Factor1.type == SymbolTableLine::BOOLEAN) {
                resultType = SymbolTableLine::BOOLEAN;
                if (!Factor1.value.empty()) {
                    if (Factor1.value == "1")
                        value = "0";
                    else
                        value = "1";
                }
            } else {
                recordSemanticError(Factor1.line, "逻辑运算符not：操作数类型错误");
            }
            leftSymbol->type = resultType;
            leftSymbol->value = value;
            break;
        }
        case 107: { // Unsign_const_variable -> num
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
        case 108: { // Unsign_const_variable -> letter
            auto letter = vectorAttribute[top];
            leftSymbol->type = SymbolTableLine::CHAR;
            leftSymbol->value = letter.attribute;
            break;
        }
        case 109:
        case 110:
        case 111: {
            id = vectorAttribute[top].attribute;
            int line = vectorAttribute[top].line;
            if (curBlock->blockQuery(id) == nullptr) {
                curBlock->insert(id, 0, 0, 0, 0);
                if (debugInfoLevel >= 3) {
                    cout << "声明" << id << endl;
                    curBlock->printBlock();
                }
            } else {
                recordSemanticError(line, "语义错误！在作用域内有重复定义的标识符");
            }
            //块内查询 id 是否存在,存在则返回所在行指针
            auto tableLine = curBlock->blockQuery(id);

            locate(); //创建子块,同时完善子块的父亲指针
            //curblock 这是新创建出来的表的指针,下面语句将原先这一行中的blockPoint指针指向新创建出来的表的指针
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
                t.entry = nullptr;
                leftSymbol->entry = nullptr;
                recordSemanticError(t.line, "未定义的引用");
                break;
            } else {
                t.entry = entry;
                leftSymbol->entry = entry;
            }
            break;
        }
        default:
            break;
    }
}

void LR1Runner::quoteID(int line, string id) {
    if (curBlock->query(id) == nullptr) {
        string t;
        t = "语义错误！引用了未定义的标识符";
        t.append(id);
        recordSemanticError(line, t);
    } else {
        if (debugInfoLevel >= 3) {
            cout << "找到引用" << id << endl;
            curBlock->printBlock();
        }
    }
}

void LR1Runner::locate() {
    SymbolBlock *childBlock;
    childBlock = SymbolBlock::makeBlock(curBlock);//父亲指针
    curBlock = childBlock;
    offset = new int(0);

    tablePointers.push(curBlock);
    offSetStack.push(offset);
    if (debugInfoLevel >= 3)
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
    if (debugInfoLevel >= 3)
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
        cout << "源代码中没有语义错误\n";
        return;
    }
    cout << "=======================================================================================================";
    cout << "\n";
    cout << "ERROR!! 源代码中有语义错误\n";
    for (const auto &s : semanticError) {
        cout << s << endl;
    }
}

void LR1Runner::recordSemanticError(int line, const string &e) {
    string tmp;
    char s[50];
    snprintf(s, 50, "第%d行\t", line);
    tmp.append(s);
    tmp.append(e);
    semanticError.push_back(tmp);
}

bool LR1Runner::isBasicType(int type) {
    return type == SymbolTableLine::BOOLEAN ||
           type == SymbolTableLine::INTEGER ||
           type == SymbolTableLine::REAL ||
           type == SymbolTableLine::CHAR;
}

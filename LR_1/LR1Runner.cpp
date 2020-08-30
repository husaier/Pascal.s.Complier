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

vectorAttributeItem::~vectorAttributeItem() {

}

void LR1Runner::run(LR1Table &table) {
    initial();
    if (debugInfoLevel >= 2) {
        cout << "-------------------------------------------------------------------------------------------Runner::run"
             << "------------------------------------------------------------------------------------------------------"
             << endl;
    }

    stackState.push(0);
    stackSymbol.push(" ");
    vectorAttribute.emplace_back(vectorAttributeItem(" ", -1, -1));    ///line
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
            vectorAttribute.emplace_back(vectorInput.at(ip).attribute, -1, vectorInput.at(ip).line);    ///line
            stackState.push(table.table[s][tempCol].index);  //根据表格中S后的数字，压入状态
            ip++;
            if (debugInfoLevel >= 2) {
                cout << "S" + to_string(table.table[s][tempCol].index) << endl;//输出动作
            }
        } else if (currentType == TableItem::REDUCTION) {
            int tempProd = table.table[s][tempCol].index;  //取出要按哪个产生式R
            Production tempProduction = table.productions.at(tempProd);
            vectorAttributeItem tempAttributeItem(tempProduction.left, -1, vectorInput.at(ip).line);    ///line
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
        if (!semanticError.empty())
            break;
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
//    string id;
    switch (op_type) {
        case 1: {   //Program -> Program_head Program_body .
            auto Program_head = vectorAttribute[top - 2];
            auto Program_body = vectorAttribute[top - 1];
            Program_head.tableLineEntry->startQuad = Program_body.startQuad;
            break;
        }
        case 2: {   //Program_head -> program id A ( Identifier_list ) ;
            auto M_id = &vectorAttribute[top - 5];
            M_id->tableLineEntry = curBlock->query(M_id->attribute);
            leftSymbol->tableLineEntry = M_id->tableLineEntry;
            leftSymbol->tableLineEntry->type = new Type(Type::PROGRAM);
            leftSymbol->tableLineEntry->startQuad = midCode.codeList.size();
            break;
        }
        case 3: {   //3. Program_body -> Const_declarations Type_declarations Var_declarations Subprogram_declarations  Statement_body
            auto Statement_body = vectorAttribute[top];
            leftSymbol->startQuad = Statement_body.startQuad;
            break;
        }
        case 4: {   // Identifier_list -> Identifier_list , id
            string id = vectorAttribute[top].attribute;
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
            string id = vectorAttribute[top].attribute;
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
            string id = vectorAttribute[top - 2].attribute;
            auto Const_variable = vectorAttribute[top];
            int line = Const_variable.line;
            if (curBlock->blockQuery(id) == nullptr) {
                SymbolTableLine *tempPoint = curBlock->insert2(id, Const_variable.type, 0, 0, 0);
                tempPoint->isConst = true;//将常量标志位置为1
                tempPoint->value = Const_variable.value;
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
        case 12: {  //12. Const_variable -> id
            string id = vectorAttribute[top].attribute;
            int line = vectorAttribute[top].line;
            Type *type{nullptr};
            quoteID(line, id);  //查看是否存在这个id
            SymbolTableLine *tempLinePoint = curBlock->query(id);
            if (tempLinePoint != nullptr) {//如果存在
                if (tempLinePoint->isConst) {
                    type = tempLinePoint->type;
                    if (op_type == 10 || op_type == 12) {
                        leftSymbol->value = tempLinePoint->value;
                    } else {
                        leftSymbol->value = "-" + tempLinePoint->value;
                    }
                } else {
                    type = new Type(Type::TYPE_ERROR);
                    recordSemanticError(line, "语义错误！此id不为常量");
                }
            } else {
                type = new Type(Type::TYPE_ERROR);
                recordSemanticError(line, "语义错误！id不存在");
            }
            leftSymbol->type = type;
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
                leftSymbol->type = new Type(Type::INTEGER);
            } else {//实数型常量
                leftSymbol->type = new Type(Type::REAL);
            }
            break;
        }
        case 16: {   //16. Const_variable -> letter
            leftSymbol->type = new Type(Type::CHAR);
            leftSymbol->width = 1;
            leftSymbol->dimension = 0;
            leftSymbol->value = vectorAttribute[top].attribute;
            break;
        }
        case 19: // Type_declaration -> Type_declaration ; id = Type
        case 20: { // Type_declaration -> id = Type
            string id = vectorAttribute[top - 2].attribute;
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
        case 21: // Type -> Standard_type
            leftSymbol->type = vectorAttribute[top].type;
            leftSymbol->width = vectorAttribute[top].width;
            break;
        case 22: { // Type -> record D22 Record_body end
            auto Record_body = vectorAttribute[top - 1];
            leftSymbol->tableEntry = curBlock;
            leftSymbol->width = *offset;
            leftSymbol->dimension = Record_body.dimension + 1;
            auto record = new Record();
            for (auto symbol : curBlock->symbolBlock) {
                EnvItem e;
                e.id = symbol->name;
                e.type = symbol->type->copy();
                record->env.push_back(e);
            }
            leftSymbol->type = (Type *) record;
//            int t = record->getWidth();
//            int tmp = record->getSize();
            relocate();
            break;
        }
        case 23: { // Type0 -> array [ Periods ] of Type
            auto Type = vectorAttribute[top];
            auto Periods = vectorAttribute[top - 3];
            auto *type = Periods.type;
            type->addElem(Type.type);
//            int t = type->getWidth();
//            int tmp = type->getSize();
            leftSymbol->type = type;
            leftSymbol->width = type->getWidth();
            leftSymbol->tableEntry = Type.tableEntry;
            leftSymbol->dimension = Periods.dimension + Type.dimension;
            break;
        }
        case 24: // Standard_type -> integer
            leftSymbol->type = new Type(Type::INTEGER);
            leftSymbol->width = 4;
            leftSymbol->dimension = 0;
            break;
        case 25: //25. Standard_type -> real
            leftSymbol->type = new Type(Type::REAL);
            leftSymbol->width = 4;
            leftSymbol->dimension = 0;
            break;
        case 26: //26. Standard_type -> boolean
            leftSymbol->type = new Type(Type::BOOLEAN);
            leftSymbol->width = 1;
            leftSymbol->dimension = 0;
            break;
        case 27: //27. Standard_type -> char
            leftSymbol->type = new Type(Type::CHAR);
            leftSymbol->width = 1;
            leftSymbol->dimension = 0;
            break;
        case 28: { //28. Record_body -> Var_declaration
            auto Var_declaration = vectorAttribute[top];
            leftSymbol->dimension = Var_declaration.dimension;
            break;
        }
        case 29: { //29. Record_body -> #
            break;
        }
        case 30: { //30. Periods0 -> Periods1 , Period
            auto period = &vectorAttribute[top];
            auto periods1 = &vectorAttribute[top - 2];
            auto &periods0 = leftSymbol;
            periods0->dimension = periods1->dimension + 1;

            auto t = period->type;
            periods1->type->addElem(t);
            periods0->type = periods1->type;
            break;
        }
        case 31: { //31. Periods -> Period
            auto Period = vectorAttribute[top];
            leftSymbol->dimension = 1;
            leftSymbol->type = Period.type;
            break;
        }
        case 32: { // 32. Period -> Const_variable .. Const_variable
            int start = vectorAttribute[top - 2].num;
            int end = vectorAttribute[top].num;
            auto *tmp = new Array(start, end);
            leftSymbol->type = tmp;
            break;
        }
        case 33: {  //
            break;
        }
        case 35: //35. Var_declaration -> Var_declaration ; Identifier_list : Type
        case 36: { //36. Var_declaration -> Identifier_list : Type
            vectorAttributeItem &Type = vectorAttribute[top];
            for (auto &item : vectorAttribute[top - 2].IDlist) {
                item->type = Type.type;
                item->offset = *offset;
                item->blockPoint = Type.tableEntry;
                item->dimension = Type.dimension;
                *offset += Type.width;
            }
            leftSymbol->dimension = Type.dimension;
            if (debugInfoLevel >= 3)
                curBlock->printBlock();
            break;
        }
        case 39: {  //Subprogram_declaration -> Subprogram_head Program_body
            auto Program_body = vectorAttribute[top];
            auto Subprogram_head = vectorAttribute[top - 1];
            leftSymbol->tableLineEntry = Subprogram_head.tableLineEntry;
            leftSymbol->tableLineEntry->startQuad = Program_body.startQuad;
            break;
        }
        case 40: {// Subprogram_head -> function id B40 Formal_parameter : Standard_type ;
            string id = vectorAttribute[top - 5].attribute;
            auto tempPoint = curBlock->query(id);
            auto Formal_parameter = vectorAttribute[top - 3];
            auto Standard_type = vectorAttribute[top - 1];
            auto func = new Func();
            func->parametersNum = Formal_parameter.IDlist.size();
            for (const auto &symbol: Formal_parameter.IDlist) {
                EnvItem e;
                e.id = symbol->name;
                e.type = symbol->type->copy();
                func->env.push_back(e);
            }
            func->reType = Standard_type.type;
            tempPoint->type = func;
            leftSymbol->tableLineEntry = tempPoint;
            break;
        }
        case 41: {// Subprogram_head -> procedure id C41 Formal_parameter ;
            string id = vectorAttribute[top - 3].attribute;
            auto tempPoint = curBlock->query(id);
            auto Formal_parameter = vectorAttribute[top - 1];
            auto proc = new Proc();
            proc->parametersNum = Formal_parameter.IDlist.size();
            for (const auto &symbol: Formal_parameter.IDlist) {
                EnvItem e;
                e.id = symbol->name;
                e.type = symbol->type->copy();
                proc->env.push_back(e);
            }
            tempPoint->type = proc;
            leftSymbol->tableLineEntry = tempPoint;
            break;
        }
        case 42: {  //42. Formal_parameter -> ( Parameter_lists )
            leftSymbol->IDlist = vectorAttribute[top - 1].IDlist; // 向上传递IDlist
            break;
        }
        case 44: {  //44. Parameter_lists -> Parameter_lists ; Parameter_list
            leftSymbol->IDlist.clear();
            leftSymbol->IDlist.insert(leftSymbol->IDlist.end(), vectorAttribute[top - 2].IDlist.begin(),
                                      vectorAttribute[top - 2].IDlist.end());
            leftSymbol->IDlist.insert(leftSymbol->IDlist.end(), vectorAttribute[top].IDlist.begin(),
                                      vectorAttribute[top].IDlist.end());
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
                item->dimension = tmp.dimension;
                *offset += tmp.width;
            }
            if (debugInfoLevel >= 3)
                curBlock->printBlock();
            leftSymbol->IDlist = vectorAttribute[top - 2].IDlist; // 向上传递IDlist
            break;
        }
        case 50: {  //50. Compound_statement -> begin Statement_list end
            auto Statement_list = vectorAttribute[top - 1];
            leftSymbol->startQuad = Statement_list.startQuad;
            break;
        }
        case 51: {  //51. Statement_list -> Statement_list1 ; M Statement
            auto Statement_list1 = &vectorAttribute[top - 3];
            auto M = vectorAttribute[top - 1];
            auto Statement = vectorAttribute[top];
            leftSymbol->startQuad = Statement_list1->startQuad;

            midCode.backPatch(Statement_list1->nextList, M.quad);
            leftSymbol->nextList = Statement.nextList;
            break;
        }
        case 52: {  //52. Statement_list -> Statement
            auto Statement = vectorAttribute[top];
            leftSymbol->startQuad = Statement.startQuad;
            leftSymbol->nextList = Statement.nextList;
            break;
        }
        case 53: { // Statement -> Variable assignop Expression
            vectorAttributeItem *Expression = &vectorAttribute[top];
            vectorAttributeItem *Variable = &vectorAttribute[top - 2];
            leftSymbol->startQuad = midCode.codeList.size();
            if (*(Variable->type) != *(Expression->type))
                recordSemanticError(Variable->line, "错误：赋值语句类型不匹配");
            //中间代码生成,赋值语句
            string arg1, arg2, res;
            if (Expression->entry) {
                arg1 = Expression->entry->id;
            } else {
                TempVar *entry{nullptr};
                entry = midCode.newTemp();
                entry->value = Expression->variableName;
                entry->type = Expression->type;
                entry->tableLineEntry = Expression->tableLineEntry;
                Expression->entry = entry;
                arg1 = Expression->entry->id;
//                arg1 = Expression.tableLineEntry->name;
//                printf("type%d\n",Expression.tableLineEntry->type->getType());
            }

            res = Variable->entry->id;
////            res = Variable.tableLineEntry->name;

            if (Variable->entry->backOffset != nullptr) {
                res = Variable->entry->backOffset->id;
                arg2 = Variable->entry->backOffset->offset->id;
                midCode.outCode(QuaternionItem::OFFSETASSIGN, arg1, arg2, res);
            } else {
                midCode.outCode(QuaternionItem::ASSIGN, arg1, arg2, res);
            }


            leftSymbol->nextList.clear();
            break;
        }
        case 54: {  //54. Statement -> Call_procedure_statement
            leftSymbol->startQuad = midCode.codeList.size();


//            leftSymbol->nextList.push_back(midCode.codeList.size());//不确定是否这里写
            break;
        }
        case 55: {  //55. Statement -> Compound_statement
            auto Compound_statement = vectorAttribute[top];
            leftSymbol->startQuad = Compound_statement.startQuad;

            leftSymbol->nextList = Compound_statement.nextList;//放最后
            break;
        }
        case 56: { // 56. Statement -> if Expression F56 then M Statement1 Else_part
            vectorAttributeItem *Expression = &vectorAttribute[top - 5];
            vectorAttributeItem *Else_part = &vectorAttribute[top];
            vectorAttributeItem *M = &vectorAttribute[top - 2];
            vectorAttributeItem *Statement1 = &vectorAttribute[top - 1];
//            printf("%p,%lu\n", &Expression, Expression.trueList.size());
            leftSymbol->startQuad = midCode.codeList.size();
            if (Expression->type->getType() != Type::BOOLEAN)
                recordSemanticError(Expression->line, "错误，if语句的判断表达式不是BOOLEAN类型");

            if (Else_part->nextList.empty()) {
                midCode.backPatch(Expression->trueList, M->quad);
                leftSymbol->nextList.clear();
                leftSymbol->nextList.insert(leftSymbol->nextList.end(), Expression->falseList.begin(),
                                            Expression->falseList.end());
                leftSymbol->nextList.insert(leftSymbol->nextList.end(), Statement1->nextList.begin(),
                                            Statement1->nextList.end());
            } else {
                midCode.backPatch(Expression->trueList, M->quad);
                midCode.backPatch(Expression->falseList, Else_part->quad);
                leftSymbol->nextList.clear();
                leftSymbol->nextList.insert(leftSymbol->nextList.end(), Statement1->nextList.begin(),
                                            Statement1->nextList.end());
                leftSymbol->nextList.insert(leftSymbol->nextList.end(), Else_part->nextList.begin(),
                                            Else_part->nextList.end());
            }
            break;
        }
        case 57: { // 57. Statement -> case Expression W of Case_body end
            leftSymbol->startQuad = midCode.codeList.size();
            auto W = &vectorAttribute[top - 3];
            auto Case_body = &vectorAttribute[top - 1];
            auto Expression = &vectorAttribute[top - 4];
            midCode.backPatch(W->testList, midCode.codeList.size());
            int tempC = 0;
            while (tempC < Case_body->num) {
                int caseSite = Case_body->caseList[tempC];  //获取第tempC+1个分支的开始地址
                vector<string> varList = Case_body->varList_list[tempC]; // 获取第C+1个分支的选择变量表
                for (auto &i : varList) {
                    string value;
                    if (Expression->value == i) {
                        value = "1";
                    } else {
                        value = "0";
                    }
                    TempVar *entry{nullptr};
                    entry = midCode.newTemp();
                    entry->value = value;
                    entry->type = new Type(Type::BOOLEAN);
                    leftSymbol->entry = entry;
                    string arg1, arg2, res;
                    arg1 = Expression->entry->id;
                    arg2 = i;
                    res = entry->id;
                    midCode.outCode(QuaternionItem::EQUAL, arg1, arg2, res);
                    string argb1, argb2, resb;
                    resb = "$" + to_string(caseSite);
                    argb1 = res;
                    midCode.outCode(QuaternionItem::IF, argb1, argb2, resb);
                }
                tempC++;
            }
            midCode.backPatch(Case_body->nextList, midCode.codeList.size());
            break;
        }
        case 58: { // 58. Statement -> while M1 Expression G58 do M2 Statement1
            auto Expression = &vectorAttribute[top - 4];
            leftSymbol->startQuad = midCode.codeList.size();
            if (Expression->type->getType() != Type::BOOLEAN)
                recordSemanticError(Expression->line, "错误，while语句的判断表达式不是BOOLEAN类型");
            ////中间代码
            auto Statement1 = &vectorAttribute[top];
            auto M1 = &vectorAttribute[top - 5];
            auto M2 = &vectorAttribute[top - 1];
            midCode.backPatch(Statement1->nextList, midCode.codeList.size());
            midCode.backPatch(Expression->trueList, M2->quad);
            leftSymbol->nextList = Expression->falseList;
            string arg1, arg2, res;
            res = "$" + to_string(M1->quad);
            midCode.outCode(QuaternionItem::GOTO, arg1, arg2, res);
            break;
        }
        case 59: { // 59. Statement -> repeat M1 Statement_list M2 until Expression H59
            auto Expression = vectorAttribute[top - 1];
            leftSymbol->startQuad = midCode.codeList.size();
            if (Expression.type->getType() != Type::BOOLEAN)
                recordSemanticError(Expression.line, "错误，repeat语句的判断表达式不是BOOLEAN类型");
            ////中间代码
            auto Statement_list = &vectorAttribute[top - 4];
            auto M1 = &vectorAttribute[top - 5];
            auto M2 = &vectorAttribute[top - 3];
            midCode.backPatch(Statement_list->nextList, M2->quad);
            midCode.backPatch(Expression.falseList, M1->quad);

            break;
        }
        case 60: { // 60. Statement -> for id assignop Expression0 Updown M1 Expression1 I60 do M2 Statement1
            auto id = &vectorAttribute[top - 9];
            auto tempPoint = curBlock->query(id->attribute);
            auto Expression0 = &vectorAttribute[top - 7];
            auto Expression1 = &vectorAttribute[top - 4];
            leftSymbol->startQuad = midCode.codeList.size();
            bool flag = true;
            if (*(Expression0->type) != *(Expression1->type))
                flag = false;
            if (*(tempPoint->type) != *(Expression0->type))
                flag = false;
            if (!flag)
                recordSemanticError(Expression0->line, "错误，for语句中id，Expression类型不一致");
            ////中间代码
            auto M1 = &vectorAttribute[top - 5];
            auto M2 = &vectorAttribute[top - 1];
            auto Statement1 = &vectorAttribute[top];
            auto Updown = &vectorAttribute[top - 6];

            midCode.backPatch(Statement1->nextList, midCode.codeList.size());
            midCode.backPatch(Expression1->trueList, M2->quad);
            leftSymbol->nextList = Expression1->falseList;
            if (Updown->value == "to") {
                TempVar *entryb{nullptr};
                entryb = midCode.newTemp();
                entryb->type = tempPoint->type;
//                entryb->value = "NotKnow";
                string argb1, argb2, resb;
                argb1 = id->entry->id;
                argb2 = "$1";
                resb = entryb->id;
                midCode.outCode(QuaternionItem::ADD, argb1, argb2, resb);

                string argc1, argc2, resc;
                argc1 = entryb->id;
                resc = id->entry->id;
                midCode.outCode(QuaternionItem::ASSIGN, argc1, argc2, resc);
            } else if (Updown->value == "downto") {
                TempVar *entryb{nullptr};
                entryb = midCode.newTemp();
                entryb->type = tempPoint->type;
//                entryb->value = "NotKnow";
                string argb1, argb2, resb;
                argb1 = id->entry->id;
                argb2 = "$1";
                resb = entryb->id;
                midCode.outCode(QuaternionItem::MINUS, argb1, argb2, resb);

                string argc1, argc2, resc;
                argc1 = entryb->id;
                resc = id->entry->id;
                midCode.outCode(QuaternionItem::ASSIGN, argc1, argc2, resc);
            }
            string argd1, argd2, resd;
            resd = "$" + to_string(M1->quad);
            midCode.outCode(QuaternionItem::GOTO, argd1, argd2, resd);

            break;
        }
        case 61: {  //61. Statement -> #
            leftSymbol->startQuad = midCode.codeList.size();
            break;
        }
        case 62: { // Variable -> id E62 Id_varparts
            auto Id_varparts = vectorAttribute[top];
            auto id = &vectorAttribute[top - 2];
            leftSymbol->type = Id_varparts.type;
//            leftSymbol->tableLineEntry = Id_varparts.tableLineEntry;
            leftSymbol->tableLineEntry = curBlock->query(id->attribute);
            leftSymbol->variableName = vectorAttribute[top - 2].attribute;
            if (Id_varparts.variableName != "Id_varparts") {
                leftSymbol->variableName += Id_varparts.variableName;
            }
            TempVar *entry{nullptr};
            entry = midCode.newTemp();
            entry->value = leftSymbol->variableName;
            entry->type = leftSymbol->type;
            entry->tableLineEntry = leftSymbol->tableLineEntry;
            Type *typePoint = id->type;
//            int tempOffset = 0;
            if (!Id_varparts.queue.empty()) {
                TempVar *entryb{nullptr};
                entryb = midCode.newTemp();
                entryb->value = "0";
                entryb->type = new Type(Type::INTEGER);
                midCode.outCode(QuaternionItem::ASSIGN, "$0", "", entryb->id);
                for (int i = 0; i < Id_varparts.queue.size(); ++i) {
                    if (typePoint->getType() == Type::ARRAY) {
                        Array *arrPoint = (Array *) typePoint;
                        auto nextPoint = arrPoint->elem;
                        TempVar *entryc{nullptr};
                        entryc = midCode.newTemp();
                        entryc->value = "0";
                        entryc->type = new Type(Type::INTEGER);

                        TempVar *entryd{nullptr};
                        entryd = midCode.newTemp();
                        entryd->value = "0";
                        entryd->type = new Type(Type::INTEGER);
                        midCode.outCode(QuaternionItem::MINUS, Id_varparts.queue[i], "$" + to_string(arrPoint->low),
                                        entryd->id);
                        midCode.outCode(QuaternionItem::MULTIPLY, entryd->id, "$" + to_string(nextPoint->getSize()),
                                        entryc->id);
                        midCode.outCode(QuaternionItem::ADD, entryb->id, entryc->id, entryb->id);
                        typePoint = nextPoint;
                    } else if (typePoint->getType() == Type::RECORD) {
                        Record *recPoint = (Record *) typePoint;
                        int recOffset = 0;
                        TempVar *entryc{nullptr};
                        entryc = midCode.newTemp();
                        entryc->value = "0";
                        entryc->type = new Type(Type::INTEGER);

                        TempVar *entryd{nullptr};
                        entryd = midCode.newTemp();
                        entryd->value = "0";
                        entryd->type = new Type(Type::INTEGER);

                        for (int j = 0; j < recPoint->env.size(); ++j) {
                            if (recPoint->env[j].id == Id_varparts.queue[i]) {
                                recOffset = j;
                                break;
                            }
                            midCode.outCode(QuaternionItem::ADD, entryb->id,
                                            "$" + to_string(recPoint->env[j].getSize()), entryb->id);
                        }
                        typePoint = recPoint->env[recOffset].type;
//                        midCode.outCode(QuaternionItem::ADD, entryb->id, "$" + to_string(recOffset), entryb->id);
//                        if (i != Id_varparts.queue.size() - 1) {
//                            Record *nextRecPoint = (Record *) typePoint;
//                            midCode.outCode(QuaternionItem::MULTIPLY, entryb->id, to_string(nextRecPoint->getSize()),
//                                            entryb->id);
//                        }
                    }
                }
                entry->offset = entryb;

                TempVar *entryc{nullptr};
                entryc = midCode.newTemp();
                entryc->type = leftSymbol->type;
                midCode.outCode(QuaternionItem::ASSIGNOFFSET, entry->id, entryb->id, entryc->id);
                entryc->backOffset = entry;
                leftSymbol->entry = entryc;
            } else {
                leftSymbol->entry = entry;
            }
            break;
        }
        case 63: { // Id_varparts0 -> Id_varparts1 Id_varpart
            auto Id_varpart = vectorAttribute[top];
            auto Id_varparts1 = vectorAttribute[top - 1];
            leftSymbol->tableLineEntry = Id_varpart.tableLineEntry;
            if (Id_varparts1.variableName != "Id_varparts") {
                leftSymbol->variableName = Id_varparts1.variableName;
            }
            if (Id_varpart.variableName != "Id_varpart") {
                leftSymbol->variableName += Id_varpart.variableName;
            }
            leftSymbol->type = Id_varpart.type;
            leftSymbol->queue = Id_varparts1.queue;
            leftSymbol->queue.insert(leftSymbol->queue.end(), Id_varpart.queue.begin(), Id_varpart.queue.end());
            break;
        }
        case 64: { // Id_varparts -> #
            leftSymbol->tableLineEntry = vectorAttribute[top].tableLineEntry;
            leftSymbol->type = vectorAttribute[top].type;
            leftSymbol->variableName = "";
            break;
        }
        case 65: {// Id_varpart -> [ Expression_list ]
            auto Expression_list = vectorAttribute[top - 1];
            auto i_type = vectorAttribute[top - 3].type;
            auto i_entry = vectorAttribute[top - 3].tableLineEntry;
            int lines = vectorAttribute[top].line;
            auto type = i_type;
            if (type->getType() == Type::ARRAY) {
                int dimension = type->getDimension();
                auto typeList = Expression_list.typeList;
                int size = typeList.size();
                if (dimension >= size) {
                    bool isExpressionsTypeInteger = true;

                    for (const auto &item : typeList)
                        if (item->getType() != Type::INTEGER) {
                            isExpressionsTypeInteger = false;
                            break;
                        }
                    if (isExpressionsTypeInteger) {
                        leftSymbol->tableLineEntry = i_entry;
                        leftSymbol->variableName = "[" + Expression_list.variableName + "]";
                        while (size--) {
                            auto t = (Array *) type;
                            type = t->elem;
                        }
                        leftSymbol->type = type;
                    } else {
                        leftSymbol->tableLineEntry = nullptr;
                        recordSemanticError(lines, "运算符[]的参数不是INTEGER");
                    }
                } else {
                    leftSymbol->tableLineEntry = nullptr;
                    recordSemanticError(lines, "类型错误，超过指定ARRAY的维度");
                }
            } else {
                leftSymbol->tableLineEntry = nullptr;
                recordSemanticError(lines, "类型错误，不是ARRAY类型，不能使用[]运算符");
            }
            leftSymbol->queue = Expression_list.queue;
            break;
        }
        case 66: { // Id_varpart -> . id
            auto id_local = vectorAttribute[top];
            auto i_type = vectorAttribute[top - 2].type;
            auto i_entry = vectorAttribute[top - 2].tableLineEntry;
            auto type = i_type;
            if (type->getType() == Type::RECORD) {
                auto recordBlock = i_entry->blockPoint;
                auto entry = recordBlock->blockQuery(id_local.attribute);
                if (entry != nullptr) {
                    leftSymbol->tableLineEntry = entry;
                    leftSymbol->variableName = "." + id_local.attribute;
                    leftSymbol->type = entry->type;
                } else {
                    leftSymbol->tableLineEntry = nullptr;
                    leftSymbol->type = new Type(Type::TYPE_ERROR);
                    stringstream o;
                    o << "语义错误，record " << i_entry->name << "中没有" << id_local.attribute << "域";
                    recordSemanticError(id_local.line, o.str());
                }
            } else {
                leftSymbol->tableLineEntry = nullptr;
                leftSymbol->type = new Type(Type::TYPE_ERROR);
                recordSemanticError(id_local.line, "类型错误，不是RECORD类型，不能使用.运算符");
            }
            ////
            leftSymbol->queue.push_back(id_local.attribute);
            break;
        }
        case 67: {  //67. Else_part -> N else M Statement
            auto M = vectorAttribute[top - 1];
            auto N = vectorAttribute[top - 3];
            auto Statement = vectorAttribute[top];
            leftSymbol->quad = M.quad;
            leftSymbol->nextList.clear();
            leftSymbol->nextList.insert(leftSymbol->nextList.end(), N.nextList.begin(), N.nextList.end());
            leftSymbol->nextList.insert(leftSymbol->nextList.end(), Statement.nextList.begin(),
                                        Statement.nextList.end());
            break;
        }
        case 68: {  //68. Else_part -> #
            leftSymbol->nextList.clear();//这条语句一般情况下是没有变化的
//            Else_part.nextlist = null;
            break;
        }
        case 69: {  //69. Case_body -> Branch_list
            auto Branch_list = vectorAttribute[top];
            leftSymbol->caseList = Branch_list.caseList;    //每一条分支语句的地址表
            leftSymbol->varList_list = Branch_list.varList_list;    //每一条分支语句的常量表
            leftSymbol->num = Branch_list.num;              //分支语句的数量
            leftSymbol->nextList = Branch_list.nextList;    //需要回填的地址表
            break;
        }
        case 70: {  // 70. Case_body -> #
            leftSymbol->caseList.clear();
            leftSymbol->varList_list.clear();
            leftSymbol->num = 0;
            leftSymbol->nextList.clear();
            break;
        }
        case 71: { // 71. Branch_list0 -> Branch_list1 ; Branch
            auto Branch = vectorAttribute[top];
            auto Branch_list1 = vectorAttribute[top - 2];
            leftSymbol->type = Branch.type;
            leftSymbol->caseList.clear();
            leftSymbol->caseList.insert(leftSymbol->caseList.end(), Branch_list1.caseList.begin(),
                                        Branch_list1.caseList.end());
            leftSymbol->caseList.insert(leftSymbol->caseList.end(), Branch.caseList.begin(), Branch.caseList.end());
            leftSymbol->varList_list.clear();
            ////注意这里的Branch_list1是新开辟出空间的结构体,不是原先那个Branch_list1
            Branch_list1.varList_list.push_back(Branch.varList);
            leftSymbol->varList_list.insert(leftSymbol->varList_list.end(), Branch_list1.varList_list.begin(),
                                            Branch_list1.varList_list.end());
            leftSymbol->num = Branch_list1.num + 1;
            leftSymbol->nextList.clear();
            leftSymbol->nextList.insert(leftSymbol->nextList.end(), Branch_list1.nextList.begin(),
                                        Branch_list1.nextList.end());
            leftSymbol->nextList.insert(leftSymbol->nextList.end(), Branch.nextList.begin(), Branch.nextList.end());
            break;
        }
        case 72: { // 72. Branch_list -> Branch
            auto i_type = vectorAttribute[top - 2].type;
            leftSymbol->type = i_type;
            auto Branch = vectorAttribute[top];
            leftSymbol->caseList = Branch.caseList;
            leftSymbol->varList_list.push_back(Branch.varList);
            leftSymbol->num = 1;
            leftSymbol->nextList = Branch.nextList;
            break;
        }
        case 73: { // 73. Branch -> Const_list : U Statement
            auto i_type = vectorAttribute[top - 5].type;//这里top-5才是Expression后的W或上一个Branch_list
            auto Const_list = vectorAttribute[top - 3];
            int line = Const_list.line;
            bool flag = true;
            for (const auto &item: Const_list.typeList) {
                if (*i_type != *item)
                    flag = false;
            }
            if (!flag)
                recordSemanticError(line, "错误，branch中常量的类型不合法");
            leftSymbol->type = i_type;
            ////中间代码生成
            auto U = vectorAttribute[top - 1];
            leftSymbol->varList = Const_list.varList;
            leftSymbol->caseList = U.caseList;
            leftSymbol->nextList.push_back(midCode.codeList.size());
            string arg1, arg2, res;
            midCode.outCode(QuaternionItem::GOTO, arg1, arg2, res);
            break;
        }
        case 74: { // 74. Const_list0 -> Const_list1 , Const_variable
            auto Const_list1 = vectorAttribute[top - 2];
            auto Const_variable = vectorAttribute[top];
            leftSymbol->typeList = Const_list1.typeList;
            leftSymbol->typeList.push_back(Const_variable.type);
            ////中间代码
            Const_list1.varList.push_back(Const_variable.value);
            leftSymbol->varList = Const_list1.varList;
            break;
        }
        case 75: { // 75. Const_list -> Const_variable
            auto Const_variable = vectorAttribute[top];
            leftSymbol->typeList.push_back(Const_variable.type);
            ////中间代码
            leftSymbol->varList.push_back(Const_variable.value);
            break;
        }
        case 76: {  //76. Updown -> to
            auto Expression0 = &vectorAttribute[top - 1];
            auto id = &vectorAttribute[top - 3];
            string arg1, arg2, res;
            if (Expression0->entry) {
                arg1 = Expression0->entry->id;
            } else {
                TempVar *entry{nullptr};
                entry = midCode.newTemp();
                entry->value = Expression0->variableName;
                entry->type = Expression0->type;
                entry->tableLineEntry = Expression0->tableLineEntry;
                Expression0->entry = entry;
                arg1 = Expression0->entry->id;
//                arg1 = Expression.tableLineEntry->name;
            }
            if (id->entry) {
                res = id->entry->id;
            } else {
                TempVar *entry{nullptr};
                entry = midCode.newTemp();
                entry->value = id->attribute;
                entry->type = id->type;
                id->tableLineEntry = curBlock->query(id->attribute);
                entry->tableLineEntry = id->tableLineEntry;
                id->entry = entry;
                res = id->entry->id;
            }
            midCode.outCode(QuaternionItem::ASSIGN, arg1, arg2, res);


            leftSymbol->value = "to";
            break;
        }
        case 77: {  //77. Updown -> downto
            auto Expression0 = &vectorAttribute[top - 1];
            auto id = &vectorAttribute[top - 3];
            string arg1, arg2, res;
            if (Expression0->entry) {
                arg1 = Expression0->entry->id;
            } else {
                TempVar *entry{nullptr};
                entry = midCode.newTemp();
                entry->value = Expression0->variableName;
                entry->type = Expression0->type;
                entry->tableLineEntry = Expression0->tableLineEntry;
                Expression0->entry = entry;
                arg1 = Expression0->entry->id;
//                arg1 = Expression.tableLineEntry->name;
            }
            if (id->entry) {
                res = id->entry->id;
            } else {
                TempVar *entry{nullptr};
                entry = midCode.newTemp();
                entry->value = id->attribute;
                entry->type = id->type;
                id->tableLineEntry = curBlock->query(id->attribute);
                entry->tableLineEntry = id->tableLineEntry;
                id->entry = entry;
                res = id->entry->id;
            }
            midCode.outCode(QuaternionItem::ASSIGN, arg1, arg2, res);
            leftSymbol->value = "downto";
            break;
        }
        case 78:    // Call_procedure_statement -> id
        case 79: {  // Call_procedure_statement -> id ( Expression_list )
            string id;
            if (op_type == 78) {
                id = vectorAttribute[top].attribute;
            } else {
                id = vectorAttribute[top - 3].attribute;
            }
            int line = vectorAttribute[top].line;
            quoteID(line, id);  //查看是否存在这个id
            SymbolTableLine *tempLinePoint = curBlock->query(id);
            if (tempLinePoint != nullptr) {//如果存在
                if (tempLinePoint->type->getType() == Type::PROC) {
                    auto proc = (Proc *) tempLinePoint->type;
                    int num = proc->parametersNum;
                    if (op_type == 78) {
                        if (num != 0) //如果为参数个数0则正确
                            recordSemanticError(line,
                                                "语义错误！缺失参数,此Proc需要" + to_string(num) +
                                                "个参数");
                    } else if (op_type == 79) {
                        auto Expression_list = vectorAttribute[top - 1];
                        if (Expression_list.typeList.size() != num) {//如果数量不等
                            recordSemanticError(line,
                                                "语义错误！参数不一致,此Proc需要" +
                                                to_string(num) +
                                                "个参数");
                        } else {//数量相等,检查类型
                            for (int i = 0; i < Expression_list.typeList.size(); ++i) {
                                auto type1 = Expression_list.typeList[i];
                                auto type2 = proc->env[i].type;
                                if (*type1 != *type2)
                                    recordSemanticError(line, "语义错误！第" + to_string(i + 1) + "个参数类型不一致");
                            }
                        }
                    }
                } else {
                    recordSemanticError(line, "语义错误！此id不为Procedure");
                }
            } else {
                recordSemanticError(line, "语义错误！引用了不存在的标识符" + id);
            }
            leftSymbol->type = new Type(Type::VOID);
            ////中间代码
            string arg1, arg2, res;
            vectorAttributeItem *idPoint = nullptr;
            if (op_type == 78) {
                idPoint = &vectorAttribute[top];
            } else if (op_type == 79) {
                idPoint = &vectorAttribute[top - 3];
            }
            if (idPoint->entry) {
                arg1 = idPoint->entry->id;
            } else {
                TempVar *entry{nullptr};
                entry = midCode.newTemp();
                entry->value = idPoint->attribute;
                entry->type = idPoint->type;
                idPoint->tableLineEntry = curBlock->query(idPoint->attribute);
                entry->tableLineEntry = idPoint->tableLineEntry;
                idPoint->entry = entry;
                arg1 = idPoint->entry->id;
            }
            if (op_type == 78) {
                arg2 = "$0";
                midCode.outCode(QuaternionItem::CALL, arg1, arg2, res);
            } else if (op_type == 79) {
                auto Expression_list = &vectorAttribute[top - 1];
                for (auto &i : Expression_list->queue) {
                    string argb1, argb2, resb;
                    argb1 = i;
                    midCode.outCode(QuaternionItem::PARAM, argb1, argb2, resb);
                }
                arg2 = to_string(Expression_list->queue.size());
                midCode.outCode(QuaternionItem::CALL, arg1, arg2, res);
            }
            break;
        }
        case 80: { // Expression_list0 -> Expression_list1 , Expression
            auto Expression_list1 = &vectorAttribute[top - 2];
            auto Expression = &vectorAttribute[top];
            leftSymbol->typeList = Expression_list1->typeList;
            leftSymbol->typeList.push_back(Expression->type);
            leftSymbol->variableName = Expression_list1->variableName + Expression->variableName;
            ////
            leftSymbol->queue = Expression_list1->queue;
            leftSymbol->queue.push_back(Expression->entry->id);
            break;
        }
        case 81: { // Expression_list -> Expression
            auto Expression = vectorAttribute[top];
            leftSymbol->typeList.push_back(Expression.type);
            leftSymbol->variableName = Expression.variableName;
            ////
            leftSymbol->queue.push_back(Expression.entry->id);
            break;
        }
        case 82: { // Expression -> Simple_expression0 = Simple_expression1
            auto Simple_expression0 = vectorAttribute[top - 2];
            auto Simple_expression1 = vectorAttribute[top];
            Type *resultType{nullptr};
            string value;
            if (*(Simple_expression0.type) == *(Simple_expression1.type)) {
                bool flag = Simple_expression0.type->isBasicType() &&
                            Simple_expression1.type->isBasicType();
                if (flag) {
                    resultType = new Type(Type::BOOLEAN);

                    if (!Simple_expression0.value.empty() && !Simple_expression1.value.empty()) {
                        if (Simple_expression0.value == Simple_expression1.value) {
                            value = "1";
                        } else {
                            value = "0";
                        }
                    }
                } else {
                    resultType = new Type(Type::TYPE_ERROR);
                    recordSemanticError(Simple_expression0.line, "逻辑运算符=：操作数类型违法");
                }
            } else {
                resultType = new Type(Type::TYPE_ERROR);
                recordSemanticError(Simple_expression0.line, "逻辑运算符=：左右操作数类型不一致");
            }
            leftSymbol->type = resultType;
            leftSymbol->value = value;
            TempVar *entry{nullptr};
            entry = midCode.newTemp();
            entry->value = value;
            entry->type = resultType;
            leftSymbol->entry = entry;
            string arg1, arg2, res;
            arg1 = Simple_expression0.entry->id;
            arg2 = Simple_expression1.entry->id;
            res = entry->id;
            midCode.outCode(QuaternionItem::EQUAL, arg1, arg2, res);
            break;
        }
        case 83: { // Expression -> Simple_expression0 <> Simple_expression1
            auto Simple_expression0 = vectorAttribute[top - 2];
            auto Simple_expression1 = vectorAttribute[top];
            Type *resultType{nullptr};
            string value;
            if (*(Simple_expression0.type) == *(Simple_expression1.type)) {
                bool flag = Simple_expression0.type->isBasicType() &&
                            Simple_expression1.type->isBasicType();
                if (flag) {
                    resultType = new Type(Type::BOOLEAN);
                    if (!Simple_expression0.value.empty() && !Simple_expression1.value.empty()) {
                        if (Simple_expression0.value == Simple_expression1.value)
                            value = "0"; // false
                        else
                            value = "1"; // true
                    }
                } else {
                    resultType = new Type(Type::TYPE_ERROR);
                    recordSemanticError(Simple_expression0.line, "逻辑运算符<>：操作数类型违法");
                }
            } else {
                resultType = new Type(Type::TYPE_ERROR);
                recordSemanticError(Simple_expression0.line, "逻辑运算符<>：左右操作数类型不一致");
            }
            leftSymbol->type = resultType;
            leftSymbol->value = value;
            TempVar *entry{nullptr};
            entry = midCode.newTemp();
            entry->value = value;
            entry->type = resultType;
            leftSymbol->entry = entry;
            string arg1, arg2, res;
            arg1 = Simple_expression0.entry->id;
            arg2 = Simple_expression1.entry->id;
            res = entry->id;
            midCode.outCode(QuaternionItem::UNEQUAL, arg1, arg2, res);
            break;
        }
        case 84: { // Expression -> Simple_expression0 < Simple_expression1
            auto Simple_expression0 = vectorAttribute[top - 2];
            auto Simple_expression1 = vectorAttribute[top];
            Type *resultType{nullptr};
            string value;
            if (*(Simple_expression0.type) == *(Simple_expression1.type)) {
                if (Simple_expression0.type->getType() == Type::BOOLEAN ||
                    Simple_expression0.type->getType() == Type::INTEGER) {
                    resultType = new Type(Type::BOOLEAN);
                    if (Simple_expression0.value.empty() || Simple_expression1.value.empty())
                        value = "";
                    else {
                        int a = stoi(Simple_expression0.value);
                        int b = stoi(Simple_expression1.value);
                        if (a < b)
                            value = "1";
                        else
                            value = "0";
                    }
                } else if (Simple_expression0.type->getType() == Type::REAL) {
                    resultType = new Type(Type::BOOLEAN);
                    if (Simple_expression0.value.empty() || Simple_expression1.value.empty())
                        value = "";
                    else {
                        float a = stof(Simple_expression0.value);
                        float b = stof(Simple_expression1.value);
                        if (a < b)
                            value = "1";
                        else
                            value = "0";
                    }
                } else if (Simple_expression0.type->getType() == Type::CHAR) {
                    resultType = new Type(Type::BOOLEAN);
                    if (Simple_expression0.value.empty() || Simple_expression1.value.empty())
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
                    resultType = new Type(Type::TYPE_ERROR);
                    recordSemanticError(Simple_expression0.line, "逻辑运算符<：操作数类型违法");
                }
            } else {
                resultType = new Type(Type::TYPE_ERROR);
                recordSemanticError(Simple_expression0.line, "逻辑运算符<：左右操作数类型不一致");
            }
            leftSymbol->type = resultType;
            leftSymbol->value = value;
            TempVar *entry{nullptr};
            entry = midCode.newTemp();
            entry->value = value;
            entry->type = resultType;
            leftSymbol->entry = entry;
            string arg1, arg2, res;
            arg1 = Simple_expression0.entry->id;
            arg2 = Simple_expression1.entry->id;
            res = entry->id;
            midCode.outCode(QuaternionItem::LESS, arg1, arg2, res);
            break;
        }
        case 85: { // Expression -> Simple_expression0 <= Simple_expression1
            auto Simple_expression0 = vectorAttribute[top - 2];
            auto Simple_expression1 = vectorAttribute[top];
            Type *resultType{nullptr};
            string value;
            if (*(Simple_expression0.type) == *(Simple_expression1.type)) {
                if (Simple_expression0.type->getType() == Type::BOOLEAN ||
                    Simple_expression0.type->getType() == Type::INTEGER) {
                    resultType = new Type(Type::BOOLEAN);
                    if (Simple_expression0.value.empty() || Simple_expression1.value.empty())
                        value = "";
                    else {
                        int a = stoi(Simple_expression0.value);
                        int b = stoi(Simple_expression1.value);
                        if (a <= b)
                            value = "1";
                        else
                            value = "0";
                    }
                } else if (Simple_expression0.type->getType() == Type::REAL) {
                    resultType = new Type(Type::BOOLEAN);
                    if (Simple_expression0.value.empty() || Simple_expression1.value.empty())
                        value = "";
                    else {
                        float a = stof(Simple_expression0.value);
                        float b = stof(Simple_expression1.value);
                        if (a <= b)
                            value = "1";
                        else
                            value = "0";
                    }
                } else if (Simple_expression0.type->getType() == Type::CHAR) {
                    resultType = new Type(Type::BOOLEAN);
                    if (Simple_expression0.value.empty() || Simple_expression1.value.empty())
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
                    resultType = new Type(Type::TYPE_ERROR);
                    recordSemanticError(Simple_expression0.line, "逻辑运算符<=：操作数类型违法");
                }
            } else {
                resultType = new Type(Type::TYPE_ERROR);
                recordSemanticError(Simple_expression0.line, "逻辑运算符<=：左右操作数类型不一致");
            }
            leftSymbol->type = resultType;
            leftSymbol->value = value;
            TempVar *entry{nullptr};
            entry = midCode.newTemp();
            entry->value = value;
            entry->type = resultType;
            leftSymbol->entry = entry;
            string arg1, arg2, res;
            arg1 = Simple_expression0.entry->id;
            arg2 = Simple_expression1.entry->id;
            res = entry->id;
            midCode.outCode(QuaternionItem::LESS_EQUAL, arg1, arg2, res);
            break;
        }
        case 86: { // Expression -> Simple_expression0 > Simple_expression1
            auto Simple_expression0 = vectorAttribute[top - 2];
            auto Simple_expression1 = vectorAttribute[top];
            Type *resultType{nullptr};
            string value;
            if (*(Simple_expression0.type) == *(Simple_expression1.type)) {
                if (Simple_expression0.type->getType() == Type::BOOLEAN ||
                    Simple_expression0.type->getType() == Type::INTEGER) {
                    resultType = new Type(Type::BOOLEAN);
                    if (Simple_expression0.value.empty() || Simple_expression1.value.empty())
                        value = "";
                    else {
                        int a = stoi(Simple_expression0.value);
                        int b = stoi(Simple_expression1.value);
                        if (a > b)
                            value = "1";
                        else
                            value = "0";
                    }
                } else if (Simple_expression0.type->getType() == Type::REAL) {
                    resultType = new Type(Type::BOOLEAN);
                    if (Simple_expression0.value.empty() || Simple_expression1.value.empty())
                        value = "";
                    else {
                        float a = stof(Simple_expression0.value);
                        float b = stof(Simple_expression1.value);
                        if (a > b)
                            value = "1";
                        else
                            value = "0";
                    }
                } else if (Simple_expression0.type->getType() == Type::CHAR) {
                    resultType = new Type(Type::BOOLEAN);
                    if (Simple_expression0.value.empty() || Simple_expression1.value.empty())
                        value = "";
                    else {
                        char a = Simple_expression0.value[0];
                        char b = Simple_expression1.value[0];
                        if (a > b)
                            value = "1";
                        else
                            value = "0";
                    }
                } else {
                    resultType = new Type(Type::TYPE_ERROR);
                    recordSemanticError(Simple_expression0.line, "逻辑运算符>：操作数类型违法");
                }
            } else {
                resultType = new Type(Type::TYPE_ERROR);
                recordSemanticError(Simple_expression0.line, "逻辑运算符>：左右操作数类型不一致");
            }
            leftSymbol->type = resultType;
            leftSymbol->value = value;
            TempVar *entry{nullptr};
            entry = midCode.newTemp();
            entry->value = value;
            entry->type = resultType;
            leftSymbol->entry = entry;
            string arg1, arg2, res;
            arg1 = Simple_expression0.entry->id;
            arg2 = Simple_expression1.entry->id;
            res = entry->id;
            midCode.outCode(QuaternionItem::MORE, arg1, arg2, res);
            break;
        }
        case 87: { // Expression -> Simple_expression0 >= Simple_expression1
            auto Simple_expression0 = vectorAttribute[top - 2];
            auto Simple_expression1 = vectorAttribute[top];
            Type *resultType{nullptr};
            string value;
            if (*(Simple_expression0.type) == *(Simple_expression1.type)) {
                if (Simple_expression0.type->getType() == Type::BOOLEAN ||
                    Simple_expression0.type->getType() == Type::INTEGER) {
                    resultType = new Type(Type::BOOLEAN);
                    if (Simple_expression0.value.empty() || Simple_expression1.value.empty())
                        value = "";
                    else {
                        int a = stoi(Simple_expression0.value);
                        int b = stoi(Simple_expression1.value);
                        if (a >= b)
                            value = "1";
                        else
                            value = "0";
                    }
                } else if (Simple_expression0.type->getType() == Type::REAL) {
                    resultType = new Type(Type::BOOLEAN);
                    if (Simple_expression0.value.empty() || Simple_expression1.value.empty())
                        value = "";
                    else {
                        float a = stof(Simple_expression0.value);
                        float b = stof(Simple_expression1.value);
                        if (a >= b)
                            value = "1";
                        else
                            value = "0";
                    }
                } else if (Simple_expression0.type->getType() == Type::CHAR) {
                    resultType = new Type(Type::BOOLEAN);
                    if (Simple_expression0.value.empty() || Simple_expression1.value.empty())
                        value = "";
                    else {
                        char a = Simple_expression0.value[0];
                        char b = Simple_expression1.value[0];
                        if (a >= b)
                            value = "1";
                        else
                            value = "0";
                    }
                } else {
                    resultType = new Type(Type::TYPE_ERROR);
                    recordSemanticError(Simple_expression0.line, "逻辑运算符>=：操作数类型违法");
                }
            } else {
                resultType = new Type(Type::TYPE_ERROR);
                recordSemanticError(Simple_expression0.line, "逻辑运算符>=：左右操作数类型不一致");
            }
            leftSymbol->type = resultType;
            leftSymbol->value = value;
            TempVar *entry{nullptr};
            entry = midCode.newTemp();
            entry->value = value;
            entry->type = resultType;
            leftSymbol->entry = entry;
            string arg1, arg2, res;
            arg1 = Simple_expression0.entry->id;
            arg2 = Simple_expression1.entry->id;
            res = entry->id;
            midCode.outCode(QuaternionItem::MORE_EQUAL, arg1, arg2, res);
            break;
        }
        case 88: { // Expression -> Simple_expression
            auto Simple_expression = vectorAttribute[top];
            leftSymbol->type = Simple_expression.type;
            leftSymbol->value = Simple_expression.value;
            leftSymbol->entry = Simple_expression.entry;
            leftSymbol->tableLineEntry = Simple_expression.tableLineEntry;
            leftSymbol->variableName = Simple_expression.variableName;
            break;
        }
        case 89: { // Simple_expression -> Term
            auto Term = vectorAttribute[top];
            leftSymbol->type = Term.type;
            leftSymbol->value = Term.value;
            leftSymbol->entry = Term.entry;
            leftSymbol->tableLineEntry = Term.tableLineEntry;
            leftSymbol->variableName = Term.variableName;
            break;
        }
        case 90: { // Simple_expression -> + Term
            auto Term = vectorAttribute[top];
            Type *resultType{nullptr};
            string value;
            if (Term.type->getType() == Type::INTEGER ||
                Term.type->getType() == Type::REAL) {
                resultType = Term.type;
                if (!Term.value.empty()) {
                    float a = stof(Term.value);
                    value = to_string(a);
                }
            } else {
                resultType = new Type(Type::TYPE_ERROR);
            }
            leftSymbol->type = Term.type;
            leftSymbol->value = Term.value;
            leftSymbol->tableLineEntry = Term.tableLineEntry;
            leftSymbol->variableName = "+" + Term.variableName;

            TempVar *entry{nullptr};
            entry = midCode.newTemp();
            entry->value = value;
            entry->type = resultType;
            leftSymbol->entry = entry;
            string arg1, arg2, res;
            auto var = midCode.newTemp(true);
            var->type = new Type(Type::INTEGER);
            var->value = "0";
            arg1 = var->id;
            arg2 = Term.entry->id;
            res = entry->id;
            midCode.outCode(QuaternionItem::ADD, arg1, arg2, res);
            break;
        }
        case 91: { // Simple_expression -> - Term
            auto Term = vectorAttribute[top];
            Type *resultType{nullptr};
            string value;
            if (Term.type->getType() == Type::INTEGER ||
                Term.type->getType() == Type::REAL) {
                resultType = Term.type;
                if (!Term.value.empty()) {
                    float a = stof(Term.value);
                    value = to_string(0 - a);
                }
            } else {
                resultType = new Type(Type::TYPE_ERROR);
            }
            leftSymbol->type = Term.type;
            leftSymbol->value = Term.value;
            leftSymbol->tableLineEntry = Term.tableLineEntry;
            leftSymbol->variableName = "-" + Term.variableName;
            TempVar *entry{nullptr};
            entry = midCode.newTemp();
            entry->value = value;
            entry->type = resultType;
            leftSymbol->entry = entry;
            string arg1, arg2, res;
            auto var = midCode.newTemp(true);
            var->type = new Type(Type::INTEGER);
            var->value = "0";
            arg1 = var->id;
            arg2 = Term.entry->id;
            res = entry->id;
            midCode.outCode(QuaternionItem::MINUS, arg1, arg2, res);
            break;
        }
        case 92: { // Simple_expression0 -> Simple_expression1 + Term
            auto Simple_expression1 = vectorAttribute[top - 2];
            auto Term = vectorAttribute[top];
            Type *resultType{nullptr};
            TempVar *entry{nullptr};
            string value;
            int flag;
            if (Simple_expression1.type->getType() == Type::INTEGER) {
                if (Term.type->getType() == Type::INTEGER)
                    flag = 1;
                else if (Term.type->getType() == Type::REAL)
                    flag = 2;
                else
                    flag = 0;
            } else if (Simple_expression1.type->getType() == Type::REAL) {
                if (Term.type->getType() == Type::INTEGER)
                    flag = 3;
                else if (Term.type->getType() == Type::REAL)
                    flag = 4;
                else
                    flag = 0;
            } else
                flag = 0;

            switch (flag) {
                case 1: { // 都是INTEGER
                    resultType = new Type(Type::INTEGER);
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
                    resultType = new Type(Type::REAL);
                    if (!Simple_expression1.value.empty() && !Term.value.empty()) {
                        float a = stof(Simple_expression1.value);
                        float b = stof(Term.value);
                        value = to_string(a + b);
                    }
                    break;
                }
                default: {
                    resultType = new Type(Type::TYPE_ERROR);
                    recordSemanticError(Simple_expression1.line, "运算符+：运算对象类型错误");
                    break;
                }
            }
            entry = midCode.newTemp();
            entry->type = resultType;
            entry->value = value;
            leftSymbol->type = resultType;
            leftSymbol->value = value;
            leftSymbol->entry = entry;
            string arg1, arg2, res;
            arg1 = Simple_expression1.entry->id;
            arg2 = Term.entry->id;
            res = entry->id;
            midCode.outCode(QuaternionItem::ADD, arg1, arg2, res);
            break;
        }
        case 93: { // Simple_expression0 -> Simple_expression1 - Term
            auto Simple_expression1 = vectorAttribute[top - 2];
            auto Term = vectorAttribute[top];
            Type *resultType{nullptr};
            TempVar *entry{nullptr};
            string value;
            int flag;
            if (Simple_expression1.type->getType() == Type::INTEGER) {
                if (Term.type->getType() == Type::INTEGER)
                    flag = 1;
                else if (Term.type->getType() == Type::REAL)
                    flag = 2;
                else
                    flag = 0;
            } else if (Simple_expression1.type->getType() == Type::REAL) {
                if (Term.type->getType() == Type::INTEGER)
                    flag = 3;
                else if (Term.type->getType() == Type::REAL)
                    flag = 4;
                else
                    flag = 0;
            } else
                flag = 0;

            switch (flag) {
                case 1: { // 都是INTEGER
                    resultType = new Type(Type::INTEGER);
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
                    resultType = new Type(Type::REAL);
                    if (!Simple_expression1.value.empty() && !Term.value.empty()) {
                        float a = stof(Simple_expression1.value);
                        float b = stof(Term.value);
                        value = to_string(a - b);
                    }
                    break;
                }
                default: {
                    resultType = new Type(Type::TYPE_ERROR);
                    recordSemanticError(Simple_expression1.line, "运算符-：运算对象类型错误");
                    break;
                }
            }
            leftSymbol->type = resultType;
            leftSymbol->value = value;
            entry = midCode.newTemp();
            entry->type = resultType;
            entry->value = value;
            leftSymbol->entry = entry;
            string arg1, arg2, res;
            arg1 = Simple_expression1.entry->id;
            arg2 = Term.entry->id;
            res = entry->id;
            midCode.outCode(QuaternionItem::MINUS, arg1, arg2, res);
            break;
        }
        case 94: { // Simple_expression0 -> Simple_expression1 or Term
            auto Simple_expression1 = vectorAttribute[top - 2];
            auto Term = vectorAttribute[top];
            Type *resultType{nullptr};
            TempVar *entry{nullptr};
            string value;
            if (Simple_expression1.type->getType() == Type::BOOLEAN &&
                Term.type->getType() == Type::BOOLEAN) {
                resultType = new Type(Type::BOOLEAN);
                if (!Simple_expression1.value.empty() && !Term.value.empty()) {
                    int a = stoi(Simple_expression1.value);
                    int b = stoi(Term.value);
                    value = to_string(a || b);
                }
            } else {
                resultType = new Type(Type::TYPE_ERROR);
                recordSemanticError(Simple_expression1.line, "运算符or：运算对象类型错误");
            }
            leftSymbol->type = resultType;
            leftSymbol->value = value;
            entry = midCode.newTemp();
            entry->type = resultType;
            entry->value = value;
            leftSymbol->entry = entry;
            string arg1, arg2, res;
            arg1 = Simple_expression1.entry->id;
            arg2 = Term.entry->id;
            res = entry->id;
            midCode.outCode(QuaternionItem::OR, arg1, arg2, res);
            break;
        }
        case 95: { // Term0 -> Term1 * Factor
            auto Term1 = vectorAttribute[top - 2];
            auto Factor = vectorAttribute[top];
            Type *resultType{nullptr};
            TempVar *entry{nullptr};
            string value;
            int flag;
            if (Term1.type->getType() == Type::INTEGER) {
                if (Factor.type->getType() == Type::INTEGER)
                    flag = 1;
                else if (Factor.type->getType() == Type::REAL)
                    flag = 2;
                else
                    flag = 0;
            } else if (Term1.type->getType() == Type::REAL) {
                if (Factor.type->getType() == Type::INTEGER)
                    flag = 3;
                else if (Factor.type->getType() == Type::REAL)
                    flag = 4;
                else
                    flag = 0;
            } else
                flag = 0;

            switch (flag) {
                case 1: { // 都是INTEGER
                    resultType = new Type(Type::INTEGER);
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
                    resultType = new Type(Type::REAL);
                    if (!Term1.value.empty() && !Factor.value.empty()) {
                        float a = stof(Term1.value);
                        float b = stof(Factor.value);
                        value = to_string(a * b);
                    }
                    break;
                }
                default: {
                    resultType = new Type(Type::TYPE_ERROR);
                    recordSemanticError(Term1.line, "运算符*：运算对象类型错误");
                    break;
                }
            }
            leftSymbol->type = resultType;
            leftSymbol->value = value;
            entry = midCode.newTemp();
            entry->type = resultType;
            entry->value = value;
            leftSymbol->entry = entry;
            string arg1, arg2, res;
            arg1 = Term1.entry->id;
            arg2 = Factor.entry->id;
            res = entry->id;
            midCode.outCode(QuaternionItem::MULTIPLY, arg1, arg2, res);
            break;
        }
        case 96: { // Term0 -> Term1 / Factor
            auto Term1 = vectorAttribute[top - 2];
            auto Factor = vectorAttribute[top];
            Type *resultType{nullptr};
            TempVar *entry{nullptr};
            string value;
            int flag;
            if ((Term1.type->getType() == Type::INTEGER || Term1.type->getType() == Type::REAL) &&
                (Factor.type->getType() == Type::INTEGER || Factor.type->getType() == Type::REAL)) {
                if (!Term1.value.empty() && !Factor.value.empty()) {
                    float a = stof(Term1.value);
                    float b = stof(Factor.value);
                    if (b != 0) {
                        resultType = new Type(Type::REAL);
                        value = to_string(a / b);
                    } else {
                        resultType = new Type(Type::TYPE_ERROR);
                        recordSemanticError(Term1.line, "除0错误");
                    }
                } else
                    resultType = new Type(Type::REAL);
            } else {
                resultType = new Type(Type::TYPE_ERROR);
                recordSemanticError(Term1.line, "运算符/：运算对象类型错误");
            }
            leftSymbol->type = resultType;
            leftSymbol->value = value;
            entry = midCode.newTemp();
            entry->type = resultType;
            entry->value = value;
            leftSymbol->entry = entry;
            string arg1, arg2, res;
            arg1 = Term1.entry->id;
            arg2 = Factor.entry->id;
            res = entry->id;
            midCode.outCode(QuaternionItem::DIVIDE, arg1, arg2, res);
            break;
        }
        case 97: { // Term0 -> Term1 div Factor
            auto Term1 = vectorAttribute[top - 2];
            auto Factor = vectorAttribute[top];
            Type *resultType{nullptr};
            TempVar *entry{nullptr};
            string value;
            if (Term1.type->getType() == Type::INTEGER &&
                Factor.type->getType() == Type::INTEGER) {
                if (!Term1.value.empty() && !Factor.value.empty()) {
                    int a = stoi(Term1.value);
                    int b = stoi(Factor.value);
                    if (b != 0) {
                        resultType = new Type(Type::INTEGER);
                        value = to_string(a / b);
                    } else {
                        resultType = new Type(Type::TYPE_ERROR);
                        recordSemanticError(Term1.line, "除0错误");
                    }
                } else
                    resultType = new Type(Type::INTEGER);
            } else {
                resultType = new Type(Type::TYPE_ERROR);
                recordSemanticError(Term1.line, "运算符div：运算对象类型错误");
            }
            leftSymbol->type = resultType;
            leftSymbol->value = value;
            entry = midCode.newTemp();
            entry->type = resultType;
            entry->value = value;
            leftSymbol->entry = entry;
            string arg1, arg2, res;
            arg1 = Term1.entry->id;
            arg2 = Factor.entry->id;
            res = entry->id;
            midCode.outCode(QuaternionItem::FLOOR_DIVIDE, arg1, arg2, res);
            break;
        }
        case 98: { // Term0 -> Term1 mod Factor
            auto Term1 = vectorAttribute[top - 2];
            auto Factor = vectorAttribute[top];
            Type *resultType{nullptr};
            string value;
            if (Term1.type->getType() == Type::INTEGER &&
                Factor.type->getType() == Type::INTEGER) {
                if (!Term1.value.empty() && !Factor.value.empty()) {
                    int a = stoi(Term1.value);
                    int b = stoi(Factor.value);
                    if (b != 0) {
                        resultType = new Type(Type::INTEGER);
                        value = to_string(a % b);
                    } else {
                        resultType = new Type(Type::TYPE_ERROR);
                        recordSemanticError(Term1.line, "除0错误");
                    }
                } else
                    resultType = new Type(Type::INTEGER);
            } else {
                resultType = new Type(Type::TYPE_ERROR);
                recordSemanticError(Term1.line, "运算符mod：运算对象类型错误");
            }
            leftSymbol->type = resultType;
            leftSymbol->value = value;
            TempVar *entry{nullptr};
            entry = midCode.newTemp();
            entry->type = resultType;
            entry->value = value;
            leftSymbol->entry = entry;
            string arg1, arg2, res;
            arg1 = Term1.entry->id;
            arg2 = Factor.entry->id;
            res = entry->id;
            midCode.outCode(QuaternionItem::MOD, arg1, arg2, res);
            break;
        }
        case 99: { // Term0 -> Term1 and Factor
            auto Term1 = vectorAttribute[top - 2];
            auto Factor = vectorAttribute[top];
            Type *resultType{nullptr};
            string value;
            if (Term1.type->getType() == Type::BOOLEAN &&
                Factor.type->getType() == Type::BOOLEAN) {
                resultType = new Type(Type::BOOLEAN);
                if (!Term1.value.empty() && !Factor.value.empty()) {
                    int a = stoi(Term1.value);
                    int b = stoi(Factor.value);
                    value = to_string(a && b);
                }
            } else {
                resultType = new Type(Type::TYPE_ERROR);
                recordSemanticError(Term1.line, "运算符and：运算对象类型错误");
            }
            leftSymbol->type = resultType;
            leftSymbol->value = value;
            TempVar *entry{nullptr};
            entry = midCode.newTemp();
            entry->type = resultType;
            entry->value = value;
            leftSymbol->entry = entry;
            string arg1, arg2, res;
            arg1 = Term1.entry->id;
            arg2 = Factor.entry->id;
            res = entry->id;
            midCode.outCode(QuaternionItem::AND, arg1, arg2, res);
            break;
        }
        case 100: { //Term -> Factor
            auto Factor = vectorAttribute[top];
            leftSymbol->type = Factor.type;
            leftSymbol->value = Factor.value;
            leftSymbol->entry = Factor.entry;
            leftSymbol->tableLineEntry = Factor.tableLineEntry;
            leftSymbol->variableName = Factor.variableName;
            break;
        }
        case 101: { //Factor -> Unsign_const_variable
            auto Unsign_const_variable = vectorAttribute[top];
            leftSymbol->type = Unsign_const_variable.type;
            leftSymbol->value = Unsign_const_variable.value;
            leftSymbol->entry = Unsign_const_variable.entry;
            leftSymbol->variableName = Unsign_const_variable.variableName;
            leftSymbol->tableLineEntry = Unsign_const_variable.tableLineEntry;
            break;
        }
        case 102: { // Factor -> Variable
            auto Variable = vectorAttribute[top];
            auto entry = Variable.entry;
            string value;
            leftSymbol->type = Variable.type;
            leftSymbol->value = value;
            leftSymbol->tableLineEntry = Variable.tableLineEntry;
            leftSymbol->variableName = Variable.variableName;
            leftSymbol->entry = Variable.entry;
            break;
        }
        case 103: { //103. Factor -> id ( Expression_list )
            auto id = &vectorAttribute[top - 3];
            int line = vectorAttribute[top - 3].line;
            quoteID(line, id->attribute);
            SymbolTableLine *tempLinePoint = curBlock->query(id->attribute);
            Type *reType{nullptr}; // 函数返回值
            if (tempLinePoint != nullptr) {//如果存在
                if (tempLinePoint->type->getType() == Type::FUNC) {
                    auto func = (Func *) tempLinePoint->type;
                    int num = func->parametersNum;
                    auto Expression_list = vectorAttribute[top - 1];
                    if (num == Expression_list.typeList.size()) {
                        bool flag = true;
                        for (int i = 0; i < Expression_list.typeList.size(); ++i) {
                            auto type1 = Expression_list.typeList[i];
                            auto type2 = func->env[i].type;
                            if (*type1 != *type2) {
                                recordSemanticError(line, "语义错误！第" + to_string(i + 1) + "个参数类型不一致");
                                flag = false;
                            }
                        }
                        if (flag)
                            reType = func->reType;
                        else
                            reType = new Type(Type::TYPE_ERROR);
                    } else {
                        reType = new Type(Type::TYPE_ERROR);
                        recordSemanticError(line,
                                            "语义错误！参数不一致,此Func需要" +
                                            to_string(num) +
                                            "个参数");
                    }
                } else {
                    reType = new Type(Type::TYPE_ERROR);
                    recordSemanticError(line, "语义错误！此id不为Function");
                }
            } else {
                reType = new Type(Type::TYPE_ERROR);
                recordSemanticError(line, "语义错误！引用了不存在的标识符" + id->attribute);
            }
            leftSymbol->type = reType;
            ////
            auto Expression_list = &vectorAttribute[top - 1];

            string arg1, arg2, res;

            if (id->entry) {
//                leftSymbol->entry = id->entry;
            } else {
                TempVar *entry{nullptr};
                entry = midCode.newTemp();
                entry->value = id->attribute;
                entry->type = id->type;
                id->tableLineEntry = curBlock->query(id->attribute);
                entry->tableLineEntry = id->tableLineEntry;
                id->entry = entry;
//                leftSymbol->entry = id->entry;
                arg1 = id->entry->id;
            }
            for (auto &i : Expression_list->queue) {
                string argb1, argb2, resb;
                argb1 = i;
                midCode.outCode(QuaternionItem::PARAM, argb1, argb2, resb);
            }
            arg2 = to_string(Expression_list->queue.size());
            midCode.outCode(QuaternionItem::CALL, arg1, arg2, res);
            string argc1, argc2, resc;
            TempVar *entryb{nullptr};
            entryb = midCode.newTemp();
            entryb->type = id->type;
            leftSymbol->entry = entryb;
            argc1 = entryb->id;
            midCode.outCode(QuaternionItem::RETURN, argc1, argc2, resc);
            break;
        }
        case 104: { // Factor -> ( Expression )
            auto Expression = vectorAttribute[top - 1];
            leftSymbol->type = Expression.type;
            leftSymbol->value = Expression.value;
            leftSymbol->entry = Expression.entry;
            break;
        }
        case 105: { // Factor0 -> not Factor1
            auto Factor1 = vectorAttribute[top];
            Type *resultType{nullptr};
            string value;
            if (Factor1.type->getType() == Type::BOOLEAN) {
                resultType = new Type(Type::BOOLEAN);
                if (!Factor1.value.empty()) {
                    if (Factor1.value == "1")
                        value = "0";
                    else
                        value = "1";
                }
            } else {
                resultType = new Type(Type::TYPE_ERROR);
                recordSemanticError(Factor1.line, "逻辑运算符not：操作数类型错误");
            }
            leftSymbol->type = resultType;
            leftSymbol->value = value;
            TempVar *entry{nullptr};
            entry = midCode.newTemp();
            entry->type = resultType;
            entry->value = value;
            leftSymbol->entry = entry;
            string arg1, arg2, res;
            arg1 = Factor1.entry->id;
            res = entry->id;
            midCode.outCode(QuaternionItem::NOT, arg1, arg2, res);
            break;
        }
        case 106: {  //106. Unsign_const_variable -> id
            string id = vectorAttribute[top].attribute;
            int line = vectorAttribute[top].line;
            quoteID(line, id);  //查看是否存在这个id
            SymbolTableLine *tempLinePoint = curBlock->query(id);
            if (tempLinePoint != nullptr) {//如果存在
                leftSymbol->type = tempLinePoint->type;
                leftSymbol->tableLineEntry = tempLinePoint;
                leftSymbol->variableName = tempLinePoint->name;

                TempVar *entry{nullptr};
                entry = midCode.newTemp();
                entry->value = leftSymbol->variableName;
                entry->type = leftSymbol->type;
                entry->tableLineEntry = leftSymbol->tableLineEntry;
                leftSymbol->entry = entry;
            } else {
                leftSymbol->type = new Type(Type::TYPE_ERROR);
                leftSymbol->tableLineEntry = nullptr;
                recordSemanticError(line, "语义错误！引用了未定义的id");
            }
            break;
        }
        case 107: { // Unsign_const_variable -> num
            auto num = vectorAttribute[top];
            bool isRealType = false;
            Type *type;
            for (auto c : num.attribute) {
                if (c == '.') //如果含有小数点就说明是实数类型
                {
                    isRealType = true;
                    break;
                }
            }
            if (isRealType) {
                type = new Type(Type::REAL);
            } else {
                type = new Type(Type::INTEGER);
            }
            leftSymbol->value = num.attribute;
            leftSymbol->variableName = num.attribute;
            leftSymbol->type = type;
            auto var = midCode.newTemp(true);
            var->type = type;
            var->value = num.attribute;
            leftSymbol->entry = var;
            break;
        }
        case 108: { // Unsign_const_variable -> letter
            auto letter = vectorAttribute[top];
            auto type = new Type(Type::CHAR);
            leftSymbol->type = type;
            leftSymbol->value = letter.attribute;
            auto var = midCode.newTemp(true);
            var->type = type;
            var->value = letter.attribute;
            leftSymbol->entry = var;
            break;
        }
        case 109: // 109. A2 -> #
        case 110: // 110. B40 -> #
        case 111: { //111. C41 -> #
            string id = vectorAttribute[top].attribute;
            int line = vectorAttribute[top].line;
            if (curBlock->blockQuery(id) == nullptr) {
                curBlock->insert(id, nullptr, 0, 0, 0);
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
        case 112: // 112. D22 -> #
            locate();
            break;
        case 113: { // E62 -> #
            vectorAttributeItem *t = &vectorAttribute[top];
            auto entry = curBlock->query(t->attribute);
            if (entry == nullptr) {
                t->tableLineEntry = nullptr;
                leftSymbol->tableLineEntry = nullptr;
                t->type = new Type(Type::TYPE_ERROR);
                leftSymbol->type = new Type(Type::TYPE_ERROR);
                recordSemanticError(t->line, "未定义的引用");
                break;
            } else {
                t->type = entry->type;
                leftSymbol->type = entry->type;
                t->tableLineEntry = entry;
                leftSymbol->tableLineEntry = entry;
            }
            break;
        }
        case 114: { //114. M -> #
            leftSymbol->quad = midCode.codeList.size();
            break;
        }
        case 115: { //115. N -> #
            leftSymbol->nextList.push_back(midCode.codeList.size());
            string arg1, arg2, res;
            midCode.outCode(QuaternionItem::GOTO, arg1, arg2, res);
            break;
        }
        case 116: { //116. W -> #
            leftSymbol->testList.push_back(midCode.codeList.size());
            string arg1, arg2, res;
            midCode.outCode(QuaternionItem::GOTO, arg1, arg2, res);
            //为了配合case的Expression和Branch_list位置都为top-5问题
            //这里把Expression的type同时赋值给W
            leftSymbol->type = vectorAttribute[top].type;
            break;
        }
        case 117: { //117. U -> #
            leftSymbol->caseList.push_back(midCode.codeList.size());
            break;
        }
        case 118: { //118. F56 -> #
            vectorAttributeItem *Expression = &vectorAttribute[top];
            Expression->trueList.push_back(midCode.codeList.size());
            Expression->falseList.push_back(midCode.codeList.size() + 1);

            string arg1, arg2, res;
            if (Expression->entry) {
                arg1 = Expression->entry->id;
            } else {
                TempVar *entry{nullptr};
                entry = midCode.newTemp();
                entry->value = Expression->value;
                entry->type = Expression->type;
                entry->tableLineEntry = Expression->tableLineEntry;
                Expression->entry = entry;
                arg1 = Expression->entry->id;
//                arg1 = Expression.tableLineEntry->name;
            }

            midCode.outCode(QuaternionItem::IF, arg1, arg2, res);
            string argb1, argb2, resb;
            midCode.outCode(QuaternionItem::GOTO, argb1, argb2, resb);
//            printf("%p,%lu\n", &Expression, Expression->trueList.size());
            break;
        }
        case 119: { //G58 -> #
            auto Expression = &vectorAttribute[top];
            Expression->trueList.push_back(midCode.codeList.size());
            Expression->falseList.push_back(midCode.codeList.size() + 1);

            string arg1, arg2, res;
            if (Expression->entry) {
                arg1 = Expression->entry->id;
            } else {
                TempVar *entry{nullptr};
                entry = midCode.newTemp();
                entry->value = Expression->value;
                entry->type = Expression->type;
                entry->tableLineEntry = Expression->tableLineEntry;
                Expression->entry = entry;
                arg1 = Expression->entry->id;
//                arg1 = Expression.tableLineEntry->name;
            }
            midCode.outCode(QuaternionItem::IF, arg1, arg2, res);
            string argb1, argb2, resb;
            midCode.outCode(QuaternionItem::GOTO, argb1, argb2, resb);
            break;
        }
        case 120: { //120. H59 -> #
            auto Expression = &vectorAttribute[top];
            Expression->falseList.push_back(midCode.codeList.size() + 1);
            string arg1, arg2, res;
            if (Expression->entry) {
                arg1 = Expression->entry->id;
            } else {
                TempVar *entry{nullptr};
                entry = midCode.newTemp();
                entry->value = Expression->value;
                entry->type = Expression->type;
                entry->tableLineEntry = Expression->tableLineEntry;
                Expression->entry = entry;
                arg1 = Expression->entry->id;
//                arg1 = Expression.tableLineEntry->name;
            }
            TempVar *entry{nullptr};
            entry = midCode.newTemp();
            string value;
            if (Expression->value == "1")
                value = "0";
            else
                value = "1";
            entry->value = value;
            entry->type = new Type(Type::BOOLEAN);
            leftSymbol->entry = entry;
            res = entry->id;
            midCode.outCode(QuaternionItem::NOT, arg1, arg2, res);
            string argb1, argb2, resb;
            argb1 = res;
            midCode.outCode(QuaternionItem::IF, argb1, argb2, resb);
            break;
        }
        case 121: { //121. I60 -> #
            auto Expression1 = &vectorAttribute[top];
            auto id = &vectorAttribute[top - 5];
            auto Updown = &vectorAttribute[top - 2];
            auto Expression0 = &vectorAttribute[top - 3];


            Expression1->trueList.push_back(midCode.codeList.size() + 1);
            Expression1->falseList.push_back(midCode.codeList.size() + 2);
            if (Updown->value == "to") {
                TempVar *entry{nullptr};
                entry = midCode.newTemp();
//                entry->value = value;
                entry->type = new Type(Type::BOOLEAN);
                string arg1, arg2, res;

                if (id->entry) {
                    res = id->entry->id;
                } else {
                    TempVar *entryb{nullptr};
                    entryb = midCode.newTemp();
                    entryb->value = id->attribute;
                    entryb->type = id->type;
                    id->tableLineEntry = curBlock->query(id->attribute);
                    entryb->tableLineEntry = id->tableLineEntry;
                    id->entry = entryb;
                    res = id->entry->id;
                }
                arg1 = id->entry->id;
                arg2 = Expression1->entry->id;
                res = entry->id;
                midCode.outCode(QuaternionItem::LESS_EQUAL, arg1, arg2, res);
                string argb1, argb2, resb3;
                argb1 = res;
                midCode.outCode(QuaternionItem::IF, argb1, argb2, resb3);
            } else if (Updown->value == "downto") {
                TempVar *entry{nullptr};
                entry = midCode.newTemp();
//                entry->value = value;
                entry->type = new Type(Type::BOOLEAN);
                string arg1, arg2, res;

                if (id->entry) {
                    res = id->entry->id;
                } else {
                    TempVar *entryb{nullptr};
                    entryb = midCode.newTemp();
                    entryb->value = id->attribute;
                    entryb->type = id->type;
                    id->tableLineEntry = curBlock->query(id->attribute);
                    entryb->tableLineEntry = id->tableLineEntry;
                    id->entry = entryb;
                    res = id->entry->id;
                }
                arg1 = id->entry->id;
                arg2 = Expression1->entry->id;
                res = entry->id;
                midCode.outCode(QuaternionItem::MORE_EQUAL, arg1, arg2, res);
                string argb1, argb2, resb3;
                argb1 = res;
                midCode.outCode(QuaternionItem::IF, argb1, argb2, resb3);
            }
            string argc1, argc2, resc;
            midCode.outCode(QuaternionItem::GOTO, argc1, argc2, resc);
            break;
        }
        case 122: { //122. Statement_body -> Compound_statement
            auto Compound_statement = vectorAttribute[top];
            leftSymbol->startQuad = Compound_statement.startQuad;
            relocate();
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
    childBlock->level = 1;
    if (curBlock != nullptr)
        childBlock->level = curBlock->level + 1;

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
    childBlock = SymbolBlock::makeBlock(curBlock);//初始化最外层的符号表
    curBlock = childBlock;
    startBlock = childBlock;
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

SymbolTableLine *LR1Runner::newTemp() {
    char t[10];
    snprintf(t, sizeof(t), "$%d", tempID++);
    string name;
    name.append(t);
    return curBlock->insert2(name, nullptr, 0, 0, 0);
}

LR1Runner::LR1Runner() {
    tempID = 0;
}

Type *LR1Runner::getType(SymbolTableLine *p) {
    return p->type;
}

void LR1Runner::printMidCode() {
    if (debugInfoLevel >= 1 && semanticError.empty()) {
        midCode.print();
    }
}

vector<SymbolTableLine *> LR1Runner::generateProFunVector(SymbolBlock *startPoint) {
    vector<SymbolTableLine *> generateVector;
    for (auto &i : startPoint->symbolBlock) {
        if (i->type && (i->type->getType() == Type::PROC || i->type->getType() == Type::FUNC ||
                        i->type->getType() == Type::PROGRAM)) {
            vector<SymbolTableLine *> tempVector = generateProFunVector(i->blockPoint);
            generateVector.insert(generateVector.end(), tempVector.begin(), tempVector.end());
            generateVector.push_back(i);
        }
    }
    return generateVector;
}
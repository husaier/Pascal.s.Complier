//
// Created by husserl on 2020/5/23.
//

#include "LR1TableMaker.h"

void LR1TableMaker::load(const string& fileName) {
    initial();
    fin.open(fileName);

    if (!fin.is_open())
        cout<<"打开文件失败！"<<endl;

    int count = 0;
    while (!fin.eof()) {
        string t;
        getline(fin, t);
        productions.push_back(parseProduction(t));
    }
    fin.close();

    recognize();

    firstSetCalculator.initial(Vn, Vt, productions);
}

LR1Table LR1TableMaker::makeTable() {
    LR1Table table;
    extendGrammar();
    table.productions = this->productions;
    packSymbols(table);
    //构造有效项目集规范族
    vector<LR1Group> family;
    makeFamily(family);
    for(const auto& group : family) {
        //构造新的一行
        vector<TableItem> newRow;
        for (int j = 0; j < table.colNum; ++j) {
            TableItem tmp(TableItem::ERROR, 0);
            newRow.push_back(tmp);
        }
        //根据项目集填入规约表项
        for (const auto& item : group.items) {
            Production p = item.production;
            if (item.reductionFlag) {
                int proIndex = findProduction(p);
                int symIndex = table.invMap[item.tail];
                if(newRow[symIndex].type == TableItem::ERROR) {
                    if(p.left == EXTENDED_BEGIN)
                        newRow[symIndex].type = TableItem::ACC;
                    else
                        newRow[symIndex].type = TableItem::REDUCTION;
                    newRow[symIndex].index = proIndex;
                }
                else {
                    cout<<"出错了！LR1分析表存在多重定义的表项\n";
                }
            }
        }
        //填入SHIFT和GOTO类型的表项
        for (const auto& edge : group.others) {
            int symIndex = table.invMap[edge.condition];
            int groupIndex = edge.target;
            if (newRow[symIndex].type == TableItem::ERROR){
                if (symIndex <= table.dividingIndex)
                    newRow[symIndex].type = TableItem::SHIFT;
                else
                    newRow[symIndex].type = TableItem::GOTO;
                newRow[symIndex].index = groupIndex;
            }
            else {
                cout<<"出错了！LR1分析表存在多重定义的表项\n";
            }
        }
        table.table.push_back(newRow);
    }
    table.rowNum = family.size();
    return table;
}

void LR1TableMaker::initial() {
    productions.clear();
    Vn.clear();
    Vt.clear();
}

Production LR1TableMaker::parseProduction(const string& str) {
    //将字符串读到input中
    stringstream input(str);
    //暂存从word中读取的字符串
    string result;
    Production p;

    input >> p.left; //把左部字符串存入p[i]
    input >> result;    //过滤掉->

    //依次输出到result中，并存入res中
    while (input >> result)
        p.right.push_back(result);
    return p;
}

void LR1TableMaker::recognize() {
    for (auto & production : productions) //第i个产生式
    {
        if (count(Vn.begin(), Vn.end(), production.left) == 0)  //Vn里没有找到第i个产生式的左部，则添加
            Vn.push_back(production.left);

        for (auto & item : production.right)   //对于产生式右部
        {
            if (item[0] >= 'A' && item[0] <= 'Z') { //产生式右部的符号，第一个字母大写为非终结符
                if (count(Vn.begin(), Vn.end(), item) == 0)  //Vn里没有找到该符号，则添加
                    Vn.push_back(item);
            }
            else { //产生式右部的符号，第一个字母小写为终结符
                if (count(Vt.begin(), Vt.end(), item) == 0)  //Vt里没有找到该符号，则添加
                    Vt.push_back(item);
            }
        }
    }
}

vector<LR1TableMaker::LR1Item> LR1TableMaker::closure(const LR1Item& origin){
    vector<LR1Item> result;
    result.push_back(origin);
    while (true){
        vector<LR1Item> Jnew = result;
        int changeFlag = false; //本轮循环是否有变动
        for (int i = 0 ; i < Jnew.size(); i++) { //每一个项目
            LR1Item item = Jnew[i];
            Production p = item.production;
            int pos = item.pos;
            string tail = item.tail;
            bool reductionFlag = item.reductionFlag;
            if (reductionFlag) //如果是规约项目，直接跳过
                continue;
            string B=p.right[pos];
            if (isVt(B)) //如果B是终结符，直接跳过
                continue;
            vector<string> beta;
            vector<string> firstSet;
            //判断beta是否为空
            if (p.right.size() - pos == 1) {
                beta.push_back(tail);
            }
            else{
                for (int j = pos + 1; j < p.right.size(); j++)
                    beta.push_back(p.right[j]);
                beta.push_back(tail);
            }
            firstSet = calculateFirst(beta);

            // 遍历产生式列表
            for (const auto& pro : productions) {
                if (pro.left == B){
                    for (auto &symbol : firstSet) {
                        LR1Item project(pro, 0, symbol);
                        //判断是否是空产生式，空产生式满足规约条件
                        project.reductionFlag = (pro.right.size() == 1 && pro.right[0] == "#");
                        //判断新项目是否已经在项目集中
                        int flag = false;
                        for (const auto& tmp : Jnew) {
                            if (tmp == project){
                                flag = true;
                                break;
                            }
                        }
                        //如果不在项目集中
                        if (! flag) {
                            Jnew.push_back(project);
                            changeFlag = true;
                        }
                    }
                }
            }
        }
        if (! changeFlag) //如果没有变动，就结束
            break;
        else
            result = Jnew;
    }
    return result;
}

bool LR1TableMaker::isVt(string s) {
    for(const auto& symbol : Vt) {
        if (s == symbol)
            return true;
    }
    return false;
}

//void LR1TableMaker::printOut() {
//    for (auto & production : productions) {
//        cout<<production.left<<" -> ";
//        for(auto & item : production.right)
//            cout<<item<<' ';
//        cout<<'\n';
//    }
//    cout<<"****************************************\n";
//    cout<<"非终结符："<<Vn.size()<<"个\n";
//    for (const auto& symbol : Vn)
//        cout<<symbol<<'\n';
//
//    cout<<"*******************************************\n";
//    cout<<"终结符："<<Vt.size()<<"个\n";
//    for (const auto& symbol : Vt)
//        cout<<symbol<<'\n';
//}
//
//void LR1TableMaker::start() {
//    load("grammar.txt");
//    firstSetCalculator.initial(Vn, Vt, productions);
//
//    extendGrammar();
//
//    vector<LR1Item> tmp;
//    LR1Item s(productions[3], 0, END);
//    s.reductionFlag = false;
//    tmp = closure(s);
//
//
////    cout<<"非终结符：\n";
////    for (auto & symbol : Vn) {
////        vector<string> set;
////        set = firstSetCalculator.parse(symbol);
////        cout<<"\n*************************************************************\n";
////        cout<<symbol<<"的first集：\n";
////        for (const auto& item : set) {
////            cout<<item<<'\n';
////        }
////    }
////    cout<<"\n终结符：\n";
////    for (auto & symbol : Vt) {
////        vector<string> set;
////        set = firstSetCalculator.parse(symbol);
////        cout<<"\n*************************************************************\n";
////        cout<<symbol<<"的first集：\n";
////        for (const auto& item : set) {
////            cout<<item<<'\n';
////        }
////    }
////
////    vector<string> set;
////    set = firstSetCalculator.parse(END);
////    cout<<"\n*************************************************************\n";
////    cout<<END<<"的first集：\n";
////    for (const auto& item : set) {
////        cout<<item<<'\n';
////    }
//
//}

vector<string> LR1TableMaker::calculateFirst(const vector<string>& symbols) {
    vector<string> result;
    for (auto &symbol : symbols) {
        vector<string> tmp;
        tmp = firstSetCalculator.parse(symbol);
        mergeFirstSet(result, tmp);
        if (!hasEpsilon(tmp))
            break;
    }
    return result;
}

bool LR1TableMaker::hasEpsilon(vector<string> t) {
    for (const auto& symbol : t) {
        if (symbol == "#")
            return true;
    }
    return false;
}

void LR1TableMaker::mergeFirstSet(vector<string> &target, vector<string> &source) {
    for (const auto& symbol : source)
        target.push_back(symbol);
    //去重
    set<string> s(target.begin(), target.end());
    target.assign(s.begin(), s.end());
    //去空
    vector<string>::iterator i;
    for (i = target.begin(); i != target.end(); ++i) {
        if (*i == "#"){
            target.erase(i);
            break;
        }
    }
}

void LR1TableMaker::extendGrammar() {
    Production pro;
    string newVn = "S'";
    pro.left = newVn;
    pro.right.push_back(productions[0].left);
    productions.insert(productions.cbegin(), pro);
    Vn.insert(Vn.begin(), newVn);
}

vector<string> LR1TableMaker::findUsefulSymbol(const LR1Group &group) {
    vector<LR1Item> items = group.items;
    vector<string> symbols;
    for (const auto& item : items) {
        if(item.reductionFlag) //可归约项目
            continue;
        Production pro = item.production;
        int pos = item.pos;
        string symbol;
        symbol = pro.right[pos];
        //去重
        int flag = false;
        for (const auto& tmp : symbols)
            if (tmp == symbol) {
                flag = true;
                break;
            }
        if(!flag) {
            symbols.push_back(symbol);
        }
    }
    return symbols;
}

LR1TableMaker::LR1Group LR1TableMaker::groupGoTo(const LR1Group& I, const string& X) {
    LR1Group group = I;
    vector<LR1Item> newItems;
    for (const auto& item : I.items) {
        if(item.reductionFlag) //可归约项目
            continue;
        Production pro = item.production;
        int pos = item.pos;
        string symbol;
        symbol = pro.right[pos];
        if (symbol == X) {
            vector<LR1Item> newG;
            LR1Item tmp = item;
            tmp.pos = tmp.pos + 1;

            tmp.reductionFlag = isReductable(tmp);

            newG = closure(tmp);

            mergeGroup(newItems, newG);
        }
    }
    group.items = newItems;
    return group;
}

bool LR1TableMaker::isReductable(const LR1TableMaker::LR1Item& item) {
    return item.production.right.size() == item.pos;
}

void LR1TableMaker::mergeGroup(vector<LR1Item> &target, vector<LR1Item> &source) {
    for (const auto& item : source) {
        int flag = false;
        for(const auto& tmp : target) {
            if (item == tmp){
                flag = true;
                break;
            }
        }
        if (!flag) {
            target.push_back(item);
        }
    }
}

void LR1TableMaker::packSymbols(LR1Table & table) {
    vector<string> tableSymbols;
    map<string, int> invMap;
    int count =0;
    for (auto & s : Vt) {
        if (s != "#" && s != "$") {
            tableSymbols.push_back(s);
            invMap.insert(map<string, int>::value_type(s, count));
            count++;
        }
    }
    tableSymbols.push_back(END);
    invMap.insert(map<string, int>::value_type(END, count));
    table.dividingIndex = count;
    count++;
    for (auto &s :Vn) {
        if (s != "S'") {
            tableSymbols.push_back(s);
            invMap.insert(map<string, int>::value_type(s, count));
            count++;
        }
    }
    table.symbols = tableSymbols;
    table.invMap = invMap;
    table.colNum = count;
}

int LR1TableMaker::findProduction(const Production& p) {
    for (int i=0; i<productions.size(); i++) {
        if(productions[i] == p){
            return i;
        }
    }
    return -1;
}

void LR1TableMaker::makeFamily(vector<LR1Group> &family) {
    LR1Item t(productions[0], 0, END);
    LR1Group origin;
    origin.items = closure(t);
    family.push_back(origin);

    while (true) {
        bool changeFlag = false;
        for (int i=0; i < family.size(); i++){
            LR1Group group = family[i];

            vector<string> usefulSymbols;
            usefulSymbols = findUsefulSymbol(group);
            //遍历每一个可能跳转的符号
            for (const auto& symbol : usefulSymbols) {
                LR1Group newG;
                newG = groupGoTo(group, symbol);
                //项目集newG在项目集规范族C中是否出现过
                //不管newG是否出现过，tmp就是其下标
                bool isAppeared = false;
                int tmp = 0;
                for(tmp = 0; tmp < family.size(); tmp++)
                    if (family[tmp] == newG){
                        isAppeared = true;
                        break;
                    }
                //如果没有出现过，就添加
                if (!isAppeared) {
                    changeFlag = true;
                    family.push_back(newG);
                }
                //如果没有添加过，为原项目集group添加关系
                int flag = false;
                for (const auto& g : family[i].others)
                    if (g.target == tmp){
                        flag = true;
                        break;
                    }
                if (!flag) {
                    family[i].addEdge(tmp, symbol);
                }
            }
        }
        //如果规范族family不再变化则退出
        if (!changeFlag)
            break;
    }
}
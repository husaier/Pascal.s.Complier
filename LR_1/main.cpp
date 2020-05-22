#include "FirstSetCalculator.h"
#include <set>

using namespace std;

class LR1Item {
public:
    explicit LR1Item(Production p) {
        production = p;
        pos = 0;
        tail = "$";
        reductionFlag = false;
    }

    LR1Item(Production p, int position, string t) {
        production = p;
        pos = position;
        tail = t;
        reductionFlag = false;
    }

    bool operator== (LR1Item &t) const {
        return this->reductionFlag == t.reductionFlag
        && this->pos == t.pos
        && this->tail == t.tail
        && this->production == t.production;
    }

    Production production;
    int pos; //圆点的位置
    string tail; //向前看符号
    bool reductionFlag;
};

class LR1TableMaker {
public:
    ~LR1TableMaker() {
        fin.close();
    }

    void initial();

    void load(const string&);

    void start();

private:
    const int PRODUCTION_MAX_LENGTH = 200; //一个产生式最长200个字符

    ifstream fin;
    vector<Production> productions; //产生式
    vector<string> Vn; //非终结符
    vector<string> Vt; //终结符
    vector<string> Vn_recursive;           //带左递归的非终结符

    FirstSetCalculator firstSetCalculator;

    void Closure();

    void ProduceTable();

    //将产生式的左右部分别放入left，right
    Production parseProduction(string str);

    //识别终结符和非终结符
    void recognize();

    //计算闭包
    vector<LR1Item> closure(LR1Item);

    //判断是不是终结符
    bool isVt(string s);

    //计算符号串的first集
    vector<string> calculateFirst(const vector<string>& symbols);

    //判断first集是否含有ε
    static bool hasEpsilon(vector<string> t);

    //合并两个first集，把source中的元素合并到target，除去ε
    static void mergeFirstSet(vector<string> &target, vector<string> &source);

    //扩展文法
    void extendGrammar();

    //生成有效项目集规范族
    void makeLR1Table();

    void printOut();
};

void LR1TableMaker::initial() {
    productions.clear();
    Vn.clear();
    Vt.clear();
    Vn_recursive.clear();
}

void LR1TableMaker::load(const string& fileName) {
    fin.open(fileName);

    if (!fin.is_open())
        cout<<"打开文件失败！"<<endl;

    int count = 0;
    while (!fin.eof()) {
        string t;
        getline(fin, t);
        productions.push_back(parseProduction(t));
//        cout<<++count<<endl;
    }
    fin.close();

    recognize();
}

Production LR1TableMaker::parseProduction(string str) {
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

vector<LR1Item> LR1TableMaker::closure(LR1Item origin){
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
                        //判断是否是空产生式
                        if (pro.right.size() == 1 && pro.right[0] == "#")
                            project.reductionFlag = true;
                        else
                            project.reductionFlag = false;
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

void LR1TableMaker::start() {
    initial();
    load("grammar.txt");
    firstSetCalculator.initial(Vn, Vt, productions);

    extendGrammar();



//    vector<LR1Item> tmp;
//    LR1Item s(productions[2], 0, "$");
//    s.reductionFlag = false;
//    tmp = closure(s);


//    cout<<"非终结符：\n";
//    for (auto & symbol : Vn) {
//        vector<string> set;
//        set = firstSetCalculator.parse(symbol);
//        cout<<"\n*************************************************************\n";
//        cout<<symbol<<"的first集：\n";
//        for (const auto& item : set) {
//            cout<<item<<'\n';
//        }
//    }
//    cout<<"\n终结符：\n";
//    for (auto & symbol : Vt) {
//        vector<string> set;
//        set = firstSetCalculator.parse(symbol);
//        cout<<"\n*************************************************************\n";
//        cout<<symbol<<"的first集：\n";
//        for (const auto& item : set) {
//            cout<<item<<'\n';
//        }
//    }
//
//    vector<string> set;
//    set = firstSetCalculator.parse("$");
//    cout<<"\n*************************************************************\n";
//    cout<<"$"<<"的first集：\n";
//    for (const auto& item : set) {
//        cout<<item<<'\n';
//    }

}

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

void LR1TableMaker::makeLR1Table() {

}

int main() {
    LR1TableMaker parser;
    parser.initial();
    parser.start();

    return 0;
}
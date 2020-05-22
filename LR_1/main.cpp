#include "FirstSetCalculator.h"

using namespace std;

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

    //获取字符串在向量里面的位置，不存在则返回 -1
    int get_Index(string str, vector<string> vec);

    //计算first集
    void Letter_First(Production *p, string str);

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

int LR1TableMaker::get_Index(string str, vector<string> vec) {
    if (count(vec.begin(), vec.end(), str) != 0) {
        vector<string>::iterator it = find(vec.begin(), vec.end(), str);
        //此时it对应的元素下标为
        return (&*it - &vec[0]);
    } else {
        return -1;
    }
}

void LR1TableMaker::printOut() {
    for (auto & production : productions) {
        cout<<production.left<<" -> ";
        for(auto & item : production.right)
            cout<<item<<' ';
        cout<<'\n';
    }
    cout<<"****************************************\n";
    cout<<"非终结符："<<Vn.size()<<"个\n";
    for (const auto& symbol : Vn)
        cout<<symbol<<'\n';

    cout<<"*******************************************\n";
    cout<<"终结符："<<Vt.size()<<"个\n";
    for (const auto& symbol : Vt)
        cout<<symbol<<'\n';
}

void LR1TableMaker::start() {
    initial();
    load("grammar.txt");
    firstSetCalculator.initial(Vn, Vt, productions);
    for (auto & symbol : Vn) {
        vector<string> set;
        set = firstSetCalculator.parse(symbol);
        cout<<"\n*************************************************************\n";
        cout<<symbol<<"的first集：\n";
        for (const auto& item : set) {
            cout<<item<<'\n';
        }
    }

//    vector<string> set;
//    set = firstSetCalculator.parse("Compound_statement");
//    cout<<"\n*************************************************************\n";
//    cout<<"Compound_statement"<<"的first集：\n";
//    for (const auto& item : set) {
//        cout<<item<<'\n';
//    }

//    printOut();
}

int main() {
    LR1TableMaker parser;
    parser.initial();
    parser.start();

    return 0;
}
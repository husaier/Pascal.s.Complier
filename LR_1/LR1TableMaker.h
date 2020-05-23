//
// Created by husserl on 2020/5/23.
//
#include "FirstSetCalculator.h"
#include <set>
#include <map>
#include <utility>
#ifndef LR_1_LR1TABLEMAKER_H
#define LR_1_LR1TABLEMAKER_H
using namespace std;

class TableItem {
public:
    TableItem(int t, int i) {
        type = t;
        index = i;
    }

    static const int ERROR = 0;
    static const int SHIFT = 1;
    static const int REDUCTION = 2;
    static const int GOTO = 3;
    static const int ACC = 4;

    int type; //初始化为0，1表示S操作，2表示R操作，3表示goto子表的操作，4表示ACC
    int index; //索引
};

//LR1分析表
class LR1Table {
public:
    //扩展文法的产生式
    vector<Production> productions;
    //符号表，建立index -> string的映射，包含了终结符和非终结符，以及$，不包含扩展文法开始符S'
    vector<string> symbols;
    //symbols数组的分界点，$的下标，index <= dividingIndex为终结符号，否则为非终结符号
    int dividingIndex = 0;
    //反映射表，建立string -> index的映射
    map<string, int> invMap;
    //表格
    vector<vector<TableItem>> table;
    //表的行数、列数
    int rowNum = 0;
    int colNum = 0;
};

class LR1TableMaker {
public:
    void load(const string&);

    //生成有效项目集规范族
    LR1Table makeTable();

private:
    ifstream fin;
    vector<Production> productions; //产生式
    vector<string> Vn; //非终结符
    vector<string> Vt; //终结符
    FirstSetCalculator firstSetCalculator; //first集计算器，可以计算直接左递归
    const string END = "$"; //结束符
    const string EXTENDED_BEGIN = "S'"; //扩展文法开始符

    //将产生式的左右部分别放入left，right
    static Production parseProduction(const string& str);

    //LR1项目
    class LR1Item {
    public:
        LR1Item(Production p, int position, string t) {
            production = std::move(p);
            pos = position;
            tail = std::move(t);
            reductionFlag = false;
        }

        bool operator== (const LR1Item &t) const {
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

    //项目集间的有向边
    class Edge{
    public:
        int target{};
        string condition;

        bool operator== (const Edge &e) const{
            return this->target == e.target && this->condition == e.condition;
        }
    };

    //LR1项目集
    class LR1Group {
    private:

    public:
        vector<LR1Item> items;
        vector<Edge> others;
//        vector<int> others;
//        vector<string> symbol;

        bool operator== (const LR1Group &t) const {
            return this->items == t.items;
        }

        void addEdge(int target, string symbol){
            Edge e;
            e.target = target;
            e.condition = std::move(symbol);
            others.push_back(e);
        }
    };

    void initial();

    //识别终结符和非终结符
    void recognize();

    //计算闭包
    vector<LR1Item> closure(const LR1Item&);

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

    //产生有效项目集规范族
    void makeFamily(vector<LR1Group> &);

    //找到对于该项目集来说，可以goto的符号
    static vector<string> findUsefulSymbol(const LR1Group& group);

    //goto函数
    LR1Group groupGoTo(const LR1Group& items, const string& X);

    //判断是不是规约项目
    static bool isReductable(const LR1Item &item);

    //合并两个项目集，把source中的元素合并到target
    static void mergeGroup(vector<LR1Item> &target, vector<LR1Item> &source);

    //向分析表装入符号信息
    void packSymbols(LR1Table &);

    //在productions数组里面找到产生式，并返回其下标
    int findProduction(const Production&);

//    void printOut(); //测试
};


#endif //LR_1_LR1TABLEMAKER_H

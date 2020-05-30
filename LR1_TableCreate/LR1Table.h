//
// Created by husserl on 2020/5/30.
//
#include <map>
#include "Production.h"
#include <fstream>
#ifndef LR1_TABLECREATE_LR1TABLE_H
#define LR1_TABLECREATE_LR1TABLE_H

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

    neb::CJsonObject toJsonbject();

    void fromJson(const neb::CJsonObject&);

    bool operator==(const TableItem&) const;

private:
    string TYPE = "type";
    string INDEX = "index";
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

    void printOut();

    void printJsonFile(const string& fileName);

    void loadJsonFile(const string& fileName);

    bool operator == (LR1Table& ) const;

private:
    //JSON tag
    string PRODUCTIONS = "productions";
    string SYMBOLS = "symbols";
    string DIVID_INDEX = "dividingIndex";
    string TABLE = "table";
    string INV_MAP = "invMap";
    string ROW_NUM = "rowNUM";
    string COL_NUM = "colNUm";
    ofstream fout;
    ifstream fin;

    neb::CJsonObject toJsonObject();

    void fromJsonObejct(neb::CJsonObject oJson);
};


#endif //LR1_TABLECREATE_LR1TABLE_H

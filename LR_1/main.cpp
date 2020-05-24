#include "LR1TableMaker.h"

using namespace std;

void print(LR1Table table) {
    vector<string> symbols = table.symbols;
    cout<<"*********************************************************\nLR1分析表：\n";
    cout.setf(ios_base::left);
    cout.width(10);
    cout<<"index";
    for (const auto& sym :symbols) {
        cout.width(10);
        cout<<sym;
    }
    cout<<'\n';
    for (int i = 0; i < table.table.size(); ++i) {
        vector<TableItem> row = table.table[i];
        cout.width(10);
        cout<<i;
        for (auto item : row) {
            string out;
            if(item.type == TableItem::SHIFT){
                out.append("S");
                out.append(to_string(item.index));
            }
            else if(item.type == TableItem::ACC)
                out.append("ACC");
            else if (item.type == TableItem::REDUCTION) {
                out.append("R");
                out.append(to_string(item.index));
            }
            else if (item.type == TableItem::GOTO)
                out.append(to_string(item.index));
            cout.width(10);
            cout<<out;
        }
        cout<<'\n';
    }
    cout<<"*********************************************************\n";
}

int main() {
    LR1TableMaker parser;
    parser.load("grammar.txt");
    //test1,txt
    //test2.txt
    //test3.txt
    //grammar.txt
    LR1Table table = parser.makeTable();
    print(table);
    return 0;
}
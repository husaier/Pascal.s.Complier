//
// Created by husserl on 2020/5/30.
//

#include "LR1Table.h"

void LR1Table::printOut() {
    cout<<"*********************************************************\nLR1分析表：\n";
    cout.setf(ios_base::left);
    cout.width(10);
    cout<<"index";
    for (const auto& sym :symbols) {
        cout.width(10);
        cout<<sym;
    }
    cout<<'\n';
    for (int i = 0; i < table.size(); ++i) {
        vector<TableItem> row = table[i];
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

void LR1Table::printJsonFile(const string& fileName) {
    fout.open(fileName);
    if(!fout.is_open()){
        cout<<"文件打开失败\n";
        return;
    }
    neb::CJsonObject oJson;
    oJson = toJsonObject();
    fout<<oJson.ToString();
    fout.close();
}

neb::CJsonObject LR1Table::toJsonObject() {
    neb::CJsonObject oJson;
    oJson.Add(ROW_NUM, rowNum);
    oJson.Add(COL_NUM, colNum);
    oJson.Add(DIVID_INDEX, dividingIndex);

    oJson.AddEmptySubArray(SYMBOLS);
    for (const auto& symbol : symbols) {
        oJson[SYMBOLS].Add(symbol);
    }

    oJson.AddEmptySubArray(PRODUCTIONS);
    for (auto pro : productions) {
        oJson[PRODUCTIONS].Add(pro.toJsonObject());
    }

    oJson.AddEmptySubArray(TABLE);
    for (auto & vector : table) {
        neb::CJsonObject tmp;
        for (auto item : vector) {
            tmp.Add(item.toJsonbject());
        }
        oJson[TABLE].Add(tmp);
    }
    return oJson;
}

void LR1Table::loadJsonFile(const string &fileName) {
    fin.open(fileName);
    if (!fin.is_open()){
        cout<<"文件打开失败\n";
        return;
    }
    string s;
    char t;
    t = fin.get();
    char a[2];
    a[0] = t;
    a[1] = '\0';
    while (!fin.eof()) {
        char a[2];
        a[0] = t;
        a[1] = '\0';
        s.append(a);
        t = fin.get();
    }
    neb::CJsonObject oJson(s);
    fromJsonObejct(oJson);
    fin.close();
}

void LR1Table::fromJsonObejct(neb::CJsonObject oJson) {
    oJson.Get(ROW_NUM, rowNum);
    oJson.Get(COL_NUM, colNum);
    oJson.Get(DIVID_INDEX, dividingIndex);

    int size = oJson[SYMBOLS].GetArraySize();
    for(int i=0; i < size; i++) {
        string t;
        oJson[SYMBOLS].Get(i, t);
        symbols.push_back(t);
        invMap.insert(map<string, int>::value_type(t, i));
    }

    size = oJson[PRODUCTIONS].GetArraySize();
    for(int i=0; i < size; i++) {
        Production p;
        neb::CJsonObject tmp;
        oJson[PRODUCTIONS].Get(i, tmp);
        p.fromJson(tmp);
        productions.push_back(p);
    }

    size = oJson[TABLE].GetArraySize();
    for (int i = 0; i < size; ++i) {
        vector<TableItem> t;
        int length = oJson[TABLE][i].GetArraySize();
        for (int j = 0; j < length; ++j) {
            TableItem item(0, 0);
            neb::CJsonObject tmp;
            oJson[TABLE][i].Get(j, tmp);
            item.fromJson(tmp);
            t.push_back(item);
        }
        table.push_back(t);
    }
}

neb::CJsonObject TableItem::toJsonbject() {
    neb::CJsonObject tmp;
    tmp.Add(TYPE, type);
    tmp.Add(INDEX, index);
    return tmp;
}

void TableItem::fromJson(const neb::CJsonObject& oJson) {
    oJson.Get(TYPE, type);
    oJson.Get(INDEX, index);
}
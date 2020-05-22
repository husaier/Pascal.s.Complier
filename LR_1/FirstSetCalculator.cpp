//
// Created by husserl on 2020/5/22.
//

#include "FirstSetCalculator.h"

void FirstSetCalculator::demo_first() {
    string input;                     //存储输入的产生式字符串
    N = 5;
    //获取产生式总数与各产生式
    string create[5] = {"Char -> char Num", "Num -> B Num num", "Num -> #","B -> B c", "B -> #"};
    //将N个产生式分别存入产生式数组p
    for (int i = 0; i < N; i++) {
        input = create[i];
        getlr(input);
    }

    recognize();//识别终结符与非终结符

    for (int k = 0; k < Vn.size(); ++k) {
        vector<string> t;
        First.push_back(t);
    }
    for (int k = 0; k < Vt.size(); ++k) {
        vector<string> t;
        first.push_back(t);
    }
//    for(int m=0;m<Vt.size();m++)
//        cout<<Vt[m]<<endl;
//    for(int m=0;m<Vn.size();m++)
//        cout<<Vn[m]<<endl;

    cout << endl;
    cout << "\n==================================" << endl;
    cout << "非终结符" << "\t\t" << "FIRST" << endl;

    //对每个非终结符计算first集
    for (int i = 0; i < Vn.size(); i++) {
        cout << "  " << Vn[i] << "\t\t\t{";
        Letter_First(Vn[i]);
        for (int j = 0; j < First[get_Index(Vn[i], Vn)].size() - 1; j++) {
            cout << First[get_Index(Vn[i], Vn)][j] << ",";
        }
        cout << First[get_Index(Vn[i], Vn)][First[get_Index(Vn[i], Vn)].size() - 1] << "} " << endl;
    }
    cout << "\n==================================" << endl;
    cout << "终结符" << "\t\t" << "FIRST" << endl;
    for (int i = 0; i < Vt.size(); i++) {
        cout << "  " << Vt[i] << "\t\t\t{";
        Letter_First(Vt[i]);
        for (int j = 0; j < first[get_Index(Vt[i], Vt)].size() - 1; j++) {
            cout << First[get_Index(Vt[i], Vt)][j] << ",";
        }
        cout << first[get_Index(Vt[i], Vt)][first[get_Index(Vt[i], Vt)].size() - 1] << "} " << endl;
    }
    cout << "\n======================================" << endl;
    Letter_First("$");
}

void FirstSetCalculator::initial(vector<string> &Vn, vector<string> &Vt, vector<Production> &p) {
    this->Vn = Vn;
    this->Vt = Vt;
    this->p = p;
    N=p.size();
    First.clear();
    first.clear();
    //可能存在内存泄露的问题
    for (int k = 0; k < Vn.size(); ++k) {
        vector<string> t;
        First.push_back(t);
    }
    for (int k = 0; k < Vt.size(); ++k) {
        vector<string> t;
        first.push_back(t);
    }
    Vn_recursive.clear();
}

vector<string> FirstSetCalculator::parse(string symbol) {
    // TODO: 为什么必须要刷新??
    // 否则的话会在连续计算的时候出错，有的
    // Vn_recursive.clear();

    if (isVn(symbol)) {
        int index = get_Index(symbol, Vn);
        if (First[index].empty()) // 如果没有计算过
            Letter_First(symbol);
        return First[index];
    }
    else if (isVt(symbol)){
        int index = get_Index(symbol, Vt);
        if (first[index].empty()) // 如果没有计算过
            Letter_First(symbol);
        return first[index];
    }
    else { //文法中未出现过，主要针对 $
        Vt.push_back(symbol);
        Letter_First(symbol);
        return first[get_Index(symbol, Vt)];
    }
}

int FirstSetCalculator::get_Index(string str, vector<string> vec) {
    if (count(vec.begin(), vec.end(), str) != 0) {
        vector<string>::iterator it = find(vec.begin(), vec.end(), str);
        //此时it对应的元素下标为
        return (&*it - &vec[0]);
    } else {
        return -1;
    }
}

void FirstSetCalculator::recognize() {
    int i, j;
    for (i = 0; i < N; i++) //第i个产生式
    {
        if (count(Vn.begin(), Vn.end(), p[i].left) == 0)  //Vn里没有找到第i个产生式的左部，则添加
            Vn.push_back(p[i].left);
        for (j = 0; j < (int) p[i].right.size(); j++)   //对于产生式右部
        {
            if (p[i].right[j][0] >= 'A' && p[i].right[j][0] <= 'Z') { //产生式右部的符号，第一个字母大写为非终结符
                if (count(Vn.begin(), Vn.end(), p[i].right[j]) == 0)  //Vn里没有找到该符号，则添加
                    Vn.push_back(p[i].right[j]);
            } else if (p[i].right[j][0] >= 'a' && p[i].right[j][0] <= 'z') { //产生式右部的符号，第一个字母小写为终结符
                if (count(Vt.begin(), Vt.end(), p[i].right[j]) == 0)  //Vt里没有找到该符号，则添加
                    Vt.push_back(p[i].right[j]);
            }
        }
    }
}

void FirstSetCalculator::getlr(string str) {
    //将字符串读到input中
    stringstream input(str);
    //暂存从word中读取的字符串
    string result;
    Production t;

    input >> t.left; //把左部字符串存入p[i]
    input >> result;    //过滤掉->

    //依次输出到result中，并存入res中
    while (input >> result)
        t.right.push_back(result);
    p.push_back(t);
}

void FirstSetCalculator::Letter_First(string str) {
    int t;
    if (get_Index(str, Vt) != -1)     //str在Vt里
        if (get_Index(str, first[get_Index(str, Vt)]) == -1) //first集没有这个符号
            first[get_Index(str, Vt)].push_back(str);

    if (get_Index(str, Vn) != -1)     //str在Vn里
    {
        for (int i = 0; i < N; i++)   //遍历产生式
        {
            if (get_Index(p[i].left, Vn_recursive) != -1) //如果产生式左部非终结符的产生式存在左递归，直接跳过
                continue;

            if (p[i].left == str)     //找到左侧符号是str的产生式
            {
                if (get_Index(p[i].right[0], Vt) != -1) {                  //右侧第一个符号是终结符
                    if (get_Index(p[i].right[0], First[get_Index(str, Vn)]) == -1)//如果first集中没有该字符，则加入Vn[i]的first集合中
                    {
                        First[get_Index(str, Vn)].push_back(p[i].right[0]);
                    }
                }
                if (p[i].right[0] == "#") {                                 //右侧第一个符号是空字符，加入Vn[i]的first集合中
                    if (get_Index("#", First[get_Index(str, Vn)]) == -1) //如果first集中没有空字符，则加入Vn[i]的first集合中
                    {
                        First[get_Index(str, Vn)].push_back("#");
                    }
                }
                if (get_Index(p[i].right[0], Vn) != -1)  //右侧第一个符号是非终结符
                {
                    if (p[i].right.size() == 1) //只有一个符号
                    {
                        if (p[i].right[0] == str) { //这个符号不能等于左侧的符号
                            Letter_First(p[i].right[0]); //把右侧符号的first集合加入到左侧符号中
                            vector<string> ff = First[get_Index(p[i].right[0], Vn)];
                            for (int k = 0; k < ff.size(); k++) {
                                if (get_Index(ff[k], First[get_Index(str, Vn)]) == -1) { //如果first集不存在该符号
                                    First[get_Index(str, Vn)].push_back(ff[k]);
                                }
                            }
                        } else
                            printf("error,");
                    }
                    else if (p[i].right.size() > 1) //有多个符号
                    {
                        for (int j = 0; j < p[i].right.size(); j++) {
                            vector<string> TT;

                            if (get_Index(p[i].right[j], Vn) != -1) {//符号为非终结符
                                if (p[i].right[j] != str) //非左递归
                                {
                                    Letter_First(p[i].right[j]);
                                    TT = First[get_Index(p[i].right[j], Vn)];
                                }
                                else { //出现左递归
                                    vector<vector<string>> dy;//存储该符号有左递归的产生式的右部
                                    vector<vector<string>> ndy;//存储该符号无左递归的产生式的右部
                                    for (int i = 0; i < N; i++) {   //遍历产生式
                                        if (p[i].left == str) { //找到左侧符号是该符号的产生式
                                            if (p[i].right[0] == str)//如果有直接左递归
                                                dy.push_back(p[i].right);
                                            else
                                                ndy.push_back(p[i].right);
                                        }
                                    }
                                    for (int i = 0; i < ndy.size(); i++) {  //对无左递归的式子进行遍历，计算first集
                                        if (get_Index(ndy[i][0], Vt) != -1) {                  //右侧第一个符号是终结符
                                            if (get_Index(ndy[i][0], First[get_Index(str, Vn)]) ==
                                                -1)//如果first集中没有该字符，则加入Vn[i]的first集合中
                                            {
                                                First[get_Index(str, Vn)].push_back(ndy[i][0]);
                                            }
                                        }
                                        if (ndy[i][0] == "#") {                            //右侧第一个符号是空字符，加入Vn[i]的first集合中
                                            if (get_Index("#", First[get_Index(str, Vn)]) ==
                                                -1) //如果first集中没有空字符，则加入Vn[i]的first集合中
                                            {
                                                First[get_Index(str, Vn)].push_back("#");
                                            }
                                        }
                                        if (get_Index(ndy[i][0], Vn) != -1)  //右侧第一个符号是非终结符
                                        {
                                            if (ndy[i].size() == 1) //只有一个符号
                                            {
                                                if (ndy[i][0] == str) { //这个符号不能等于左侧的符号
                                                    Letter_First(ndy[i][0]); //把右侧符号的first集合加入到左侧符号中
                                                    vector<string> ff = First[get_Index(ndy[i][0], Vn)];
                                                    for (int k = 0; k < ff.size(); k++) {
                                                        if (get_Index(ff[k], First[get_Index(str, Vn)]) ==
                                                            -1) { //如果first集不存在该符号
                                                            First[get_Index(str, Vn)].push_back(ff[k]);
                                                        }
                                                    }
                                                } else
                                                    printf("in error,");
                                            }
                                            else if (ndy[i].size() > 1) //有多个符号
                                            {
                                                for (int j = 0; j < ndy[i].size(); j++) {
                                                    vector<string> TT;

                                                    if (get_Index(ndy[i][j], Vn) != -1) //符号为非终结符
                                                        if (ndy[i][j] != str) //非左递归
                                                        {
                                                            Letter_First(ndy[i][j]);
                                                            TT = First[get_Index(ndy[i][j], Vn)];
                                                        } else { //出现隐式左递归
                                                            //printf("in error1")
                                                            vector<string> di;
                                                            //把前面的字符都认为是空，把产生式加入dy
                                                            for (int m = 0; m < ndy[i].size() - j; m++) {
                                                                di.push_back(ndy[i][j + m]);
                                                            }
                                                            dy.push_back(di);
                                                            break;
                                                        }
                                                    else if (get_Index(ndy[i][j], Vt) != -1) //符号为终结符
                                                    {
                                                        Letter_First(ndy[i][j]);
                                                        TT = first[get_Index(ndy[i][j], Vt)];
                                                    } else {
                                                        printf("in error1,");
                                                        break;
                                                    }


                                                    if (get_Index("#", TT) != -1 &&
                                                        (j + 1) <
                                                        ndy[i].size()) //如果是非末位符号且first集里有空,则需要看下一个符号的first集
                                                    {
                                                        for (int t = 0; t < TT.size(); t++) {
                                                            if (TT[t] != "#" &&
                                                                get_Index(TT[t], First[get_Index(str, Vn)]) ==
                                                                -1)//右侧字母的无空first集加入到Vn[i]的first集
                                                                First[get_Index(str, Vn)].push_back(TT[t]);
                                                        }
                                                    } else  //集合里无空或有空但是最后一个，直接加入，并退出循环
                                                    {
                                                        for (int t = 0; t < TT.size(); t++) {
                                                            if (get_Index(TT[t], First[get_Index(str, Vn)]) == -1)
                                                                First[get_Index(str, Vn)].push_back(TT[t]);
                                                        }
                                                        break;
                                                    }
                                                }
                                            }
                                        }
                                    }

                                    if (get_Index("#", First[get_Index(str, Vn)]) != -1) { //若first集里面有空字符
                                        for (int i = 0; i < dy.size(); i++) {  //对有递归的式子进行遍历，计算first集
                                            if (get_Index(dy[i][0], Vn) != -1)  //右侧第一个符号是非终结符
                                            {
                                                if (dy[i].size() == 1) //只有一个符号，肯定出错
                                                    printf("in1 error,");
                                                else if (dy[i].size() > 1) //有多个符号
                                                {
                                                    for (int j = 0; j < dy[i].size(); j++) {
                                                        vector<string> TT;

                                                        if (get_Index(dy[i][j], Vn) != -1) //符号为非终结符
                                                            if (dy[i][j] != str) //非左递归
                                                            {
                                                                Letter_First(dy[i][j]);
                                                                TT = First[get_Index(dy[i][j], Vn)];
                                                            } else { //出现左递归
                                                                //printf("in error1")
                                                                //这个字符置空，跳过这个字符的分析
                                                                continue;
                                                            }
                                                        else if (get_Index(dy[i][j], Vt) != -1) //符号为终结符
                                                        {
                                                            Letter_First(dy[i][j]);
                                                            TT = first[get_Index(dy[i][j], Vt)];
                                                        } else {
                                                            printf("in1 error1,");
                                                            break;
                                                        }


                                                        if (get_Index("#", TT) != -1 &&
                                                            (j + 1) <
                                                            dy[i].size()) //如果是非末位符号且first集里有空,则需要看下一个符号的first集
                                                        {
                                                            for (int t = 0; t < TT.size(); t++) {
                                                                if (TT[t] != "#" &&
                                                                    get_Index(TT[t], First[get_Index(str, Vn)]) ==
                                                                    -1)//右侧字母的无空first集加入到Vn[i]的first集
                                                                    First[get_Index(str, Vn)].push_back(TT[t]);
                                                            }
                                                        } else  //集合里无空或有空但是最后一个，直接加入，并退出循环
                                                        {
                                                            for (int t = 0; t < TT.size(); t++) {
                                                                if (get_Index(TT[t], First[get_Index(str, Vn)]) == -1)
                                                                    First[get_Index(str, Vn)].push_back(TT[t]);
                                                            }
                                                            break;
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                    Vn_recursive.push_back(str);//存在左递归的符号加入Vn_recursive，不再计算first集
                                    break;
                                }
                            }
                            else if (get_Index(p[i].right[j], Vt) != -1) //符号为终结符
                            {
                                Letter_First(p[i].right[j]);
                                TT = first[get_Index(p[i].right[j], Vt)];
                            } else {
                                printf("error1,");
                                break;
                            }


                            if (get_Index("#", TT) != -1 &&
                                (j + 1) < p[i].right.size()) //如果是非末位符号且first集里有空,则需要看下一个符号的first集
                            {
                                for (int t = 0; t < TT.size(); t++) {
                                    if (TT[t] != "#" &&
                                        get_Index(TT[t], First[get_Index(str, Vn)]) == -1)//右侧字母的无空first集加入到Vn[i]的first集
                                        First[get_Index(str, Vn)].push_back(TT[t]);
                                }
                            } else  //集合里无空或有空但是最后一个，直接加入，并退出循环
                            {
                                for (int t = 0; t < TT.size(); t++) {
                                    if (get_Index(TT[t], First[get_Index(str, Vn)]) == -1)
                                        First[get_Index(str, Vn)].push_back(TT[t]);
                                }
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
}

bool FirstSetCalculator::isVn(string s) {
    return get_Index(s, Vn) != -1;
}

bool FirstSetCalculator::isVt(string s) {
    return get_Index(s, Vt) != -1;
}
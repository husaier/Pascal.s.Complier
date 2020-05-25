//
// Created by husserl on 2020/5/17.
//

#ifndef LEXICALANALYZE_LEXICAL_ANALYZER_H
#define LEXICALANALYZE_LEXICAL_ANALYZER_H
#include <fstream>
#include <algorithm>
#include <vector>
using namespace std;

static const int NUM = 0; // 数字
static const int RELOP = 1; // = <> < <= > >=
static const int ADDOP = 2; // +  - or
static const int MULOP = 3; // * / div mod and
static const int ASSIGNOP = 4; // :=
static const int DELIMITER = 5; // , ; . ( ) [ ]
static const int SUBDELIMITER = 6; // ..
static const int KEY = 7; // 关键字
static const int ID = 8; // 标识符
static const int STRING = 9; // '字符串'

static const int S0 = 0;

static const int SUB1_S1 = 1;
static const int SUB1_S2 = 2;
static const int SUB1_S3 = 3;
static const int SUB1_S4 = 4;
static const int SUB1_S5 = 5;
static const int SUB1_S6 = 6;

static const int SUB2_S1 = 7;

static const int SUB3_S1 = 8;
static const int SUB3_S2 = 9;
static const int SUB3_S3 = 10;

static const int SUB4_S1 = 11;

static const int SUB5_S1 = 12;
static const int SUB5_S2 = 13;
static const int SUB5_S3 = 14;

static const int SUB6_S1 = 15;
static const int SUB6_S2 = 16;

static const int SUB7_S1 = 17;
static const int SUB7_S2 = 18;

static const int SUB8_S1 = 19;
static const int SUB8_S2 = 20;

static const int SUB9_S1 = 21;
static const int SUB9_S2 = 22;

static const int ADD_RELOP = 23;
static const int ADD_ADDOP = 24;
static const int ADD_MULOP = 25;
static const int ADD_DELIMITER = 26;

class LexicalItem
{
public:
    LexicalItem(int type, string s) {
        token = type;
        attribute = s;
    }

    int token;
    string symbol;
    string attribute;

    string tokenToString() const;
};

class LexicalAnalyzer
{
public:
    LexicalAnalyzer() {
        initial();
    }

    ~LexicalAnalyzer() {
        tokenFlow.clear();
        if (fin.is_open())
            fin.close();
    }

    bool openFile(const string& fileName) {
        fin.open(fileName);
        return fin.is_open();
    }

    void closeFile() {
        if (fin.is_open())
            fin.close();
    }

    vector<LexicalItem> analyze();

private:
    ifstream fin; //输入文件
    vector<LexicalItem> tokenFlow; //记号流
    vector<string> errors; // 错误信息
    vector<string> annotates; // 注释
    int lines; // 行数
    int letters; // 字符数
    int state; // 状态机
    string buffer;//暂存

    vector<string> keyWord = { "and", "array", "begin", "boolean", "case",
                               "case", "const", "div", "do", "downto",
                               "else", "end", "for", "function", "if",
                               "integer", "mod", "not", "of", "or",
                               "procedure", "program", "real", "record", "repeat",
                               "then", "to", "type", "until", "var", "while", "char"};

    void initial();

    void DFA(char);

    bool isKeyWord(string s);

    static bool isLetter(char c);

    static bool isDigit(char c);

    void putToBuffer(char a);

    //错误恢复
    void errorCtr(string s);

    void addToken(int type);
};

#endif //LEXICALANALYZE_LEXICAL_ANALYZER_H

#include <iostream>
#include <fstream>
#include <vector>
// #include <Windows.h>
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
    string attribute;

    string tokenToString() {
        switch (token) {
            case NUM:
                return "NUM";
            case RELOP:
                return "RELOP";
            case ADDOP:
                return "ADDOP";
            case MULOP:
                return "MULOP";
            case ASSIGNOP:
                return "ASSIGNOP";
            case DELIMITER:
                return "DELIMITER";
            case SUBDELIMITER:
                return "SUBDELIMITER";
            case KEY:
                return "KEY";
            case ID:
                return "ID";
            case STRING:
                return "STRING";
        }
    }
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

    vector<LexicalItem> analyze() {
        initial();
        char t;
        fin.get(t);
        while (!fin.eof()) {
            if (t == '\n')
                lines++;
            else if (t != ' ' && t != '\t')
                letters++;
            DFA(t);
            fin.get(t);
        }
        t = '\n';
        DFA(t);//出循环之后再进行一次跳转，为的是输出
        return tokenFlow;
    }

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
                               "then", "to", "type", "until", "var", "while" };

    void initial() {
        tokenFlow.clear();
        annotates.clear();
        errors.clear();
        lines = 0;
        letters = 0;
        state = 0;
    }

    void DFA(char t) {
        switch (state) {
            case S0: {
                if (isDigit(t)) {
                    state = SUB1_S1;
                    putToBuffer(t);
                }
                else if (isLetter(t)) {
                    state = SUB2_S1;
                    putToBuffer(t);
                }
                else if (t == '(') {
                    state = SUB3_S1;
                    putToBuffer(t);
                }
                else if (t == '{') {
                    state = SUB4_S1;
                    putToBuffer(t);
                }
                else if (t == '<') {
                    state = SUB5_S1;
                    putToBuffer(t);
                }
                else if (t == '>') {
                    state = SUB6_S1;
                    putToBuffer(t);
                }
                else if (t == '\'') {
                    state = SUB7_S1;
                    putToBuffer(t);
                }
                else if (t == '.') {
                    state = SUB8_S1;
                    putToBuffer(t);
                }
                else if (t == ':') {
                    state = SUB9_S1;
                    putToBuffer(t);
                }
                else if (t == '=') {
                    state = ADD_RELOP;
                    putToBuffer(t);
                }
                else if (t == '+' || t == '-') {
                    state = ADD_ADDOP;
                    putToBuffer(t);
                }
                else if (t == '*' || t == '/') {
                    state = ADD_MULOP;
                    putToBuffer(t);
                }
                else if (t == ',' || t == ';' || t == ')' || t == '[' || t == ']') {
                    state = ADD_DELIMITER;
                    putToBuffer(t);
                }
                else if (t == ' ' || t == '\t' || t == '\n')
                    state = S0;
                else {
                    state = S0;
                    putToBuffer(t);
                    string s = "未识别的符号：";
                    errorCtr(s + buffer);
                }
                break;
            }
            case SUB1_S1: {
                if (isDigit(t)) {
                    state = SUB1_S1;
                    putToBuffer(t);
                }
                else if (t == '.') {
                    state = SUB1_S2;
                    putToBuffer(t);
                }
                else if (t == 'E') {
                    state = SUB1_S4;
                    putToBuffer(t);
                }
                else {
                    state = S0;
                    addToken(NUM);
                }
                break;
            }
            case SUB1_S2: {
                if (isDigit(t)) {
                    state = SUB1_S3;
                    putToBuffer(t);
                }
                else {
                    state = S0;
                    string s = "附近有不合法的数字：";
                    errorCtr(s + buffer);
                }
                break;
            }
            case SUB1_S3: {
                if (isDigit(t)) {
                    state = SUB1_S3;
                    putToBuffer(t);
                }
                else if (t == 'E') {
                    state = SUB1_S4;
                    putToBuffer(t);
                }
                else {
                    state = S0;
                    addToken(NUM);
                }
                break;
            }
            case SUB1_S4: {
                if (isDigit(t)) {
                    state = SUB1_S6;
                    putToBuffer(t);
                }
                else if (t == '+' || t == '-') {
                    state = SUB1_S5;
                    putToBuffer(t);
                }
                else {
                    string s = "附近有不合法的数字：";
                    errorCtr(s + buffer);
                }
                break;
            }
            case SUB1_S5: {
                if (isDigit(t)) {
                    state = SUB1_S6;
                    putToBuffer(t);
                }
                else {
                    string s = "附近有不合法的数字：";
                    errorCtr(s + buffer);
                }
                break;
            }
            case SUB1_S6: {
                if (isDigit(t)) {
                    state = SUB1_S6;
                    putToBuffer(t);
                }
                else {
                    state = S0;
                    addToken(NUM);
                }
                break;
            }
            case SUB2_S1: {
                if (isLetter(t) || isDigit(t)) {
                    state = SUB2_S1;
                    putToBuffer(t);
                }
                else {
                    state = S0;
                    if (buffer.size() <= 8) {
                        if (isKeyWord(buffer))
                            if (buffer == "or")
                                addToken(ADDOP);
                            else if (buffer == "div" || buffer == "mod" || buffer == "and")
                                addToken(MULOP);
                            else
                                addToken(KEY);
                        else
                            addToken(ID);
                    }
                    else {
                        string s = "标识符过长：";
                        errorCtr(s + buffer);
                    }
                }
                break;
            }
            case SUB3_S1: {
                if (t == '*') {
                    state = SUB3_S2;
                    putToBuffer(t);
                }
                else {
                    state = S0;
                    addToken(DELIMITER);
                }
                break;
            }
            case SUB3_S2: {
                if (t == '*') {
                    state = SUB3_S3;
                    putToBuffer(t);
                }
                else {
                    state = SUB3_S2;
                    putToBuffer(t);
                }
                break;
            }
            case SUB3_S3: {
                if (t == ')') {
                    state = S0;
                    putToBuffer(t);
                    // string real = StringToUtf(buffer);
                    annotates.push_back(buffer);//存入，最后输出
                    buffer.clear();
                }
                else {
                    state = SUB3_S2;
                    putToBuffer(t);
                }
                break;
            }
            case SUB4_S1: {
                if (t == '}') {
                    state = S0;
                    putToBuffer(t);
                    // string real = StringToUtf(buffer);
                    annotates.push_back(buffer);//存入，最后输出
                    buffer.clear();
                }
                else {
                    state = SUB4_S1;
                    putToBuffer(t);
                }
                break;
            }
            case SUB5_S1: {
                if (t == '=') {
                    state = SUB5_S2;
                    putToBuffer(t);
                }
                else if (t == '>') {
                    state = SUB5_S3;
                    putToBuffer(t);
                }
                else {
                    state = S0;
                    addToken(RELOP);
                }
                break;
            }
            case SUB6_S1: {
                if (t == '=') {
                    state = SUB6_S2;
                    putToBuffer(t);
                }
                else {
                    state = S0;
                    addToken(RELOP);
                }
                break;
            }
            case SUB5_S2:
            case SUB5_S3:
            case SUB6_S2:
            case ADD_RELOP: {
                state = S0;
                addToken(RELOP);
                break;
            }
            case SUB7_S1: {
                if (t == '\'') {
                    state = SUB7_S2;
                    putToBuffer(t);
                }
                else {
                    state = SUB7_S1;
                    putToBuffer(t);
                }
                break;
            }
            case SUB7_S2: {
                state = S0;
                addToken(STRING);
                break;
            }
            case SUB8_S1: {
                if (t == '.') {
                    state = SUB8_S2;
                    putToBuffer(t);
                }
                else {
                    state = S0;
                    addToken(DELIMITER);
                }
                break;
            }
            case SUB8_S2: {
                state = S0;
                addToken(SUBDELIMITER);
                break;
            }
            case SUB9_S1: {
                if (t == '=') {
                    state = SUB9_S2;
                    putToBuffer(t);
                }
                else {
                    state = S0;
                    string s = "未识别的字符：";
                    errorCtr(s + buffer);
                }
                break;
            }
            case SUB9_S2: {
                state = S0;
                addToken(ASSIGNOP);
                break;
            }
            case ADD_ADDOP: {
                state = S0;
                addToken(ADDOP);
                break;
            }
            case ADD_MULOP: {
                state = S0;
                addToken(MULOP);
                break;
            }
            case ADD_DELIMITER: {
                state = S0;
                addToken(DELIMITER);
                break;
            }
        }
    }

    bool isKeyWord(string s)
    {
        for (const auto& item : keyWord)
        {
            if (s == item)
                return true;
        }
        return false;
    }

    static bool isLetter(char c)
    {
        return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
    }

    static bool isDigit(char c)
    {
        return c >= '0' && c <= '9';
    }

    void putToBuffer(char a)
    {
        char s[2];
        s[0] = a;
        s[1] = '\0';
        buffer.append(s);
    }

    //错误恢复
    void errorCtr(string s) {
        char str[1024];
        sprintf(str, "ERROR:第%d行出错!  可能的出错原因：%s\n", lines + 1, s.c_str());
        string temp;
        temp.append(str);
        errors.push_back(temp);
        state = S0;
        char k = fin.get();
        while (isDigit(k) || isLetter(k))
        {
            letters++;
            k = fin.get();
        }
        fin.seekg(-1, ios::cur);//文件指针回退一个
        buffer.clear();
    }

    //windows下, 转换string的编码格式，避免中文乱码问题
    // static string StringToUtf(string strValue)
    // {
    // 	int nwLen = MultiByteToWideChar(CP_UTF8, 0, strValue.c_str(), -1, NULL, 0);
    // 	wchar_t* pwBuf = new wchar_t[nwLen + 1];//加上末尾'\0'
    // 	memset(pwBuf, 0, nwLen * 2 + 2);
    // 	MultiByteToWideChar(CP_UTF8, 0, strValue.c_str(), strValue.length(), pwBuf, nwLen);
    // 	int nLen = WideCharToMultiByte(CP_ACP, 0, pwBuf, -1, NULL, NULL, NULL, NULL);
    // 	char* pBuf = new char[nLen + 1];
    // 	memset(pBuf, 0, nLen + 1);
    // 	WideCharToMultiByte(CP_ACP, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);
    // 	std::string retStr = pBuf;
    // 	delete[]pBuf;
    // 	delete[]pwBuf;
    // 	return retStr;
    // }

    void addToken(int type) {
        // string attri = StringToUtf(buffer);
        LexicalItem lexicalItem(type, buffer);
        tokenFlow.push_back(lexicalItem);
        buffer.clear();
        if (!fin.eof()) {
            fin.seekg(-1, ios::cur);//文件指针回退1个
            char t = fin.get();
            if (t == '\n')
                lines--;
            else if (t != ' ' && t != '\t')
                letters--;
            fin.seekg(-1, ios::cur);
        }
    }
};

int main() {
    cout << "in put your file name:" << endl;
    string fileName;
    cin >> fileName;
    LexicalAnalyzer lexicalAnalyzer;
    if (!lexicalAnalyzer.openFile(fileName)) {
        cout << "fail to open it" << endl;
        return 0;
    }
    vector<LexicalItem> result;
    result = lexicalAnalyzer.analyze();
    lexicalAnalyzer.closeFile();
    for (auto item : result) {
        cout << item.tokenToString() << "\t" << item.attribute << endl;
    }
    return 0;
}
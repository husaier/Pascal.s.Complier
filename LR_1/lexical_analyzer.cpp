//
// Created by husserl on 2020/5/17.
//
#include "lexical_analyzer.h"

string LexicalItem::tokenToString() const {
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

vector<LexicalItem> LexicalAnalyzer::analyze() {
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

void LexicalAnalyzer::initial() {
    lines = 0;
    letters = 0;
    state = S0;
    buffer.clear();
    annotates.clear();
    errors.clear();
}

void LexicalAnalyzer::DFA(char t) {
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
            else if (t == ' ' || t == '\t' || t == '\n' || t == '\r')
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
            else if (t == '.') {
                state = SPECIAL;
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
                transform(buffer.begin(), buffer.end(), buffer.begin(), ::tolower);
                if (isKeyWord(buffer)) {
                    if (buffer == "or")
                        addToken(ADDOP);
                    else if (buffer == "div" || buffer == "mod" || buffer == "and")
                        addToken(MULOP);
                    else
                        addToken(KEY);
                }
                else if (buffer.size() <= 8) {
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
            }
            else {
                state = SUB7_S1;
                putToBuffer(t);
            }
            break;
        }
        case SUB7_S2: {
            if(t == '\'') {
                state = SUB7_S1;
                putToBuffer(t);
            } else {
                state = S0;
                addToken(STRING);
            }
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
                addToken(DELIMITER);
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
        case SPECIAL: {
            state = S0;
            addToken(SPECIAL);
            break;
        }
    }
}

bool LexicalAnalyzer::isKeyWord(string s)
{
    for (const auto& item : keyWord)
    {
        if (s == item)
            return true;
    }
    return false;
}

bool LexicalAnalyzer::isLetter(char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool LexicalAnalyzer::isDigit(char c)
{
    return c >= '0' && c <= '9';
}

void LexicalAnalyzer::putToBuffer(char a)
{
    char s[2];
    s[0] = a;
    s[1] = '\0';
    buffer.append(s);
}

//错误恢复
void LexicalAnalyzer::errorCtr(string s) {
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

void LexicalAnalyzer::addToken(int type) {
    // string attri = StringToUtf(buffer);
    LexicalItem lexicalItem(type, buffer);
    lexicalItem.symbol = buffer;
    lexicalItem.line = lines;
    if (type == ID)
        lexicalItem.symbol = "id";
    else if(type == ASSIGNOP)
        lexicalItem.symbol = "assignop";
    else if(type == STRING)
        lexicalItem.symbol = "string";
    else if(type == NUM)
        lexicalItem.symbol = "num";
    else if(type == SPECIAL) {
        string s;
        char t;
        for (auto c : buffer) {
            if (c == '.')
                break;
            else {
                char a[2];
                a[0] = c;
                a[1] = '\0';
                s.append(a);
            }
        }
        LexicalItem item(NUM, s);
        item.symbol = "num";
        item.line = lines;
        tokenFlow.push_back(item);
        lexicalItem.token = SUBDELIMITER;
        lexicalItem.attribute = "..";
        lexicalItem.symbol = "..";
    }
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

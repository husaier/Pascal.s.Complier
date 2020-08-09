#include "LR1Table.h"
#include "LR1Runner.h"
#include "lexical_analyzer.h"

using namespace std;

const string fileName = "grammar.json";

int main() {
    LR1Table table;
    table.loadJsonFile(fileName);
    // 词法分析
    string fileName = "test5.pas";
    LexicalAnalyzer lexicalAnalyzer;
    if (!lexicalAnalyzer.openFile(fileName)) {
        cout << "fail to open it" << endl;
        return 0;
    }
    vector<LexicalItem> result;
    result = lexicalAnalyzer.analyze();
    lexicalAnalyzer.closeFile();
    // 语法分析、语义分析
    LR1Runner runner;
    runner.load(result);
    runner.debugInfoLevel = 1;
    runner.run(table);
    runner.printSemanticError();
    runner.printMidCode();
//    string s = "0.23";
//    float f = stof(s);
//    cout << f << endl;
//    s = "123";
//    f = stof(s);
//    cout << f << endl;
    return 0;
}
#include "LR1Table.h"
#include "LR1Runner.h"
#include "lexical_analyzer.h"

using namespace std;

const string fileName = "grammar.json";

int main() {
    LR1Table table;
    table.loadJsonFile(fileName);
    //table.printOut();

    string fileName = "test4.pas";
    LexicalAnalyzer lexicalAnalyzer;
    if (!lexicalAnalyzer.openFile(fileName)) {
        cout << "fail to open it" << endl;
        return 0;
    }
    vector<LexicalItem> result;
    result = lexicalAnalyzer.analyze();
    lexicalAnalyzer.closeFile();

//    for (const auto& item : result) {
//        cout << item.tokenToString() << "\t" << item.attribute<< '\t'<< item.symbol << endl;
//    }

    LR1Runner runner;
    runner.load(result);

    runner.debugInfoLevel = 3;

    runner.run(table);

    runner.printSemanticError();
//    string s = "0.23";
//    float f = stof(s);
//    cout << f << endl;
//    s = "123";
//    f = stof(s);
//    cout << f << endl;
    return 0;
}
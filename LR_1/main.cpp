#include "LR1Table.h"
#include "LR1Runner.h"
#include "lexical_analyzer.h"

using namespace std;

const string fileName = "grammar.json";

int main() {
    LR1Table table;
    table.loadJsonFile(fileName);
    table.printOut();

    string fileName = "SourceFile.pas";
    LexicalAnalyzer lexicalAnalyzer;
    if (!lexicalAnalyzer.openFile(fileName)) {
        cout << "fail to open it" << endl;
        return 0;
    }
    vector<LexicalItem> result;
    result = lexicalAnalyzer.analyze();
    lexicalAnalyzer.closeFile();
    for (const auto& item : result) {
        cout << item.tokenToString() << "\t" << item.attribute<< '\t'<< item.symbol << endl;
    }

    LR1Runner::load(result);

    LR1Runner::run(table);
    return 0;
}
#include <iostream>
#include "lexical_analyzer.h"
using namespace std;

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

#include "LR1Table.h"
#include "LR1Runner.h"
#include "lexical_analyzer.h"
#include "Pcode_Interpreter.h"
#include "TransformPcode.h"

using namespace std;

const string fileName = "grammar.json";

int main() {
    LR1Table table;
    table.loadJsonFile(fileName);
    // 词法分析
    string pasfileName = "test5.pas";
    LexicalAnalyzer lexicalAnalyzer;
    if (!lexicalAnalyzer.openFile(pasfileName)) {
        cout << "fail to open it" << endl;
        return 0;
    }
    vector<LexicalItem> result;
    result = lexicalAnalyzer.analyze();
    lexicalAnalyzer.closeFile();
    // 语法分析、语义分析
    LR1Runner runner;
    runner.load(result);
    runner.debugInfoLevel = 3;
    runner.run(table);
    runner.printSemanticError();
    if  (!runner.semanticError.empty())
        return 0;

    runner.printMidCode();
    vector<SymbolTableLine *> proFunVector = runner.generateProFunVector(runner.startBlock);
    // 开始转pcode码
    TransformPcode transformPcode;
    transformPcode.init(proFunVector, runner.midCode);//初始化
    transformPcode.initialValueData();
    transformPcode.transformPcode(runner.midCode);

    cout <<"pcode:"<< endl;
    for (int i = 0; i < transformPcode.allPcode.size(); i++) {
        cout << i << "\t" << transformPcode.opToString(transformPcode.allPcode[i].OP) << "\t"
             << transformPcode.allPcode[i].L << "\t";
        if (string(transformPcode.allPcode[i].D.type().name()) == "i") {
            auto d = any_cast<int>(transformPcode.allPcode[i].D);
            cout << d << " ";
        } else if (string(transformPcode.allPcode[i].D.type().name()) == "d") {
            auto d = any_cast<double>(transformPcode.allPcode[i].D);
            cout << d << " ";
        } else if (string(transformPcode.allPcode[i].D.type().name()) == "b") {
            auto d = any_cast<bool>(transformPcode.allPcode[i].D);
            cout << d << " ";
        } else if (string(transformPcode.allPcode[i].D.type().name()) == "c") {
            auto d = any_cast<char>(transformPcode.allPcode[i].D);
            cout << d << " ";
        } else {
            printf("error：栈顶值无法打印");
        }
        cout << endl;
    }
    cout << endl<< "code start:" << endl ;

    // pcode解释执行
    Pcode_Interpreter interpreter;
    interpreter.interpreter(transformPcode.allPcode);

    //    下列代码可以从文件里面读出p-code并执行
//    Pcode_Interpreter interpreter;
//    interpreter.interpreter(Pcode_Interpreter::readFile("test.txt"));


//    string s = "0.23";
//    float f = stof(s);
//    cout << f << endl;
//    s = "123";
//    f = stof(s);
//    cout << f << endl;
    return 0;
}
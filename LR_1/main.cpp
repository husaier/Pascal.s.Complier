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
    // �ʷ�����
    string fileName = "test5.pas";
    LexicalAnalyzer lexicalAnalyzer;
    if (!lexicalAnalyzer.openFile(fileName)) {
        cout << "fail to open it" << endl;
        return 0;
    }
    vector<LexicalItem> result;
    result = lexicalAnalyzer.analyze();
    lexicalAnalyzer.closeFile();
    // �﷨�������������
    LR1Runner runner;
    runner.load(result);
    runner.debugInfoLevel = 1;
    runner.run(table);
    runner.printSemanticError();
    runner.printMidCode();
    vector<SymbolTableLine *> proFunVector = runner.generateProFunVector(runner.startBlock);
    // ��ʼתpcode��
    TransformPcode transformPcode;
    transformPcode.init(proFunVector, runner.midCode);//��ʼ��
    transformPcode.initialValueData();
    transformPcode.transformPcode(runner.midCode);

    // pcode����ִ��
    Pcode_Interpreter interpreter;
    interpreter.interpreter(transformPcode.allPcode);

    //    ���д�����Դ��ļ��������p-code��ִ��
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
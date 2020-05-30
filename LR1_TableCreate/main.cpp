#include "LR1TableMaker.h"
#include "CJsonObject.hpp"

using namespace std;

//void print(LR1Table table) {
//    vector<string> symbols = table.symbols;
//    cout<<"*********************************************************\nLR1分析表：\n";
//    cout.setf(ios_base::left);
//    cout.width(10);
//    cout<<"index";
//    for (const auto& sym :symbols) {
//        cout.width(10);
//        cout<<sym;
//    }
//    cout<<'\n';
//    for (int i = 0; i < table.table.size(); ++i) {
//        vector<TableItem> row = table.table[i];
//        cout.width(10);
//        cout<<i;
//        for (auto item : row) {
//            string out;
//            if(item.type == TableItem::SHIFT){
//                out.append("S");
//                out.append(to_string(item.index));
//            }
//            else if(item.type == TableItem::ACC)
//                out.append("ACC");
//            else if (item.type == TableItem::REDUCTION) {
//                out.append("R");
//                out.append(to_string(item.index));
//            }
//            else if (item.type == TableItem::GOTO)
//                out.append(to_string(item.index));
//            cout.width(10);
//            cout<<out;
//        }
//        cout<<'\n';
//    }
//    cout<<"*********************************************************\n";
//}

//void json_demo() {
//    int iValue;
//    double fTimeout;
//    std::string strValue;
//    neb::CJsonObject oJson("{\"refresh_interval\":60,"
//                           "\"test_float\":[18.0, 10.0, 5.0],"
//                           "\"timeout\":12.5,"
//                           "\"dynamic_loading\":["
//                           "{"
//                           "\"so_path\":\"plugins/User.so\", \"load\":false, \"version\":1,"
//                           "\"cmd\":["
//                           "{\"cmd\":2001, \"class\":\"neb::CmdUserLogin\"},"
//                           "{\"cmd\":2003, \"class\":\"neb::CmdUserLogout\"}"
//                           "],"
//                           "\"module\":["
//                           "{\"path\":\"im/user/login\", \"class\":\"neb::ModuleLogin\"},"
//                           "{\"path\":\"im/user/logout\", \"class\":\"neb::ModuleLogout\"}"
//                           "]"
//                           "},"
//                           "{"
//                           "\"so_path\":\"plugins/ChatMsg.so\", \"load\":false, \"version\":1,"
//                           "\"cmd\":["
//                           "{\"cmd\":2001, \"class\":\"neb::CmdChat\"}"
//                           "],"
//                           "\"module\":[]"
//                           "}"
//                           "]"
//                           "}");
//    std::cout << oJson.ToString() << std::endl;
//    std::cout << "-------------------------------------------------------------------" << std::endl;
//    std::cout << oJson["dynamic_loading"][0]["cmd"][1]("class") << std::endl;
//    oJson["dynamic_loading"][0]["cmd"][0].Get("cmd", iValue);
//    std::cout << "iValue = " << iValue << std::endl;
//    oJson["dynamic_loading"][0]["cmd"][0].Replace("cmd", -2001);
//    oJson["dynamic_loading"][0]["cmd"][0].Get("cmd", iValue);
//    std::cout << "iValue = " << iValue << std::endl;
//    oJson.Get("timeout", fTimeout);
//    std::cout << "fTimeout = " << fTimeout << std::endl;
//    oJson["dynamic_loading"][0]["module"][0].Get("path", strValue);
//    std::cout << "strValue = " << strValue << std::endl;
//    std::cout << "-------------------------------------------------------------------" << std::endl;
//    oJson.AddEmptySubObject("depend");
//    oJson["depend"].Add("nebula", "https://github.com/Bwar/Nebula");
//    oJson["depend"].AddEmptySubArray("bootstrap");
//    oJson["depend"]["bootstrap"].Add("BEACON");
//    oJson["depend"]["bootstrap"].Add("LOGIC");
//    oJson["depend"]["bootstrap"].Add("LOGGER");
//    oJson["depend"]["bootstrap"].Add("INTERFACE");
//    oJson["depend"]["bootstrap"].Add("ACCESS");
//    std::cout << oJson.ToString() << std::endl;
//    std::cout << "-------------------------------------------------------------------" << std::endl;
//    std::cout << oJson.ToFormattedString() << std::endl;
//
//    std::cout << "-------------------------------------------------------------------" << std::endl;
//    neb::CJsonObject oCopyJson = oJson;
//    if (oCopyJson == oJson)
//    {
//        std::cout << "json equal" << std::endl;
//    }
//    oCopyJson["depend"]["bootstrap"].Delete(1);
//    oCopyJson["depend"].Replace("nebula", "https://github.com/Bwar/CJsonObject");
//    std::cout << oCopyJson.ToString() << std::endl;
//    std::cout << "-------------------------key traverse------------------------------" << std::endl;
//    std::string strTraversing;
//    while(oJson["dynamic_loading"][0].GetKey(strTraversing))
//    {
//        std::cout << "traversing:  " << strTraversing << std::endl;
//    }
//    std::cout << "---------------add a new key, then key traverse---------------------" << std::endl;
//    oJson["dynamic_loading"][0].Add("new_key", "new_value");
//    while(oJson["dynamic_loading"][0].GetKey(strTraversing))
//    {
//        std::cout << "traversing:  " << strTraversing << std::endl;
//    }
//
//    std::cout << oJson["test_float"].GetArraySize() << std::endl;
//    float fTestValue = 0.0;
//    for (int i = 0; i < oJson["test_float"].GetArraySize(); ++i)
//    {
//        oJson["test_float"].Get(i, fTestValue);
//        std::cout << fTestValue << std::endl;
//    }
//    oJson.AddNull("null_value");
//    std::cout << oJson.IsNull("test_float") << "\t" << oJson.IsNull("null_value") << std::endl;
//    oJson["test_float"].AddNull();
//    std::cout << oJson.ToString() << std::endl;
//}


int main() {
    LR1TableMaker parser;

    string fileName = "grammar";
    string in = ".txt";
    string out = ".json";

    parser.load(fileName + in);
    LR1Table table = parser.makeTable();
    table.printJsonFile(fileName + out);
    table.printOut();
//    LR1Table table2;
//    table2.loadJsonFile(fileName + out);
//    table2.printOut();
    return 0;
}

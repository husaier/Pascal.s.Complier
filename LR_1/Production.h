//
// Created by husserl on 2020/5/30.
//
#include <iostream>
#include <string>
#include <vector>
#include "CJsonObject.hpp"
#ifndef LR1_TABLECREATE_PRODUCTION_H
#define LR1_TABLECREATE_PRODUCTION_H
using namespace std;

class Production {
public:
    string left;         //左部只有一个符号
    vector<string> right;//右部可能有多个符号

    bool operator== (const Production &t) const {
        return this->left == t.left && this->right == t.right;
    }

    neb::CJsonObject toJsonObject();

    void fromJson(neb::CJsonObject);

private:
    //JSON tag
    string LEFT = "left";
    string RIGHT = "right";
};


#endif //LR1_TABLECREATE_PRODUCTION_H

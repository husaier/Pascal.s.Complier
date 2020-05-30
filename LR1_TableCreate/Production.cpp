//
// Created by husserl on 2020/5/30.
//

#include "Production.h"

neb::CJsonObject Production::toJsonObject() {
    neb::CJsonObject oJson;
    oJson.Add(LEFT, left);
    oJson.AddEmptySubArray(RIGHT);
    for (const auto& s : right) {
        oJson[RIGHT].Add(s);
    }
    return oJson;
}

void Production::fromJson(neb::CJsonObject oJson) {
    oJson.Get(LEFT, left);
    for (int i = 0; i < oJson[RIGHT].GetArraySize(); ++i) {
        string s;
        oJson[RIGHT].Get(i, s);
        right.push_back(s);
    }
}

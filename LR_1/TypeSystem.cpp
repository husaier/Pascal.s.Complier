//
// Created by husserl on 2020/7/16.
//

#include "TypeSystem.h"


Type::Type(int a) {
    x = a;
}

Type::~Type() = default;

bool Type::operator==(Type &a) const {
    return x == a.x;
}

bool Type::operator!=(Type &a) const {
    return x != a.x;
}

int Type::getType() const {
    return x;
}

void Type::addElem(Type *e) {

}

int Type::getWidth() {
    switch (x) {
        case INTEGER:
        case REAL:
            return 4;
        case CHAR:
        case BOOLEAN:
            return 1;
        default:
            return 0;
    }
}

bool Type::isBasicType() const {
    return x == BOOLEAN ||
           x == INTEGER ||
           x == REAL ||
           x == CHAR;
}

int Type::getDimension() {
    return 0;
}

Type* Type::copy() {
    Type *t = this;
    if(t->getType() == ARRAY){
        auto array = (Array*)t;
        return new Array(*array);
    } else if (t->getType() == RECORD) {
        auto record = (Record*)t;
        return new Record(*record);
    } else {
        return new Type(*t);
    }
}

Array::Array(int l, int h):
    Type(Type::ARRAY)
{
    low = l;
    high = h;
    length = h - l + 1;
}

Array::Array(const Array &a):
    Type(Type::ARRAY)
{
    low = a.low;
    high = a.high;
    length = high - low;
    elem = nullptr;
    auto head = a.elem;
    int type = head->getType();
    if(type == Type::ARRAY) {
        auto array = (Array*)head;
        elem = new Array(*array);
    } else if (type == Type::RECORD) {
        auto record = (Record*)head;
        elem = new Record(*record);
    } else {
        elem = new Type(*head);
    }
}

Array::~Array() {
    delete elem;
}

bool Array::operator==(Type &a) const {
    if (!Type::operator==(a))
        return false;
    else {
        auto *array = (Array*)&a;
        return low == array->low && high == array->high
        && (*elem == *(array->elem));
    }
}

bool Array::operator!=(Type &a) const {
    return !(*this == a);
}

void Array::addElem(Type *e) {
    auto head = (Array*)this;
    while(head != nullptr) {
        if (head->elem == nullptr
        || head->elem->getType() != Type::ARRAY)
            break;
        head = (Array*)head->elem;
    }
    head->elem = e;
}

int Array::getWidth() {
    return length * (elem->getWidth());
}

int Array::getDimension() {
    return elem->getDimension() + 1;
}

EnvItem::EnvItem() = default;

EnvItem::~EnvItem() {
    delete type;
}

bool EnvItem::operator==(const EnvItem &a) const {
    return id == id && *type == *a.type;
}

bool EnvItem::operator!=(const EnvItem &a) const{
    return !(EnvItem::operator==(a));
}

EnvItem::EnvItem(const EnvItem &e) {
    id = e.id;
    if (e.type->getType() == Type::RECORD) {
        auto record = (Record*)e.type;
        type = new Record(*record);
    } else if (e.type->getType() == Type::ARRAY) {
        auto array = (Array*)e.type;
        type = new Array(*array);
    } else {
        type = new Type(*e.type);
    }
}

Record::Record(): Type(Type::RECORD) { }

Record::Record(const Record &r):
        Type(Type::RECORD)
{
    env = r.env;
}

Record::~Record() {
    env.clear();
}

bool Record::operator==(Type &a) const {
    bool flag = true;
    if (a.getType() != Type::RECORD)
        return false;
    auto record = (Record*)&a;
    return env == record->env;
}

bool Record::operator!=(Type &a) const {
    return !(Record::operator==(a));
}

int Record::getWidth() {
    return Type::getWidth();
}

int Record::getDimension() {
    return Type::getDimension();
}

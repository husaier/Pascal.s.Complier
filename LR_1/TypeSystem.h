//
// Created by husserl on 2020/7/16.
//

#ifndef LR_1_TYPESYSTEM_H
#define LR_1_TYPESYSTEM_H
#include <iostream>
#include <vector>
using namespace std;

class Type {
public:
    enum {
        TYPE_ERROR = -1, VOID = 0,
        INTEGER = 1, REAL = 2, BOOLEAN = 3,
        CHAR = 4, ARRAY = 5, RECORD = 6,
        FUNC = 7, PROC = 8 , PROGRAM = 9
    };

    explicit Type(int);

    virtual ~Type();

    virtual bool operator == (Type&) const;

    virtual bool operator != (Type&) const;

    virtual void addElem(Type*);

    virtual int getWidth(); // 获取类型的位宽，单位：比特

    virtual int getSize(); // 获取变量个数

    virtual int getDimension();

    int getType() const;

    bool isBasicType() const;

    Type* copy();

protected:
    int x;
};

class Array: public Type{
public:
    explicit Array(int,int);

    Array(const Array&);

    ~Array() override;

    bool operator == (Type&) const override;

    bool operator != (Type&) const override;

    void addElem(Type*) override;

    int getWidth() override;

    int getSize() override;

    int getDimension() override;

    Type *elem{nullptr};
    int low, high;
    int length;
private:

};

class EnvItem{
public:
    EnvItem();

    EnvItem(const EnvItem&);

    ~EnvItem();

    bool operator == (const EnvItem&) const;

    bool operator != (const EnvItem&) const;

    int getWidth() const;

    int getSize() const;

    string id;
    Type *type{nullptr};
};

class Record: public Type{
public:
    explicit Record();

    Record(const Record&);

    ~Record() override;

    bool operator == (Type&) const override;

    bool operator != (Type&) const override;

    int getWidth() override;

    int getSize() override;

    int getDimension() override;

    vector<EnvItem> env;
};

class Func: public Type{
public:
    explicit Func();

    bool operator == (Type&) const override;

    bool operator != (Type&) const override;

    int parametersNum;//传参个数
    vector<EnvItem> env;//存储各个参数的类型
    Type *reType{nullptr}; // 返回值类型
};

class Proc: public Type{
public:
    explicit Proc();

    bool operator == (Type&) const override;

    bool operator != (Type&) const override;

    int parametersNum;//传参个数
    vector<EnvItem> env;//存储各个参数的类型
};

#endif //LR_1_TYPESYSTEM_H

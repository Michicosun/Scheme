#pragma once

#include "error.h"
#include "tokenizer.h"

class Object {
public:
    virtual ~Object() = default;
};

class Number : public Object {
public:
    Number(int64_t num) : num_{num} {
    }

    ~Number() override = default;

    int64_t GetValue() const {
        return num_;
    }

private:
    int64_t num_;
};

class Bool : public Object {
public:
    Bool(bool var) : var_{var} {
    }

    ~Bool() override = default;

    bool GetValue() const {
        return var_;
    }

private:
    bool var_;
};

class Symbol : public Object {
public:
    Symbol(const std::string& symbol) : symbol_{symbol} {
    }

    ~Symbol() override = default;

    const std::string& GetName() const {
        return symbol_;
    }

private:
    std::string symbol_;
};

class Cell : public Object {
public:
    ~Cell() override = default;

    Object*& GetFirst() {
        return first_;
    }

    Object*& GetSecond() {
        return second_;
    }

private:
    Object* first_{nullptr};
    Object* second_{nullptr};
};

template <class T>
T* As(Object* obj) {
    return dynamic_cast<T*>(obj);
}

template <class T>
bool Is(Object* obj) {
    auto ptr = dynamic_cast<T*>(obj);
    return ptr != nullptr;
}

#pragma once
#include <string>
#include <vector>

#include "garbage_collector.h"
#include "object.h"
#include "scope.h"

class Interpreter {
public:
    Interpreter();

    std::string Run(const std::string& expr);

    Scope* GetGlobalScope();

    ~Interpreter();

private:
    Scope* global_scope_;
};

bool CheckPair(Object* ptr);

bool CheckList(Object* ptr);

Object* CalcExpression(Object* object, Scope* scope);

std::vector<Object*> Convert(Object* ptr);
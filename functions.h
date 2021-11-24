#pragma once

#include <cstddef>
#include <vector>
#include "garbage_collector.h"
#include "object.h"
#include "scheme.h"
#include "tokenizer.h"
#include "scope.h"

class Function : public Object {
public:
    virtual ~Function() = default;

    virtual Object* Invoke(Object* ptr, Scope* scope) = 0;  // 1 - function
};

Function* GetVariable(Object* ptr, Scope* scope);
Function* GenerateFunction(Object* ptr, Scope* scope);

// Numbers
class IsNumberFunction : public Function {
public:
    ~IsNumberFunction() override = default;

    Object* Invoke(Object* ptr, Scope* scope) override;
};

class SumFunction : public Function {
public:
    ~SumFunction() override = default;

    Object* Invoke(Object* ptr, Scope* scope) override;
};

class SubtractFunction : public Function {
public:
    ~SubtractFunction() override = default;

    Object* Invoke(Object* ptr, Scope* scope) override;
};

class EqualFunction : public Function {
public:
    ~EqualFunction() override = default;

    Object* Invoke(Object* ptr, Scope* scope) override;
};

class GreaterFunction : public Function {
public:
    ~GreaterFunction() override = default;

    Object* Invoke(Object* ptr, Scope* scope) override;
};

class GreaterOrEqualFunction : public Function {
public:
    ~GreaterOrEqualFunction() override = default;

    Object* Invoke(Object* ptr, Scope* scope) override;
};

class LessFunction : public Function {
public:
    ~LessFunction() override = default;

    Object* Invoke(Object* ptr, Scope* scope) override;
};

class LessOrEqualFunction : public Function {
public:
    ~LessOrEqualFunction() override = default;

    Object* Invoke(Object* ptr, Scope* scope) override;
};

class MultFunction : public Function {
public:
    ~MultFunction() override = default;

    Object* Invoke(Object* ptr, Scope* scope) override;
};

class DivFunction : public Function {
public:
    ~DivFunction() override = default;

    Object* Invoke(Object* ptr, Scope* scope) override;
};

class MaxFunction : public Function {
public:
    ~MaxFunction() override = default;

    Object* Invoke(Object* ptr, Scope* scope) override;
};

class MinFunction : public Function {
public:
    ~MinFunction() override = default;

    Object* Invoke(Object* ptr, Scope* scope) override;
};

class AbsFunction : public Function {
public:
    ~AbsFunction() override = default;

    Object* Invoke(Object* ptr, Scope* scope) override;
};

// Booleans
class IsBoolFunction : public Function {
public:
    ~IsBoolFunction() override = default;

    Object* Invoke(Object* ptr, Scope* scope) override;
};

class NotFunction : public Function {
public:
    ~NotFunction() override = default;

    Object* Invoke(Object* ptr, Scope* scope) override;
};

class AndFunction : public Function {
public:
    ~AndFunction() override = default;

    Object* Invoke(Object* ptr, Scope* scope) override;
};

class OrFunction : public Function {
public:
    ~OrFunction() override = default;

    Object* Invoke(Object* ptr, Scope* scope) override;
};

// Quotes
class QuoteFunction : public Function {
public:
    ~QuoteFunction() = default;

    Object* Invoke(Object* ptr, Scope* scope) override;
};

// Lists
class IsListFunction : public Function {
public:
    ~IsListFunction() override = default;

    Object* Invoke(Object* ptr, Scope* scope) override;
};

class IsPairFunction : public Function {
public:
    ~IsPairFunction() override = default;

    Object* Invoke(Object* ptr, Scope* scope) override;
};

class IsNullFunction : public Function {
public:
    ~IsNullFunction() override = default;

    Object* Invoke(Object* ptr, Scope* scope) override;
};

class ConsFunction : public Function {
public:
    ~ConsFunction() override = default;

    Object* Invoke(Object* ptr, Scope* scope) override;
};

class CarFunction : public Function {
public:
    ~CarFunction() override = default;

    Object* Invoke(Object* ptr, Scope* scope) override;
};

class CdrFunction : public Function {
public:
    ~CdrFunction() override = default;

    Object* Invoke(Object* ptr, Scope* scope) override;
};

class ListFunction : public Function {
public:
    ~ListFunction() override = default;

    Object* Invoke(Object* ptr, Scope* scope) override;
};

class ListRefFunction : public Function {
public:
    ~ListRefFunction() override = default;

    Object* Invoke(Object* ptr, Scope* scope) override;
};

class ListTailFunction : public Function {
public:
    ~ListTailFunction() override = default;

    Object* Invoke(Object* ptr, Scope* scope) override;
};

// IF
class IfFunction : public Function {
public:
    ~IfFunction() override = default;

    Object* Invoke(Object* ptr, Scope* scope) override;
};

// Variable
class Variable : public Function {
public:
    Variable(Object* ptr);

    ~Variable() override = default;

    Object* Invoke(Object* ptr, Scope* scope) override;

    Object* GetVal();

    Object* var_;
};

class IsSymbolFunction : public Function {
public:
    ~IsSymbolFunction() override = default;

    Object* Invoke(Object* ptr, Scope* scope) override;
};

class DefineFunction : public Function {
public:
    ~DefineFunction() override = default;

    Object* Invoke(Object* ptr, Scope* scope) override;
};

class SetFunction : public Function {
public:
    ~SetFunction() override = default;

    Object* Invoke(Object* ptr, Scope* scope) override;
};

class SetCarFunction : public Function {
public:
    ~SetCarFunction() override = default;

    Object* Invoke(Object* ptr, Scope* scope) override;
};

class SetCdrFunction : public Function {
public:
    ~SetCdrFunction() override = default;

    Object* Invoke(Object* ptr, Scope* scope) override;
};

// Lambda
class Lambda : public Function {
public:
    Lambda(Scope* scope, const std::vector<Object*>& params, const std::vector<Object*>& actions);

    ~Lambda() override = default;

    Object* Invoke(Object* ptr, Scope* scope) override;

    Scope* par_scope_;
    std::vector<Object*> params_;
    std::vector<Object*> actions_;
};

class LambdaGenerator : public Function {
public:
    LambdaGenerator(Scope* scope);

    ~LambdaGenerator() override = default;

    Object* Invoke(Object* ptr, Scope* scope) override;

    Scope* scope_;
};
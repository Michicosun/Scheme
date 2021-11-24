#include "functions.h"
#include <cstddef>
#include <vector>
#include "error.h"
#include "object.h"
#include "scheme.h"
#include "scope.h"
#include "garbage_collector.h"

Function* GetVariable(Object* ptr, Scope* scope) {
    if (Is<Number>(ptr)) {
        return GetGC().New<Variable>(ptr);
    } else if (Is<Symbol>(ptr)) {
        auto gen = scope;
        auto ans = gen->Get(As<Symbol>(ptr)->GetName());
        if (ans != nullptr) {
            if (!Is<Variable>(ans)) {
                ans = GetGC().New<Variable>(ans);
            }
            return As<Variable>(ans);
        }
        if (As<Symbol>(ptr)->GetName() == "lambda") {
            return GetGC().New<Variable>(GetGC().New<LambdaGenerator>(scope));
        }
    } else if (Is<Bool>(ptr)) {
        return GetGC().New<Variable>(ptr);
    }
    if (scope->par_scope_ == nullptr) {
        throw NameError("There is no variable with such name");
    } else {
        return GetVariable(ptr, scope->par_scope_);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
/// general
template <class T>
std::vector<Object*> CalcVector(const std::vector<Object*>& input, Scope* scope) {
    std::vector<Object*> ans;
    for (size_t i = 1; i < input.size(); ++i) {
        ans.push_back(CalcExpression(input[i], scope));
        if (!Is<T>(ans.back())) {
            throw RuntimeError("Bad input");
        }
    }
    return ans;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Number
Object* IsNumberFunction::Invoke(Object* ptr, Scope* scope) {
    auto input = Convert(ptr);
    if (input.size() != 2) {
        throw RuntimeError("Is Number function should have only 1 parameter");
    }
    if (Is<Number>(CalcExpression(input[1], scope))) {
        return GetGC().New<Bool>(true);
    } else {
        return GetGC().New<Bool>(false);
    }
}

Object* SumFunction::Invoke(Object* ptr, Scope* scope) {
    auto input = Convert(ptr);
    auto params = CalcVector<Number>(input, scope);
    int64_t sum = 0;
    for (const auto& el : params) {
        sum += As<Number>(el)->GetValue();
    }
    return GetGC().New<Number>(sum);
}

Object* SubtractFunction::Invoke(Object* ptr, Scope* scope) {
    auto input = Convert(ptr);
    auto params = CalcVector<Number>(input, scope);
    if (params.empty()) {
        throw RuntimeError("Subtrack function sould have > 0 parameters");
    }
    int64_t sum = As<Number>(params[0])->GetValue();
    for (size_t i = 1; i < params.size(); ++i) {
        sum -= As<Number>(params[i])->GetValue();
    }
    return GetGC().New<Number>(sum);
}

Object* EqualFunction::Invoke(Object* ptr, Scope* scope) {
    auto input = Convert(ptr);
    auto params = CalcVector<Number>(input, scope);
    for (size_t i = 1; i < params.size(); ++i) {
        if (As<Number>(params[0])->GetValue() != As<Number>(params[i])->GetValue()) {
            return GetGC().New<Bool>(false);
        }
    }
    return GetGC().New<Bool>(true);
}

Object* GreaterFunction::Invoke(Object* ptr, Scope* scope) {
    auto input = Convert(ptr);
    auto params = CalcVector<Number>(input, scope);
    for (size_t i = 1; i < params.size(); ++i) {
        if (As<Number>(params[i - 1])->GetValue() <= As<Number>(params[i])->GetValue()) {
            return GetGC().New<Bool>(false);
        }
    }
    return GetGC().New<Bool>(true);
}

Object* GreaterOrEqualFunction::Invoke(Object* ptr, Scope* scope) {
    auto input = Convert(ptr);
    auto params = CalcVector<Number>(input, scope);
    for (size_t i = 1; i < params.size(); ++i) {
        if (As<Number>(params[i - 1])->GetValue() < As<Number>(params[i])->GetValue()) {
            return GetGC().New<Bool>(false);
        }
    }
    return GetGC().New<Bool>(true);
}

Object* LessFunction::Invoke(Object* ptr, Scope* scope) {
    auto input = Convert(ptr);
    auto params = CalcVector<Number>(input, scope);
    for (size_t i = 1; i < params.size(); ++i) {
        if (As<Number>(params[i - 1])->GetValue() >= As<Number>(params[i])->GetValue()) {
            return GetGC().New<Bool>(false);
        }
    }
    return GetGC().New<Bool>(true);
}

Object* LessOrEqualFunction::Invoke(Object* ptr, Scope* scope) {
    auto input = Convert(ptr);
    auto params = CalcVector<Number>(input, scope);
    for (size_t i = 1; i < params.size(); ++i) {
        if (As<Number>(params[i - 1])->GetValue() > As<Number>(params[i])->GetValue()) {
            return GetGC().New<Bool>(false);
        }
    }
    return GetGC().New<Bool>(true);
}

Object* MultFunction::Invoke(Object* ptr, Scope* scope) {
    auto input = Convert(ptr);
    auto params = CalcVector<Number>(input, scope);
    int64_t prod = 1;
    for (const auto& el : params) {
        prod *= As<Number>(el)->GetValue();
    }
    return GetGC().New<Number>(prod);
}

Object* DivFunction::Invoke(Object* ptr, Scope* scope) {
    auto input = Convert(ptr);
    auto params = CalcVector<Number>(input, scope);
    if (params.empty()) {
        throw RuntimeError("Div function should have >= 2 parameters");
    }
    int64_t ans = As<Number>(params[0])->GetValue();
    for (size_t i = 1; i < params.size(); ++i) {
        ans /= As<Number>(params[i])->GetValue();
    }
    return GetGC().New<Number>(ans);
}

Object* MaxFunction::Invoke(Object* ptr, Scope* scope) {
    auto input = Convert(ptr);
    auto params = CalcVector<Number>(input, scope);
    if (params.empty()) {
        throw RuntimeError("Max function should have >= 1 parameters");
    }
    int64_t ans = As<Number>(params[0])->GetValue();
    for (size_t i = 1; i < params.size(); ++i) {
        ans = std::max(ans, As<Number>(params[i])->GetValue());
    }
    return GetGC().New<Number>(ans);
}

Object* MinFunction::Invoke(Object* ptr, Scope* scope) {
    auto input = Convert(ptr);
    auto params = CalcVector<Number>(input, scope);
    if (params.empty()) {
        throw RuntimeError("Min function should have >= 1 parameters");
    }
    int64_t ans = As<Number>(params[0])->GetValue();
    for (size_t i = 1; i < params.size(); ++i) {
        ans = std::min(ans, As<Number>(params[i])->GetValue());
    }
    return GetGC().New<Number>(ans);
}

Object* AbsFunction::Invoke(Object* ptr, Scope* scope) {
    auto input = Convert(ptr);
    auto params = CalcVector<Number>(input, scope);
    if (params.size() != 1) {
        throw RuntimeError("Abs function should have 1 parameter");
    }
    return GetGC().New<Number>(std::abs(As<Number>(params[0])->GetValue()));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Bool
Bool* GenerateBool(Object* ptr) {
    if (Is<Bool>(ptr)) {
        return As<Bool>(ptr);
    } else {
        return GetGC().New<Bool>(true);
    }
}

Object* IsBoolFunction::Invoke(Object* ptr, Scope* scope) {
    auto input = Convert(ptr);
    if (input.size() != 2) {
        throw RuntimeError("Is bool function should have 1 parameter");
    }
    if (Is<Bool>(CalcExpression(input[1], scope))) {
        return GetGC().New<Bool>(true);
    } else {
        return GetGC().New<Bool>(false);
    }
}

Object* NotFunction::Invoke(Object* ptr, Scope* scope) {
    auto input = Convert(ptr);
    if (input.size() != 2) {
        throw RuntimeError("Not function should have 1 parameter");
    }
    auto obj = CalcExpression(input[1], scope);
    auto ans = GenerateBool(obj);
    return GetGC().New<Bool>(!ans->GetValue());
}

Object* AndFunction::Invoke(Object* ptr, Scope* scope) {
    auto input = Convert(ptr);
    if (input.size() == 1) {
        return GetGC().New<Bool>(true);
    }
    for (size_t i = 1; i < input.size(); ++i) {
        auto obj = CalcExpression(input[i], scope);
        if (!GenerateBool(obj)->GetValue()) {
            return obj;
        }
    }
    return CalcExpression(input.back(), scope);
}

Object* OrFunction::Invoke(Object* ptr, Scope* scope) {
    auto input = Convert(ptr);
    if (input.size() == 1) {
        return GetGC().New<Bool>(false);
    }
    for (size_t i = 1; i < input.size(); ++i) {
        auto obj = CalcExpression(input[i], scope);
        if (GenerateBool(obj)->GetValue()) {
            return obj;
        }
    }
    return CalcExpression(input.back(), scope);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Quotes
int64_t CntParameters(Object* ptr) {
    int64_t ans = 0;
    Object* last = As<Cell>(ptr)->GetSecond();
    while (last) {
        if (!Is<Cell>(last)) {
            throw RuntimeError("Bad quote expression");
        }
        ++ans;
        last = As<Cell>(last)->GetSecond();
    }
    return ans;
}

Object* QuoteFunction::Invoke(Object* ptr, Scope*) {
    if (!Is<Cell>(ptr)) {
        throw RuntimeError("Quote must have a parameter");
    }
    if (CntParameters(ptr) != 1) {
        throw RuntimeError("Quote must have 1 parameter");
    }
    return As<Cell>(As<Cell>(ptr)->GetSecond())->GetFirst();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Lists

Object* IsNullFunction::Invoke(Object* ptr, Scope* scope) {
    auto input = Convert(ptr);
    if (input.size() != 2) {
        throw RuntimeError("Is null function should have 1 parameter");
    }
    auto obj = CalcExpression(input[1], scope);
    return GetGC().New<Bool>(obj == nullptr);
}

Object* IsPairFunction::Invoke(Object* ptr, Scope* scope) {
    auto input = Convert(ptr);
    if (input.size() != 2) {
        throw RuntimeError("Is pair function should have 1 parameter");
    }
    auto obj = CalcExpression(input[1], scope);
    auto list = Convert(obj);
    return GetGC().New<Bool>(Is<Cell>(obj) && list.size() == 2);
}

Object* IsListFunction::Invoke(Object* ptr, Scope* scope) {
    auto input = Convert(ptr);
    if (input.size() != 2) {
        throw RuntimeError("Is list function should have 1 parameter");
    }
    ptr = CalcExpression(input[1], scope);
    while (Is<Cell>(ptr)) {
        ptr = As<Cell>(ptr)->GetSecond();
    }
    return GetGC().New<Bool>(ptr == nullptr);
}

Object* ConsFunction::Invoke(Object* ptr, Scope* scope) {
    auto input = Convert(ptr);
    if (input.size() != 3) {
        throw RuntimeError("Cons function should have 2 parameters");
    }
    auto car = CalcExpression(input[1], scope);
    auto cdr = CalcExpression(input[2], scope);
    Cell* ans(GetGC().New<Cell>());
    ans->GetFirst() = car;
    ans->GetSecond() = cdr;
    return ans;
}

Object* CarFunction::Invoke(Object* ptr, Scope* scope) {
    auto input = Convert(ptr);
    if (input.size() != 2) {
        throw RuntimeError("Car function must have 1 parameter");
    }
    ptr = CalcExpression(input[1], scope);
    if (!ptr || !Is<Cell>(ptr)) {
        throw RuntimeError("Car function's parameter should be a pair or a non-empty list");
    }
    return As<Cell>(ptr)->GetFirst();
}

Object* CdrFunction::Invoke(Object* ptr, Scope* scope) {
    auto input = Convert(ptr);
    auto params = CalcVector<Object>(input, scope);
    if (input.size() != 2) {
        throw RuntimeError("Cdr function must have 1 parameter");
    }
    if (!params[0] || !Is<Cell>(params[0])) {
        throw RuntimeError("Cdr function's parameter should be a pair or a non-empty list");
    }
    return As<Cell>(params[0])->GetSecond();
}

Object* ListFunction::Invoke(Object* ptr, Scope* scope) {
    auto input = Convert(ptr);
    auto params = CalcVector<Number>(input, scope);
    if (params.empty()) {
        return nullptr;
    }
    Cell* start(GetGC().New<Cell>());
    auto last = start;
    for (size_t i = 0; i < params.size(); ++i) {
        last->GetFirst() = params[i];
        if (i + 1 < params.size()) {
            last->GetSecond() = GetGC().New<Cell>();
            last = As<Cell>(last->GetSecond());
        }
    }
    return start;
}

Object* ListRefFunction::Invoke(Object* ptr, Scope* scope) {
    auto input = Convert(ptr);
    auto params = CalcVector<Object>(input, scope);
    if (params.size() != 2) {
        throw RuntimeError("List ref function should have 2 params");
    }
    if (!(Is<Cell>(params[0]) && Is<Number>(params[1]))) {
        throw RuntimeError("List ref function bad params");
    }
    input = Convert(params[0]);
    size_t ind = As<Number>(params[1])->GetValue();
    if (ind >= input.size()) {
        throw RuntimeError("List ref function is not in the correct range");
    } else {
        return input[ind];
    }
}

Object* ListTailFunction::Invoke(Object* ptr, Scope* scope) {
    auto input = Convert(ptr);
    auto params = CalcVector<Object>(input, scope);
    if (params.size() != 2) {
        throw RuntimeError("List tail function should have 2 params");
    }
    if (!(Is<Cell>(params[0]) && Is<Number>(params[1]))) {
        throw RuntimeError("List tail function bad params");
    }
    input = Convert(params[0]);
    size_t ind = As<Number>(params[1])->GetValue();
    if (ind > input.size()) {
        throw RuntimeError("List ref function is not in the correct range");
    } else {
        input.erase(input.begin(), input.begin() + ind);
        if (input.empty()) {
            return nullptr;
        }
        Cell* start(GetGC().New<Cell>());
        auto last = start;
        for (size_t i = 0; i < input.size(); ++i) {
            last->GetFirst() = input[i];
            if (i + 1 < input.size()) {
                last->GetSecond() = GetGC().New<Cell>();
                last = As<Cell>(last->GetSecond());
            }
        }
        return start;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
/// If

Object* IfFunction::Invoke(Object* ptr, Scope* scope) {
    auto input = Convert(ptr);
    if (input.size() <= 2) {
        throw SyntaxError("If must have at least 1 statement and 1 branch");
    }
    auto stat_ans = CalcExpression(input[1], scope);
    if (!Is<Bool>(stat_ans)) {
        throw SyntaxError("If's statement should convert to bool");
    }
    if (As<Bool>(stat_ans)->GetValue()) {
        return CalcExpression(input[2], scope);
    } else {
        if (input.size() < 4) {
            return nullptr;
        } else {
            return CalcExpression(input[3], scope);
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Variable

Variable::Variable(Object* ptr) : var_{ptr} {
}

Object* Variable::Invoke(Object* ptr, Scope*) {
    auto input = Convert(ptr);
    if (input.size() > 1) {
        throw RuntimeError("Variables evaluates to temselfs, they need no parameters");
    }
    return var_;
}

Object* Variable::GetVal() {
    return var_;
}

Object* DefineLambda(Object* ptr, Scope* scope) {
    auto input = Convert(ptr);
    auto lambda_capture = Convert(input[1]);
    std::vector<Object*> lambda_capture_params;
    for (size_t i = 0; i < lambda_capture.size(); ++i) {
        if (!Is<Symbol>(lambda_capture[i])) {
            throw RuntimeError(
                "Define with lambda sugar should has only symbols in first parameter");
        }
        lambda_capture_params.push_back(lambda_capture[i]);
    }
    auto name = lambda_capture_params[0];

    lambda_capture_params.erase(lambda_capture_params.begin());

    auto lambda_actions = input;
    lambda_actions.erase(lambda_actions.begin(), lambda_actions.begin() + 2);

    auto rhs = GetGC().New<Lambda>(scope, lambda_capture_params, lambda_actions);
    scope->Set(As<Symbol>(name)->GetName(), rhs);
    return rhs;
}

Object* DefineFunction::Invoke(Object* ptr, Scope* scope) {
    auto input = Convert(ptr);
    if (input.size() < 2) {
        throw SyntaxError("Any define should have 2 or more parameters");
    }
    if (Is<Cell>(input[1])) {
        return DefineLambda(ptr, scope);
    }
    if (input.size() != 3) {
        throw SyntaxError("Define should have 2 parameters");
    }
    auto lhs = input[1];
    auto rhs = CalcExpression(input[2], scope);
    if (!Is<Symbol>(lhs)) {
        throw RuntimeError("Define should have a string as a first parameter");
    }
    if (Is<Lambda>(rhs)) {
        scope->Set(As<Symbol>(lhs)->GetName(), rhs);
    } else {
        scope->Set(As<Symbol>(lhs)->GetName(), GetGC().New<Variable>(rhs));
    }
    return rhs;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Set

void TrySet(Object* name, Object* val, Scope* scope) {
    bool is_set = scope->TrySet(As<Symbol>(name)->GetName(), val);
    if (is_set) {
        return;
    }
    if (scope->par_scope_ == nullptr) {
        throw NameError("There is no variable with such name");
    }
    TrySet(name, val, scope->par_scope_);
}

Object* SetFunction::Invoke(Object* ptr, Scope* scope) {
    auto input = Convert(ptr);
    if (input.size() != 3) {
        throw SyntaxError("Set function should have 2 parameters");
    }
    if (!Is<Symbol>(input[1])) {
        throw RuntimeError("Set function should have symbol as the first parameter");
    }
    auto name = input[1];
    auto val = CalcExpression(input[2], scope);
    auto ret_val = val;
    if (!Is<Lambda>(val)) {
        val = GetGC().New<Variable>(val);
    }
    TrySet(name, val, scope);
    return ret_val;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
/// SetCar

void TrySetCar(Object* name, Object* val, Scope* scope) {
    bool is_set = scope->TrySetCar(As<Symbol>(name)->GetName(), val);
    if (is_set) {
        return;
    }
    if (scope->par_scope_ == nullptr) {
        throw NameError("There is no variable with such name");
    }
    TrySetCar(name, val, scope->par_scope_);
}

Object* SetCarFunction::Invoke(Object* ptr, Scope* scope) {
    auto input = Convert(ptr);
    if (input.size() != 3) {
        throw RuntimeError("Set-car function should have 2 parameters");
    }
    auto name = CalcExpression(input[1], scope);
    if (!Is<Cell>(name)) {
        throw RuntimeError("Set-car function should have pair or list as the first parameter");
    }
    auto val = CalcExpression(input[2], scope);
    As<Cell>(name)->GetFirst() = val;
    return val;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
/// SetCdr

void TrySetCdr(Object* name, Object* val, Scope* scope) {
    bool is_set = scope->TrySetCdr(As<Symbol>(name)->GetName(), val);
    if (is_set) {
        return;
    }
    if (scope->par_scope_ == nullptr) {
        throw NameError("There is no variable with such name");
    }
    TrySetCdr(name, val, scope->par_scope_);
}

Object* SetCdrFunction::Invoke(Object* ptr, Scope* scope) {
    auto input = Convert(ptr);
    if (input.size() != 3) {
        throw RuntimeError("Set-cdr function should have 2 parameters");
    }
    auto name = CalcExpression(input[1], scope);
    if (!Is<Cell>(name)) {
        throw RuntimeError("Set-cdr function should have symbol as the first parameter");
    }
    auto val = CalcExpression(input[2], scope);
    As<Cell>(name)->GetSecond() = val;
    return val;
}

Object* IsSymbolFunction::Invoke(Object* ptr, Scope* scope) {
    auto input = Convert(ptr);
    if (input.size() != 2) {
        throw RuntimeError("Is Symbol function should have only 1 parameter");
    }
    if (Is<Symbol>(CalcExpression(input[1], scope))) {
        return GetGC().New<Bool>(true);
    } else {
        return GetGC().New<Bool>(false);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Lambda

Lambda::Lambda(Scope* scope, const std::vector<Object*>& params,
               const std::vector<Object*>& actions)
    : par_scope_{scope}, params_{params}, actions_{actions} {
}

Object* Lambda::Invoke(Object* ptr, Scope* scope) {
    auto input = Convert(ptr);
    if (input.size() != 1 + params_.size()) {
        throw RuntimeError("Lambda's parameter pack and call pack lengths differ");
    }
    auto input_params = CalcVector<Object>(input, scope);
    Scope* inner_scope(GetGC().New<Scope>(par_scope_));
    for (size_t i = 0; i < input_params.size(); ++i) {
        if (Is<Lambda>(input_params[i])) {
            inner_scope->Set(As<Symbol>(params_[i])->GetName(), input_params[i]);
        } else {
            inner_scope->Set(As<Symbol>(params_[i])->GetName(),
                             GetGC().New<Variable>(input_params[i]));
        }
    }
    Object* ans;
    for (size_t i = 0; i < actions_.size(); ++i) {
        ans = CalcExpression(actions_[i], inner_scope);
    }
    return ans;
}

LambdaGenerator::LambdaGenerator(Scope* scope) : scope_{scope} {
}

std::vector<Object*> GetParams(Object* ptr) {
    if (ptr && !Is<Cell>(ptr)) {
        throw RuntimeError("Incorrect lambda parameters");
    }
    std::vector<Object*> ans;
    while (Is<Cell>(ptr)) {
        ans.push_back(As<Cell>(ptr)->GetFirst());
        if (!Is<Symbol>(ans.back())) {
            throw RuntimeError("Lambda should has only symbols in parameters");
        }
        ptr = As<Cell>(ptr)->GetSecond();
    }
    if (ptr != nullptr) {
        ans.push_back(ptr);
        if (!Is<Symbol>(ans.back())) {
            throw RuntimeError("Lambda should has only symbols in parameters");
        }
    }
    return ans;
}

std::vector<Object*> GetActions(std::vector<Object*> obj) {
    obj.erase(obj.begin(), obj.begin() + 2);
    return obj;
}

Object* LambdaGenerator::Invoke(Object* ptr, Scope*) {
    auto input = Convert(ptr);
    if (input.size() < 3) {
        throw SyntaxError("Lambda should have 1 parameter pack and at least 1 action");
    }
    auto params = GetParams(input[1]);
    auto actions = GetActions(input);
    return GetGC().New<Lambda>(scope_, params, actions);
}
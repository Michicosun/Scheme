#include "scheme.h"
#include <string>
#include <vector>
#include "error.h"
#include "garbage_collector.h"
#include "object.h"
#include "parser.h"
#include "tokenizer.h"
#include "functions.h"
#include <cassert>

std::vector<Object*> Convert(Object* ptr) {
    std::vector<Object*> ans;
    while (Is<Cell>(ptr)) {
        ans.push_back(As<Cell>(ptr)->GetFirst());
        ptr = As<Cell>(ptr)->GetSecond();
    }
    if (ptr != nullptr) {
        ans.push_back(ptr);
    }
    return ans;
}

Object* CalcExpression(Object* object, Scope* scope) {
    if (object == nullptr) {
        throw RuntimeError("Lists Are Not Self Evaluating");
    }
    if (Is<Cell>(object)) {
        auto F = CalcExpression(As<Cell>(object)->GetFirst(), scope);
        if (!Is<Function>(F)) {
            throw RuntimeError("You can call only functions");
        }
        return As<Function>(F)->Invoke(object, scope);
    } else {
        auto F = GetVariable(object, scope);
        return F->Invoke(object, scope);
    }
}

std::string GetString(Number* ptr) {
    return std::to_string(ptr->GetValue());
}

std::string GetString(Symbol* ptr) {
    return ptr->GetName();
}

std::string GetString(Bool* ptr) {
    if (ptr->GetValue()) {
        return "#t";
    } else {
        return "#f";
    }
}

bool CheckList(Object* ptr) {
    return Is<Cell>(ptr) &&
           (As<Cell>(ptr)->GetSecond() == nullptr || Is<Cell>(As<Cell>(ptr)->GetSecond()));
}

bool CheckPair(Object* ptr) {
    return Is<Cell>(ptr) &&
           (As<Cell>(ptr)->GetSecond() != nullptr && !Is<Cell>(As<Cell>(ptr)->GetSecond()));
}

std::string GetString(Object* ptr) {
    std::string ans;
    if (Is<Cell>(ptr)) {
        if (As<Cell>(ptr)->GetFirst() == ptr && As<Cell>(ptr)->GetSecond() == ptr) {
            ans = "#0 = (#0# . #0#)";
        } else if (As<Cell>(ptr)->GetSecond() == ptr) {
            ans = "#0 = (" + GetString(As<Cell>(ptr)->GetFirst()) + " . " + "#0#)";
        } else {
            if (As<Cell>(ptr)->GetFirst() == ptr) {
                ans = "#0 = ";
            }
            ans += "(";
            while (Is<Cell>(ptr)) {
                if (As<Cell>(ptr)->GetFirst() == ptr) {
                    ans += "#0# ";
                } else {
                    ans += GetString(As<Cell>(ptr)->GetFirst()) + " ";
                }
                ptr = As<Cell>(ptr)->GetSecond();
            }
            if (ptr != nullptr) {
                ans += ". ";
                ans += GetString(ptr) + ")";
            } else {
                ans.back() = ')';
            }
        }
    } else if (Is<Number>(ptr)) {
        ans = GetString(As<Number>(ptr));
    } else if (Is<Symbol>(ptr)) {
        ans = GetString(As<Symbol>(ptr));
    } else if (Is<Bool>(ptr)) {
        ans = GetString(As<Bool>(ptr));
    } else if (Is<Lambda>(ptr)) {
        ans = "Lambda function";
    } else if (Is<Function>(ptr)) {
        ans = "built-in function";
    } else if (!ptr) {
        ans = "()";
    }
    return ans;
}

std::string Interpreter::Run(const std::string& expr) {
    auto check_list = Read(expr);
    if (check_list.size() != 1) {
        throw RuntimeError("bad expression");
    }
    auto ans = CalcExpression(check_list[0], GetGlobalScope());
    std::string s_ans = GetString(ans);
    GetGC().CleanUp();
    if (global_scope_ != GetGC().memory_[0].get()) {
        assert(false);
    }
    return s_ans;
}

Scope* Interpreter::GetGlobalScope() {
    return global_scope_;
}

Interpreter::Interpreter() {
    GetGC().ClearAll();
    global_scope_ = GetGC().New<Scope>();
    // Quotes
    global_scope_->Set("'", GetGC().New<QuoteFunction>());
    global_scope_->Set("quote", GetGC().New<QuoteFunction>());
    // Numbers
    global_scope_->Set("number?", GetGC().New<IsNumberFunction>());
    global_scope_->Set("+", GetGC().New<SumFunction>());
    global_scope_->Set("-", GetGC().New<SubtractFunction>());
    global_scope_->Set("=", GetGC().New<EqualFunction>());
    global_scope_->Set(">", GetGC().New<GreaterFunction>());
    global_scope_->Set(">=", GetGC().New<GreaterOrEqualFunction>());
    global_scope_->Set("<", GetGC().New<LessFunction>());
    global_scope_->Set("<=", GetGC().New<LessOrEqualFunction>());
    global_scope_->Set("*", GetGC().New<MultFunction>());
    global_scope_->Set("/", GetGC().New<DivFunction>());
    global_scope_->Set("max", GetGC().New<MaxFunction>());
    global_scope_->Set("min", GetGC().New<MinFunction>());
    global_scope_->Set("abs", GetGC().New<AbsFunction>());
    // Bools
    global_scope_->Set("#t", GetGC().New<Variable>(GetGC().New<Bool>(true)));
    global_scope_->Set("#f", GetGC().New<Variable>(GetGC().New<Bool>(false)));
    global_scope_->Set("boolean?", GetGC().New<IsBoolFunction>());
    global_scope_->Set("not", GetGC().New<NotFunction>());
    global_scope_->Set("and", GetGC().New<AndFunction>());
    global_scope_->Set("or", GetGC().New<OrFunction>());
    // Lists
    global_scope_->Set("pair?", GetGC().New<IsPairFunction>());
    global_scope_->Set("null?", GetGC().New<IsNullFunction>());
    global_scope_->Set("list?", GetGC().New<IsListFunction>());
    global_scope_->Set("cons", GetGC().New<ConsFunction>());
    global_scope_->Set("car", GetGC().New<CarFunction>());
    global_scope_->Set("cdr", GetGC().New<CdrFunction>());
    global_scope_->Set("list", GetGC().New<ListFunction>());
    global_scope_->Set("list-ref", GetGC().New<ListRefFunction>());
    global_scope_->Set("list-tail", GetGC().New<ListTailFunction>());
    // If
    global_scope_->Set("if", GetGC().New<IfFunction>());
    // Define
    global_scope_->Set("symbol?", GetGC().New<IsSymbolFunction>());
    global_scope_->Set("define", GetGC().New<DefineFunction>());
    global_scope_->Set("set!", GetGC().New<SetFunction>());
    global_scope_->Set("set-car!", GetGC().New<SetCarFunction>());
    global_scope_->Set("set-cdr!", GetGC().New<SetCdrFunction>());
}

Interpreter::~Interpreter() {
    GetGC().ClearAll();
}
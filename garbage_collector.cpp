#include "garbage_collector.h"
#include "error.h"
#include "functions.h"
#include "object.h"
#include <unordered_set>

void Dfs(Object* v, std::unordered_set<Object*>& used) {
    used.insert(v);
    if (!v) {
        return;
    }
    if (Is<Cell>(v)) {
        auto ptr = As<Cell>(v);
        if (!used.count(ptr->GetFirst())) {
            Dfs(ptr->GetFirst(), used);
        }
        if (!used.count(ptr->GetSecond())) {
            Dfs(ptr->GetSecond(), used);
        }
    } else if (Is<Variable>(v)) {
        auto ptr = As<Variable>(v);
        if (!used.count(ptr->var_)) {
            Dfs(ptr->var_, used);
        }
    }  else if (Is<Variable>(v)) {
        auto ptr = As<Variable>(v);
        if (!used.count(ptr->var_)) {
            Dfs(ptr->var_, used);
        }
    } else if (Is<LambdaGenerator>(v)) {
        auto ptr = As<LambdaGenerator>(v);
        if (!used.count(ptr->scope_)) {
            Dfs(ptr->scope_, used);
        }
    } else if (Is<Lambda>(v)) {
        auto ptr = As<Lambda>(v);
        if (!used.count(ptr->par_scope_)) {
            Dfs(ptr->par_scope_, used);
        }
        for (auto el : ptr->params_) {
            if (!used.count(el)) {
                Dfs(el, used);
            }
        }
        for (auto el : ptr->actions_) {
            if (!used.count(el)) {
                Dfs(el, used);
            }
        }
    } else if (Is<Scope>(v)) {
        auto ptr = As<Scope>(v);
        if (!used.count(ptr->par_scope_)) {
            Dfs(ptr->par_scope_, used);
        }
        for (const auto& el : ptr->mp_) {
            if (!used.count(el.second)) {
                Dfs(el.second, used);
            }
        }
    } else if (Is<Number>(v)) {
    } else if (Is<Bool>(v)) {
    } else if (Is<Symbol>(v)) {
    }
}

void GarbageCollector::CleanUp() {
    std::unordered_set<Object*> used;
    Dfs(memory_.front().get(), used);
    for (size_t i = 0; i < memory_.size(); ++i) {
        while (memory_.size() > i && !used.count(memory_[i].get())) {
            std::swap(memory_[i], memory_.back());
            memory_.pop_back();
        }
    }
}

void GarbageCollector::ClearAll() {
    memory_.clear();
}

GarbageCollector& GetGC() {
    static GarbageCollector gc;
    return gc;
}
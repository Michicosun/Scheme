#include "scope.h"
#include "error.h"
#include "functions.h"
#include "object.h"
#include <string>

Scope::Scope(Scope* sc) : par_scope_{sc} {
}

Object* Scope::Get(const std::string& name) {
    if (mp_.find(name) == mp_.end()) {
        return nullptr;
    }
    return mp_[name];
}

void Scope::Set(const std::string& name, Object* obj) {
    mp_[name] = obj;
}

bool Scope::TrySet(const std::string& name, Object* obj) {
    if (mp_.find(name) == mp_.end()) {
        return false;
    }
    mp_[name] = obj;
    return true;
}

bool Scope::TrySetCar(const std::string& name, Object* obj) {
    if (mp_.find(name) == mp_.end()) {
        return false;
    }
    auto ptr = mp_[name];
    if (!Is<Variable>(ptr) || !Is<Cell>(As<Variable>(ptr)->GetVal())) {
        throw RuntimeError("Set-car must change a list or pair");
    }
    As<Cell>(As<Variable>(ptr)->GetVal())->GetFirst() = obj;
    return true;
}

bool Scope::TrySetCdr(const std::string& name, Object* obj) {
    if (mp_.find(name) == mp_.end()) {
        return false;
    }
    auto ptr = mp_[name];
    if (!Is<Variable>(ptr) || !Is<Cell>(As<Variable>(ptr)->GetVal())) {
        throw RuntimeError("Set-cdr must change a list or pair");
    }
    As<Cell>(As<Variable>(ptr)->GetVal())->GetSecond() = obj;
    return true;
}
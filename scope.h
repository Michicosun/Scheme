#pragma once
#include <string>
#include <unordered_map>
#include <string>
#include "object.h"

class Scope : public Object {
public:
    Scope() = default;

    Scope(Scope* sc);

    Object* Get(const std::string& name);

    void Set(const std::string& name, Object* obj);

    bool TrySet(const std::string& name, Object* obj);

    bool TrySetCar(const std::string& name, Object* obj);

    bool TrySetCdr(const std::string& name, Object* obj);

    Scope* par_scope_;
    std::unordered_map<std::string, Object*> mp_;
};
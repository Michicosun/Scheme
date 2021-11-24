#pragma once

#include "object.h"

#include <utility>
#include <vector>
#include <memory>

class GarbageCollector {
public:
    GarbageCollector() = default;

    template <class T, class... Args>
    T* New(Args&&... args) {
        memory_.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
        return As<T>(memory_.back().get());
    }

    void CleanUp();

    void ClearAll();

    std::vector<std::unique_ptr<Object>> memory_;
};

GarbageCollector& GetGC();
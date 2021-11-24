#include "../scheme.h"
#include "../test/scheme_test.h"
#include "../test/fuzzer.h"
#include "allocations_checker.h"

#include <iostream>

constexpr uint32_t kShotsCount = 100000;

TEST_CASE("Fuzzing") {
    Fuzzer fuzzer;
    Interpreter interpreter;

    alloc_checker::ResetCounters();

    for (uint32_t i = 0; i < kShotsCount; ++i) {
        try {
            auto req = fuzzer.Next();
            // std::cerr << req << std::endl;  // uncomment to debug, random is deterministic
            interpreter.Run(req);
        } catch (const SyntaxError&) {
        } catch (const RuntimeError&) {
        } catch (const NameError&) {
        }
    }

    int64_t alloc_count = alloc_checker::AllocCount(),
            dealloc_count = alloc_checker::DeallocCount();

    std::cerr << "Fuzzer:\n";
    std::cerr << "Allocations: " << alloc_count << "\n";
    std::cerr << "Deallocations: " << dealloc_count << "\n\n";

    // If falling here:
    // - if it happens on advanced task, check that you invoke GC after each command
    // - if it happens on basic task, contact us
    REQUIRE(alloc_count - dealloc_count <= 10'000);
}

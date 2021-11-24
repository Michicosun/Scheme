#include <catch.hpp>

#include "../test/fuzzer.h"
#include "../tokenizer.h"
#include "../parser.h"
#include "../error.h"

#include <iostream>

constexpr uint32_t kShotsCount = 100000;

TEST_CASE("Fuzzzzzzing") {
    Fuzzer fuzzer;

    for (uint32_t i = 0; i < kShotsCount; ++i) {
        try {
            auto req = fuzzer.Next();
            // std::cerr << req << std::endl;  // uncomment to debug, random is deterministic
            std::stringstream ss{req};
            Tokenizer tokenizer{&ss};
            while (!tokenizer.IsEnd()) {
                Read(&tokenizer);
            }
        } catch (const SyntaxError&) {
        }
    }
}

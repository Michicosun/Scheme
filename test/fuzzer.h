#pragma once

#include <sstream>
#include <random>
#include <array>

constexpr uint32_t kSeed = 16;
constexpr uint32_t kSeqCount = 21;

class Fuzzer {
public:
    Fuzzer() : gen_(kSeed) {
    }

    std::string Next() {
        std::uniform_int_distribution<uint32_t> dist(1, kSeqCount);
        ss_.str("");
        std::shuffle(sequences_.begin(), sequences_.end(), gen_);
        uint32_t len = dist(gen_);

        for (uint32_t j = 0; j < len; ++j) {
            ss_ << sequences_[j] << " ";
        }

        return ss_.str();
    }

private:
    std::array<std::string, kSeqCount> sequences_ = {"(", "(",  "(", "(",  ")", ")",      ")",
                                                     ")", "-",  "+", "#t", "",  "symbol", "0",
                                                     "1", "-2", ".", ".",  ".", ".",      "'"};

    std::stringstream ss_;
    std::mt19937 gen_;
};

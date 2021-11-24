#pragma once

#include <stdexcept>
#include <string>

struct SyntaxError : public std::runtime_error {
    using std::runtime_error::runtime_error;
    SyntaxError(const std::string& s) : std::runtime_error(s) {
    }
};

struct RuntimeError : public std::runtime_error {
    using std::runtime_error::runtime_error;
    RuntimeError(const std::string& s) : std::runtime_error(s) {
    }
};

struct NameError : public std::runtime_error {
    using std::runtime_error::runtime_error;
    NameError(const std::string& s) : std::runtime_error(s) {
    }
};

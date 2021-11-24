#pragma once

#include <variant>
#include <string>
#include <iostream>

struct SymbolToken {
    std::string name;

    bool operator==(const SymbolToken& other) const {
        return name == other.name;
    }
};

struct QuoteToken {
    bool operator==(const QuoteToken&) const {
        return true;
    }
};

struct DotToken {
    bool operator==(const DotToken&) const {
        return true;
    }
};

enum class BracketToken { OPEN, CLOSE };

struct ConstantToken {
    int value;

    bool operator==(const ConstantToken& other) const {
        return value == other.value;
    }
};

using Token = std::variant<ConstantToken, BracketToken, SymbolToken, QuoteToken, DotToken>;

bool CheckCloseBracketToken(const Token& a);

bool CheckOpenBracketToken(const Token& a);

bool CheckDotToken(const Token& a);

class Tokenizer {
public:
    Tokenizer(std::istream* in);

    Token GetToken();

    bool IsEnd();

    void Next();

private:
    bool ready_{false};
    Token parsed_token_;
    std::istream* in_;

    bool IsStart(char c);

    bool IsInner(char c);

    bool IsDigit(char c);

    bool IsPM(char c);

    bool GoodSymbol(char c);

    void SkipWhitespaces();
};
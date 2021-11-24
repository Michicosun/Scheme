#include "tokenizer.h"
#include <string>
#include <iostream>
#include "error.h"

bool CheckCloseBracketToken(const Token& a) {
    const BracketToken* x = std::get_if<BracketToken>(&a);
    return x != nullptr && *x == BracketToken::CLOSE;
}

bool CheckOpenBracketToken(const Token& a) {
    const BracketToken* x = std::get_if<BracketToken>(&a);
    return x != nullptr && *x == BracketToken::OPEN;
}

bool CheckDotToken(const Token& a) {
    const DotToken* x = std::get_if<DotToken>(&a);
    return x != nullptr;
}

Tokenizer::Tokenizer(std::istream* in) : in_{in} {
    SkipWhitespaces();
}

Token Tokenizer::GetToken() {
    if (!ready_) {
        Next();
    }
    return parsed_token_;
}

bool Tokenizer::IsEnd() {
    return in_->eof() && (!ready_);
}

void Tokenizer::Next() {
    ready_ = false;
    SkipWhitespaces();
    if (in_->eof()) {
        return;
    }
    ready_ = true;
    if (in_->peek() == '(') {
        parsed_token_ = BracketToken::OPEN;
        in_->get();
    } else if (in_->peek() == ')') {
        parsed_token_ = BracketToken::CLOSE;
        in_->get();
    } else if (in_->peek() == '.') {
        parsed_token_ = DotToken{};
        in_->get();
    } else if (in_->peek() == '\'') {
        parsed_token_ = QuoteToken{};
        in_->get();
    } else if (IsPM(in_->peek())) {
        std::string s;
        s.push_back(in_->get());
        if (IsDigit(in_->peek())) {
            while (!in_->eof() && IsDigit(in_->peek())) {
                s.push_back(in_->get());
            }
            parsed_token_ = ConstantToken{std::stoi(s)};
        } else {
            parsed_token_ = SymbolToken{s};
        }
    } else if (IsDigit(in_->peek())) {
        std::string s;
        s.push_back(in_->get());
        while (!in_->eof() && IsDigit(in_->peek())) {
            s.push_back(in_->get());
        }
        parsed_token_ = ConstantToken{std::stoi(s)};
    } else if (IsStart(in_->peek())) {
        std::string s;
        s.push_back(in_->get());
        while (!in_->eof() && IsInner(in_->peek())) {
            s.push_back(in_->get());
        }
        parsed_token_ = SymbolToken{s};
    }
    if (!in_->eof() && !GoodSymbol(in_->peek())) {
        throw SyntaxError("bad symbol");
    }
}

bool Tokenizer::IsStart(char c) {
    // a-zA-Z<=>*#
    return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || c == '<' || c == '=' || c == '>' ||
           c == '*' || c == '#' || c == '/';
}

bool Tokenizer::IsInner(char c) {
    // a-zA-Z<=>*#
    // a-zA-Z<=>*#0-9?!-
    return IsStart(c) || ('0' <= c && c <= '9') || c == '?' || c == '!' || c == '-';
}

bool Tokenizer::IsDigit(char c) {
    return '0' <= c && c <= '9';
}

bool Tokenizer::IsPM(char c) {
    return c == '+' || c == '-';
}

bool Tokenizer::GoodSymbol(char c) {
    return c == '(' || c == ')' || c == '.' || c == '\'' || IsPM(c) || IsDigit(c) || IsStart(c) ||
           IsInner(c) || std::isspace(c) || c == -1;
}

void Tokenizer::SkipWhitespaces() {
    while (!in_->eof() && std::isspace(in_->peek())) {
        if (!in_->eof() && !GoodSymbol(in_->peek())) {
            throw SyntaxError("bad symbol");
        }
        in_->get();
    }
}

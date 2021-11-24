#include <memory>
#include <sstream>
#include <variant>
#include <vector>

#include "error.h"
#include "object.h"
#include "parser.h"
#include "tokenizer.h"
#include "garbage_collector.h"

Object* ReadOneToken(Tokenizer* tokenizer) {  // expr ...
    if (tokenizer->IsEnd()) {
        throw SyntaxError("eof in the beginning of read");
    }
    Token expr = tokenizer->GetToken();
    if (CheckOpenBracketToken(expr)) {
        return ReadList(tokenizer);
    } else if (CheckCloseBracketToken(expr)) {
        throw SyntaxError("close bracket in read");
    } else if (ConstantToken* x = std::get_if<ConstantToken>(&expr)) {
        tokenizer->Next();
        return GetGC().New<Number>(x->value);
    } else if (SymbolToken* x = std::get_if<SymbolToken>(&expr)) {
        tokenizer->Next();
        return GetGC().New<Symbol>(x->name);
    } else if (std::get_if<QuoteToken>(&expr)) {
        return ReadQuote(tokenizer);
    } else {
        throw SyntaxError("unknown type in read");
    }
}

Object* Read(Tokenizer* tokenizer) {
    auto ans = ReadOneToken(tokenizer);
    if (!tokenizer->IsEnd()) {
        throw SyntaxError("Read must read 1 token");
    }
    return ans;
}

Object* ReadQuote(Tokenizer* tokenizer) {  // '
    Cell* ans(GetGC().New<Cell>());
    ans->GetFirst() = GetGC().New<Symbol>("quote");
    tokenizer->Next();
    if (!tokenizer->IsEnd()) {
        ans->GetSecond() = GetGC().New<Cell>();
        As<Cell>(ans->GetSecond())->GetFirst() = ReadOneToken(tokenizer);
    } else {
        throw SyntaxError("After quote must be a token");
    }
    return ans;
}

Object* ReadList(Tokenizer* tokenizer) {  // ( expr . ... )
    tokenizer->Next();
    Token expr = tokenizer->GetToken();
    if (CheckCloseBracketToken(expr)) {  // () -> nullptr
        tokenizer->Next();
        return nullptr;
    }
    Cell* ans(GetGC().New<Cell>());
    Cell* last = ans;
    while (!tokenizer->IsEnd()) {
        last->GetFirst() = ReadOneToken(tokenizer);
        if (tokenizer->IsEnd()) {
            throw SyntaxError("no close bracket in list");
        }
        expr = tokenizer->GetToken();  // ( expr expr2 ... )
        if (CheckDotToken(expr)) {     // ( expr . expr2 )
            tokenizer->Next();
            last->GetSecond() = ReadOneToken(tokenizer);
            if (tokenizer->IsEnd()) {
                throw SyntaxError("no close bracket in list");
            }
            expr = tokenizer->GetToken();
            if (!CheckCloseBracketToken(expr)) {
                throw SyntaxError("double expression after dot in list");
            }
            tokenizer->Next();
            return ans;
        }
        if (CheckCloseBracketToken(expr)) {
            tokenizer->Next();
            return ans;
        }
        Cell* next_cell(GetGC().New<Cell>());
        last->GetSecond() = next_cell;
        last = next_cell;
    }
    throw SyntaxError("no close bracket in list");
}

std::vector<Object*> Read(const std::string& s) {
    std::stringstream ss(s);
    Tokenizer t(&ss);
    std::vector<Object*> ans;
    while (!t.IsEnd()) {
        ans.push_back(Read(&t));
    }
    return ans;
}
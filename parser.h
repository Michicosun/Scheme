#pragma once

#include <vector>

#include "object.h"
#include "tokenizer.h"
#include "error.h"

Object* ReadQuote(Tokenizer* tokenizer);

Object* ReadList(Tokenizer* tokenizer);

Object* Read(Tokenizer* tokenizer);

std::vector<Object*> Read(const std::string& string);
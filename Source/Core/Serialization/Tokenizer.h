// Tokenizer.h

#pragma once

#include <string>
#include <vector>

enum class ETokenType
{
	Identifier,
	Number,
	String,
	Symbol,
	EndOfFile
};

struct Token
{
  static std::vector<Token> Tokenize(const std::string& file);
	ETokenType type;
	std::string text;
	int line;
	int column;
};


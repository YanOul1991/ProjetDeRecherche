// Parser.h

#pragma once

#include "Core/Reflection/OptimReflection.h"
#include "Core/Serialization/Tokenizer.h"

#include <unordered_map>

class Parser
{
 public:
  static void* CreateObject(Parser& parser);

  Parser(const std::vector<Token>& param_tokens);

  const Token& peek(int i = 0) const;

  const Token& consume();

  bool isEnd() const;

  const std::vector<Token> tokens;
  size_t index;
};
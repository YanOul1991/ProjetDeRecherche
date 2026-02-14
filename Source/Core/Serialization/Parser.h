// Parser.h

#pragma once

#include "Core/Reflection/OptimReflection.h"
#include "Core/Serialization/Tokenizer.h"

#include <unordered_map>

class Parser
{
 public:
  static std::unordered_map<void*, TypeInfo*> instanciateObjects(std::vector<Token>& tokens);

  static void* CreateObject(Parser& parser);

  // static void ParseMeshObject(Parser& p, Mesh& mesh);

  Parser(const std::vector<Token>& param_tokens);

  const std::vector<Token> tokens;

  size_t       index;
  const Token& peek(int i = 0) const;

  const Token& consume();

  bool isEnd() const;

};
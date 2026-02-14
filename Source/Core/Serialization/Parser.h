// Parser.h

#pragma once

#include "Core/Serialization/Tokenizer.h"
#include "Core/Reflection/OptimReflection.h"
#include "Core/graphics/Mesh.h"

class Parser
{
 public:
  static std::unordered_map<void*, TypeInfo*> instanciateObjects(std::vector<Token>& tokens);

  static void ParseMeshObject(Parser& p, Mesh& mesh);

  std::vector<Token> tokens;
  size_t index = 0;

  const Token& peek() const {
    return tokens[index];
  }

  const Token& advance() {
    return tokens[index++];
  }

  bool isEnd() const {
    return index >= tokens.size();
  }
};
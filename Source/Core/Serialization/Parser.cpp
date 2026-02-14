#include "Core/Serialization/Parser.h"
#include "Core/Reflection/Internal/TypeReflection.h"

static bool matchSymbol(Parser& p, const char* s) {
  if (p.peek().type == ETokenType::Symbol && p.peek().text == s) {
    p.advance();
    return true;
  }
  return false;
}

static void expectTokenSymbol(Parser& p, const char* s) { 
  if (!matchSymbol(p, s)) { 
    std::cout << "Symbol not matching.\n";
    std::cout << "Value   : " << p.peek().text << "\n";
    std::cout << "Expected: " << s << '\n';
    throw std::runtime_error(std::string("Expected symbol: ") + s); 
  } 
}

static Token expectTokenIdentifier(Parser& p) {
  if (p.peek().type != ETokenType::Identifier) {
    std::cout << "Identifier not found:\n";
    throw std::runtime_error("Expected identifier");
  }
  std::cout << "Identifier found for: " << p.peek().text << '\n';
  return p.advance();
}

static Token expectNumber(Parser& p) { 
  if (p.peek().type != ETokenType::Number) {
    throw std::runtime_error("Expected number");
  }
  return p.advance();
}

std::unordered_map<void*, TypeInfo*> Parser::instanciateObjects(std::vector<Token>& tokens) {
  int cursor = 0;

  for (Token& token : tokens) {

    if (GetTypeRegistry().contains(token.text.c_str())) {
      TypeInfo* typeInfo = GetTypeRegistry()[token.text.c_str()];

      if (typeInfo->typeData == TypeData::Object) {
        //std::cout << "Identifier: " << token.text << " is a type. " << typeInfo->name << '\n';
      }
    }
  }

  return std::unordered_map<void*, TypeInfo*>();
}

//static void ParseMeshField(Parser& p, Mesh& mesh) {
//  std::cout << "Starting to parse mesh fields...\n";
//  // When starting to parse fields of a Mesh 
//  // object the next token should be of type identifier
//  Token fieldName = expectIdentifier(p);
//
//  std::cout << "parsing Mesh field field: " << fieldName.text << '\n';
//}

void Parser::ParseMeshObject(Parser& p, Mesh& mesh) {
  return;
  std::cout << "Next expected token: " << p.peek().text << "| Cursor index: " << p.index << '\n';
  Token name = expectTokenIdentifier(p);

  std::cout << "Next expected token: " << p.peek().text << "| Cursor index: " << p.index << '\n';
  expectTokenSymbol(p, "=");

  // Next token should be start of Mesh object
  expectTokenSymbol(p, "(");


  //ParseMeshField(p, mesh);
  //while (!matchSymbol(p, ")")) {
  //}
}

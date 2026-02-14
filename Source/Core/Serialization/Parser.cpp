#include "Core/Serialization/Parser.h"

#include "Core/Reflection/OptimReflection.h"

/*
static bool matchSymbol(Parser& p, const char* s) {
  if (p.peek().type == ETokenType::Symbol && p.peek().text == s) {
    p.consume();
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
  return p.consume();
}

static Token expectNumber(Parser& p) {
  if (p.peek().type != ETokenType::Number) {
    throw std::runtime_error("Expected number");
  }
  return p.consume();
}
*/
/*
 static void ParseMeshField(Parser& p, Mesh& mesh) {
   std::cout << "Starting to parse mesh fields...\n";
   // When starting to parse fields of a Mesh
   // object the next token should be of type identifier
   Token fieldName = expectIdentifier(p);

   std::cout << "parsing Mesh field field: " << fieldName.text << '\n';
 }
void Parser::ParseMeshObject(Parser& p, Mesh& mesh) {
  return;
  std::cout << "Next expected token: " << p.peek().text << "| Cursor index: " << p.index << '\n';
  Token name = expectTokenIdentifier(p);

  std::cout << "Next expected token: " << p.peek().text << "| Cursor index: " << p.index << '\n';
  expectTokenSymbol(p, "=");

  // Next token should be start of Mesh object
  expectTokenSymbol(p, "(");

  // ParseMeshField(p, mesh);
  // while (!matchSymbol(p, ")")) {
  // }
}
*/

std::unordered_map<void*, TypeInfo*> Parser::instanciateObjects(std::vector<Token>& tokens) {
  int cursor = 0;

  for (Token& token : tokens) {

    if (GetTypeRegistry().contains(token.text.c_str())) {
      TypeInfo* typeInfo = GetTypeRegistry()[token.text.c_str()];

      if (typeInfo->typeData == TypeData::Object) {
        // std::cout << "Identifier: " << token.text << " is a type. " << typeInfo->name << '\n';
      }
    }
  }
  return std::unordered_map<void*, TypeInfo*>();
}

/**
 * Helper functions
 */

static bool expectIdentifier(Parser& parser) {
  return parser.peek().type == ETokenType::Identifier;
}

static bool expectSymbol(Parser& parser, const char* symbol) {
  return parser.peek().text == symbol;
}

static const FieldInfo* findFieldInfo(const std::string& fieldName, const TypeInfo* type) {
  for (auto& field : type->fields) {
    if (fieldName == field.name) {
      return &field;
    }
  }

  return nullptr;
}

/**
 * @brief
 * Parses the fields of an instanciated object.
 * The expectations of the parsing logic depend on the TypeInfo of the object.
 * The instance of the object to parse must always a type inheriting
 * from the Object class hierarchy, as only those can be instanciated into
 * a scene.
 *
 * @param parser
 * Parser object with validated object instance with the index
 * pointing to a token of type indentifier.
 *
 * @param instance
 * A pointer to the instanciated object;
 *
 * @param typeInfo
 * Pointer to the TypeInfo struct of the object.
 *
 */
static void parseFields(Parser& parser, void* instance, const TypeInfo* typeInfo) {
  /*
  // First thing to consume
  while (!expectSymbol(parser, ")")) {

    if (!expectIdentifier(parser)) {
      printf("The next token [%s] IS NOT an identifier for field.\n", parser.peek().text.c_str());
    }

    // Consume Field Identifier and = symbol
    std::string fieldName = parser.consume().text;
    parser.consume();
    const FieldInfo* fieldInfo = findFieldInfo(fieldName, typeInfo);

    printf("Next field name [%s] : %s\n", fieldName.c_str(), fieldInfo->typeInfo->name);

    if (!fieldInfo) {
      printf("COULD NOT FIND the field info for [%s]\n", fieldName.c_str());
      return;
    }

    // If the type is a structure:
    // Field then parse its fields as well
    if (Optim::Internal::Reflection::TypeHasFields(fieldInfo->typeInfo)) {
      parser.consume(); // Consume Symbol (

      void* pField = (char*)instance + fieldInfo->offset;
      parseFields(parser, pField, fieldInfo->typeInfo);

      // consume (
      // At this point the next token should be the Identifier for the next field.
      std::cout << "[CHECKPOINT] " << parser.peek().text << '\n';
    }
    // If the Data type is a primitive then write the value
    if (fieldInfo->typeInfo->typeData == TypeData::Primitive) {
      std::string strVal = parser.consume().text;

      std::cout << "reading primitive: " << strVal << '\n';

      typeInfo->fromString(instance, strVal);

      std::cout << "Primitive saved: " << strVal << '\n';
    }
    if (expectSymbol(parser, ",")) {
      parser.consume();
    }
  }
  */
}

// Member functions
void* Parser::CreateObject(Parser& parser) {
  if (!expectIdentifier(parser)) {
    printf("The next token %s is NOT an identifier\n", parser.peek().text.c_str());
    return nullptr;
  }

  // Get if the indetifier is a registered type and if its
  // of a type Object.

  if (!GetTypeRegistry().contains(parser.peek().text.c_str())) {
    printf("The iditifier [%s] is not a valid type.\n", parser.peek().text.c_str());
    return nullptr;
  }
  else {
    printf("The iditifier [%s] IS a valid type.\n", parser.peek().text.c_str());
  }

  const TypeInfo* pInfo = GetTypeRegistry()[parser.peek().text.c_str()];

  if (pInfo->typeData == TypeData::Object) {
    printf("The iditifier [%s] IS an Object type.\n", parser.peek().text.c_str());
  }
  else {
    printf("The iditifier [%s] IS an Object type.\n", parser.peek().text.c_str());
    return nullptr;
  }

  // Consume identifie token.
  parser.consume();

  // If all is checked then create a neew instance of the object
  void* instance = pInfo->createFn();

  if (!expectSymbol(parser, "=")) {
    printf("[Parser] Error: Could not find symbol \"=\"\n");
  }
  else {
    // printf("[Parser] Found symbol \"=\"\n");
    parser.consume();
  }

  if (!expectSymbol(parser, "(")) {
    printf("[Parser] Error: Could not find symbol \"(\"\n");
  }
  else {
    // printf("[Parser] Found symbol \"(\"\n");
    parser.consume();
  }

  parseFields(parser, instance, pInfo);

  // After finding both symbol to that
  // determine start of object fields
  // Start parsing through fields.

  return nullptr;
}

Parser::Parser(const std::vector<Token>& param_tokens) :
    tokens(param_tokens),
    index(0) {
}

const Token& Parser::peek(int i) const {
  return tokens[index + i];
}

const Token& Parser::consume() {
  std::cout << "Consuming token: " << peek().text << '\n';
  return tokens[index++];
}

bool Parser::isEnd() const {
  return index >= tokens.size();
}

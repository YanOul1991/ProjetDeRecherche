#include "Core/Serialization/Parser.h"
#include "Core/Reflection/OptimReflection.h"
#include "Core/Math/OptimMathematics.h"

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
static void parseFields(Parser& parser, void* pInstance, const TypeInfo* param_typeInfo) {
  std::cout << "[Call]\n" << __FUNCSIG__ << '\n';

  // Iterrate through object until end
  while (!expectSymbol(parser, ")")) {
    std::cout << "Loop start......\n";

    //std::cout << "Next identifier: " << parser.peek().text << '\n';
    if (!expectIdentifier(parser)) {
      throw std::exception("Expected identifier token.\n");
    }
    // Get the identifier and validate if the identifier
    // is a field name of the object.
    std::string identifierName = parser.consume().text;

    const FieldInfo* fieldInfo = findFieldInfo(identifierName, param_typeInfo);

    // Except an = symbol after identifier declaration
    if (!expectSymbol(parser, "=")) {
      throw std::exception("Expected symbol token: =.\n");
    }
    parser.consume();

    if (fieldInfo != nullptr) {
      //std::cout << "An object of type <" << typeInfo->name << "> contains a field of <" << fieldInfo->name << "> of type <" << fieldInfo->typeInfo->name << ">\n";
    }
    else {
      std::cout << "An object of type: <" << param_typeInfo->name << "> DOES NOT HAVE A FIELD <" << identifierName << ">\n";
      throw std::exception("[Parser Exception] Could not find field in object.\n");
    }


    if (fieldInfo->typeInfo->typeData == TypeData::Structure || fieldInfo->typeInfo->typeData == TypeData::Object) {
      std::cout << "The field <" << identifierName << "> is of type <Strcuture/Object>\n";
      //std::cout << "Address of next field: " << std::hex << pField << std::dec << '\n';

      if (!expectSymbol(parser, "(")) {
        throw std::exception("Expected symbol token: =.\n");
      }
      parser.consume();
      std::cout << "Peeking parser: " << parser.peek().text << "\n";

      void* pField = (uint8*)pInstance + fieldInfo->offset;

      parseFields(parser, pField, fieldInfo->typeInfo);

      if (!expectSymbol(parser, ")")) {
        throw std::exception("Expected symbol token: =.\n");
      }

      parser.consume();
    }
    else if(fieldInfo->typeInfo->typeData == TypeData::Primitive) {
      std::cout << "The field <" << identifierName << "> is of type <Primitive>\n";
      std::cout << "Peeking parser: " << parser.peek().text << "\n";

      void* pField = (uint8*)pInstance + fieldInfo->offset;

      std::cout << "Address of next field: " << std::hex << pField << std::dec << '\n';
      std::cout << "Address of function:   " << std::hex << fieldInfo->typeInfo->fromString << std::dec << '\n';

      fieldInfo->typeInfo->fromString(pField, parser.consume().text);

    }
    else {
      throw std::exception("[Parser Exception] - Unkownd or unsupported type data.\n");
    }

    if (expectSymbol(parser, ",")) {
      parser.consume();
    }
    std::cout << "Peeking parser: " << parser.peek().text << "\n";
  }
}

// Member functions
void* Parser::CreateObject(Parser& parser) {
  try {
    std::cout << "Parsing Object...\n";

    // If already parsed to the then return nothing
    if (parser.isEnd()) {
      std::cout << "Already end of file.";
      return nullptr;
    }

    if (expectSymbol(parser, ",")) {
      parser.consume();
    }

    if (!expectIdentifier(parser)) {
      throw std::exception("The next token is NOT an identifier");
      //printf("The next token %s is NOT an identifier\n", parser.peek().text.c_str());
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

    // After finding both symbol to that
    // determine start of object fields
    // Start parsing through fields.
    parseFields(parser, instance, pInfo);

    if (!expectSymbol(parser, ")")) {
      throw std::exception("Expected symbol token: =.\n");
    }

    parser.consume();

    std::cout << "All fields have been parsed\n";

    //if (expectSymbol(parser, ",")) {
    //  std::cout << "Found symbol ,\n";
    //  parser.consume();
    //}

    std::cout << "Returning new instance\n";
    return instance;
  }
  catch (const std::exception& e) {
    std::cout << "[Error - Exception]:\n" << e.what();
    return nullptr;
  }

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

#include "Core/Serialization/Parser.h"

#include "Core/Math/OptimMathematics.h"
#include "Core/Reflection/OptimReflection.h"

/**
 * @brief
 * Check if the next token is an identifier
 */
static bool expectIdentifier(Parser& parser) {
  return parser.peek().type == ETokenType::Identifier;
}

/**
 * @brief
 * Check if the next token is a certain symbole
 */
static bool expectSymbol(Parser& parser, const char* symbol) {
  return parser.peek().text == symbol;
}

/**
 * @brief
 * Checks if the current object type object
 * contains a certain field.
 */
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
 * Skip object type field (Object/Structures)
 */
static void skipObjectField(Parser& parser) {
  // Consume all tokens tokens until end of object
  while (!expectSymbol(parser, ")")) {
    // If another object is detected skip the object
    if (expectSymbol(parser, "(")) {
      skipObjectField(parser);
    }
    parser.consume();
  }
  parser.consume();
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
    // Itterate through fields of object.
  while (!expectSymbol(parser, ")")) {

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
      if (fieldInfo->typeInfo->typeData == TypeData::Structure || fieldInfo->typeInfo->typeData == TypeData::Object) {

        if (!expectSymbol(parser, "(")) {
          throw std::exception("Expected symbol token: =.\n");
        }
        parser.consume();
        // std::cout << "Peeking parser: " << parser.peek().text << "\n";

        void* pField = (uint8*)pInstance + fieldInfo->offset;

        parseFields(parser, pField, fieldInfo->typeInfo);

        if (!expectSymbol(parser, ")")) {
          throw std::exception("Expected symbol token: =.\n");
        }

        parser.consume();
      }
      else if (fieldInfo->typeInfo->typeData == TypeData::Primitive) {
        void* pField = (uint8*)pInstance + fieldInfo->offset;
        fieldInfo->typeInfo->fromString(pField, parser.consume().text);
      }
      else {
        throw std::exception("[Parser Exception] - Unkownd or unsupported type data.\n");
      }
    }
    else { // IF CANNOT FIND FIELD
      std::cout << "An object of type: <" << param_typeInfo->name << "> DOES NOT HAVE A FIELD <" << identifierName << ">. Skipping field.\n";

      if (expectSymbol(parser, "(")) {
        parser.consume();
        skipObjectField(parser);
      }
      else {
        // If the filed is a primitive like value then simply skip the field
        parser.consume();

        if (expectSymbol(parser, ",")) {
          parser.consume();
        }
      }
    }

    if (expectSymbol(parser, ",")) {
      parser.consume();
    }
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
      std::string message;

      message.append("The next token:<").append(parser.peek().text).append("> is NOT an identifier: Peek->").append(parser.peek().text);

      throw std::exception(message.c_str());
    }

    // Get if the indetifier is a registered type and if its
    // of a type Object.
    if (!GetTypeRegistry().contains(parser.peek().text.c_str())) {
      printf("The iditifier [%s] is not a valid type.\n", parser.peek().text.c_str());
      return nullptr;
    }
    else {
      // printf("The iditifier [%s] IS a valid type.\n", parser.peek().text.c_str());
    }

    const TypeInfo* pInfo = GetTypeRegistry()[parser.peek().text.c_str()];

    if (pInfo->typeData == TypeData::Object) {
      // printf("The iditifier [%s] IS an Object type.\n", parser.peek().text.c_str());
    }
    else {
      //printf("The iditifier [%s] IS an Object type.\n", parser.peek().text.c_str());
      return nullptr;
    }

    // Consume identifie token.
    parser.consume();

    // If all is checked then create a neew instance of the object
    void* instance = pInfo->createFn();

    if (!expectSymbol(parser, "=")) {
      //printf("[Parser] Error: Could not find symbol \"=\"\n");
    }
    else {
      // printf("[Parser] Found symbol \"=\"\n");
      parser.consume();
    }

    if (!expectSymbol(parser, "(")) {
      //printf("[Parser] Error: Could not find symbol \"(\"\n");
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

    // std::cout << "All fields have been parsed\n";

    //std::cout << "Object created...\n";

    return instance;
  }
  catch (const std::exception& e) {
    std::cout << "[Error - Exception]: " << e.what();
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
  // std::cout << "Consuming token: " << peek().text << '\n';
  return tokens[index++];
}

bool Parser::isEnd() const {
  return index >= tokens.size();
}

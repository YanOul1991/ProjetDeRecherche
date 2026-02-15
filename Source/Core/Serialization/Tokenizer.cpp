#include "Core/Serialization/Tokenizer.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string_view>

class Lexer
{
 public:
  Lexer(std::string& txt) :
      text{txt} {
  }

  const std::string& text;
  size_t             cursor{0};

  char peek(int r = 0) const {
    return text[cursor + r];
  }

  char advance() {
    return text[cursor++];
  }

  bool isEof() const {
    return cursor >= text.size();
  }
};

/**
 * Checks if the character is a space or a newline
 * if so advance.
 */
static bool isWhiteSpace(Lexer& lexer) {
  if (std::isspace(lexer.peek())) {
    return true;
  }
  return false;
}

static bool expectTokenIdentifier(Lexer& lexer) {
  if (std::isalpha(lexer.peek())) {
    return true;
  }
  return false;
}

static bool expectTokenSymbol(Lexer& lexer) {
  char c = lexer.peek();
  return c == '=' || c == '(' || c == ')' || c == ',';
}

static bool expectTokenNumeric(Lexer& lexer) {
  return lexer.peek() == '-' || lexer.peek() == '+' || std::isdigit(lexer.peek());
}

static bool expectTokenString(Lexer& lexer) {
  return lexer.peek() == '"';
}

static std::string readTokenIdentifier(Lexer& lexer) {
  std::string tokenName;
  while (std::isalnum(lexer.peek()) || lexer.peek() == '_') {
    tokenName += lexer.advance();
  }
  return tokenName;
}

static char readTokenSymbol(Lexer& lexer) {
  return lexer.advance();
}

static std::string readTokenNumeric(Lexer& lexer) {
  std::string token;

  token += lexer.advance();

  // Read all 0-9 digits value
  while (std::isdigit(lexer.peek())) {
    token += lexer.advance();
  }

  // For decimal parts
  if (lexer.peek() == '.' && lexer.peek(1)) {
    token += lexer.advance();

    // Read decimal part
    while (std::isdigit(lexer.peek())) {
      token += lexer.advance();
    }
  }

  return token;
}

static std::string readTokenString(Lexer& lexer) {
  std::string token;

  // Get the first "
  token += lexer.advance();

  // Read string until next "
  while (lexer.peek() != '"') {
    token += lexer.advance();
  }
  // Then add last "
  token += lexer.advance();

  return token;
}

/**
 * @brief
 * Get the next expected token type.
 */
static ETokenType readNextExpectedTokenType(Lexer& lexer) {
  if (expectTokenIdentifier(lexer)) {
    return ETokenType::Identifier;
  }
  if (expectTokenSymbol(lexer)) {
    return ETokenType::Symbol;
  }
  if (expectTokenNumeric(lexer)) {
    return ETokenType::Number;
  }
  if (expectTokenString(lexer)) {
    return ETokenType::String;
  }
  return ETokenType::EndOfFile;
}

std::vector<Token> Token::Tokenize(const std::string& file) {
  std::vector<Token> tokenList;
  std::ifstream input(file);

  if (!input.is_open()) {
    std::cout << "Could not open file at: " << file << '\n';
    return tokenList;
  }

  std::string src = std::string(std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>());

  Lexer lexer(src);

  //std::cout << lexer.text << '\n';

  while (!lexer.isEof()) {
    while (isWhiteSpace(lexer)) {
      lexer.advance();
    }

    Token nextToken;
    ETokenType expectedTokenType = readNextExpectedTokenType(lexer);

    switch (expectedTokenType) {
    case ETokenType::Identifier: {
      std::string strToken = readTokenIdentifier(lexer);
      nextToken.text = strToken;
      nextToken.type = expectedTokenType;
      tokenList.push_back(nextToken);
       // std::cout << "IDENTIFIER : " << strToken << '\n';
      break;
    }
    case ETokenType::Symbol: {
      std::string strToken;
      strToken += readTokenSymbol(lexer);
      nextToken.text = strToken;
      nextToken.type = expectedTokenType;
      tokenList.push_back(nextToken);
       // std::cout << "SYMBOL     : " << strToken << '\n';
      break;
    }
    case ETokenType::Number: {
      std::string strToken = readTokenNumeric(lexer);
      nextToken.text = strToken;
      nextToken.type = expectedTokenType;
      tokenList.push_back(nextToken);
       // std::cout << "NUMERIC    : " << strToken << '\n';
      break;
    }
    case ETokenType::String: {
      std::string strToken = readTokenString(lexer);
      nextToken.text = strToken;
      nextToken.type = expectedTokenType;
      tokenList.push_back(nextToken);
       // std::cout << "STRING     : " << strToken << '\n';
      break;
    }
    default:
      lexer.advance();
      break;
    }
  }

  input.close();
  return tokenList;
}


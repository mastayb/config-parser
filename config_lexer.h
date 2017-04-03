#ifndef CONFIG_LEXER_H
#define CONFIG_LEXER_H

#include <string>
#include <istream>
#include <vector>
#include <set>

namespace SimpleConfig
{

typedef enum tokenType
{
   LEFT_BRACKET, RIGHT_BRACKET, EQUALS,
   IDENTIFIER, INTEGER, REAL_NUMBER, STRING, BOOL,
   END_OF_FILE
} TokenType;

typedef struct token
{
   TokenType type;
   std::string lexeme;
   int lineNum;
} Token;


class ConfigLexer
{
public:
   ConfigLexer();
   ~ConfigLexer();

   const std::vector<Token> Scan(std::istream& source);
   Token GetNextToken(std::istream& source);

private:
   Token LexBoolOrIdentifier(std::istream& source);
   Token LexNumber(std::istream& source);
   Token LexString(std::istream& source);
   void LexComment(std::istream& source);
   void UnexpectedCharacterError(char c);
   void UnterminatedStringError(int startLine);

   int line;

   static const std::set<char> whitespace;
   static const std::set<char> digits;
   static const std::set<char> letters;
   static const std::set<char> octalDigits;
   static const std::set<char> hexDigits;


};

}
#endif /*CONFIG_LEXER_H*/

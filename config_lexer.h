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
}TokenType;

typedef struct token
{
    TokenType type;
    std::string lexeme;
    int lineNum;
}Token;


class ConfigLexer
{
public:
    ConfigLexer();
    ~ConfigLexer();
   
    const std::vector<Token> Scan(std::istream& source);
private:
    Token GetNextToken(std::istream& source);
    Token LexToken(std::istream& source);
    Token LexString(std::istream& source);
    void LexComment(std::istream& source);
    void UnexpectedCharacterError(char c);
    void UnterminatedStringError(int startLine);
    
    int line;

    static const std::set<char> whitespace;
    static const std::set<char> numeric;
    static const std::set<char> alhpaNumeric;

    
};

}
#endif /*CONFIG_LEXER_H*/

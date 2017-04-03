#include "config_lexer.h"
#include <sstream>
#include <cstdio>
#include <cstring>
#include "parse_utilities.h"

namespace SimpleConfig
{


const char _whitespace[] = {' ', '\t', '\r', '\f', '\v'};
const std::set<char> ConfigLexer::whitespace(_whitespace, _whitespace+5);
const char* _digits= "0123456789";
const std::set<char> ConfigLexer::digits(_digits, _digits+strlen(_digits));
const char* _octal= "01234567";
const std::set<char> ConfigLexer::octalDigits(_octal, _octal+strlen(_octal));
const char* _hex= "0123456789ABCDEF";
const std::set<char> ConfigLexer::hexDigits(_hex, _hex+strlen(_hex));
const char *_letters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
const std::set<char> ConfigLexer::letters(_letters, _letters+strlen(_letters)); 

ConfigLexer::ConfigLexer(): line(1) 
{}


ConfigLexer::~ConfigLexer(){}

const std::vector<Token> ConfigLexer::Scan(std::istream& source)
{
    std::vector<Token> tokens;

    Token curToken = GetNextToken(source);
    while(curToken.type != END_OF_FILE)
    {
        tokens.push_back(curToken);
        curToken = GetNextToken(source);
    }
    tokens.push_back(curToken);

    return tokens;
}

Token ConfigLexer::GetNextToken(std::istream& source)
{
    while(true)
    {
        int c = source.get();

        if(whitespace.count(c))
        {
            continue;
        }
        else if (c == '[')
        {
            Token t = {LEFT_BRACKET, "[", line};
            return t;
        }
        else if (c == ']')
        {
            Token t = {RIGHT_BRACKET, "]", line};
            return t;
        }
        else if (c == '=')
        {
            Token t = {EQUALS, "=", line};
            return t;
        }
        else if (c == '\n')
        {
            line++;
        }
        else if (c == '"')
        {
            return LexString(source);
        }
        else if (c == '#')
        {
            LexComment(source);
        }
        else if (digits.count(c) || c == '-')
        {
            source.unget();
            return LexNumber(source);
        }
        else if (letters.count(c) || c == '_')
        {
            source.unget();
            return LexBoolOrIdentifier(source);
        }
        else if (c == EOF)
        {
            Token t = {END_OF_FILE, "", line};
            return t;
        }
        else
        {
            UnexpectedCharacterError(c);
        }
    }
}

Token ConfigLexer::LexString(std::istream& source)
{
    int startLine = line;
    std::stringstream buf;
    while(true)
    {
        char c = source.get();
        Token t = {STRING, buf.str(), startLine};
        switch(c)
        {
        case EOF:
            UnterminatedStringError(startLine);
            break;
        case '"':
            return t; 
        case '\n':
            line++;
        default:
            buf.put(c);
            break;
        }
    }
}

void ConfigLexer::LexComment(std::istream& source)
{
    int c = source.get();
    while(c != '\n' && c != EOF)
    {
        c = source.get();
    }
    source.unget();
}

Token ConfigLexer::LexBoolOrIdentifier(std::istream& source)
{
    std::stringstream lexeme;
    char c = source.get();
    while(letters.count(c) || digits.count(c) || c == '_')
    {
        lexeme << c;
        c = source.get();
    }
    source.unget();

    std::string upperCasedWord = ToUppered(lexeme.str());
    if(upperCasedWord == "TRUE" || upperCasedWord == "FALSE")
    {
        Token t = {BOOL, lexeme.str(), line};
        return t;
    }
    Token t = {IDENTIFIER, lexeme.str(), line};
    return t;
}
    
Token ConfigLexer::LexNumber(std::istream& source)
{
    std::stringstream lexeme;
    bool real=false;
    char c = source.get();
    std::set<char> baseDigits;
    baseDigits = digits;

    if(c == '-')
    {
        lexeme << c;
        c = source.get();
    }
    if(c == '0')
    {
        lexeme << c;
        c = source.get();
        baseDigits = octalDigits;
        if(c == 'x')
        {
            lexeme << c;
            c = source.get();
            baseDigits = hexDigits;
        }
    }
    while(baseDigits.count(c))
    {
        lexeme << c;
        c = source.get();
    }
    if(c == '.')
    {
        real=true;
        lexeme << c;
        c = source.get();
    }
    while(baseDigits.count(c))
    {
        lexeme << c;
        c = source.get();
    }
    if(c == 'e' || c == 'E')
    {
        real=true;
        lexeme << c;
        c = source.get();
    }
    while(baseDigits.count(c))
    {
        lexeme << c;
        c = source.get();
    }
    source.unget();

    if(real)
    {
        Token t = {REAL_NUMBER, lexeme.str(), line};
        return t;
    }
    Token t = {INTEGER, lexeme.str(), line};
    return t;
}

void ConfigLexer::UnexpectedCharacterError(char c)
{
    std::ostringstream messageBuf;
    messageBuf << "Unexpected character " << c << " on line " << line;
    throw std::logic_error(messageBuf.str());
}

void ConfigLexer::UnterminatedStringError(int startLine)
{
    std::ostringstream messageBuf;
    messageBuf<< "Unterminiated string on line "<< startLine;
    throw std::logic_error(messageBuf.str());
}




}

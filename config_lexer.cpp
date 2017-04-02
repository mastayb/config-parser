#include "config_lexer.h"
#include <sstream>
#include <cstdio>
#include <cstring>
#include "parse_utilities.h"

namespace SimpleConfig
{


const char _whitespace[] = {' ', '\t', '\r', '\f', '\v'};
const std::set<char> ConfigLexer::whitespace(_whitespace, _whitespace+5);
const char* _numeric = "+-0123456789";
const std::set<char> ConfigLexer::numeric(_numeric, _numeric+strlen(_numeric));
const char *_alphanumeric = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_0123456789+-";
const std::set<char> ConfigLexer::alhpaNumeric(_alphanumeric, _alphanumeric+strlen(_alphanumeric)); 

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
        else if (alhpaNumeric.count(c))
        {
            source.unget();
            return LexToken(source);
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

Token ConfigLexer::LexToken(std::istream& source)
{
    std::string word;
    source >> word;

    std::string upperCasedWord = ToUppered(word);
    if(upperCasedWord == "TRUE" || upperCasedWord == "FALSE")
    {
        Token t = {BOOL, word, line};
        return t;
    }
    else if (numeric.count(word[0]))
    {
        if(word.find('.') == std::string::npos)
        {
            Token t = {INTEGER, word, line};
            return t;
        }

        Token t = {REAL_NUMBER, word, line};
        return t;
    }
    else 
    {
        Token t = {IDENTIFIER, word, line};
        return t;
    }
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

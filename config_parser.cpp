#include "config_parser.h"
#include "parse_utilities.h"
#include <fstream>
#include <stdexcept>
#include <sstream>

namespace SimpleConfig
{

ConfigParser::ConfigParser()
{}

ConfigParser::~ConfigParser()
{}

void ConfigParser::Parse(const char *filename)
{
   std::ifstream file(filename);
   mFilename = filename;
   if (! file.is_open() )
   {
      std::string fName(filename);
      throw std::runtime_error("Could not open file " + fName);
   }
   Parse(file);
}

void ConfigParser::Parse(std::istream& configStream)
{
    mCurToken = lexer.GetNextToken(configStream);
    mCurSection = "";
    while(mCurToken.type != END_OF_FILE)
    {
        switch(mCurToken.type)
        {
        case LEFT_BRACKET:
            curSection = ParseSectionHeader(configStream);
            break;
        case IDENTIFIER:
            ParseAssignment(configStream);
            break;
        default:
            ParseError("Expected identifier or '['");
            break;
        }
        mCurToken = lexer.GetNextToken(configStream);
    }
}

void ConfigParser::ParseSectionHeader(std::istream& configStream)
{
    mCurToken = lexer.GetNextToken(configStream);
    if(mCurToken.type == IDENTIFIER)
    {
        mCurSection = mCurToken.lexeme;
    }
    else
    {
        ParseError("Expected identifier");
    }

    mCurToken = lexer.GetNextToken(configStream);
    if(mCurToken.type != RIGHT_BRACKET)
    {
        ParseError("Expected ']'");
    }
}

void ConfigParser::ParseAssignment(std::istream& configStream)
{
    std::string id = mCurToken.lexeme;
    mCurToken = lexer.GetNextToken(configStream);
    if(mCurToken.type != EQUALS)
    {
        ParseError("Expected '='");
    }
    mCurToken = lexer.GetNextToken(configStream);
    if(!IsLiteral(mCurToken))
    {
        ParseError("Expected literal"); 
    }
    parseMap[curSection][id] = mCurToken;
}

bool ConfigParser::LookupBoolean(const std::string& section, const std::string& key)
{
   bool value;
   if(parseMap[section].count(key) != 0)
   {
      try 
      {
         value = Str2Bool(parseMap[section][key]);
      }
      catch(std::logic_error& e)
      {
         ConversionError(section, key, e.what());
      }
   }
   else
   {
      KeyError(key);
   }
   return value;
}

bool ConfigParser::LookupBoolean(const std::string& key)
{
   return LookupBoolean("", key);
}

double ConfigParser::LookupDouble(const std::string& key)
{
   double value;
   if(parseMap.count(key) != 0)
   {
      try 
      {
         value = Str2Double(parseMap[key]);
      }
      catch(std::logic_error& e)
      {
         ConversionError(key, e.what());
      }
   }
   else
   {
      KeyError(key);
   }
   return value;
}

int ConfigParser::LookupInteger(const std::string& key)
{
   int value;
   if(parseMap.count(key) != 0)
   {
      try 
      {
         value = Str2Int(parseMap[key]);
      }
      catch(std::logic_error& e)
      {
         ConversionError(key, e.what());
      }
   }
   else
   {
      KeyError(key);
   }
   return value;
}

std::string ConfigParser::LookupString(const std::string& key)
{
   std::string value;
   if(parseMap.count(key) != 0)
   {
      value = parseMap[key];
   }
   else
   {
      KeyError(key);
   }
   return value;
}


void ConfigParser::ParseError(int lineNum, std::string& line)
{
   std::stringstream msgBuf;
   msgBuf << "Error parsing:\n";
   if (!mFilename.empty())
   {
      msgBuf << mFilename;
   }
   msgBuf << "(" << lineNum << "):" << line; 

   throw std::runtime_error(msgBuf.str());
}

void ConfigParser::KeyError(std::string key)
{
   throw std::logic_error("Key: " + key + " does not exist");
}

void ConfigParser::ConversionError(std::string key, std::string caughtExceptionMsg)
{
   std::string msg = "Error looking up key (" + key + ") " + caughtExceptionMsg;
   throw std::logic_error(msg);
}

}

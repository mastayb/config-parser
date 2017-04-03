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
    while(curToken.type != END_OF_FILE)
    {
        switch(curToken.type)
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
    }
}

void ConfigParser::ParseLine(int& lineNum, std::string& line)
{
      lineNum++;
      Trim(line);

      if(line[0] == COMMENT_INDICATOR || line.empty())
      {
         return;
      }

      ParseKeyValuePair(lineNum, line);
}

void ConfigParser::ParseKeyValuePair(int lineNum, std::string& statement)
{
   std::string key;
   std::istringstream statementStream(statement);

   if(std::getline(statementStream, key, '='))
   {
      std::string value;
      if(std::getline(statementStream, value)) 
      {
         Trim(key);
         Trim(value);
         parseMap[key] = value;
      }
      else 
      {
         ParseError(lineNum, statement);
      }
   }
   else
   {
      ParseError(lineNum, statement);
   }
}


bool ConfigParser::LookupBoolean(const std::string& key)
{
   bool value;
   if(parseMap.count(key) != 0)
   {
      try 
      {
         value = Str2Bool(parseMap[key]);
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

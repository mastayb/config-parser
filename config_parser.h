#ifndef CONFIG_PARSER_H
#define CONFIG_PARSER_H

#include <string>
#include <map>
#include "config_lexer.h"

namespace SimpleConfig
{

class ConfigParser
{
public:
   ConfigParser();
   ~ConfigParser();

   void Parse(const char *filename);
   void Parse(std::istream& configStream);

   bool LookupBoolean(const std::string& section, const std::string& key);
   bool LookupBoolean(const std::string& key);

   double LookupDouble(const std::string& section, const std::string& key);
   double LookupDouble(const std::string& key);

   int LookupInteger(const std::string& section, const std::string& key);
   int LookupInteger(const std::string& key);

   std::string LookupString(const std::string& section, const std::string& key);
   std::string LookupString(const std::string& key);

private:
   void ParseSectionHeader(std::istream& configStream);
   void ParseAssignment(std::istream& configStream);

   void ParseError(std::string& msg);
   void KeyError(std::string key);
   void ConversionError(std::string key, std::string caughtExceptionMsg);

   std::map<std::string, std::map<std::string, Token>> parseMap;
   std::string mFilename;
   ConfigLexer lexer;

   Token mCurToken;
   std::string mCurSection;
};


}

#endif /* CONFIG_PARSER_H */

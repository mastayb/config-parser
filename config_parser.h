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
   void Parse(const std::string& filename);
   void Parse(std::istream& configStream);

   Token Lookup(const std::string& section, const std::string& key) const;

   bool LookupBoolean(const std::string& section, const std::string& key) const;
   bool LookupBoolean(const std::string& key) const;

   double LookupDouble(const std::string& section, const std::string& key) const;
   double LookupDouble(const std::string& key) const;

   int LookupInteger(const std::string& section, const std::string& key) const;
   int LookupInteger(const std::string& key) const;

   std::string LookupString(const std::string& section, const std::string& key) const;
   std::string LookupString(const std::string& key) const;

private:
   void ParseSectionHeader(std::istream& configStream);
   void ParseAssignment(std::istream& configStream);
   bool IsLiteral(const Token& tok);

   void ConversionError(std::string section, std::string key, std::string caughtMsg, int sourceLine) const;
   void ParseError(const char* expected);

   std::map<std::string, std::map<std::string, Token> > parseMap;
   ConfigLexer lexer;

   Token mCurToken;
   std::string mCurSection;
};


}

#endif /* CONFIG_PARSER_H */

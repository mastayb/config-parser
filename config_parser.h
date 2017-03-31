#ifndef CONFIG_PARSER_H
#define CONFIG_PARSER_H 

#include <string>
#include <map>

namespace SimpleConfig
{

class ConfigParser
{
public:
   ConfigParser();
   virtual ~ConfigParser();

   //Parses line by line with an extremely simple/rudimentary grammar
   //Blank lines and lines starting with COMMENT_INDICATOR are ignored
   //All other lines should be in the form:
   //key_value_pair: key, {ws}, '=', {ws}, value
   //and will be entered into the map for query by key
   virtual void Parse(const char *filename);
   virtual void Parse(std::istream& configStream);

   virtual bool LookupBoolean(const std::string& key);
   
   virtual double LookupDouble(const std::string& key);

   virtual int LookupInteger(const std::string& key);

   virtual std::string LookupString(const std::string& key);

   static const char COMMENT_INDICATOR = '#';

private:
   void ParseLine(int& lineNum, std::string& line);
   void ParseKeyValuePair(int lineNum, std::string& statement);

   void ParseError(int lineNum, std::string& line);
   void KeyError(std::string key);
   void ConversionError(std::string key, std::string caughtExceptionMsg);

   std::map<std::string, std::string> parseMap;
   std::string mFilename;

};


}

#endif /* CONFIG_PARSER_H */

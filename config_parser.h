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

   virtual void Parse(const char *filename);
   virtual void Parse(std::istream& configStream);

   virtual bool LookupBoolean(const std::string& key);
   
   virtual double LookupDouble(const std::string& key);

   virtual float LookupFloat(const std::string& key);

   virtual int LookupInteger(const std::string& key);

   virtual std::string LookupString(const std::string& key);

private:
   std::map<std::string, std::string> parseMap;


};


}

#endif /* CONFIG_PARSER_H */

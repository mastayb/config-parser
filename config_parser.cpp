#include "config_parser.h"
#include "parse_utilities.h"
#include <fstream>
#include <stdexcept>

namespace SimpleConfig
{

ConfigParser::ConfigParser()
{}

ConfigParser::~ConfigParser()
{}

void ConfigParser::Parse(const char *filename)
{
   std::ifstream file(filename);
   if (! file.is_open() )
   {
      std::string fName(filename);
      throw std::runtime_error("Could not open file " + fName);
   }

   Parse(file);
}

void ConfigParser::Parse(std::istream& configStream)
{
   std::string line;

   //while(std::getline(configStream, line))
   //{


   
}

bool ConfigParser::LookupBoolean(const std::string& key)
{
   return false;
}

double ConfigParser::LookupDouble(const std::string& key)
{
   return 0.0;
}

float ConfigParser::LookupFloat(const std::string& key)
{
   return 0.0;
}

int ConfigParser::LookupInteger(const std::string& key)
{
   return 0;
}

std::string ConfigParser::LookupString(const std::string& key)
{
   return "";
}

}

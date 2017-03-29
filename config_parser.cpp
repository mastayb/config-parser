#include "config_parser.h"
#include <istream>

ConfigParser::ConfigParser()
{}

ConfigParser::~ConfigParser()
{}

void ConfigParser::Parse(const std::string& filename)
{

}

void ConfigParser::Parse(std::istream configStream)
{

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


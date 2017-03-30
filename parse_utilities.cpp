#include "parse_utilities.h"
#include <cctype>
#include <algorithm>
#include <functional>
#include <cstdlib>
#include <cerrno>
#include <stdexcept>

namespace SimpleConfig
{

void LTrim(std::string &s)
{
   s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun(std::isspace))));
}

void RTrim(std::string &s)
{
   s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun(std::isspace))).base(), s.end());
}

void Trim(std::string &s)
{
   LTrim(s);
   RTrim(s);
}

std::string LTrimmed(std::string s)
{
   LTrim(s);
   return s;
}

std::string RTrimmed(std::string s)
{
   RTrim(s);
   return s;
}

std::string Trimmed(std::string s)
{
   Trim(s);
   return s;
}

void ToUpper(std::string &s)
{
   std::transform(s.begin(), s.end(), s.begin(), std::toupper); 
}

std::string ToUppered(std::string s)
{
   ToUpper(s);
   return s;
}

int Str2Int(std::string s, int base /* =0 */)
{
   return Str2Int(s.c_str(), base);
}

int Str2Int(const char *s, int base /* =0 */)
{
   char *end;
   int i = std::strtol(s, &end, base);

   if(errno == ERANGE)
   {
      std::string iStr(s);
      errno = 0;
      throw std::out_of_range(iStr + " out of range of long");
   }

   if(*s == '\0')
   {
      throw std::invalid_argument("cannot convert empty string to int");
   }

   if(*end != '\0')
   {
      std::string iStr(s);
      throw std::invalid_argument("cannot convert" + iStr + "to int");
   }

   return i;
}


double Str2Double(std::string s)
{
   return Str2Double(s.c_str());
}

double Str2Double(const char *s)
{
   char *end;
   double d = std::strtod(s, &end);

   if(errno == ERANGE)
   {
      std::string iStr(s);
      errno = 0;
      throw std::out_of_range(iStr + " out of range of double");
   }

   if(*s == '\0')
   {
      throw std::invalid_argument("cannot convert empty string to double"); 
   }

   if(*end != '\0')
   {
      std::string iStr(s);
      throw std::invalid_argument("cannot convert" + iStr + "to double");
   }
   return d;
}

bool Str2Bool(std::string s)
{
   ToUpper(s);
   
   if (s == "TRUE") 
   {
      return true;
   }
   else if (s == "FALSE")
   {
      return false;
   }
   else
   {
      return Str2Int(s);
   }
}


bool Str2Bool(const char *s)
{
   return Str2Bool(std::string(s));
}
}

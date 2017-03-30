#ifndef PARSE_UTILITIES_H
#define PARSE_UTILITIES_H 

#include <string>

namespace SimpleConfig
{
   //Basic string utilities
   void LTrim(std::string &s);
   void RTrim(std::string &s);
   void Trim(std::string &s);
   std::string LTrimmed(std::string s);
   std::string RTrimmed(std::string s);
   std::string Trimmed(std::string s);

   void ToUpper(std::string &s);
   std::string ToUppered(std::string s);

   //String conversion functions
   //Wrap C style conversions to use exceptions
   //Recreates C++11 string conversions
   int Str2Int(std::string s, int base = 0);
   int Str2Int(const char *s, int base = 0);

   double Str2Double(std::string s);
   double Str2Double(const char *s);

   bool Str2Bool(std::string s);
   bool Str2Bool(const char *s);

}



#endif /* PARSE_UTILITIES_H */

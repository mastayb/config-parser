#include "config_parser.h"
#include "gtest/gtest.h"
#include <sstream>
#include <stdexcept>

namespace 
{

class ConfigParserTest : public ::testing::Test 
{
protected:

   ConfigParserTest() :
      goodConfig(
         "testInt=1"
         "testFloat=2.5"
         "testNegativeInt=-5"
         "testNegativeFloat=-3.3"
         "testScientific=-3.5e7"
         "testString=Hello World"),
      goodConfigStream(goodConfig)
   {}


   //Objects here can be used by all tests
   std::string goodConfig;
   std::istringstream goodConfigStream;
};

TEST_F(ConfigParserTest, LookupBoolTrue) {
   SimpleConfig::ConfigParser c;
   EXPECT_EQ(true, c.LookupBoolean("a"));
}

TEST(ParseTest, NonexistantFile)
{
   SimpleConfig::ConfigParser c;
   EXPECT_THROW(c.Parse("Nonextantfile.txt"), std::runtime_error);
}

TEST(ParseTest, ExistingFile)
{
   SimpleConfig::ConfigParser c;
   EXPECT_NO_THROW(c.Parse("testFile.txt"));
}

TEST(ParseTest, BadConfigFormat)
{
   std::string badConfig = 
      "bcd";
   std::istringstream badConfStream(badConfig);
   SimpleConfig::ConfigParser c;
   EXPECT_THROW(c.Parse(badConfStream), std::runtime_error);

}


}

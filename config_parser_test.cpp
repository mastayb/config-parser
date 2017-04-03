#include "config_parser.h"
#include "gtest/gtest.h"
#include <sstream>
#include <stdexcept>

namespace
{

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
      "avc=xyz\n"
      "bcd";
   std::istringstream badConfStream(badConfig);
   SimpleConfig::ConfigParser c;
   EXPECT_THROW(c.Parse(badConfStream), std::runtime_error);
}


class ConfigParserTest : public ::testing::Test
{
protected:

   ConfigParserTest() :
      goodConfig(
         "testInt=1\n"
         "testDouble=2.5\n"
         "#comment\n"
         "\n" //empty line
         "testNegativeInt=-5\n"
         "testNegativeDouble=-3.3\n"
         "testScientific=-3.5e7\n"
         "testBoolTrue=true\n"
         "testBoolFalse = FALSE  \t \n"
         "testString=Hello World\n"),
      goodConfigStream(goodConfig)
   {
      testConfigParser.Parse(goodConfigStream);
   }


   //Objects here can be used by all tests
   std::string goodConfig;
   std::istringstream goodConfigStream;
   SimpleConfig::ConfigParser testConfigParser;
};

TEST_F(ConfigParserTest, LookupBoolTrueWorks)
{
   EXPECT_EQ(true, testConfigParser.LookupBoolean("testBoolTrue"));
}

TEST_F(ConfigParserTest, LookupBoolFalseWorks)
{
   EXPECT_EQ(false, testConfigParser.LookupBoolean("testBoolFalse"));
}

TEST_F(ConfigParserTest, BadBoolLookupThrows)
{
   EXPECT_THROW(testConfigParser.LookupBoolean("testString"), std::logic_error);
}

TEST_F(ConfigParserTest, BadKeyBoolLookupThrows)
{
   EXPECT_THROW(testConfigParser.LookupBoolean("testNonExistantKey"), std::logic_error);
}

TEST_F(ConfigParserTest, LookupIntWorks)
{
   EXPECT_EQ(1, testConfigParser.LookupInteger("testInt"));
}

TEST_F(ConfigParserTest, LookupNegativeIntWorks)
{
   EXPECT_EQ(-5, testConfigParser.LookupInteger("testNegativeInt"));
}

TEST_F(ConfigParserTest, BadIntLookupThrows)
{
   EXPECT_THROW(testConfigParser.LookupInteger("testString"), std::logic_error);
}

TEST_F(ConfigParserTest, BadKeyIntLookupThrows)
{
   EXPECT_THROW(testConfigParser.LookupInteger("testNonExistantKey"), std::logic_error);
}

TEST_F(ConfigParserTest, LookupDoubleWorks)
{
   EXPECT_DOUBLE_EQ(2.5, testConfigParser.LookupDouble("testDouble"));
}

TEST_F(ConfigParserTest, LookupNegativeDoubleWorks)
{
   EXPECT_DOUBLE_EQ(-3.3, testConfigParser.LookupDouble("testNegativeDouble"));
}

TEST_F(ConfigParserTest, LookupScientificDoubleWorks)
{
   EXPECT_DOUBLE_EQ(-3.5e7, testConfigParser.LookupDouble("testScientific"));
}

TEST_F(ConfigParserTest, BadDoubleLookupThrows)
{
   EXPECT_THROW(testConfigParser.LookupDouble("testString"), std::logic_error);
}

TEST_F(ConfigParserTest, BadKeyDoubleLookupThrows)
{
   EXPECT_THROW(testConfigParser.LookupDouble("testNonExistantKey"), std::logic_error);
}

TEST_F(ConfigParserTest, LookupStringWorks)
{
   EXPECT_EQ(testConfigParser.LookupString("testString"), "Hello World");
}

TEST_F(ConfigParserTest, BadKeyLookupStringThrows)
{
   EXPECT_THROW(testConfigParser.LookupString("testNonExistantKey"), std::logic_error);
}


}

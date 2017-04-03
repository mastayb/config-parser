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
         "testString=\"Hello World\"\n"),
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


class SectionedConfigParseTest : public ::testing::Test
{
protected:

   SectionedConfigParseTest() :
      goodConfig(
         "testInt=1\n"
         "testDouble=2.5\n"
         "#comment\n"
         "\n" //empty line
         "[Section1]\n"
         "testNegativeInt=-5\n"
         "testNegativeDouble=-3.3\n"
         "testScientific=-3.5e7\n"
         "testBoolTrue=true\n"
         "[Section2]\n"
         "testBoolFalse = FALSE  \t \n"
         "testString=\"Hello World\"\n"),
      goodConfigStream(goodConfig)
   {
      testConfigParser.Parse(goodConfigStream);
   }


   //Objects here can be used by all tests
   std::string goodConfig;
   std::istringstream goodConfigStream;
   SimpleConfig::ConfigParser testConfigParser;
};

TEST_F(SectionedConfigParseTest, LookupBoolTrueWorks)
{
   EXPECT_EQ(true, testConfigParser.LookupBoolean("Section1","testBoolTrue"));
}

TEST_F(SectionedConfigParseTest, LookupBoolFalseWorks)
{
   EXPECT_EQ(false, testConfigParser.LookupBoolean("Section2", "testBoolFalse"));
}

TEST_F(SectionedConfigParseTest, BadBoolLookupThrows)
{
   EXPECT_THROW(testConfigParser.LookupBoolean("Section2", "testString"), std::logic_error);
}

TEST_F(SectionedConfigParseTest, BadKeyBoolLookupThrows)
{
   EXPECT_THROW(testConfigParser.LookupBoolean("Section2", "testNonExistantKey"), std::logic_error);
}

TEST_F(SectionedConfigParseTest, LookupIntWorks)
{
   EXPECT_EQ(1, testConfigParser.LookupInteger("testInt"));
}

TEST_F(SectionedConfigParseTest, LookupNegativeIntWorks)
{
   EXPECT_EQ(-5, testConfigParser.LookupInteger("Section1", "testNegativeInt"));
}

TEST_F(SectionedConfigParseTest, BadIntLookupThrows)
{
   EXPECT_THROW(testConfigParser.LookupInteger("Section2", "testString"), std::logic_error);
}

TEST_F(SectionedConfigParseTest, BadKeyIntLookupThrows)
{
   EXPECT_THROW(testConfigParser.LookupInteger("Section2", "testNonExistantKey"), std::logic_error);
}

TEST_F(SectionedConfigParseTest, LookupDoubleWorks)
{
   EXPECT_DOUBLE_EQ(2.5, testConfigParser.LookupDouble("testDouble"));
}

TEST_F(SectionedConfigParseTest, LookupNegativeDoubleWorks)
{
   EXPECT_DOUBLE_EQ(-3.3, testConfigParser.LookupDouble("Section1", "testNegativeDouble"));
}

TEST_F(SectionedConfigParseTest, LookupScientificDoubleWorks)
{
   EXPECT_DOUBLE_EQ(-3.5e7, testConfigParser.LookupDouble("Section1", "testScientific"));
}

TEST_F(SectionedConfigParseTest, BadDoubleLookupThrows)
{
   EXPECT_THROW(testConfigParser.LookupDouble("Section2", "testString"), std::logic_error);
}

TEST_F(SectionedConfigParseTest, BadKeyDoubleLookupThrows)
{
   EXPECT_THROW(testConfigParser.LookupDouble("Section2", "testNonExistantKey"), std::logic_error);
}

TEST_F(SectionedConfigParseTest, LookupStringWorks)
{
   EXPECT_EQ(testConfigParser.LookupString("Section2", "testString"), "Hello World");
}

TEST_F(SectionedConfigParseTest, BadKeyLookupStringThrows)
{
   EXPECT_THROW(testConfigParser.LookupString("Section2", "testNonExistantKey"), std::logic_error);
}

}

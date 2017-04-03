#include "parse_utilities.h"
#include "gtest/gtest.h"
#include <stdexcept>


namespace
{

class TrimUtilitiesTest: public ::testing::Test
{
protected:
   TrimUtilitiesTest():
      baseString("test string"),
      sampleWhiteSpace(" \t \t"),
      baseStringWithLeftWhiteSpace(sampleWhiteSpace+"test string"),
      baseStringWithRightWhiteSpace("test string"+sampleWhiteSpace),
      baseStringWithLeftAndRightWhiteSpace(sampleWhiteSpace+"test string"+sampleWhiteSpace)
   {}

   const std::string baseString;
   const std::string sampleWhiteSpace;
   const std::string baseStringWithLeftWhiteSpace;
   const std::string baseStringWithRightWhiteSpace;
   const std::string baseStringWithLeftAndRightWhiteSpace;

};

TEST_F(TrimUtilitiesTest, BaseStringUnchanged)
{
   std::string testString = baseString;
   SimpleConfig::LTrim(testString);
   EXPECT_EQ(baseString, testString);

   testString = baseString;
   SimpleConfig::RTrim(testString);
   EXPECT_EQ(baseString, testString);

   testString = baseString;
   SimpleConfig::Trim(testString);
   EXPECT_EQ(baseString, testString);

   testString = SimpleConfig::LTrimmed(baseString);
   EXPECT_EQ(baseString, testString);

   testString = baseString;
   testString = SimpleConfig::RTrimmed(baseString);
   EXPECT_EQ(baseString, testString);

   testString = baseString;
   testString = SimpleConfig::Trimmed(baseString);
   EXPECT_EQ(baseString, testString);
}

TEST_F(TrimUtilitiesTest, LeftTrimWorks)
{
   std::string testString = baseStringWithLeftWhiteSpace;
   SimpleConfig::LTrim(testString);
   EXPECT_EQ(baseString, testString);

   testString = baseStringWithRightWhiteSpace;
   SimpleConfig::LTrim(testString);
   EXPECT_EQ(baseStringWithRightWhiteSpace, testString);

   testString = baseStringWithLeftAndRightWhiteSpace;
   SimpleConfig::LTrim(testString);
   EXPECT_EQ(baseStringWithRightWhiteSpace, testString);
}

TEST_F(TrimUtilitiesTest, RightTrimWorks)
{
   std::string testString = baseStringWithRightWhiteSpace;
   SimpleConfig::RTrim(testString);
   EXPECT_EQ(baseString, testString);

   testString = baseStringWithLeftWhiteSpace;
   SimpleConfig::RTrim(testString);
   EXPECT_EQ(baseStringWithLeftWhiteSpace, testString);

   testString = baseStringWithLeftAndRightWhiteSpace;
   SimpleConfig::RTrim(testString);
   EXPECT_EQ(baseStringWithLeftWhiteSpace, testString);
}

TEST_F(TrimUtilitiesTest, TrimWorks)
{
   std::string testString = baseStringWithLeftWhiteSpace;
   SimpleConfig::Trim(testString);
   EXPECT_EQ(baseString, testString);

   testString = baseStringWithRightWhiteSpace;
   SimpleConfig::Trim(testString);
   EXPECT_EQ(baseString, testString);

   testString = baseStringWithLeftAndRightWhiteSpace;
   SimpleConfig::Trim(testString);
   EXPECT_EQ(baseString, testString);
}

TEST_F(TrimUtilitiesTest, LeftTrimmedWorks)
{
   std::string testString;
   testString = SimpleConfig::LTrimmed(baseStringWithLeftWhiteSpace);
   EXPECT_EQ(baseString, testString);

   testString = SimpleConfig::LTrimmed(baseStringWithRightWhiteSpace);
   EXPECT_EQ(baseStringWithRightWhiteSpace, testString);

   testString = SimpleConfig::LTrimmed(baseStringWithLeftAndRightWhiteSpace);
   EXPECT_EQ(baseStringWithRightWhiteSpace, testString);
}

TEST_F(TrimUtilitiesTest, RightTrimmedWorks)
{
   std::string testString;
   testString = SimpleConfig::RTrimmed(baseStringWithRightWhiteSpace);
   EXPECT_EQ(baseString, testString);

   testString = SimpleConfig::RTrimmed(baseStringWithLeftWhiteSpace);
   EXPECT_EQ(baseStringWithLeftWhiteSpace, testString);

   testString = SimpleConfig::RTrimmed(baseStringWithLeftAndRightWhiteSpace);
   EXPECT_EQ(baseStringWithLeftWhiteSpace, testString);
}


TEST_F(TrimUtilitiesTest, TrimmedWorks)
{
   std::string testString;
   testString = SimpleConfig::Trimmed(baseStringWithRightWhiteSpace);
   EXPECT_EQ(baseString, testString);

   testString = SimpleConfig::Trimmed(baseStringWithLeftWhiteSpace);
   EXPECT_EQ(baseString, testString);

   testString = SimpleConfig::Trimmed(baseStringWithLeftAndRightWhiteSpace);
   EXPECT_EQ(baseString, testString);
}

TEST_F(TrimUtilitiesTest, EmptyString)
{
   std::string testString = "";
   SimpleConfig::LTrim(testString);
   EXPECT_EQ("", testString);

   testString = "";
   SimpleConfig::RTrim(testString);
   EXPECT_EQ("", testString);

   testString = "";
   SimpleConfig::Trim(testString);
   EXPECT_EQ("", testString);

   testString = SimpleConfig::LTrimmed("");
   EXPECT_EQ("", testString);

   testString = SimpleConfig::RTrimmed("");
   EXPECT_EQ("", testString);

   testString = SimpleConfig::Trimmed("");
   EXPECT_EQ("", testString);
}

TEST_F(TrimUtilitiesTest, OnlyWhiteSpace)
{
   const std::string whiteSpaceString = " \t ";
   std::string testString = whiteSpaceString;
   SimpleConfig::LTrim(testString);
   EXPECT_EQ("", testString);

   testString = whiteSpaceString;
   SimpleConfig::RTrim(testString);
   EXPECT_EQ("", testString);

   testString = whiteSpaceString;
   SimpleConfig::Trim(testString);
   EXPECT_EQ("", testString);

   testString = SimpleConfig::LTrimmed(whiteSpaceString);
   EXPECT_EQ("", testString);

   testString = SimpleConfig::RTrimmed(whiteSpaceString);
   EXPECT_EQ("", testString);

   testString = SimpleConfig::Trimmed(whiteSpaceString);
   EXPECT_EQ("", testString);
}



TEST(Str2IntTest, OutOfRange)
{
   std::string testString = "4000000000";
   EXPECT_THROW(SimpleConfig::Str2Int(testString), std::out_of_range);
   EXPECT_THROW(SimpleConfig::Str2Int(testString.c_str()), std::out_of_range);

   testString = "-4000000000";
   EXPECT_THROW(SimpleConfig::Str2Int(testString), std::out_of_range);
   EXPECT_THROW(SimpleConfig::Str2Int(testString.c_str()), std::out_of_range);
}

TEST(Str2IntTest, EmptyString)
{
   std::string testString;
   EXPECT_THROW(SimpleConfig::Str2Int(testString), std::invalid_argument);
   EXPECT_THROW(SimpleConfig::Str2Int(testString.c_str()), std::invalid_argument);
}

TEST(Str2IntTest, UnexpectedForm)
{
   std::string testString = " abc";
   EXPECT_THROW(SimpleConfig::Str2Int(testString), std::invalid_argument);
   EXPECT_THROW(SimpleConfig::Str2Int(testString.c_str()), std::invalid_argument);

   testString = "123abc";
   EXPECT_THROW(SimpleConfig::Str2Int(testString), std::invalid_argument);
   EXPECT_THROW(SimpleConfig::Str2Int(testString.c_str()), std::invalid_argument);

   testString = "123 123";
   EXPECT_THROW(SimpleConfig::Str2Int(testString), std::invalid_argument);
   EXPECT_THROW(SimpleConfig::Str2Int(testString.c_str()), std::invalid_argument);

   testString = "123.123";
   EXPECT_THROW(SimpleConfig::Str2Int(testString), std::invalid_argument);
   EXPECT_THROW(SimpleConfig::Str2Int(testString.c_str()), std::invalid_argument);
}

TEST(Str2IntTest, Str2IntWorks)
{
   std::string testString = "12345";
   EXPECT_EQ(SimpleConfig::Str2Int(testString), 12345);

   testString = "-12345";
   EXPECT_EQ(SimpleConfig::Str2Int(testString), -12345);

   testString = "0x12345";
   EXPECT_EQ(SimpleConfig::Str2Int(testString), 0x12345);

   testString = "12345";
   EXPECT_EQ(SimpleConfig::Str2Int(testString, 16), 0x12345);

   testString = "1001";
   EXPECT_EQ(SimpleConfig::Str2Int(testString, 2), 9);
}

TEST(Str2DoubleTest, OutOfRange)
{
   std::string testString = "100e1000000";
   EXPECT_THROW(SimpleConfig::Str2Double(testString), std::out_of_range);
   EXPECT_THROW(SimpleConfig::Str2Double(testString.c_str()), std::out_of_range);
   testString = "-100e10000000";
   EXPECT_THROW(SimpleConfig::Str2Double(testString), std::out_of_range);
   EXPECT_THROW(SimpleConfig::Str2Double(testString.c_str()), std::out_of_range);
}

TEST(Str2DoubleTest, EmptyString)
{
   std::string testString = "";
   EXPECT_THROW(SimpleConfig::Str2Double(testString), std::invalid_argument);
   EXPECT_THROW(SimpleConfig::Str2Double(testString.c_str()), std::invalid_argument);
}

TEST(Str2DoubleTest, UnexpectedForm)
{
   std::string testString = "abcd";
   EXPECT_THROW(SimpleConfig::Str2Double(testString), std::invalid_argument);
   EXPECT_THROW(SimpleConfig::Str2Double(testString.c_str()), std::invalid_argument);


   testString = "123.Z";
   EXPECT_THROW(SimpleConfig::Str2Double(testString), std::invalid_argument);
   EXPECT_THROW(SimpleConfig::Str2Double(testString.c_str()), std::invalid_argument);

   testString = "123.0   13";
   EXPECT_THROW(SimpleConfig::Str2Double(testString), std::invalid_argument);
   EXPECT_THROW(SimpleConfig::Str2Double(testString.c_str()), std::invalid_argument);
}

TEST(Str2DoubleTest, Str2DoubleWorks)
{
   std::string testString = "123.512341234";
   EXPECT_DOUBLE_EQ(SimpleConfig::Str2Double(testString), 123.512341234);

   testString = "-1235123.41234";
   EXPECT_DOUBLE_EQ(SimpleConfig::Str2Double(testString), -1235123.41234);
   testString = "-1235.214e10";
   EXPECT_DOUBLE_EQ(SimpleConfig::Str2Double(testString), -1235.214e10);
}

TEST(ToUpperTest, ToUpperWorks)
{
   std::string testString = "abc d.1Ef";
   SimpleConfig::ToUpper(testString);
   EXPECT_EQ(testString, "ABC D.1EF");
   testString = SimpleConfig::ToUppered("abc d.1Ef");
   EXPECT_EQ(testString,"ABC D.1EF");
}

TEST(Str2Bool, Str2BoolWorks)
{
   std::string testString = "true";
   EXPECT_TRUE(SimpleConfig::Str2Bool(testString));
   EXPECT_TRUE(SimpleConfig::Str2Bool(testString.c_str()));

   testString = "True";
   EXPECT_TRUE(SimpleConfig::Str2Bool(testString));
   EXPECT_TRUE(SimpleConfig::Str2Bool(testString.c_str()));

   testString = "false";
   EXPECT_FALSE(SimpleConfig::Str2Bool(testString));
   EXPECT_FALSE(SimpleConfig::Str2Bool(testString.c_str()));

   testString = "FALSE";
   EXPECT_FALSE(SimpleConfig::Str2Bool(testString));
   EXPECT_FALSE(SimpleConfig::Str2Bool(testString.c_str()));

   testString = "1";
   EXPECT_TRUE(SimpleConfig::Str2Bool(testString));
   EXPECT_TRUE(SimpleConfig::Str2Bool(testString.c_str()));

   testString = "0";
   EXPECT_FALSE(SimpleConfig::Str2Bool(testString));
   EXPECT_FALSE(SimpleConfig::Str2Bool(testString.c_str()));
}

TEST(Str2Bool, UnexpectedForm)
{
   std::string testString = "abc";
   EXPECT_THROW(SimpleConfig::Str2Bool(testString), std::invalid_argument);
}

}

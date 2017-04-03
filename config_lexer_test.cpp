#include "config_lexer.h"
#include "gtest/gtest.h"
#include <sstream>
#include <stdexcept>

namespace
{


TEST(ScanTest, EOFTokenLexed)
{
    SimpleConfig::ConfigLexer l;
    std::istringstream testSource("");
    const std::vector<SimpleConfig::Token> testTokens = l.Scan(testSource);
    SimpleConfig::Token testToken= {SimpleConfig::END_OF_FILE, "", 0};
    EXPECT_TRUE(!testTokens.empty());
    EXPECT_EQ(testTokens.front().type, SimpleConfig::END_OF_FILE);
}

TEST(ScanTest, LeftBracketTokenLexed)
{
    SimpleConfig::ConfigLexer l;
    std::istringstream testSource("[");
    const std::vector<SimpleConfig::Token> testTokens = l.Scan(testSource);
    EXPECT_EQ(testTokens.front().type, SimpleConfig::LEFT_BRACKET);
}

TEST(ScanTest, RightBracketLexed)
{
    SimpleConfig::ConfigLexer l;
    std::istringstream testSource("]");
    const std::vector<SimpleConfig::Token> testTokens = l.Scan(testSource);
    EXPECT_EQ(testTokens.front().type, SimpleConfig::RIGHT_BRACKET);
}

TEST(ScanTest, EqualsLexed)
{
    SimpleConfig::ConfigLexer l;
    std::istringstream testSource("=");
    const std::vector<SimpleConfig::Token> testTokens = l.Scan(testSource);
    EXPECT_EQ(testTokens.front().type, SimpleConfig::EQUALS);
}

TEST(ScanTest, WhitespaceSkipped)
{
    SimpleConfig::ConfigLexer l;
    std::istringstream testSource("     ]     ");
    const std::vector<SimpleConfig::Token> testTokens = l.Scan(testSource);
    EXPECT_EQ(testTokens.front().type, SimpleConfig::RIGHT_BRACKET);
}

TEST(ScanTest, NewlinesCounted)
{
    SimpleConfig::ConfigLexer l;
    std::istringstream testSource("\n\n\n ]");
    const std::vector<SimpleConfig::Token> testTokens = l.Scan(testSource);
    EXPECT_EQ(testTokens.front().lineNum, 4);
}

TEST(ScanTest, StringLiteralLexingWorks)
{
    SimpleConfig::ConfigLexer l;
    std::istringstream testSource(" \"Hello World!\"");
    const std::vector<SimpleConfig::Token> testTokens = l.Scan(testSource);
    EXPECT_EQ(testTokens.front().type, SimpleConfig::STRING);
    EXPECT_EQ(testTokens.front().lexeme, "Hello World!");
    EXPECT_EQ(testTokens.front().lineNum, 1);
}

TEST(ScanTest, CommentLexingWorks)
{
    SimpleConfig::ConfigLexer l;
    std::istringstream testSource(" \"Hello World!\" #Comment here \n ]");
    const std::vector<SimpleConfig::Token> testTokens = l.Scan(testSource);
    EXPECT_EQ(testTokens.front().type, SimpleConfig::STRING);
    EXPECT_EQ(testTokens.front().lexeme, "Hello World!");
    EXPECT_EQ(testTokens.front().lineNum, 1);
    EXPECT_EQ(testTokens[1].type, SimpleConfig::RIGHT_BRACKET);
    EXPECT_EQ(testTokens[1].lexeme, "]");
    EXPECT_EQ(testTokens[1].lineNum, 2);
}

TEST(ScanTest, BoolLexingWorks)
{
    SimpleConfig::ConfigLexer l;
    std::istringstream testSource(" True FALSE"); 
    const std::vector<SimpleConfig::Token> testTokens = l.Scan(testSource);
    EXPECT_EQ(testTokens[0].type, SimpleConfig::BOOL);
    EXPECT_EQ(testTokens[0].lexeme, "True");
    EXPECT_EQ(testTokens[0].lineNum, 1);
    EXPECT_EQ(testTokens[1].type, SimpleConfig::BOOL);
    EXPECT_EQ(testTokens[1].lexeme, "FALSE");
    EXPECT_EQ(testTokens[1].lineNum, 1);
}

TEST(ScanTest, IntLexingWorks)
{
    SimpleConfig::ConfigLexer l;
    std::istringstream testSource(" -0x1234FFFF"); 
    const std::vector<SimpleConfig::Token> testTokens = l.Scan(testSource);
    EXPECT_EQ(testTokens[0].type, SimpleConfig::INTEGER);
    EXPECT_EQ(testTokens[0].lexeme, "-0x1234FFFF");
    EXPECT_EQ(testTokens[0].lineNum, 1);
}

TEST(ScanTest, RealNumberLexingWorks)
{
    SimpleConfig::ConfigLexer l;
    std::istringstream testSource(" -1432.4352e3 "); 
    const std::vector<SimpleConfig::Token> testTokens = l.Scan(testSource);
    EXPECT_EQ(testTokens[0].type, SimpleConfig::REAL_NUMBER);
    EXPECT_EQ(testTokens[0].lexeme, "-1432.4352e3");
    EXPECT_EQ(testTokens[0].lineNum, 1);
}

TEST(ScanTest, IdLexingWorks)
{
    SimpleConfig::ConfigLexer l;
    std::istringstream testSource(" test "); 
    const std::vector<SimpleConfig::Token> testTokens = l.Scan(testSource);
    EXPECT_EQ(testTokens[0].type, SimpleConfig::IDENTIFIER);
    EXPECT_EQ(testTokens[0].lexeme, "test");
    EXPECT_EQ(testTokens[0].lineNum, 1);
}

TEST(ScanTest, UnterminatedStringThrows)
{
    SimpleConfig::ConfigLexer l;
    std::istringstream testSource(" \"no termination "); 
    EXPECT_THROW(l.Scan(testSource), std::logic_error);
}

TEST(ScanTest, UnexpecetedCharThrows)
{
    SimpleConfig::ConfigLexer l;
    std::istringstream testSource(" $ "); 
    EXPECT_THROW(l.Scan(testSource), std::logic_error);
}

TEST(ScanTest, TokenSequenceMatchesExpected)
{
    SimpleConfig::ConfigLexer l;
    std::istringstream testSource(" test = true #comment\n\n test2 = 100 \n [Section] \n test4 = \"str\" \n "); 
    const std::vector<SimpleConfig::Token> testTokens = l.Scan(testSource);
    EXPECT_EQ(testTokens[0].type, SimpleConfig::IDENTIFIER);
    EXPECT_EQ(testTokens[0].lexeme, "test");
    EXPECT_EQ(testTokens[0].lineNum, 1);
    EXPECT_EQ(testTokens[1].type, SimpleConfig::EQUALS);
    EXPECT_EQ(testTokens[1].lexeme, "=");
    EXPECT_EQ(testTokens[1].lineNum, 1);
    EXPECT_EQ(testTokens[2].type, SimpleConfig::BOOL);
    EXPECT_EQ(testTokens[2].lexeme, "true");
    EXPECT_EQ(testTokens[2].lineNum, 1);
    EXPECT_EQ(testTokens[3].type, SimpleConfig::IDENTIFIER);
    EXPECT_EQ(testTokens[3].lexeme, "test2");
    EXPECT_EQ(testTokens[3].lineNum, 3);
    EXPECT_EQ(testTokens[4].type, SimpleConfig::EQUALS);
    EXPECT_EQ(testTokens[4].lexeme, "=");
    EXPECT_EQ(testTokens[4].lineNum, 3);
    EXPECT_EQ(testTokens[5].type, SimpleConfig::INTEGER);
    EXPECT_EQ(testTokens[5].lexeme, "100");
    EXPECT_EQ(testTokens[5].lineNum, 3);
    EXPECT_EQ(testTokens[7].type, SimpleConfig::IDENTIFIER);
    EXPECT_EQ(testTokens[7].lexeme, "Section");
    EXPECT_EQ(testTokens[7].lineNum, 4);
    EXPECT_EQ(testTokens[11].type, SimpleConfig::STRING);
    EXPECT_EQ(testTokens[11].lexeme, "str");
    EXPECT_EQ(testTokens[11].lineNum, 5);
}

}

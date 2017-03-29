#include "config_parser.h"
#include "gtest/gtest.h"

namespace {

   class ConfigParserTest : public ::testing::Test 
   {
   protected:

      ConfigParserTest() {}

      virtual void SetUp() {

      }

      //Objects here can be used by all tests
   };

   TEST_F(ConfigParserTest, LookupBoolTrue) {
      ConfigParser c;
      EXPECT_EQ(true, c.LookupBoolean("a"));
   }

}

int main(int argc, char **argv) {
   ::testing::InitGoogleTest(&argc, argv);
   return RUN_ALL_TESTS();
}



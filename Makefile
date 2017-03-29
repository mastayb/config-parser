
GTEST_DIR= ../gtest


# Flags passed to the preprocessor.
# Set Google Test's header directory as a system directory, such that
# the compiler doesn't generate warnings in Google Test headers.
CPPFLAGS += -isystem $(GTEST_DIR)/include -L $(GTEST_DIR)/lib -lgtest -lgtest_main

# Flags passed to the C++ compiler.
CXXFLAGS += -g -Wall -Wextra -pthread

TESTS = config_parsertest

# All Google Test headers.  Usually you shouldn't change this
# definition.
GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
                $(GTEST_DIR)/include/gtest/internal/*.h

all: $(TESTS)

clean:
	rm -f $(TESTS) *.o *.h.gch


config_parser.o: config_parser.cpp config_parser.h 
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c ConfigParser.cpp

config_parsertest.o : config_parsertest.cpp config_parser.h 
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c config_parsertest.cpp

config_parsertest: config_parser.o config_parsertest.o 
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@

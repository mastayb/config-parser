# A sample Makefile for building Google Test and using it in user
# tests.  Please tweak it to suit your environment and project.  You
# may want to move it to your project's root directory.
#
# SYNOPSIS:
#
#   make [all]  - makes everything.
#   make TARGET - makes the given target.
#   make clean  - removes all files generated by make.

# Please tweak the following variable definitions as needed by your
# project, except GTEST_HEADERS, which you can use in your own targets
# but shouldn't modify.

# Points to the root of Google Test, relative to where this file is.
# Remember to tweak this if you move this file.
GTEST_DIR = ../googletest/googletest

# Where to find user code.
USER_DIR = .

# Flags passed to the preprocessor.
# Set Google Test's header directory as a system directory, such that
# the compiler doesn't generate warnings in Google Test headers.
CPPFLAGS += -isystem $(GTEST_DIR)/include

# Flags passed to the C++ compiler.
CXXFLAGS += -g -Wall -Wextra -pthread

# All tests produced by this Makefile.  Remember to add new tests you
# created to the list.
TESTS = config_parser_test parse_utilities_test config_lexer_test all_config_tests 

# All Google Test headers.  Usually you shouldn't change this
# definition.
GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
                $(GTEST_DIR)/include/gtest/internal/*.h

# House-keeping build targets.

all : $(TESTS)

clean :
	rm -f $(TESTS) gtest.a gtest_main.a *.o

# Builds gtest.a and gtest_main.a.

# Usually you shouldn't tweak such internal variables, indicated by a
# trailing _.
GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)

# For simplicity and to avoid depending on Google Test's
# implementation details, the dependencies specified below are
# conservative and not optimized.  This is fine as Google Test
# compiles fast and for ordinary users its source rarely changes.
gtest-all.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest-all.cc

gtest_main.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest_main.cc

gtest.a : gtest-all.o
	$(AR) $(ARFLAGS) $@ $^

gtest_main.a : gtest-all.o gtest_main.o
	$(AR) $(ARFLAGS) $@ $^

# A test should link with either gtest.a or
# gtest_main.a, depending on whether it defines its own main()
# function.
#

parse_utilities.o: $(USER_DIR)/parse_utilities.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(USER_DIR)/parse_utilities.cpp

parse_utilities_test.o : $(USER_DIR)/parse_utilities_test.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(USER_DIR)/parse_utilities_test.cpp

parse_utilities_test : parse_utilities.o parse_utilities_test.o gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@

config_lexer.o : $(USER_DIR)/config_lexer.cpp 
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(USER_DIR)/config_lexer.cpp

config_lexer_test.o : $(USER_DIR)/config_lexer_test.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(USER_DIR)/config_lexer_test.cpp

config_parser.o : $(USER_DIR)/config_parser.cpp 
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(USER_DIR)/config_parser.cpp

config_parser_test.o : $(USER_DIR)/config_parser_test.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(USER_DIR)/config_parser_test.cpp

config_parser_test : config_parser.o parse_utilities.o config_lexer.o config_parser_test.o gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@

config_lexer_test : config_lexer.o parse_utilities.o config_lexer_test.o gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@

all_config_tests : config_parser.o parse_utilities.o config_lexer.o config_parser_test.o config_lexer_test.o parse_utilities_test.o gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@

maf_dmo_simulation_protocols.o : $(USER_DIR)/maf_dmo_simulation_protocols.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(USER_DIR)/maf_dmo_simulation_protocols.cpp

maf_dmo_simulation_protocols_test.o : $(USER_DIR)/maf_dmo_simulation_protocols_test.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(USER_DIR)/maf_dmo_simulation_protocols_test.cpp

maf_dmo_simulation_protocols_test : maf_dmo_simulation_protocols_test.o maf_dmo_simulation_protocols.o config_parser.o parse_utilities.o config_lexer.o gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@

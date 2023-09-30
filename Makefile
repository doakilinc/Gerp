###
### Makefile for Gerp Project
###
### Author:  Doga Kilinc & Cansu Birsen

MAKEFLAGS += -L 

CXX      = clang++
CXXFLAGS = -g3 -Wall -Wextra -Wpedantic -Wshadow
LDFLAGS  = -g3 

gerp: main.o MapBuilder.o DictionaryBuilder.o FSTree.o DirNode.o
	$(CXX) $(CXXFLAGS) -o gerp main.o MapBuilder.o DictionaryBuilder.o  \
							FSTree.o DirNode.o

# This rule builds MapBuilder.o
MapBuilder.o: MapBuilder.h MapBuilder.cpp
	$(CXX) $(CXXFLAGS) -c MapBuilder.cpp

# This rule builds DictionaryBuilder.o
DictionaryBuilder.o: DictionaryBuilder.h DictionaryBuilder.cpp MapBuilder.h
	$(CXX) $(CXXFLAGS) -c DictionaryBuilder.cpp

# This rule builds main.o
main.o: main.cpp MapBuilder.o DictionaryBuilder.o
	$(CXX) $(CXXFLAGS) -c main.cpp

# The below rule will be used by unit_test.
unit_test: unit_test_driver.o MapBuilder.o DictionaryBuilder.o FSTree.o  \
													DirNode.o
	$(CXX) $(CXXFLAGS) $^

# This is the rule for provide
provide:
	provide comp15  proj4_gerp MapBuilder.cpp MapBuilder.h \
					   DictionaryBuilder.cpp unit_tests.h \
					   DictionaryBuilder.h Makefile README \
					   main.cpp sampleFile.txt  \
					   tinyDataQuery.txt tinyDataQuery_ref.txt  \
						tinyData_ourSorted.txt tinyData_refSorted.txt \
						tinyData_newourSorted.txt tinyData_newrefSorted.txt \
						smallGutenbergQuery.txt smallGutenbergQuery_ref.txt \
				smallGutenberg_ourSorted.txt smallGutenberg_refSorted.txt \
			
				 
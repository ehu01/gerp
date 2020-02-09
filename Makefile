# 
# Makefile for gerp
#
# COMP15 Project 2 (Fall 2019)
#

CXX      = clang++
CXXFLAGS = -g3 -O2 -Wall -Wextra -std=c++11 


gerp: main.o HashTable.o StringNode.o FSTree.o DirNode.o
	${CXX} -o $@ $^

treeTraversal: FSTreeTraversal.o FSTree.o DirNode.o
	${CXX} -o $@ $^

stringProcessing: stringProcessing.o
	${CXX} -o $@ $^

%.o: %.cpp $(shell echo *.h)
	${CXX} ${CXXFLAGS} -c $<

clean:
	rm -rf gerp *.o *.dSYM

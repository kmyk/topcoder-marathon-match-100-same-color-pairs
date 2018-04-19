.PHONY: build run

PROBLEM := SameColorPairs
CXX := g++
CXXFLAGS := -std=c++11 -Wall -O2 -g -DLOCAL

build: a.out tester.jar
run: a.out tester.jar
	java -jar tester.jar -exec ./a.out

a.out: main.cpp ${PROBLEM}.cpp
	${CXX} ${CXXFLAGS} $<

tester.jar: ${PROBLEM}Vis.java
	-mkdir build
	javac -d build $<
	cd build && jar cvfe ../tester.jar ${PROBLEM}Vis *

URL := https://community.topcoder.com/longcontest/?module=ViewProblemStatement&rd=17143&pm=14889
submit:
	oj submit '${URL}' --language C++ ${PROBLEM}.cpp -y --open
submit/full:
	oj submit '${URL}' --language C++ ${PROBLEM}.cpp -y --open --full

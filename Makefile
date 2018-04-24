.PHONY: build run

PROBLEM := SameColorPairs
CXX := g++
CXXFLAGS := -std=c++11 -Wall -O2 -g -DLOCAL

build: a.out tester.jar
run: a.out tester.jar
	java -jar tester.jar -exec ./a.out

a.out: main.cpp ${PROBLEM}.cpp
	${CXX} ${CXXFLAGS} $<

tester.jar: ${PROBLEM}Vis.java 19d764e35decad6789722b32b58bb1bd/GifWriter.java
	-rm -rf build
	mkdir build
	cp $^ build
	cd build && javac $<
	cd build && jar cvfe ../tester.jar ${PROBLEM}Vis *

URL := https://community.topcoder.com/longcontest/?module=ViewProblemStatement&rd=17143&pm=14889
submit:
	oj submit '${URL}' --language C++ ${PROBLEM}.cpp -y --open
submit/full:
	oj submit '${URL}' --language C++ ${PROBLEM}.cpp -y --open --full

score.tsv: a.out tester.jar seed.tsv
	{ echo 'seed\tH\tW\tC\tremaining\tarea' ; for seed in $$(awk '$$4 == 6 { print $$1 }' seed.tsv | head -n 1000) ; do echo seed = $$seed ; time java -jar tester.jar -exec ./a.out -novis -seed $$seed | tee /dev/stderr | grep '[HWC] =\|Score =' ; done | sed 'N ; N ; N ; N ; s/\w* = //mg ; s/\n/\t/g ; s! / !\t!' ; } > score.tsv

visualizer.gif: a.out tester.jar
	# seed = 459 is a hard one
	java -jar tester.jar -exec ./a.out -delay 0 -size 10 -seed 459 -gif $@ -close
	# make 5x faster
	gifsicle --batch $@ -O3 --loopcount=forever --delay=1 $$(seq -f '#%g' 0 5 $$(gifsicle --info $@ | awk 'NR == 1 { print $$3 }')) --delay=400 '#-1'

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

SEED := 3 4 13 14 19 22 24 31 33 37 43 50 53 57 58 82 91 99 105 113 116 123 125 134 136 142 143 154 166 177 184 187 196 198 199 201 215 221 227 228 229 236 243 259 268 273 281 282 288 303 321 327 328 330 335 336 341 343 347 352 354 362 371 373 377 380 381 384 385 392 400 401 411 413 414 416 422 430 431 432 443 445 446 448 451 455 457 459 461 466 467 472 475 476 478 483 484 485 487 489 490 492 494 500 507 508 515 516 521 528 530 535 542 543 555 569 572 573 586 593 617 619 622 627 629 630 631 633 640 654 657 658 667 671 691 699 701 713 718 725 727 733 737 744 752 755 762 765 771 774 781 784 787 790 803 809 814 818 843 845 853 854 858 870 876 879 881 882 884 886 892 900 905 909 911 927 935 943 952 956 958 959 962 976 980 981 988 995 997 998 999
score.tsv: a.out tester.jar
	{ echo 'seed\tH\tW\tC\tremaining\tarea' ; for seed in ${SEED} ; do echo seed = $$seed ; time java -jar tester.jar -exec ./a.out -novis -seed $$seed | tee /dev/stderr | grep '[HWC] =\|Score =' ; done | sed 'N ; N ; N ; N ; s/\w* = //mg ; s/\n/\t/g ; s! / !\t!' ; } > score.tsv

visualizer.gif: a.out tester.jar
	# seed = 459 is a hard one
	java -jar tester.jar -exec ./a.out -delay 0 -size 10 -seed 459 -gif $@ -close
	# make 5x faster
	gifsicle --batch $@ -O3 --loopcount=forever --delay=1 $$(seq -f '#%g' 0 5 $$(gifsicle --info $@ | awk 'NR == 1 { print $$3 }')) --delay=400 '#-1'

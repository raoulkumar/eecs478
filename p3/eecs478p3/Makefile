####  Makefile 	####
# Usage:           #
#   >make          #
#   >make clean	   #
####################

FILES =  truthTable.cpp circuit.cpp main.cpp
SIM = truthTable.cpp circuit.cpp simulation.cpp

all: 
	g++ -O3 -Wall -lm -g -o project3 ${FILES}
	g++ -O3 -Wall -lm -g -o sim $(SIM)

backup:
	cp -rf *.c *.h Makefile backup/*

clean:
	rm -f project3 *~

all: OneginSorter
	./OneginSorter evgeniy_onegin.txt

OneginSorter: LineSorter.o main.o
	g++ -std=c++11 *.o -o OneginSorter

LineSorter.o: LineSorter.cpp LineSorter.h
	g++ -std=c++11 -c LineSorter.cpp

main.o: main.cpp
	g++ -std=c++11 -c main.cpp

clean:
	rm -f *.o OneginSorter evgeniy_onegin_sorted.txt
CXX = g++
CXXFLAGS = -Wall

driver: mytest.cpp pqueue.o
	$(CXX) $(CXXFLAGS) mytest.cpp pqueue.o -o driver

pqueue.o: pqueue.h pqueue.cpp
	$(CXX) $(CXXFLAGS) -c pqueue.cpp

run:
	./driver

val:
	valgrind ./driver

debug:
	gdb gdbarg1 --args ./driver


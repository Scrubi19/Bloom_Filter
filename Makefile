filter: build/filter.o
	g++ -o filter build/filter.o

build/filter.o:
	g++ -o build/filter.o -c  src/filter.cpp 

clean:
	rm -f *.o filter
	rm -f ./build/*.o
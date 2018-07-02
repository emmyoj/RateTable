DEBUG	= -O3

CPPFLAGS	= $(DEBUG) -Wall  -Winline -pipe

motor: mainEncoder.o encoder.o pid.o
	g++ mainEncoder.o encoder.o pid.o  -o motor -lwiringPi -lpthread
	
mainEncoder.o: mainEncoder.cpp encoder.h pid.h
	g++ -c $(CPPFLAGS) mainEncoder.cpp -o mainEncoder.o -lwiringPi -lpthread
encoder.o: encoder.h encoder.cpp 
	g++ -c $(CPPFLAGS) encoder.cpp -o encoder.o -lwiringPi -lpthread
pid.o: pid.cpp pid.h
	g++ -c $(CPPFLAGS) pid.cpp -o pid.o -lwiringPi -lpthread
clean:
	rm -rf encoder.o mainEncoder.o motor pid.o

DEBUG	= -O3

CPPFLAGS	= $(DEBUG) -Wall  -Winline -pipe
all: speed angle

speed: mainEncoderSpeed.o encoder.o pid.o
	g++ mainEncoderSpeed.o encoder.o pid.o  -o speed -lwiringPi -lpthread

angle: mainEncoderAngle.o encoder.o pid.o
	g++ mainEncoderAngle.o encoder.o pid.o  -o angle -lwiringPi -lpthread
	
mainEncoderSpeed.o: mainEncoderSpeed.cpp encoder.h pid.h
	g++ -c $(CPPFLAGS) mainEncoderSpeed.cpp -o mainEncoderSpeed.o -lwiringPi -lpthread

mainEncoder.o: mainEncoderAngle.cpp encoder.h pid.h
	g++ -c $(CPPFLAGS) mainEncoderAngle.cpp -o mainEncoderAngle.o -lwiringPi -lpthread
encoder.o: encoder.h encoder.cpp 
	g++ -c $(CPPFLAGS) encoder.cpp -o encoder.o -lwiringPi -lpthread
pid.o: pid.cpp pid.h
	g++ -c $(CPPFLAGS) pid.cpp -o pid.o -lwiringPi -lpthread
clean:
	rm -rf encoder.o mainEncoderSpeed.o mainEncoderAngle.o speed angle pid.o

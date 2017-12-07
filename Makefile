

all: RushHour

RushHour: RushHour.o 
	g++ -o RushHour RushHour.o 

clean:
	rm -f RushHour; rm -f RushHour.o
	
RushHour.o: RushHour.cpp 

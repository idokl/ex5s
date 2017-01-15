all: server.out client.out

server.out: Server.o LuxuryCab.o ProgramFlow.o StandardCab.o TaxiCenter.o src/BfsAlgorithm.h src/Graph.h src/Node.h Grid.o Point.o InputParsing.o Trip.o Passenger.o Driver.o Cab.o CabFactory.o Socket.o Udp.o 
	g++ -g -std=c++0x Server.o LuxuryCab.o ProgramFlow.o StandardCab.o TaxiCenter.o src/BfsAlgorithm.h src/Graph.h src/Node.h Grid.o Point.o InputParsing.o Trip.o Passenger.o Cab.o CabFactory.o Driver.o Socket.o Udp.o -lboost_serialization -o server.out
	
client.out: Client.o LuxuryCab.o ProgramFlow.o StandardCab.o TaxiCenter.o src/BfsAlgorithm.h src/Graph.h src/Node.h  Grid.o Point.o InputParsing.o Trip.o Passenger.o Driver.o Cab.o CabFactory.o Socket.o Udp.o
	g++ -g -std=c++0x Client.o LuxuryCab.o ProgramFlow.o StandardCab.o TaxiCenter.o src/BfsAlgorithm.h src/Graph.h src/Node.h Grid.o Point.o InputParsing.o Trip.o Passenger.o Cab.o CabFactory.o Socket.o Udp.o Driver.o -lboost_serialization -o client.out

CabFactory.o: src/CabFactory.cpp src/CabFactory.h
	g++ -g -c -std=c++0x src/CabFactory.cpp	

Passenger.o: src/Grid.cpp src/Passenger.h
	g++ -g -c -std=c++0x src/Passenger.cpp

Driver.o: src/Driver.cpp src/Driver.h
	g++ -g -c -std=c++0x src/Driver.cpp

InputParsing.o: src/InputParsing.cpp src/InputParsing.h
	g++ -g -c -std=c++0x src/InputParsing.cpp

LuxuryCab.o: src/LuxuryCab.cpp src/LuxuryCab.h
	g++ -g -c -std=c++0x src/LuxuryCab.cpp

ProgramFlow.o: src/ProgramFlow.cpp src/ProgramFlow.h
	g++ -g -c -std=c++0x src/ProgramFlow.cpp

StandardCab.o: src/StandardCab.cpp src/StandardCab.h
	g++ -g -c -std=c++0x src/StandardCab.cpp

TaxiCenter.o: src/TaxiCenter.cpp src/TaxiCenter.h
	g++ -g -c -std=c++0x src/TaxiCenter.cpp

Trip.o: src/Trip.cpp src/Trip.h
	g++ -g -c -std=c++0x src/Trip.cpp

Grid.o: src/Grid.cpp src/Grid.h
	g++ -g -c -std=c++0x src/Grid.cpp

Cab.o: src/Cab.cpp src/Cab.h
	g++ -g -c -std=c++0x src/Cab.cpp

Point.o: src/Point.cpp src/Point.h
	g++ -g -c -std=c++0x src/Point.cpp

Server.o: src/Server.cpp
	g++ -g -c -std=c++0x src/Server.cpp
	
Client.o: src/Client.cpp
	g++ -g -c -std=c++0x src/Client.cpp
	
Socket.o: src/Socket.cpp src/Socket.h
	g++ -g -c -std=c++0x src/Socket.cpp	
	
Udp.o: src/Udp.cpp src/Udp.h
	g++ -g -c -std=c++0x src/Udp.cpp
	
clean:
	rm -f *.o server.out client.out
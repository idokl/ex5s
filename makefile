all: server.out client.out

server.out: Server.o LuxuryCab.o ProgramFlow.o StandardCab.o TaxiCenter.o src/BfsAlgorithm.h src/Graph.h src/Node.h Grid.o Point.o InputParsing.o Trip.o Passenger.o Driver.o Cab.o CabFactory.o Socket.o Tcp.o src/easylogging++.h
	g++ -g Server.o LuxuryCab.o ProgramFlow.o StandardCab.o TaxiCenter.o src/BfsAlgorithm.h src/Graph.h src/Node.h Grid.o Point.o InputParsing.o Trip.o Passenger.o Cab.o CabFactory.o Driver.o Socket.o Tcp.o src/easylogging++.h -pthread -lboost_serialization -std=c++0x -D _DISABLE_LOGS -o server.out
	
client.out: Client.o LuxuryCab.o ProgramFlow.o StandardCab.o TaxiCenter.o src/BfsAlgorithm.h src/Graph.h src/Node.h  Grid.o Point.o InputParsing.o Trip.o Passenger.o Driver.o Cab.o CabFactory.o Socket.o Tcp.o src/easylogging++.h
	g++ -g Client.o LuxuryCab.o ProgramFlow.o StandardCab.o TaxiCenter.o src/BfsAlgorithm.h src/Graph.h src/Node.h Grid.o Point.o InputParsing.o Trip.o Passenger.o Cab.o CabFactory.o Socket.o Tcp.o Driver.o src/easylogging++.h -pthread -lboost_serialization -std=c++0x -D _DISABLE_LOGS -o client.out

CabFactory.o: src/CabFactory.cpp src/CabFactory.h
	g++ -g -c -std=c++0x -D _DISABLE_LOGS src/CabFactory.cpp	

Passenger.o: src/Grid.cpp src/Passenger.h
	g++ -g -c -std=c++0x -D _DISABLE_LOGS src/Passenger.cpp

Driver.o: src/Driver.cpp src/Driver.h
	g++ -g -c -std=c++0x -D _DISABLE_LOGS src/Driver.cpp

InputParsing.o: src/InputParsing.cpp src/InputParsing.h
	g++ -g -c -std=c++0x -D _DISABLE_LOGS src/InputParsing.cpp

LuxuryCab.o: src/LuxuryCab.cpp src/LuxuryCab.h
	g++ -g -c -std=c++0x -D _DISABLE_LOGS src/LuxuryCab.cpp

ProgramFlow.o: src/ProgramFlow.cpp src/ProgramFlow.h
	g++ -g -c -std=c++0x -D _DISABLE_LOGS src/ProgramFlow.cpp

StandardCab.o: src/StandardCab.cpp src/StandardCab.h
	g++ -g -c -std=c++0x -D _DISABLE_LOGS src/StandardCab.cpp

TaxiCenter.o: src/TaxiCenter.cpp src/TaxiCenter.h
	g++ -g -c -std=c++0x -D _DISABLE_LOGS src/TaxiCenter.cpp

Trip.o: src/Trip.cpp src/Trip.h
	g++ -g -c -std=c++0x -D _DISABLE_LOGS src/Trip.cpp

Grid.o: src/Grid.cpp src/Grid.h
	g++ -g -c -std=c++0x -D _DISABLE_LOGS src/Grid.cpp

Cab.o: src/Cab.cpp src/Cab.h
	g++ -g -c -std=c++0x -D _DISABLE_LOGS src/Cab.cpp

Point.o: src/Point.cpp src/Point.h
	g++ -g -c -std=c++0x -D _DISABLE_LOGS src/Point.cpp

Server.o: src/Server.cpp
	g++ -g -c -std=c++0x -D _DISABLE_LOGS src/Server.cpp
	
Client.o: src/Client.cpp
	g++ -g -c -std=c++0x -D _DISABLE_LOGS src/Client.cpp
	
Socket.o: src/Socket.cpp src/Socket.h
	g++ -g -c -std=c++0x -D _DISABLE_LOGS src/Socket.cpp	
	
Tcp.o: src/Tcp.cpp src/Tcp.h
	g++ -g -c -std=c++0x -D _DISABLE_LOGS src/Tcp.cpp
	
clean:
	rm -f *.o server.out client.out
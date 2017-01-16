#include "ProgramFlow.h"



using namespace std;

int globalX=0;

TaxiCenter ProgramFlow::createTaxiCenter(BfsAlgorithm<Point> bfs) {
    return TaxiCenter(bfs);
}

Graph<Point> *ProgramFlow::createGrid(int width, int height, vector<Point> listOfObstacles) {
    Graph<Point> *g = new Grid(width, height, listOfObstacles);
    return g;
}


void *ProgramFlow::threadsRun(void* threadStruct){
    int runOnce =0;
    string inputString;
    threadData *threadData = (struct threadData*)threadStruct;
    int socketDescriptor = threadData->socketDescriptor;
    Socket *socket = threadData->socket;
    TaxiCenter *taxiCenter = threadData->taxiCenter;
    char buffer[1024];
    while(true) {
        if(runOnce==0) {
            switch (globalX) {
                case 1:{
                    socket->reciveData(buffer, sizeof(buffer), socketDescriptor);
                    string driverIdString = string(buffer);
                    int driverId = stoi(driverIdString);
                    //send taxi data
                    string dataOfCabOfDriver = taxiCenter->getCabString(driverId);
                    socket->sendData(dataOfCabOfDriver, socketDescriptor);
                }
                case 4: {
                    getline(cin, inputString);
                    try {
                        // here we have to add (in ex5) the command: find the socket of the
                        // corresponding driver
                        socket->sendData("4", socketDescriptor);
                        socket->reciveData(buffer, sizeof(buffer), socketDescriptor);
                        Point driverLocation;
                        string locationStr(buffer, sizeof(buffer));
                        SerializationClass<Point> serializeClass;
                        driverLocation =
                                serializeClass.deSerializationObject(locationStr, driverLocation);
                        //print driver location
                        cout << driverLocation << '\n';
                    } catch (const char *msg) {
                        cerr << msg << endl;
                    }
                    runOnce = 1;
                    break;
                }
                case 7: {
                    exit(0);
                }
                default:
                    break;

            }
        }
    }
}





void ProgramFlow::run(Socket *mainSocket) {
    string inputString;
    //get the grid dimensions
    getline(cin, inputString);
    InputParsing inputParsing = InputParsing();
    InputParsing::gridDimensions gd = inputParsing.parseGridDimensions(inputString);
    //get number of obstacles
    getline(cin, inputString);
    int numOfObstacles;
    numOfObstacles = stoi(inputString);
    vector<Point> listOfObstacles = vector<Point>();
    //if there are any obstacles, get their locations.
    if (numOfObstacles > 0) {
        for (int i = 0; i < numOfObstacles; i++) {
            getline(cin, inputString);
            Point p(inputParsing.parsePoint(inputString));
            listOfObstacles.push_back(p);
        }
    }
    //create the grid and the taxi center
    Graph<Point> *grid = ProgramFlow::createGrid(gd.gridWidth, gd.gridHeight, listOfObstacles);
    BfsAlgorithm<Point> bfs(grid);
    TaxiCenter taxiCenter = ProgramFlow::createTaxiCenter(bfs);
    Cab *cabForDriver = NULL;
    int expectedNumberOfDrivers = 0;
    int timer = 0;
    threadData * threadData = new struct threadData;
    while (true) {
        //get number of option and do the defined operation
        getline(cin, inputString);
        switch (stoi(inputString)) {
            case 1: {
                //create a driver (according to the given parameters) and add it to the taxi center
                getline(cin, inputString);
                expectedNumberOfDrivers = stoi(inputString);
                //(in ex5 this condition has to be deleted)
                if (expectedNumberOfDrivers == 0) {
                    break;
                }else{
                    for(unsigned int i=0; i<expectedNumberOfDrivers; i++){
                        int descriptor = ProgramFlow::acceptConnection(mainSocket);
                        globalX =1;
                        
                        threadData->socket = mainSocket;
                        threadData->socketDescriptor = descriptor;
                        threadData->taxiCenter = &taxiCenter;
                        pthread_t pthread;
                        pthread_create(&pthread, NULL, ProgramFlow::threadsRun, threadData);
                   }
                }
                break;
            }
            case 2: {
                //create a trip (according to the given parameters) and add it to the taxi center
                getline(cin, inputString);
                InputParsing::parsedTripData trip = inputParsing.parseTripData(inputString);
                taxiCenter.createTrip(trip);
                break;
            }
            case 3: {
                //create a cab (according to the given parameters) and add it to the taxi center
                //save string of parameters in the taxi center in order to pass it to the
                //corresponding driver afterwards.
                getline(cin, inputString);
                cabForDriver = CabFactory::createCab(inputString);
                taxiCenter.addCab(cabForDriver);
                taxiCenter.addCabString(cabForDriver->getId(), inputString);
                break;
            }
            case 4: {
                globalX = 4;
                //query about the location of a specific driver
                getline(cin, inputString);
                try {
                    // here we have to add (in ex5) the command: find the socket of the
                    // corresponding driver
                    //socket->sendData("4");
                    char buffer[1024];
                   // socket->reciveData(buffer, sizeof(buffer));
                    Point driverLocation;
                    string locationStr(buffer, sizeof(buffer));
                    SerializationClass<Point> serializeClass;
                    driverLocation =
                            serializeClass.deSerializationObject(locationStr, driverLocation);
                    //print driver location
                    cout << driverLocation << '\n';
                } catch (const char *msg) {
                    cerr << msg << endl;
                }
                break;
            }
            case 7: {
                //ask the client to shutdown itself
               // socket->sendData("7");
                //deallocate memory and terminate the program
                delete grid;
                exit(0);
            }
            case 9: {
                timer++;
                // flag that indicate whether there was assigning trip in this time
                // (for preventing assigning of trip and movement in the same time)
                int assignFlag = 0;
                //assign trip to the driver if now is the starting time of the trip
                for (unsigned int i = 0; i < taxiCenter.getListOfTrips().size(); i++) {
                    if (taxiCenter.getListOfTrips().at(i)->getTime() == timer) {
                        //option 10 (of driver): assign a trip to the driver
                      //  socket->sendData("10");
                        SerializationClass<Trip *> serializeClass;
                        string serializedTrip = serializeClass.serializationObject(
                                taxiCenter.getListOfTrips().at(i));
                        //socket->sendData(serializedTrip);
                        delete taxiCenter.getListOfTrips().at(i);
                        taxiCenter.deleteTrip(i);
                        assignFlag = 1;
                        break;
                    }
                }
                if (assignFlag == 0) {
                    //sending 9 in order to advance the driver one step
                   // socket->sendData("9");
                }
                break;
            }
            default:
                break;
        }
    }
}

int ProgramFlow::acceptConnection(Socket *socket) {
        struct sockaddr_in client_sin;
        unsigned int addr_len;
        addr_len = sizeof(client_sin);
        cout << "accept" << endl;
        int descriptor = accept(socket->getSocketDescriptor(),
                                (struct sockaddr *) &client_sin, &addr_len);
        return descriptor;
}

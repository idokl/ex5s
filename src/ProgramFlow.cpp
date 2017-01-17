#define debugMassagesProgramFlow

#include "ProgramFlow.h"

using namespace std;

int globalX=0;
int runOnce =0;
pthread_mutex_t listOfTripsMutex;
pthread_mutex_t driverLocationsMapMutex;


TaxiCenter ProgramFlow::createTaxiCenter(BfsAlgorithm<Point> bfs) {
    return TaxiCenter(bfs);
}

Graph<Point> *ProgramFlow::createGrid(int width, int height, vector<Point> listOfObstacles) {
    Graph<Point> *g = new Grid(width, height, listOfObstacles);
    return g;
}


void *ProgramFlow::threadsRun(void* threadStruct) {
    int timeOfTheLastAction = -1;
    string inputString;
    threadData *threadData = (struct threadData *) threadStruct;
    int socketDescriptor = threadData->socketDescriptor;
    Socket *socket = threadData->socket;
    TaxiCenter *taxiCenter = threadData->taxiCenter;

    char buffer[8192];

    socket->reciveData(buffer, sizeof(buffer), socketDescriptor);
    string driverIdString = string(buffer);
    threadData->id = stoi(driverIdString);
    //send taxi data
    string dataOfCabOfDriver = taxiCenter->getCabString(threadData->id);
    socket->sendData(dataOfCabOfDriver, socketDescriptor);
    runOnce = 1;

    while (true) {
        switch (globalX) {
/*
            case 1: {
                if (runOnce == 1) {
                    break;
                }
                if (globalX != 1) {
                    break;
                }
                socket->reciveData(buffer, sizeof(buffer), socketDescriptor);
                string driverIdString = string(buffer);
                threadData->id = stoi(driverIdString);
                //send taxi data
                string dataOfCabOfDriver = taxiCenter->getCabString(threadData->id);
                socket->sendData(dataOfCabOfDriver, socketDescriptor);
                runOnce = 1;
                break;
            }
*/
            case 7: {
                exit(0);
            }
            case 9: {
                //if (runOnce == 1) {
                if (timeOfTheLastAction == taxiCenter->getTimer()) {
                    break;
                }
                // flag that indicate whether there was assigning trip in this time
                // (for preventing assigning of trip and movement in the same time)
                int assignFlag = 0;
                //assign trip to the driver if now is the starting time of the trip
                pthread_mutex_lock(&listOfTripsMutex);
                for (unsigned int i = 0; i < taxiCenter->getListOfTrips().size(); i++) {
                    if (taxiCenter->getListOfTrips().at(i)->getTime() ==
                        taxiCenter->getTimer()) {
                        if (taxiCenter->getDriverLocation(threadData->id) ==
                            taxiCenter->getListOfTrips().at(i)->getStartingPoint()) {
                            //option 10 (of driver): assign a trip to the driver
                            socket->sendData("10", socketDescriptor);
                            SerializationClass<Trip *> serializeClass;
                            string serializedTrip = serializeClass.serializationObject(
                                    taxiCenter->getListOfTrips().at(i));
                            socket->sendData(serializedTrip, socketDescriptor);
                            delete taxiCenter->getListOfTrips().at(i);
                            taxiCenter->deleteTrip(i);
#ifdef debugMassagesProgramFlow
                            cout << "secondary thread: (socket descriptor: " << socketDescriptor << "): trip was sent to the client" << endl;
#endif
                            assignFlag = 1;
                            break;
                        }
                    }
                }
                pthread_mutex_unlock(&listOfTripsMutex);
                if (assignFlag == 0) {
                    //sending 9 in order to advance the driver one step
                    socket->sendData("9", socketDescriptor);
                    socket->reciveData(buffer, sizeof(buffer), socketDescriptor);
                    string locationStr(buffer, sizeof(buffer));
                    Point driverLocation;
                    SerializationClass<Point> serializeClass;
                    driverLocation =
                            serializeClass.deSerializationObject(locationStr, driverLocation);
                    pthread_mutex_lock(&driverLocationsMapMutex);
#ifdef debugMassagesProgramFlow
                    cout << "secondary thread: (socket descriptor: " << socketDescriptor << "): 9 was sent to the client" << endl;
                    cout << "secondary thread: (socket descriptor: " << socketDescriptor << "): the point " << driverLocation << " was received from the client" << endl;
#endif
                    taxiCenter->addDriverLocation(threadData->id, driverLocation);
                    pthread_mutex_unlock(&driverLocationsMapMutex);
                }
                //runOnce = 1;
                timeOfTheLastAction = taxiCenter->getTimer();
                break;
            }
            default:
                break;

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
    //int timer = 0;
    threadData * threadData = new struct threadData;
    while (true) {
        //get number of option and do the defined operation
        getline(cin, inputString);
        switch (stoi(inputString)) {
            case 1: {
#ifdef debugMassagesProgramFlow
                cout << "main thread: case1 begin" << endl;
#endif
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
                        pthread_mutex_init(&listOfTripsMutex, 0);
                        pthread_t pthread;
                        pthread_create(&pthread, NULL, ProgramFlow::threadsRun, threadData);
                   }
                }
#ifdef debugMassagesProgramFlow
                cout << "main thread: case1 end" << endl;
#endif
                break;
            }
            case 2: {
#ifdef debugMassagesProgramFlow
                cout << "main thread: case2 begin" << endl;
#endif
                //create a trip (according to the given parameters) and add it to the taxi center
                getline(cin, inputString);
                InputParsing::parsedTripData trip = inputParsing.parseTripData(inputString);
                taxiCenter.createTrip(trip);
#ifdef debugMassagesProgramFlow
                cout << "main thread: case2 end" << endl;
#endif
                break;
            }
            case 3: {
#ifdef debugMassagesProgramFlow
                cout << "main thread: case3 begin" << endl;
#endif
                //create a cab (according to the given parameters) and add it to the taxi center
                //save string of parameters in the taxi center in order to pass it to the
                //corresponding driver afterwards.
                getline(cin, inputString);
                cabForDriver = CabFactory::createCab(inputString);
                taxiCenter.addCab(cabForDriver);
                taxiCenter.addCabString(cabForDriver->getId(), inputString);
#ifdef debugMassagesProgramFlow
                cout << "main thread: case3 end" << endl;
#endif
                break;
            }
            case 4: {
#ifdef debugMassagesProgramFlow
                cout << "main thread: case4 begin" << endl;
#endif
                //query about the location of a specific driver
                getline(cin, inputString);
                int id = stoi(inputString);
                pthread_mutex_lock(&driverLocationsMapMutex);
                Point location = taxiCenter.getDriverLocation(id);
                pthread_mutex_unlock(&driverLocationsMapMutex);
                cout << location << endl;
#ifdef debugMassagesProgramFlow
                cout << "main thread: case4 end" << endl;
#endif
                break;
            }
            case 7: {
#ifdef debugMassagesProgramFlow
                cout << "main thread: case7" << endl;
#endif
                //deallocate memory and terminate the program
                delete grid;
                exit(0);
            }
            case 9: {
#ifdef debugMassagesProgramFlow
                cout << "main thread: case9 begin. current time: " << taxiCenter.getTimer() << endl;
#endif
                taxiCenter.setTimer();
                globalX = 9;
                runOnce=0;
#ifdef debugMassagesProgramFlow
                cout << "main thread: case9 end" << endl;
#endif
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

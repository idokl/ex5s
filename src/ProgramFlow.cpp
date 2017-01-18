#define debugMassagesProgramFlow

#include "ProgramFlow.h"

using namespace std;

int globalX=0;
int circleFinish;
pthread_mutex_t listOfTripsMutex;
pthread_mutex_t driverLocationsMapMutex;
pthread_mutex_t circleFinishMutex;
pthread_mutex_t timerMutex;


TaxiCenter ProgramFlow::createTaxiCenter(BfsAlgorithm<Point> bfs) {
    return TaxiCenter(bfs);
}

Graph<Point> *ProgramFlow::createGrid(int width, int height, vector<Point> listOfObstacles) {
    Graph<Point> *g = new Grid(width, height, listOfObstacles);
    return g;
}


void *ProgramFlow::threadsRun(void* threadStruct) {
    bool loopCondition = true;
    string reciveNotification;
    int timeOfTheLastAction = -1;
    string inputString;
    threadData *threadData = (struct threadData *) threadStruct;
    int socketDescriptor = threadData->socketDescriptor;
    int id = threadData->id;
    Socket *socket = threadData->socket;
    TaxiCenter *taxiCenter = threadData->taxiCenter;

    char buffer[100000];

    while (loopCondition) {
            //circleFinish=0;
            switch (globalX) {
                case 7: {
                    delete threadData;
                    loopCondition = false;
                }
                case 9: {
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
                                memset(buffer, 0, sizeof(buffer));
                                socket->reciveData(buffer, sizeof(buffer), socketDescriptor); //recive "thanks to Nevo"
                                SerializationClass<Trip *> serializeClass;
                                string serializedTrip = serializeClass.serializationObject(
                                        taxiCenter->getListOfTrips().at(i));
                                socket->sendData(serializedTrip, socketDescriptor);
                                memset(buffer, 0, sizeof(buffer));
                                socket->reciveData(buffer, sizeof(buffer), socketDescriptor); //recive "thanks to Nevo"
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
                        memset(buffer, 0, sizeof(buffer));
                        socket->reciveData(buffer, sizeof(buffer), socketDescriptor);
                        string locationStr(buffer, sizeof(buffer));
                        Point driverLocation;
                        SerializationClass<Point> serializeClass;
                        driverLocation =
                                serializeClass.deSerializationObject(locationStr, driverLocation);
                        pthread_mutex_lock(&driverLocationsMapMutex);
                        taxiCenter->addDriverLocation(threadData->id, driverLocation);
                        pthread_mutex_unlock(&driverLocationsMapMutex);
#ifdef debugMassagesProgramFlow
                        cout << "secondary thread: (socket descriptor: " << socketDescriptor << "): 9 was sent to the client" << endl;
                        cout << "secondary thread: (socket descriptor: " << socketDescriptor << "): the point " << driverLocation << " was received from the client" << endl;
#endif
                    }
                    timeOfTheLastAction = taxiCenter->getTimer();
                    pthread_mutex_lock(&circleFinishMutex);
                    circleFinish--;
#ifdef debugMassagesProgramFlow
                    cout << "secondary thread: (socket descriptor: " << socketDescriptor << "): I did: circleFinish--. now circleFinish = " << circleFinish << endl;
#endif
                    pthread_mutex_unlock(&circleFinishMutex);

                    break;
                }
                default:
                    break;

        }
    }
}






void ProgramFlow::run(Socket *mainSocket) {
    string reciveNotification;

    char buffer[100000];

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
    //list<pthread_t> listOfThreads();
/**/
    vector<pthread_t> pthreadVector = vector<pthread_t>();
    pthread_mutex_init(&listOfTripsMutex, 0);
    pthread_mutex_init(&driverLocationsMapMutex, 0);
    pthread_mutex_init(&circleFinishMutex, 0);
    pthread_mutex_init(&timerMutex, 0);
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

    while (true) {
        //get number of option and do the defined operation
        getline(cin, inputString);
#ifdef debugMassagesProgramFlow
        cout << inputString << endl;
#endif
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
                } else {
//                    circleFinish = expectedNumberOfDrivers;
                    for (unsigned int i = 0; i < expectedNumberOfDrivers; i++) {
#ifdef debugMassagesProgramFlow
                        cout << "main thread: case1 before accept" << endl;
#endif
                        int descriptor = ProgramFlow::acceptConnection(mainSocket);
#ifdef debugMassagesProgramFlow
                        cout << "main thread: case1 after accept" << endl;
#endif
                        memset(buffer, 0, sizeof(buffer));
                        mainSocket->reciveData(buffer, sizeof(buffer), descriptor);
#ifdef debugMassagesProgramFlow
                        cout << "main thread: case1 recived driverId" << endl;
#endif
                        threadData * threadDataStruct = new struct threadData;
                        string driverIdString = string(buffer);
                        threadDataStruct->id = stoi(driverIdString);
                        //send taxi data
#ifdef debugMassagesProgramFlow
                        cout << "main thread: case1 before checking and sending cabString" << endl;
#endif

                        //vector<threadData*> *listOfStructs = new vector<threadData*>();
                        string dataOfCabOfDriver = taxiCenter.getCabString(threadDataStruct->id);
//                        mainSocket->reciveData(buffer, sizeof(buffer), descriptor);
                        mainSocket->sendData(dataOfCabOfDriver, descriptor);
                        memset(buffer, 0, sizeof(buffer));
/**/                    threadDataStruct->socket = mainSocket;
                        threadDataStruct->socketDescriptor = descriptor;
                        threadDataStruct->taxiCenter = &taxiCenter;
                        //listOfStructs->push_back(threadDataStruct);
                        pthread_t pthread;

                        mainSocket->reciveData(buffer, sizeof(buffer), descriptor); //recive "thanks to Nevo"
                        //pthread_t pthread;
                        pthread_create(&pthread, NULL, ProgramFlow::threadsRun, threadDataStruct);
                        pthreadVector.push_back(pthread);

#ifdef debugMassagesProgramFlow
                        cout << "main thread: case1 - END OF ITERATION OF THE LOOP" << endl;
#endif
                    }
//                    for (unsigned long i = 0; i <expectedNumberOfDrivers; i++){
//                        pthread_t pthread;
//                        pthread_create(&pthread, NULL, ProgramFlow::threadsRun, listOfStructs.at(i));
//                    }
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
                //pthread_mutex_lock(&driverLocationsMapMutex);
                Point location = taxiCenter.getDriverLocation(id);
                //pthread_mutex_unlock(&driverLocationsMapMutex);
                cout << location << endl;
#ifdef debugMassagesProgramFlow
                cout << "main thread: case4 end" << endl;
#endif
                break;
            }
            case 7: {


                for(unsigned long i = 0; i<pthreadVector.size(); i++){
                    pthread_join(pthreadVector.at(i), NULL);
                }
                //pthread_join(pthread, NULL);
                sleep(1);
#ifdef debugMassagesProgramFlow
                cout << "main thread: case7" << endl;
#endif
                //deallocate memory and terminate the program
                delete grid;
                exit(0);
            }
            case 9: {
                circleFinish = expectedNumberOfDrivers;

                pthread_mutex_lock(&listOfTripsMutex);
                taxiCenter.setTimer();
                pthread_mutex_unlock(&listOfTripsMutex);
#ifdef debugMassagesProgramFlow
                cout << "main thread: case9 begin. current time: " << taxiCenter.getTimer() << endl;
#endif
                circleFinish = expectedNumberOfDrivers;
                globalX = 9;

                do {
                } while (circleFinish !=0);

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
        int descriptor = accept(socket->getSocketDescriptor(),
                                (struct sockaddr *) &client_sin, &addr_len);
        cout << "accept descriptor number" << descriptor << endl;

    return descriptor;
}

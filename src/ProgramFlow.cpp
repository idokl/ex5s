#include "ProgramFlow.h"

using namespace std;

//variable that helps to the main thread to pass signals to the other threads
int globalX = 0;
//counter that helps  to the main thread to check all the threads finished their action
int circleFinish;
//mutexes:
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

/*
 * This function is executed by a secondary thread that was created by the main thread
 * in the function run().
 * It is responsible to the communication with one client (the driver).
 * The function maintains the client details (number of descriptor, id of driver).
 * It recives data about the input of the program by the variable "globalX"
 * that is updated by the main thread.
 * The function execute the corresponding actions of communication with the driver.
 * (The thread also uses the taxiCenter object to get additional data about its driver).
 */
void *ProgramFlow::threadsRun(void *threadStruct) {
    bool loopCondition = true;
    int timeOfTheLastAction = -1;
    threadData *threadData = (struct threadData *) threadStruct;
    int socketDescriptor = threadData->socketDescriptor;
    int driverId = threadData->id;
    Socket *socket = threadData->socket;
    TaxiCenter *taxiCenter = threadData->taxiCenter;
    bool arrived = true;
    Point arrivedPoint(0, 0);
    bool giveHimTrip = true;
    char buffer[100000];
    while (loopCondition) {
        switch (globalX) {
            //exit (break the loop and return)
            case 7: {
                socket->sendData("7", socketDescriptor);
                //delete threadData;
                loopCondition = false;
            }
                //ask the driver to move one step (or assign trip to the driver, if we should)
            case 9: {
                if (timeOfTheLastAction == taxiCenter->getTimer()) {
                    break;
                }
                // flag that indicate whether there was assigning trip in this time
                // (for preventing assigning of trip and movement in the same time)
                int assignFlag = 0;
                /*
                 assign trip to the driver if now is the starting time of the trip
                 and the driver is available ("arrived")
                */
                pthread_mutex_lock(&listOfTripsMutex);
                for (unsigned int i = 0; i < taxiCenter->getListOfTrips().size(); i++) {
                    int tripStartTime = taxiCenter->getListOfTrips().at(i)->getTime();
                    int currentTime = taxiCenter->getTimer();
                    Point currentDriverLocation = taxiCenter->getDriverLocation(driverId);
                    Point tripStartingPoint =
                            taxiCenter->getListOfTrips().at(i)->getStartingPoint();
                    if (((tripStartTime == currentTime)
                         && (currentDriverLocation == tripStartingPoint)) && (arrived)) {

                        arrivedPoint = taxiCenter->getListOfTrips().at(i)->getEndingPoint();
                        arrived = false;
                        //option 10 (of driver): assign a trip to the driver
                        socket->sendData("10", socketDescriptor);
                        memset(buffer, 0, sizeof(buffer));
                        //recive massage from the driver that confirms he has
                        // received our massage (gratitude to friend that teach us
                        //about the importance of reciving massage after every sending)
                        socket->reciveData(buffer, sizeof(buffer),
                                           socketDescriptor); //recive "thanks to Nevo"
                        SerializationClass<Trip *> serializeClass;
                        string serializedTrip = serializeClass.serializationObject(
                                taxiCenter->getListOfTrips().at(i));
                        //send trip to the driver
                        socket->sendData(serializedTrip, socketDescriptor);
                        memset(buffer, 0, sizeof(buffer));
                        //recive massage from the driver that confirms he has
                        // received our massage
                        socket->reciveData(buffer, sizeof(buffer),
                                           socketDescriptor); //recive "thanks to Nevo"
                        delete taxiCenter->getListOfTrips().at(i);
                        taxiCenter->deleteTrip(i);
                        LINFO << "secondary thread: (socket descriptor: "
                              << socketDescriptor << "): trip was sent to the client";
                        assignFlag = 1;
                        break;
                    }
                }
                pthread_mutex_unlock(&listOfTripsMutex);
                if (assignFlag == 0) {
                    //sending 9 in order to advance the driver one step
                    socket->sendData("9", socketDescriptor);
                    memset(buffer, 0, sizeof(buffer));
                    //recive from the driver his location (after the moving, if he moved)
                    socket->reciveData(buffer, sizeof(buffer), socketDescriptor);
                    string locationStr(buffer, sizeof(buffer));
                    Point driverLocation;
                    SerializationClass<Point> serializeClass;
                    driverLocation =
                            serializeClass.deSerializationObject(locationStr, driverLocation);
                    //if the driver arrived to the end point of his trip
                    //and he's available to get more trips, update the variable "arrived"
                    if (driverLocation == arrivedPoint) {
                        arrived = true;
                    }
                    pthread_mutex_lock(&driverLocationsMapMutex);
                    //update the driver location in the taxi center
                    taxiCenter->addDriverLocation(driverId, driverLocation);
                    if (arrived) {
                        taxiCenter->addDriverToListOfArrivedDrivers(driverId, driverLocation);
                    }
                    pthread_mutex_unlock(&driverLocationsMapMutex);
                    LINFO << "secondary thread: (socket descriptor: " << socketDescriptor
                          << "): 9 was sent to the client";
                    LINFO << "secondary thread: (socket descriptor: " << socketDescriptor
                          << "): the point " << driverLocation
                          << " was received from the client";
                }
                timeOfTheLastAction = taxiCenter->getTimer();
                pthread_mutex_lock(&circleFinishMutex);
                circleFinish--;
                LINFO << "secondary thread: (socket descriptor: " << socketDescriptor
                      << "): I did: circleFinish--. now circleFinish = " << circleFinish;
                pthread_mutex_unlock(&circleFinishMutex);

                break;
            }
            default:
                break;
        }
    }
    return NULL;
}

//(Documentation is in the ProgramFlow.h file)
void ProgramFlow::run(Socket *mainSocket) {
    char buffer[100000];
    bool runLoop = true;
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
    vector<pthread_t> pthreadVector = vector<pthread_t>();
    vector<threadData *> vectorOfStructs = vector<threadData *>();
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

    while (runLoop) {
        //get number of option and do the defined operation
        getline(cin, inputString);
        LINFO << inputString;
        switch (stoi(inputString)) {
            //case 1: accept connection with the clients and run new thread
            //for each of them in order to manage the communication with them
            case 1: {
                LINFO << "main thread: case1 begin";
                //create a driver (according to the given parameters) and add it to the taxi center
                getline(cin, inputString);
                expectedNumberOfDrivers = stoi(inputString);
                if (expectedNumberOfDrivers == 0) {
                    break;
                } else {
                    for (unsigned int i = 0; i < expectedNumberOfDrivers; i++) {
                        LINFO << "main thread: case1 before accept";
                        int descriptor = ProgramFlow::acceptConnection(mainSocket);
                        LINFO << "main thread: case1 after accept";
                        memset(buffer, 0, sizeof(buffer));
                        mainSocket->reciveData(buffer, sizeof(buffer), descriptor);
                        LINFO << "main thread: case1 recived driverId";
                        threadData *threadDataStruct = new struct threadData;
                        string driverIdString = string(buffer);
                        threadDataStruct->id = stoi(driverIdString);
                        //send taxi data
                        LINFO << "main thread: case1 before checking and sending cabString";

                        string dataOfCabOfDriver = taxiCenter.getCabString(threadDataStruct->id);
                        mainSocket->sendData(dataOfCabOfDriver, descriptor);
                        memset(buffer, 0, sizeof(buffer));
                        threadDataStruct->socket = mainSocket;
                        threadDataStruct->socketDescriptor = descriptor;
                        threadDataStruct->taxiCenter = &taxiCenter;
                        pthread_t pthread;
                        //recive massage from the driver that confirms he has received our massage
                        mainSocket->reciveData(
                                buffer, sizeof(buffer), descriptor); //recive "thanks to Nevo"
                        pthread_create(&pthread, NULL, ProgramFlow::threadsRun, threadDataStruct);
                        pthreadVector.push_back(pthread);
                        vectorOfStructs.push_back(threadDataStruct);
                        LINFO << "main thread: case1 - END OF ITERATION OF THE LOOP";
                        LINFO << "main thread: case1 - END OF ITERATION OF THE LOOP "
                                "(secondary thread was created)";
                    }

                }
                LINFO << "main thread: case1 end";
                break;
            }
                //case2: add trip to the data base (i.e. to the taxiCenter)
            case 2: {
                LINFO << "main thread: case2 begin";
                //create a trip (according to the given parameters) and add it to the taxi center
                getline(cin, inputString);
                InputParsing::parsedTripData trip = inputParsing.parseTripData(inputString);
                taxiCenter.createTrip(trip);
                LINFO << "main thread: case2 end";
                break;
            }
                //case3: create a cab
            case 3: {
                LINFO << "main thread: case3 begin";
                //create a cab (according to the given parameters) and add it to the taxi center
                //save string of parameters in the taxi center in order to pass it to the
                //corresponding driver afterwards.
                getline(cin, inputString);
                cabForDriver = CabFactory::createCab(inputString);
                taxiCenter.addCab(cabForDriver);
                taxiCenter.addCabString(cabForDriver->getId(), inputString);
                LINFO << "main thread: case3 end";
                break;
            }
                //case4: print location of specific driver
            case 4: {
                LINFO << "main thread: case4 begin";
                //query about the location of a specific driver
                getline(cin, inputString);
                int id = stoi(inputString);
                //get information about the driver location from the taxi center
                Point location = taxiCenter.getDriverLocation(id);
                cout << location << endl;
                LINFO << "main thread: case4 end";
                break;
            }
                //case7: exit - terminate all the threads
            case 7: {
                globalX = 7;
                for (unsigned long i = 0; i < pthreadVector.size(); i++) {
                    pthread_join(pthreadVector.at(i), NULL);
                }
                for (unsigned long i = 0; i < vectorOfStructs.size(); i++) {
                    delete vectorOfStructs.at(i);
                }

                sleep(1);
                LINFO << "main thread: case7 - exit";
                //deallocate memory and terminate the program
                delete grid;
                runLoop = false;
                break;
            }
                //case9: advance the time and do the suitable actions (assigning trips/moving)
            case 9: {
                circleFinish = expectedNumberOfDrivers;

                pthread_mutex_lock(&listOfTripsMutex);
                //advance the time
                taxiCenter.setTimer();
                pthread_mutex_unlock(&listOfTripsMutex);

                LINFO << "main thread: case9 begin. current time: " << taxiCenter.getTimer();
                //every driver that finished his action will decrease the value of "circleFinish"
                circleFinish = expectedNumberOfDrivers;
                globalX = 9;

                do {
                } while (circleFinish != 0);

                LINFO << "main thread: case9 end";
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
    LINFO << "accept descriptor number" << descriptor;
    socket->getListOfDescriptors().push_back(descriptor);
    return descriptor;
}

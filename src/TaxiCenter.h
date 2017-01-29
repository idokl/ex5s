#ifndef EX1_VERSION_1_1_TAXICENTER_H
#define EX1_VERSION_1_1_TAXICENTER_H

#include "Driver.h"
#include "Trip.h"
#include "Passenger.h"
#include "BfsAlgorithm.h"
#include "InputParsing.h"
#include <pthread.h>



/*
 * This class is responsible for communication with all the drivers, cabs and trips.
 * It should make them work in coordination and cooperation.
 */
class TaxiCenter {
private:
    //vector<Driver> listOfDrivers;
    vector<Trip *> listOfTrips;
    vector<Cab *> listOfCabs;
    map<int,string> mapOfCabStrings;
    map<int,Point> mapOfDriversLocations;
    BfsAlgorithm<Point> bfsInstance;
    stack <Node<Point>> nextPointsOfPath;
    int timer;
    vector<map<int,Point>> listOfArrivedDrivers;
public:

    TaxiCenter();

    //constructor
    TaxiCenter(BfsAlgorithm<Point> &bfsInstance);

    void bfsNavigate(Node<Point> startNode,  Node<Point> endNode);

    void addDriverLocation(int id, Point location);
        //add cab to the taxi center
    void addCab(Cab *cab);

    void addCabString(int id, string cabString);

    static void* runBfsThread(void * nodeOfPoints);
    // allocate the trips that were received in the system to the appropriate drivers
    // and command each of them to drive to the end point of its trip


    //get list of the trips
    const vector<Trip *> &getListOfTrips() const;

    //return the current location of a specific driver (according to its id number)
    //(if no such driver in the system, throw "no driver found")
    Point getDriverLocation(int driverId);

    //add a trip to the taxi center
    void addTrip(Trip *trip);

    //create a trip (according to the parameters that are given in the 'parsed trip data' struct)
    //and add the trip to the taxi center
    bool createTrip(InputParsing::parsedTripData);

    //get cab that is belonging to the taxi center (according to the cab id).
    //(if no such cab in the system, throw "no cab found")
    Cab *getCab(int id);

    string getCabString(int id);

    void deleteTrip(int i);

    void bfsWrapper(Node<Point> startNode, Node<Point> endNode, TaxiCenter *taxiCenter);

    int getTimer();

    int getNumOfDrivers();

    void addDriverToListOfArrivedDrivers(int driverId,Point driverLocation);

    vector<map<int,Point>> getlistOfArrivedDrivers();
    void setTimer();
    //destructor
    ~TaxiCenter();
};


#endif

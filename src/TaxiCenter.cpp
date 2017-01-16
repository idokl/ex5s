#include "TaxiCenter.h"


TaxiCenter::TaxiCenter(BfsAlgorithm<Point> &bfsInstance) : bfsInstance(bfsInstance), timer(0) {}

void TaxiCenter::createTrip(InputParsing::parsedTripData parsedTripDataTrip) {
    Node<Point> startNode(parsedTripDataTrip.start);
    Node<Point> endNode(parsedTripDataTrip.end);
    this->bfsWrapper(startNode,endNode,this);
    Trip *trip = new Trip(parsedTripDataTrip.id, parsedTripDataTrip.start, parsedTripDataTrip.end,
                          parsedTripDataTrip.numberOfPassengers, parsedTripDataTrip.tariff, nextPointsOfPath, parsedTripDataTrip.time);
    listOfTrips.push_back(trip);
}
/*
const vector<Driver> &TaxiCenter::getListOfDrivers() const {
    return listOfDrivers;
}
*/
const vector<Trip *> &TaxiCenter::getListOfTrips() const {
    return listOfTrips;
}


/*
Point TaxiCenter::getDriverLocation(int driverId) {
    for (unsigned int i = 0; i < listOfDrivers.size(); i++) {
        if (listOfDrivers.at(i).getId() == driverId) {
            return listOfDrivers.at(i).currentPlace();
        }
    }
    throw "No Driver found";
}*/

void TaxiCenter::addTrip(Trip *trip) {
    listOfTrips.push_back(trip);
}

void TaxiCenter::addCab(Cab *cab) {
    listOfCabs.push_back(cab);
}

void TaxiCenter::addCabString(int id, string cabString) {
    mapOfCabStrings[id] = cabString;
}

string TaxiCenter::startDriving() {
    for (unsigned int i = 0; i < mapOfDriversLocations.size(); i++) {
        if (listOfTrips.size() > 0) {
            for (unsigned int j = 0; j < listOfTrips.size(); j++) {
                if ((mapOfDriversLocations.at(i)==listOfTrips.at(j)->getStartingPoint())) {
                    //listOfDrivers.at(i).assignTrip(listOfTrips.at(j));
                    SerializationClass<Trip *> serializeClass;
                    string str = serializeClass.serializationObject(listOfTrips.at(j));
                    delete listOfTrips[j];
                    listOfTrips.erase(listOfTrips.begin() + j);
                    return str;
                }
            }
        }
    }
    return NULL;
}

Cab *TaxiCenter::getCab(int id) {
    for (unsigned int i = 0; i < listOfCabs.size(); i++) {
        if (listOfCabs.at(i)->getId() == id) {
            return listOfCabs.at(i);
        }
    }
    throw "no cab found";
}

string TaxiCenter::getCabString(int id) {
    return mapOfCabStrings[id];
}

void TaxiCenter::addDriverLocation(int id, Point location) {
    mapOfDriversLocations[id] = location;
}

TaxiCenter::~TaxiCenter() {
    for (unsigned int i = 0; i < listOfCabs.size(); i++) {
        delete listOfCabs[i];
    }
}

void TaxiCenter::deleteTrip(int i) {
    this->listOfTrips.erase(this->listOfTrips.begin() + i);
}

struct nodeOfPoints{
    Node<Point> startNode;
    Node<Point> endNode;
    TaxiCenter* taxiCenter;
};

void *TaxiCenter::runBfsThread(void *t) {
    nodeOfPoints *x =(struct nodeOfPoints*)t;
    x->taxiCenter->bfsNavigate(x->startNode,x->endNode);
    return NULL;
}

void TaxiCenter::bfsNavigate(Node<Point> startNode,  Node<Point> endNode) {
    this->nextPointsOfPath = this->bfsInstance.navigate(startNode, endNode);
}

void TaxiCenter::bfsWrapper(Node<Point> startNode, Node<Point> endNode, TaxiCenter *taxiCenter) {

   nodeOfPoints *nodeOfPoints = new struct nodeOfPoints;

    nodeOfPoints->startNode = startNode;
    nodeOfPoints->endNode = endNode;
    nodeOfPoints->taxiCenter=taxiCenter;

    pthread_t bfsThread;
    pthread_create(&bfsThread, NULL, runBfsThread, nodeOfPoints);
    pthread_join(bfsThread, NULL);
}

TaxiCenter::TaxiCenter() : bfsInstance(NULL){

}

int TaxiCenter::getTimer() {
    return timer;
}

int TaxiCenter::setTimer() {
    this->timer++;
}

Point TaxiCenter::getDriverLocation(int driverId) {
    return Point();
}




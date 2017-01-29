#include "TaxiCenter.h"


TaxiCenter::TaxiCenter(BfsAlgorithm<Point> &bfsInstance) : bfsInstance(bfsInstance), timer(0) {}

bool TaxiCenter::createTrip(InputParsing::parsedTripData parsedTripDataTrip) {
    bool pathExists = true;
    Node<Point> startNode(parsedTripDataTrip.start);
    Node<Point> endNode(parsedTripDataTrip.end);
    this->bfsWrapper(startNode,endNode,this);
    //if no path, return false
    if (this->nextPointsOfPath == stack<Node<Point>>()) {
        pathExists = false;
        return pathExists;
    }
    this->nextPointsOfPath.pop();
    Trip *trip = new Trip(parsedTripDataTrip.id, parsedTripDataTrip.start, parsedTripDataTrip.end,
                          parsedTripDataTrip.numberOfPassengers, parsedTripDataTrip.tariff, nextPointsOfPath, parsedTripDataTrip.time);
    listOfTrips.push_back(trip);
    //if path exist and trip has been created, return true
    return pathExists;
}

const vector<Trip *> &TaxiCenter::getListOfTrips() const {
    return listOfTrips;
}

void TaxiCenter::addTrip(Trip *trip) {
    listOfTrips.push_back(trip);
}

void TaxiCenter::addCab(Cab *cab) {
    listOfCabs.push_back(cab);
}

void TaxiCenter::addCabString(int id, string cabString) {
    mapOfCabStrings[id] = cabString;
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
    delete nodeOfPoints;
}

TaxiCenter::TaxiCenter() : bfsInstance(NULL){

}

int TaxiCenter::getTimer() {
    return timer;
}

void TaxiCenter::setTimer() {
    this->timer++;
}

Point TaxiCenter::getDriverLocation(int driverId) {
    //throw exception if driverId is not in the mapOfDriversLocations
    return this->mapOfDriversLocations.at(driverId);
}

int TaxiCenter::getNumOfDrivers() {
    return (int)this->mapOfDriversLocations.size();
}

void TaxiCenter::addDriverToListOfArrivedDrivers(int driverId, Point driverLocation) {
    map<int,Point> driverArrived = map<int,Point>();
    driverArrived[driverId] = driverLocation;
    this->listOfArrivedDrivers.push_back(driverArrived);
}

vector<map<int, Point>> TaxiCenter::getlistOfArrivedDrivers() {
    return this->listOfArrivedDrivers;
}



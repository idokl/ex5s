#include "TaxiCenter.h"


TaxiCenter::TaxiCenter(BfsAlgorithm<Point> &bfsInstance) : bfsInstance(bfsInstance), timer(0) {}

void TaxiCenter::createTrip(InputParsing::parsedTripData parsedTripDataTrip) {
    Trip *trip = new Trip(parsedTripDataTrip.id, parsedTripDataTrip.start, parsedTripDataTrip.end,
                          parsedTripDataTrip.numberOfPassengers, parsedTripDataTrip.tariff,
                          parsedTripDataTrip.time);
    this->tripQueue.push(trip);
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

TaxiCenter::TaxiCenter() : bfsInstance(NULL) {

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
    return (int) this->mapOfDriversLocations.size();
}

void TaxiCenter::addDriverToListOfArrivedDrivers(int driverId, Point driverLocation) {
    map<int, Point> driverArrived = map<int, Point>();
    driverArrived[driverId] = driverLocation;
    this->listOfArrivedDrivers.push_back(driverArrived);
}

vector<map<int, Point>> TaxiCenter::getlistOfArrivedDrivers() {
    return this->listOfArrivedDrivers;
}


void TaxiCenter::threadPoolBfsCalc(int threadsNum) {
    this->threads = new pthread_t[threadsNum];
    this->stop = false;
    pthread_mutex_init(&lock, NULL);
    for (int i = 0; i < threadsNum; i++) {
        pthread_create(this->threads + i, NULL, runBfsThread, this);
    }
}


void TaxiCenter::execute() {
    while (!stop) {
        pthread_mutex_lock(&lock);
        if (!tripQueue.empty()) {
            Trip* trip = tripQueue.front();
            tripQueue.pop();
            pthread_mutex_unlock(&lock);
            Node<Point> nodeStart(trip->getStartingPoint());
            Node<Point> nodeEnd(trip->getEndingPoint());
            trip->setNextPointOfPath(this->bfsInstance.navigate(nodeStart,nodeEnd));
            this->listOfTrips.push_back(trip);
        }
        else {
            pthread_mutex_unlock(&lock);
            sleep(1);
        }
    }
    pthread_exit(NULL);
}

void *TaxiCenter::runBfsThread(void *taxiCenterArg) {
    TaxiCenter *taxiCenter = (TaxiCenter*)taxiCenterArg;
    taxiCenter->execute();
    return NULL;
}

void TaxiCenter::terminate() {
    this->stop = true;

}


TaxiCenter::~TaxiCenter() {
    for (unsigned int i = 0; i < listOfCabs.size(); i++) {
        delete listOfCabs[i];
    }
    delete threads;
    pthread_mutex_destroy(&lock);
}
#include "Trip.h"

Trip::Trip(int rideId, Point startingPoint, Point endingPoint, int numOfPassengers, double taarif, int time):
        rideId(rideId), startingPoint(startingPoint),
        endingPoint(endingPoint),
        numOfPassengers(numOfPassengers), time(time) {
    rateOfTrip = taarif;
    passedMeters = 0;
    isReady = false;
    isPassablePath = true;
}

int Trip::getRideId() const {
    return rideId;
}

const Point &Trip::getStartingPoint() const {
    return startingPoint;
}

const Point &Trip::getEndingPoint() const {
    return endingPoint;
}

const list <Passenger> &Trip::getCustomers() const {
    return customers;
}

int Trip::getNumberOfPassengers() const {
    return (int) customers.size();
}

double Trip::getRateOfTrip() const {
    return rateOfTrip;
}

int Trip::getPassedMeters() const {
    return passedMeters;
}

const list <Point> &Trip::getPath() const {
    return nextPointsOfPath;
}

void Trip::removeNextPointOfPath() {
    if (!this->nextPointsOfPath.empty()) {
        this->nextPointsOfPath.pop_front();
        passedMeters++;
    }
}

Trip::~Trip() {
}

Trip::Trip() : rideId(0), passedMeters(0), startingPoint(0, 0),
               endingPoint(0, 0), numOfPassengers(0), rateOfTrip(0.0) {
}

int Trip::getTime() {
    return time;
};


void Trip::setNextPointOfPath(stack<Node<Point>> nextPointsOfPathStack) {
    if(!nextPointsOfPathStack.empty()) {
        nextPointsOfPathStack.pop();
    }
    while (! nextPointsOfPathStack.empty()) {
        Node<Point> n =  nextPointsOfPathStack.top();
        Point p = n.getValue();
        nextPointsOfPath.push_back(p);
        nextPointsOfPathStack.pop();
    }
}

void Trip::setIsReady() {
        this->isReady = true;
}

bool Trip::getIsReady() {
    return this->isReady;
}

bool Trip::isPassable() const {
    return isPassablePath;
}

void Trip::setIsPassable() {
    this->isPassablePath = false;
}
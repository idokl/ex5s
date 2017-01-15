#include "Trip.h"

Trip::Trip(int rideId, Point startingPoint, Point endingPoint, int numOfPassengers, double taarif,
       stack<Node<Point>> stackOfPathWithoutStartingPoint, int time): rideId(rideId), startingPoint(startingPoint),
                                      endingPoint(endingPoint), numOfPassengers(numOfPassengers), time(time) {
    while(!stackOfPathWithoutStartingPoint.empty()) {
        Node<Point> n = stackOfPathWithoutStartingPoint.top();
        Point p = n.getValue();
        nextPointsOfPath.push_back(p);
        stackOfPathWithoutStartingPoint.pop();
    }
    rateOfTrip = taarif;
    passedMeters = 0;
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

const list<Point> &Trip::getPath() const {
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

Trip::Trip() : rideId(0), passedMeters(0),startingPoint(0,0),
               endingPoint(0,0), numOfPassengers(0),rateOfTrip(0.0) {
}

int Trip::getTime() {
    return time;
};


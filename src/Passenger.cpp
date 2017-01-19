#include "Passenger.h"

Passenger::Passenger(Point source, Point destination, bool isLuxury) :
        source(source), destination(destination), isLuxury(isLuxury) {}

int Passenger::getSatisfaction() const {
    int satisfaction = rand() % 5 + 1;
    return satisfaction;
}

bool Passenger::getIsLuxury() {
    //TBD
    //ex1, ex2 and ex3 have not defined option to choose type of taxi by the customer
    return false;
}

int Passenger::giveFeedBack() {
    return this->getSatisfaction();
}

Passenger::Passenger() : source(0, 0), destination(0, 0), isLuxury(true) {
}

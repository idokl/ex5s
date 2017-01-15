#include "StandardCab.h"

StandardCab::StandardCab(int id, Model_Of_Car carModel, Color_Of_Car color) :
        Cab(id, carModel, color), speed(SPEED_OF_STANDARD_CAB), rate(RATE_OF_STANDARD_CAB) {}

Taxi_Type StandardCab::getTaxiType() {
    return STANDARD_CAB;
}

double StandardCab::getSpeed() const {
    return speed;
}

double StandardCab::getRate() const {
    return rate;
}

StandardCab::~StandardCab() {
}

StandardCab::StandardCab() : rate(0), speed(0) {
}

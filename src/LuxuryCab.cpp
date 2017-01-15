#include "LuxuryCab.h"

LuxuryCab::LuxuryCab(int id, Model_Of_Car carModel, Color_Of_Car color) :
        Cab(id, carModel, color), speed(SPEED_OF_LUXURY_CAB), rate(RATE_OF_LUXURY_CAB) {}

Taxi_Type LuxuryCab::getTaxiType() {
    return LUXURY_CAB;
}

double LuxuryCab::getSpeed() const {
    return speed;
}

double LuxuryCab::getRate() const {
    return rate;
}

LuxuryCab::~LuxuryCab() {
}

LuxuryCab::LuxuryCab() : rate(0), speed(0) {
}
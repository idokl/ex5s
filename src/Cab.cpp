#include "Cab.h"

Cab::Cab(int id, Model_Of_Car carModel, Color_Of_Car color) : id(id), carModel(carModel),
                                                              color(color) {
    passedKilometers = 0;
}

int Cab::getId() const {
    return id;
}

int Cab::getPassedKilometers() const {
    return passedKilometers;
}

Model_Of_Car Cab::getCarModel() const {
    return carModel;
}

Color_Of_Car Cab::getColor() const {
    return color;
}

Cab::~Cab() {
}

Cab::Cab() {
}

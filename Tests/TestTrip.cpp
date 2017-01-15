/*
#include "gtest/gtest.h"
#include "../src/Trip.h"

class TestTrip : public ::testing::Test {

protected:
    Trip trip;
    list <Passenger> passengersList;
    Passenger reuven, shimon, ohad, ido;

    TestTrip() : reuven(Point(0, 0), Point(1, 0), false),
                 shimon(Point(9, 9), Point(1, 0), false),
            //2 passengers with the same arguments;
                 ohad(Point(5, 5), Point(3, 3), false),
                 ido(Point(5, 5), Point(3, 3), false),
                 passengersList(),
                 trip(1384, Point(0, 0), Point(3, 3), 0, 20) {};

    virtual void setUp() {
        passengersList.push_back(reuven);
        passengersList.push_back(shimon);
        passengersList.push_back(ohad);
        passengersList.push_back(ido);

    }

};

TEST_F(TestTrip, GettersCheck) {
    ASSERT_EQ(trip.getRideId(), 1384);
    ASSERT_EQ(trip.getStartingPoint(), Point(0, 0));
    ASSERT_EQ(trip.getEndingPoint(), Point(3, 3));
    ASSERT_EQ(trip.getPassedMeters(), 0) << "ERROR: passed meters of constructed trip isn't 0";
}
*/
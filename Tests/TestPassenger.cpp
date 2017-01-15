#include "gtest/gtest.h"
#include "../src/Passenger.h"

class TestPassenger: public ::testing::Test {

protected:
    Point reuvenSourcePoint,reuvenDestinationPoint;
    Passenger reuven;
    TestPassenger() : reuvenSourcePoint(2,2), reuvenDestinationPoint(1,0),
                      reuven(reuvenSourcePoint, reuvenDestinationPoint, false){};
};

TEST_F(TestPassenger,sanityCheck){
    ASSERT_EQ(reuven.getIsLuxury(), false) << "check the cab type";
    for (int i = 0; i < 20; i++) {
        int reuvenSatisfaction = reuven.getSatisfaction();
        ASSERT_GE(reuven.getSatisfaction(),1) << "check the satisfaction getter";
        ASSERT_LE(reuven.getSatisfaction(),5) << "check the satisfaction getter";
    }
}
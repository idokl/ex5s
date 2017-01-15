/*
#include "gtest/gtest.h"
#include "../src/Driver.h"
#include "../src/StandardCab.h"
#include "../src/LuxuryCab.h"

class TestDriver: public ::testing::Test {

protected:
    Driver driver,driver2,driver3;
    Point reuvenSourcePoint,reuvenDestinationPoint;
    Passenger reuven;
    Cab* cabForDriver1;
    Cab* cabForDriver2;
    Cab* cabForDriver3;
    TestDriver() : cabForDriver1(new StandardCab(1122233, HONDA, BLUE)),
                   cabForDriver2(new LuxuryCab(223344, SUBARO, RED)),
                   cabForDriver3(new LuxuryCab(555555, TESLA, GREEN)),
                   driver(1, 30, MARRIED, 5, cabForDriver1),
                   driver2(2, 25, SINGLE, 5, cabForDriver2),
                   driver3(3, 28, SINGLE, 5, cabForDriver3),
                   reuvenSourcePoint(2,2),
                   reuvenDestinationPoint(1,0),
                   reuven(reuvenSourcePoint, reuvenDestinationPoint, false){}


    virtual void SetUp() {
        driver.clientSatisfactions(5);
        driver.clientSatisfactions(4);
        driver2.clientSatisfactions(6);
        driver3.clientSatisfactions(0);
        driver.addPassenger(reuven);
    }

    virtual void TearDown(){
        delete cabForDriver1;
        delete cabForDriver2;
        delete cabForDriver3;
    }
};


    TEST_F(TestDriver, GettersCheck) {
    ASSERT_EQ(driver.getAge(), 30) << "test the age of the driver";
    ASSERT_EQ(driver.getId(), 1) << "test the id of the driver";
    ASSERT_EQ(driver.getStatus(), MARRIED) << "test the Marriage status of driver";
    ASSERT_EQ(driver.getYearsOfExperience(), 5) << "test years of experience";
    ASSERT_EQ(driver.getSatisfactions(), 4.5) << "test the average satisfaction change";
    EXPECT_NE(driver2.getSatisfactions(),6) << "average satisfaction can't be >5";
    EXPECT_NE(driver3.getSatisfactions(),0) << "average satisfaction can't be <1";
    EXPECT_EQ(driver.getListOfPassengers().front().getIsLuxury(),reuven.getIsLuxury()) <<
                                                           "check if inserted passenger works";
    EXPECT_EQ(driver2.getListOfPassengers().size(),0)<<"check that empty listOfPassenger is empty";

}
*/
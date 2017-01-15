/*
#include "gtest/gtest.h"
#include "../src/TaxiCenter.h"
#include "../src/StandardCab.h"

class TestTaxiCenter : public ::testing::Test {

protected:

    Grid *grid;
    BfsAlgorithm<Point> bfs;
    TaxiCenter taxiCenter;
    Point reuvenSourcePoint, reuvenDestinationPoint;
    Passenger reuven;
    Driver driver1, driver2;
    StandardCab *cabForDriver1;
    StandardCab *cabForDriver2;

    TestTaxiCenter() : grid(new Grid(10, 10)), bfs(grid), taxiCenter(bfs),
                       reuvenSourcePoint(2, 2), reuvenDestinationPoint(0, 1),
                       reuven(reuvenSourcePoint, reuvenDestinationPoint, false),
                       cabForDriver1(new StandardCab(1122233, HONDA, BLUE)),
                       cabForDriver2(new StandardCab(4455566, FIAT, WHITE)),
                       driver1(555555555, 80, WIDOWED, 50, cabForDriver1),
                       driver2(121212121, 40, MARRIED, 3, cabForDriver2) {}

    virtual void TearDown() {
        delete cabForDriver1;
        delete cabForDriver2;
        delete grid;
    }

};

TEST_F(TestTaxiCenter, SanityChecks) {
    //check that no drivers and no trips when taxiCenter is initialized
    ASSERT_EQ(taxiCenter.getListOfDrivers().empty(), true);
    ASSERT_EQ(taxiCenter.getListOfTrips().empty(), true);
    //check addDriver(Driver) method and getListOfDrivers() method
    taxiCenter.addDriver(driver1);
    vector<Driver> driversList = taxiCenter.getListOfDrivers();
    ASSERT_EQ(driversList.size(), 1);
    Driver taxiCenterDriver = driversList.front();
    ASSERT_EQ(taxiCenterDriver.getId(), 555555555);

    taxiCenter.addDriver(driver2);
    Point driver1Position = driver2.currentPlace();
    Point driver1PositionAccordingToTaxiCenter = taxiCenter.getDriverLocation(driver2.getId());
    ASSERT_EQ(driver1PositionAccordingToTaxiCenter, driver1Position);

}
*/
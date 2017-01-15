#include <gtest/gtest.h>
#include "../src/InputParsing.h"

class TestInputParsing: public ::testing::Test {
protected:
    InputParsing inputParsing;

    TestInputParsing() : inputParsing() {}
};

TEST_F(TestInputParsing, pointParsing) {
    string pointString = "2,3";
    Point p = inputParsing.parsePoint(pointString);
    ASSERT_EQ(p, Point(2, 3)) << "error of parsing point";
}


TEST_F(TestInputParsing, gridParsing) {
    string gridDimensionsString = "5 7";
    InputParsing::gridDimensions gridDimensionsStruct =
            inputParsing.parseGridDimensions(gridDimensionsString);
    ASSERT_EQ(gridDimensionsStruct.gridWidth, 5) << "error of parsing grid dimensions";
    ASSERT_EQ(gridDimensionsStruct.gridHeight, 7) << "error of parsing grid dimensions";
}

TEST_F(TestInputParsing, driverParsing) {
    string driverString = "123456789,30,W,5,1122233";
    InputParsing::parsedDriverData driverDataStruct = inputParsing.parseDriverData(driverString);
    ASSERT_EQ(driverDataStruct.id,123456789) << "error of parsing driver id";
    ASSERT_EQ(driverDataStruct.age,30) << "error of parsing driver age";
    Driver:Status_Of_Marriage correctStatus = WIDOWED;
    ASSERT_EQ(driverDataStruct.status,correctStatus) << "error of parsing driver status";
    ASSERT_EQ(driverDataStruct.vehicleId,1122233) << "error of parsing driver vehicle id";
    ASSERT_EQ(driverDataStruct.yearsOfExperience,5)
                                << "error of parsing number of years of experience";
}

TEST_F(TestInputParsing, tripParsing) {
    InputParsing::parsedTripData tripDataStruct = inputParsing.parseTripData("0,2,10,9,3,4,3.20");
    ASSERT_EQ(tripDataStruct.id,0) << "error of parsing trip";
    ASSERT_EQ(tripDataStruct.start,Point(2,10)) << "error of parsing trip";
    ASSERT_EQ(tripDataStruct.end,Point(9,3)) << "error of parsing trip";
    ASSERT_EQ(tripDataStruct.numberOfPassengers,4) << "error of parsing trip";
    ASSERT_EQ(tripDataStruct.tariff,3.20) << "error of parsing trip";
}

TEST_F(TestInputParsing, cabParsing) {
    InputParsing::parsedCabData cabDataStruct = inputParsing.parseVehicleData("9900088,1,S,W");
    ASSERT_EQ(cabDataStruct.taxiType,STANDARD_CAB) << "error of parsing cab";
    ASSERT_EQ(cabDataStruct.id,9900088) << "error of parsing cab";
    ASSERT_EQ(cabDataStruct.color,WHITE) << "error of parsing cab";
    ASSERT_EQ(cabDataStruct.manufacturer,SUBARO) << "error of parsing cab";

    cabDataStruct = inputParsing.parseVehicleData("0,2,F,P");
    ASSERT_EQ(cabDataStruct.id,0) << "error of parsing cab";
    ASSERT_EQ(cabDataStruct.taxiType,LUXURY_CAB) << "error of parsing cab";
    ASSERT_EQ(cabDataStruct.manufacturer,FIAT) << "error of parsing cab";
    ASSERT_EQ(cabDataStruct.color,PINK) << "error of parsing cab";
 }

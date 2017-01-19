#ifndef EX1_VERSION_1_1_INPUTPARSING_H
#define EX1_VERSION_1_1_INPUTPARSING_H

#include <iostream>
#include <vector>
#include <map>
#include "Point.h"
#include "Cab.h"

class Driver;

using namespace std;

typedef enum {
    SINGLE, MARRIED, DIVORCED, WIDOWED
} Status_Of_Marriage;

class InputParsing {
private:
    Driver *driver;
    map<string, Status_Of_Marriage> mapOfLetterToStatus;
    map<int, Taxi_Type> mapOfNumberToTaxiType;
    map<string, Model_Of_Car> mapOfLetterToModel;
    map<string, Color_Of_Car> mapOfLetterToColor;

    vector<string> splitStrings(string stringWithCommas, int numberOfSeparatedWords);

public:
    InputParsing();

    typedef struct {
        int gridWidth;
        int gridHeight;
    } gridDimensions;

    //"gridData" string format: "gridWidth gridHeight" (int int). example: 3 3
    gridDimensions parseGridDimensions(string gridData);

    //"pointData" string format: "Xcoordinate,Ycoordinate" (int,int). example: 0,0
    Point parsePoint(string pointData);

    typedef struct {
        //Driver driver;
        int id;
        int age;
        Status_Of_Marriage status;
        int yearsOfExperience;
        int vehicleId;
    } parsedDriverData;

    /*
     * "driverData" string format: "id,age,status,experience,vehicleId"
     * (int,int,char:{S,M,D,W},int,int)
     *'status' is one of the following letters:
     * S / M / D / W  (represents: Single / Married / Divorced / Widowed)
     * example: 123456789,30,M,5,1122233
     */
    parsedDriverData parseDriverData(string driverData);

    /*
     * "tripData" string format: "id,xStart,yStart,xEnd,yEnd,numPassengers,tariff,startingTime"
     * (int,int,int,int,int,int,double,int)
     */
    typedef struct {
        int id;
        Point start;
        Point end;
        int numberOfPassengers;
        double tariff;
        int time;
    } parsedTripData;

    parsedTripData parseTripData(string tripData);

    /*
     * "vehicleData" string format: "id,taxiType,manufacturer,color"
     * (int,{1:NormalCab,2:LuxuryCab},char:{H,S,T,F},char:{R,B,G,P,W})
     * 'taxiType' is one of the following characters:
     * 1 / 2 (represents: StandardCab / LuxuryCab).
     * 'manufacturer' is one of the following letters:
     * H / S / T / F (represents: HONDA, SUBARO, TESLA, FIAT)
     * 'color' is one of the following letters:
     * R / B / G / P / W (represents: RED, BLUE, GREEN, PINK, WHITE)
     */
    typedef struct {
        int id;
        Taxi_Type taxiType;
        Model_Of_Car manufacturer;
        Color_Of_Car color;
    } parsedCabData;

    parsedCabData parseVehicleData(string vehicleData);
};


#endif //EX1_VERSION_1_1_INPUTPARSING_H

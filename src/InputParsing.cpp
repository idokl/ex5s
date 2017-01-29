#include <cstring>
#include "InputParsing.h"

//new functions
InputParsing::InputParsing() {
    digits = "0123456789";
    //construct map of status of marriage
    mapOfLetterToStatus["S"] = SINGLE;
    mapOfLetterToStatus["M"] = MARRIED;
    mapOfLetterToStatus["D"] = DIVORCED;
    mapOfLetterToStatus["W"] = WIDOWED;
    //construct map of taxi type (regular/luxury)
    mapOfNumberToTaxiType[1] = STANDARD_CAB;
    mapOfNumberToTaxiType[2] = LUXURY_CAB;
    //construct map of car model (=manufacturer)
    mapOfLetterToModel["H"] = HONDA;
    mapOfLetterToModel["S"] = SUBARO;
    mapOfLetterToModel["T"] = TESLA;
    mapOfLetterToModel["F"] = FIAT;
    //construct map of car color
    mapOfLetterToColor["R"] = RED;
    mapOfLetterToColor["B"] = BLUE;
    mapOfLetterToColor["G"] = GREEN;
    mapOfLetterToColor["P"] = PINK;
    mapOfLetterToColor["W"] = WHITE;
}

InputParsing::gridDimensions InputParsing::parseGridDimensions(string gridData) {
    InputParsing::gridDimensions dimensions;
    string::size_type spacePosition = gridData.find(" ");
    if (spacePosition == string::npos) {
        //throw exception: no space in the grid dimensions input (between width and height)
        LINFO << "InputParsing: no space in the input of grid dimensions (between width and height)";
        throw exception();
    }
    string widthString = gridData.substr(0, spacePosition);
    string heightString = gridData.substr(spacePosition + 1);
    expectToPositiveNumber(widthString);
    expectToPositiveNumber(heightString);
    dimensions.gridHeight = stoi(heightString);
    dimensions.gridWidth = stoi(widthString);
    return dimensions;
}

Point InputParsing::parsePoint(string pointData) {
    vector<string> coordinates = this->splitStrings(pointData, 2);
    expectToNonNegativeNumber(coordinates[0]);
    expectToNonNegativeNumber(coordinates[1]);
    Point p = Point(stoi(coordinates[0]), stoi(coordinates[1]));
    return p;
}

InputParsing::parsedDriverData InputParsing::parseDriverData(string driverData) {
    vector<string> listOfStringsOfDriverParameters = this->splitStrings(driverData, 5);
    parsedDriverData driverParameters;
    expectToNonNegativeNumber(listOfStringsOfDriverParameters[0]); //test id isn't negative
    driverParameters.id = stoi(listOfStringsOfDriverParameters[0]);
    expectToNonNegativeNumber(listOfStringsOfDriverParameters[1]); //test age isn't negative
    driverParameters.age = stoi(listOfStringsOfDriverParameters[1]);
    driverParameters.status = mapOfLetterToStatus.at(listOfStringsOfDriverParameters[2]);
    expectToNonNegativeNumber(listOfStringsOfDriverParameters[3]); //test experience isn't negative
    driverParameters.yearsOfExperience = stoi(listOfStringsOfDriverParameters[3]);
    expectToNonNegativeNumber(listOfStringsOfDriverParameters[4]); //test vehicleId isn't negative
    driverParameters.vehicleId = stoi(listOfStringsOfDriverParameters[4]);
    return driverParameters;
}

InputParsing::parsedTripData InputParsing::parseTripData(string tripData) {
    vector<string> listOfStringsOfTripParameters = this->splitStrings(tripData, 8);
    InputParsing::parsedTripData tripParameters;
    expectToNonNegativeNumber(listOfStringsOfTripParameters[0]); //test id isn't negative
    tripParameters.id = stoi(listOfStringsOfTripParameters[0]);
    expectToNonNegativeNumber(listOfStringsOfTripParameters[1]); //test coordinate isn't negative
    int x = stoi(listOfStringsOfTripParameters[1]);
    expectToNonNegativeNumber(listOfStringsOfTripParameters[2]); //test coordinate isn't negative
    int y = stoi(listOfStringsOfTripParameters[2]);
    tripParameters.start = Point(x, y);
    expectToNonNegativeNumber(listOfStringsOfTripParameters[3]); //test coordinate isn't negative
    x = stoi(listOfStringsOfTripParameters[3]);
    expectToNonNegativeNumber(listOfStringsOfTripParameters[4]); //test coordinate isn't negative
    y = stoi(listOfStringsOfTripParameters[4]);
    tripParameters.end = Point(x, y);
    expectToNonNegativeNumber(listOfStringsOfTripParameters[5]); //test passengers num not negative
                                                                 //(although it isn't required)
    tripParameters.numberOfPassengers = stoi(listOfStringsOfTripParameters[5]);
    expectToNonNegativeNumber(listOfStringsOfTripParameters[6]); //test tariff isn't negative
    tripParameters.tariff = stod(listOfStringsOfTripParameters[6]);
    expectToPositiveNumber(listOfStringsOfTripParameters[7]); //test time is positive
    tripParameters.time = stoi(listOfStringsOfTripParameters[7]);
    return tripParameters;
}

InputParsing::parsedCabData InputParsing::parseVehicleData(string vehicleData) {
    vector<string> listOfStringsOfCabParameters = this->splitStrings(vehicleData, 4);
    parsedCabData cabParameters;
    expectToNonNegativeNumber(listOfStringsOfCabParameters[0]); //test id isn't negative
    cabParameters.id = stoi(listOfStringsOfCabParameters[0]);
    cabParameters.taxiType = mapOfNumberToTaxiType.at(stoi(listOfStringsOfCabParameters[1]));
    cabParameters.manufacturer = mapOfLetterToModel.at(listOfStringsOfCabParameters[2]);
    cabParameters.color = mapOfLetterToColor.at(listOfStringsOfCabParameters[3]);
    return cabParameters;
}

vector<string> InputParsing::splitStrings(string stringWithCommas, int numberOfSeparatedWords) {
    vector<string> separatedWords = vector<string>();
    for (int i = 1; i < numberOfSeparatedWords; i++) {
        string::size_type commaPosition = stringWithCommas.find(",");
        if (stringWithCommas.find(",") == string::npos) {
            //throw exception: too few commas in string
            LINFO << "InputParsing: too few commas in the input string";
            throw exception();
        }
        string nextWord = stringWithCommas.substr(0, commaPosition);
        separatedWords.push_back(nextWord);
        stringWithCommas = stringWithCommas.substr(commaPosition + 1);
    }
    if (stringWithCommas.find(",") != string::npos) {
        //throw exception: too many commas in string
        LINFO << "InputParsing: too many commas in the input string";
        throw exception();
    }
    separatedWords.push_back(stringWithCommas);
    return separatedWords;
}

void InputParsing::expectToNonNegativeNumber(string stringThatShouldRepresentNonNegativeInt) {
    if (stringThatShouldRepresentNonNegativeInt.find_first_not_of(digits) != std::string::npos) {
        LINFO << "InputParsing: the string" << stringThatShouldRepresentNonNegativeInt
        << "is not valid because it isn't a nonNegative number";
        throw exception();
    }
}

void InputParsing::expectToPositiveNumber(string stringThatShouldRepresentPositiveInteger) {
    this->expectToNonNegativeNumber(stringThatShouldRepresentPositiveInteger);
    if (stoi(stringThatShouldRepresentPositiveInteger) == 0) {
        LINFO << "InputParsing: the string" << stringThatShouldRepresentPositiveInteger
              << "is not valid because it is 0 and it isn't positive number";
        throw exception();
    }
}




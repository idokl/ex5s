#include <cstring>
#include "InputParsing.h"

//new functions
InputParsing::InputParsing() {
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
    int width = stoi(gridData.substr(0, spacePosition));
    int height = stoi(gridData.substr(spacePosition + 1));
    dimensions.gridHeight = height;
    dimensions.gridWidth = width;
    return dimensions;
}

Point InputParsing::parsePoint(string pointData) {
    vector<string> coordinates = this->splitStrings(pointData, 2);
    Point p = Point(stoi(coordinates[0]), stoi(coordinates[1]));
    return p;
}

InputParsing::parsedDriverData InputParsing::parseDriverData(string driverData) {
    vector<string> listOfStringsOfDriverParameters = this->splitStrings(driverData, 5);
    parsedDriverData driverParameters;
    driverParameters.id = stoi(listOfStringsOfDriverParameters[0]);
    driverParameters.age = stoi(listOfStringsOfDriverParameters[1]);
    //fix status of marriage according to the character in the third string of the list (S/M/D/W)
    driverParameters.status = mapOfLetterToStatus.at(listOfStringsOfDriverParameters[2]);
    driverParameters.yearsOfExperience = stoi(listOfStringsOfDriverParameters[3]);
    driverParameters.vehicleId = stoi(listOfStringsOfDriverParameters[4]);
    return driverParameters;
}

InputParsing::parsedTripData InputParsing::parseTripData(string tripData) {
    vector<string> listOfStringsOfTripParameters = this->splitStrings(tripData, 8);
    InputParsing::parsedTripData tripParameters;
    tripParameters.id = stoi(listOfStringsOfTripParameters[0]);
    int x = stoi(listOfStringsOfTripParameters[1]);
    int y = stoi(listOfStringsOfTripParameters[2]);
    tripParameters.start = Point(x, y);
    x = stoi(listOfStringsOfTripParameters[3]);
    y = stoi(listOfStringsOfTripParameters[4]);
    tripParameters.end = Point(x, y);
    tripParameters.numberOfPassengers = stoi(listOfStringsOfTripParameters[5]);
    tripParameters.tariff = stod(listOfStringsOfTripParameters[6]);
    tripParameters.time = stoi(listOfStringsOfTripParameters[7]);
    return tripParameters;
}

InputParsing::parsedCabData InputParsing::parseVehicleData(string vehicleData) {
    vector<string> listOfStringsOfCabParameters = this->splitStrings(vehicleData, 4);
    parsedCabData cabParameters;
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
        string nextWord = stringWithCommas.substr(0, commaPosition);
        separatedWords.push_back(nextWord);
        stringWithCommas = stringWithCommas.substr(commaPosition + 1);
    }
    separatedWords.push_back(stringWithCommas);
    return separatedWords;
}




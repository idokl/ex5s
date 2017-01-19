#define debugMassagesDriver

#include "Driver.h"
#include "easylogging++.h"

Driver::Driver(int id, int age, Status_Of_Marriage status, int yearsOfExperience, int vehicleId) :
        id(id), age(age), status(status), yearsOfExperience(yearsOfExperience),
        vehicleId(vehicleId),currentLocation(0,0) {
    numOfTrips = 1;
    averageSatisfactions = 0;
    currentTrip = NULL;
}

int Driver::getId() const {
    return id;
}

int Driver::getAge() const {
    return age;
}

Status_Of_Marriage Driver::getStatus() const {
    return status;
}

int Driver::getYearsOfExperience() const {
    return yearsOfExperience;
}

double Driver::getSatisfactions() const {
    return averageSatisfactions;
}

Cab* Driver::getInfoCab() const{
    return cabOfDriver;
}

Point Driver::currentPlace() {
    return currentLocation;
}

void Driver::clientSatisfactions(int clientSatisfactions){
    if((numOfTrips==0) || (numOfTrips ==1)){
        averageSatisfactions = clientSatisfactions;
    }else{
        averageSatisfactions = (averageSatisfactions+clientSatisfactions)/numOfTrips;
    }
    if(averageSatisfactions>5){
        averageSatisfactions=5;
    }else if(averageSatisfactions<1){
        averageSatisfactions =1;
    }
    numOfTrips++;
}

bool Driver::isAvailable() {
    return currentTrip != NULL;
}

void Driver::attachCabToDriver(Cab *infoCab) {
    cabOfDriver = infoCab;
}

vector<Passenger> Driver::getListOfPassengers() const{
    return listOfPassengers;
}

void Driver::addPassenger(Passenger passenger) {
    listOfPassengers.push_back(passenger);
}

void Driver::setCurrentLocation(Point point) {
    currentLocation = point;
}

void Driver::assignTrip(Trip *trip) {
        currentTrip = trip;
}

void Driver::moveOneStep() {
    //if no current trip, return:
    if (this->currentTrip == NULL) {
        return;
    }
    //if the driver has LuxuryCab, he has to skip one point of the path:
    if (this->cabOfDriver->getTaxiType() == LUXURY_CAB &&
            this->currentTrip->getPath().size() > 1) {
        this->currentTrip->removeNextPointOfPath();
    }
    //move to the next point (one block):
    Point locationAfterStep = this->currentTrip->getPath().front();
    this->currentTrip->removeNextPointOfPath();
    //Point locationAfterStep = nextPointOfPath.getValue();
    this->setCurrentLocation(locationAfterStep);
    //if the path was terminated, set "currentTrip" member to NULL:
    if (this->currentTrip->getPath().size() == 0) {
        delete this->currentTrip;
        this->currentTrip = NULL;
    }
    return;
}

//communicate with the server (documentation is in Driver.h file)
void Driver::run(Socket *socket) {
    char buffer[100000];
    stringstream ss;
    ss << this->id;
    //send our id to the server
    socket->sendData(ss.str(), 0);
    memset(buffer, 0, sizeof(buffer));
    //recive information about the driver's cab and create it
    socket->reciveData(buffer, sizeof(buffer), 0);
    string cabDataString = string(buffer);
    cabOfDriver = CabFactory::createCab(cabDataString);
    socket->sendData("thanks to Nevo", 0);
    memset(buffer, 0, sizeof(buffer));
    Trip *trip = NULL;
    while (true) {
        memset(buffer, 0, sizeof(buffer));
        socket->reciveData(buffer, sizeof(buffer), 0);
        string numberOfOption = string(buffer);
        switch (stoi(numberOfOption)) {
            //case7: exit
            case 7: {
                LINFO << "Driver: case7 - exit";
                //terminate the program
                delete cabOfDriver;
                return;
            }
            //case9: move one step and send our current location to the server
            case 9: {
                LINFO << "Driver: case9 (moveOneStep) begin";
                moveOneStep();
                LINFO << "current location after move" << this->currentLocation;
                //serialization:
                SerializationClass<Point> serializeClass;
                string serializedPointStr =
                        serializeClass.serializationObject(this->currentPlace());
                //pass point to server
                socket->sendData(serializedPointStr, 0);
                memset(buffer, 0, sizeof(buffer));
                LINFO << "Driver: case9 end";
                break;
            }
            //case10: assign a trip.
            case 10: {
                LINFO << "Driver: case10 (receiving trip) begin";
                socket->sendData("thanks to Nevo", 0);
                memset(buffer, 0, sizeof(buffer));
                socket->reciveData(buffer, sizeof(buffer),0);
                socket->sendData("thanks to Nevo", 0);
                string strTrip(buffer, sizeof(buffer));
                SerializationClass<Trip *> serializeTripClass;
                trip = serializeTripClass.deSerializationObject(strTrip, trip);
                assignTrip(trip);
                LINFO << "Driver: case10 end";
                break;
            }
            default: {
                break;
            }
        }
    }
}
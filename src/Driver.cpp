#include "Driver.h"

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

//communicate with the server
void Driver::run(Socket *socket) {
    char buffer[1024];
    stringstream ss;
    ss << this->id;
    socket->sendData(ss.str(), 0);

    socket->reciveData(buffer, sizeof(buffer), 0);
    string cabDataString = string(buffer);
    cabOfDriver = CabFactory::createCab(cabDataString);
    Trip *trip = NULL;
    while (true) {
        socket->reciveData(buffer, sizeof(buffer), 0);
        string numberOfOption = string(buffer);
        switch (stoi(numberOfOption)) {
            case 4: {
                //serialization:
                SerializationClass<Point> serializeClass;
                string serializedPointStr =
                        serializeClass.serializationObject(this->currentPlace());
                //pass point to server
                socket->sendData(serializedPointStr, 0);
                break;
            }
            case 7: {
                //terminate the program
                delete cabOfDriver;
                return;
            }
            case 9: {
                moveOneStep();
                break;
            }
            //option 10: assign a trip.
            case 10: {
                socket->reciveData(buffer, sizeof(buffer),0);
                string strTrip(buffer, sizeof(buffer));
                SerializationClass<Trip *> serializeTripClass;
                trip = serializeTripClass.deSerializationObject(strTrip, trip);
                assignTrip(trip);
                break;
            }
            default: {
                break;
            }
        }
    }
}
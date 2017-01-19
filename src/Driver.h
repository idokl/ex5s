#ifndef EX1_VERSION_1_1_DRIVER_H
#define EX1_VERSION_1_1_DRIVER_H

#include <vector>

#include "Trip.h"
#include "Socket.h"
#include "SerializationClass.h"
#include "CabFactory.h"
#include <string.h>

//typedef enum {SINGLE, MARRIED, DIVORCED, WIDOWED} Status_Of_Marriage;

class Driver {
private:
    int id;
    int age;
    double averageSatisfactions;
    Status_Of_Marriage status;
    int yearsOfExperience;
    int vehicleId;
    Cab *cabOfDriver;
    int numOfTrips;
    Point currentLocation;
    vector<Passenger> listOfPassengers;
    Trip *currentTrip;
public:

    Driver(int id, int age, Status_Of_Marriage status, int yearsOfExperience, int vehicleId);

    int getId() const;

    int getAge() const;

    Status_Of_Marriage getStatus() const;

    int getYearsOfExperience() const;

    void attachCabToDriver(Cab *infoCab);

    double getSatisfactions() const;

    Cab *getInfoCab() const;

    bool isAvailable();

    Point currentPlace();

    void setCurrentLocation(Point point);

    void clientSatisfactions(int clientSatisfactions);

    vector<Passenger> getListOfPassengers() const;

    void addPassenger(Passenger passenger);

    void assignTrip(Trip *trip);

    /*
     * move one step - change the location of the driver according to the current trip.
     */
    void moveOneStep();

    /*
     * activate the driver - communicate with the server.
     * get massages from the server and action accordingly:
     * - create the driver's cab.
     * - support options of getting trip, moving one step and terminating of the program.
     */
    void run(Socket *socket);
};

#endif

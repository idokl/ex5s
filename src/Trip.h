#ifndef EX2_TRIP_H
#define EX2_TRIP_H

#include <list>
#include <stack>
#include "Point.h"
#include "Passenger.h"
#include <boost/serialization/list.hpp>

class Trip {

private:
    int rideId;
    int passedMeters; //total meters passed at the current point
    Point startingPoint;
    Point endingPoint;
    list <Point> nextPointsOfPath;
    list <Passenger> customers; //passengers
    int numOfPassengers;
    double rateOfTrip; //tariff
    int time;
public:

    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive &ar, const unsigned int version) {
        ar & rideId;
        ar & passedMeters;
        ar & startingPoint;
        ar & endingPoint;
        ar & nextPointsOfPath;
        ar & customers;
        ar & numOfPassengers;
        ar & rateOfTrip;
        ar & time;
    }

    Trip();

    Trip(int rideId, Point startingPoint, Point endingPoint, int numOfPassengers, double taarif //);
        , stack <Node<Point>> pathWithoutStartingPoint, int time);

    int getRideId() const;

    int getPassedMeters() const;

    int getNumberOfPassengers() const;

    const Point &getStartingPoint() const;

    const Point &getEndingPoint() const;

    const list <Passenger> &getCustomers() const;

    double getRateOfTrip() const;

    const list<Point> &getPath() const;

    void removeNextPointOfPath();
    int getTime();

    ~Trip();
};


#endif

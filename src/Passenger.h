#ifndef EX1_VERSION_1_1_PASSENGER_H
#define EX1_VERSION_1_1_PASSENGER_H

#include "Point.h"

class Passenger {
private:
    Point source;
    Point destination;
    bool isLuxury;

public:

    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive &ar, const unsigned int version) {
        ar & source;
        ar & destination;
        ar & isLuxury;
    }

    Passenger();

    Passenger(Point source, Point destination, bool isLuxury);

    int getSatisfaction() const;

    bool getIsLuxury();

    int giveFeedBack();
};


#endif

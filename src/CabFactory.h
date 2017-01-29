#ifndef EX4_CABFACTORY_H
#define EX4_CABFACTORY_H

#include "Cab.h"
#include "StandardCab.h"
#include "LuxuryCab.h"
#include "InputParsing.h"

using namespace std;

class CabFactory {
public:
    static Cab *createCab(InputParsing::parsedCabData cabData);
};

#endif //EX4_CABFACTORY_H

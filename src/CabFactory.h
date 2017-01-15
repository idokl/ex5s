#ifndef EX4_CABFACTORY_H
#define EX4_CABFACTORY_H
#include "Cab.h"
#include "InputParsing.h"
#include "StandardCab.h"
#include "LuxuryCab.h"

class CabFactory {
public:
    static Cab *createCab(string inputString);
};

#endif //EX4_CABFACTORY_H

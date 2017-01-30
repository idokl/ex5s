#include "CabFactory.h"

Cab *CabFactory::createCab(InputParsing::parsedCabData cab) {
    InputParsing inputParsing;
    if (cab.taxiType == 1) {
        Cab *standard = new StandardCab(cab.id, cab.manufacturer, cab.color);
        return standard;
    } else {
        Cab *luxury = new LuxuryCab(cab.id, cab.manufacturer, cab.color);
        return luxury;
    }
}
#include "gtest/gtest.h"
#include "../src/Cab.h"
#include "../src/StandardCab.h"
#include "../src/LuxuryCab.h"

class TestCabs: public ::testing::Test {

protected:
    Cab* standardCab;
    Cab* luxuryCab;

    virtual void SetUp() {
        standardCab = new StandardCab(1234567, SUBARO, WHITE);
        luxuryCab = new LuxuryCab(7654321, TESLA, RED);
    }

    virtual void TearDown() {
        delete standardCab;
        delete luxuryCab;
    }
};

TEST_F(TestCabs, GettersCheck) {
    ASSERT_EQ(standardCab->getId(),1234567);
    ASSERT_EQ(luxuryCab->getId(),7654321);

    ASSERT_EQ(standardCab->getPassedKilometers(), 0) << "ERROR:"
                        "passed kilometers of constructed cab isn't 0";
    ASSERT_EQ(luxuryCab->getPassedKilometers(), 0) << "ERROR:"
                        "passed kilometers of constructed cab isn't 0";

    ASSERT_EQ(standardCab->getCarModel(),SUBARO);
    ASSERT_EQ(luxuryCab->getCarModel(),TESLA);

    ASSERT_EQ(standardCab->getColor(),WHITE);
    ASSERT_EQ(luxuryCab->getColor(),RED);
}


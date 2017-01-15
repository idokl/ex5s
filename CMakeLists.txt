cmake_minimum_required(VERSION 3.6)
project(ex5s)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -pthread" )
set(BOOST_ROOT CACHE PATH "/usr/include/boost/")
find_package(Boost 1.58.0 COMPONENTS serialization system)
include_directories(${Boost_INCLUDE_DIR})

add_subdirectory(lib/googletest-master)
include_directories(lib/googletest-master/googletest/include)
include_directories(lib/googletest-master/googlemock/include)


set(SOURCE_FILES src/BfsAlgorithm.h src/Grid.cpp src/Grid.h
        src/Node.h src/Graph.h src/Point.cpp src/Point.h src/InputParsing.cpp src/InputParsing.h
        src/ProgramFlow.cpp src/ProgramFlow.h src/TaxiCenter.cpp src/TaxiCenter.h src/Passenger.cpp
        src/Passenger.h src/Driver.cpp src/Driver.h src/Trip.cpp src/Trip.h src/Cab.cpp src/Cab.h
        src/StandardCab.cpp src/StandardCab.h src/LuxuryCab.cpp src/LuxuryCab.h src/Socket.cpp src/Socket.h src/SerializationClass.h src/CabFactory.cpp src/CabFactory.h src/Tcp.cpp src/Tcp.h)

set(TEST_FILES Tests/TestPoint.cpp Tests/TestProgramFlow.cpp
        Tests/TestDriver.cpp Tests/TestGrid.cpp Tests/TestTrip.cpp Tests/TestCabs.cpp
        Tests/TestInputParsing.cpp Tests/TestTaxiCenter.cpp Tests/TestBfsAlgorithm.cpp Tests/TestPassenger.cpp)

add_executable(server ${SOURCE_FILES} ${TEST_FILES} src/Server.cpp)
add_executable(client ${SOURCE_FILES} ${TEST_FILES} src/Client.cpp)

target_link_libraries(server gtest ${Boost_LIBRARIES})
target_link_libraries(client gtest ${Boost_LIBRARIES})
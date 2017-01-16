#ifndef EX1_VERSION_1_1_PROGRAMFLOW_H
#define EX1_VERSION_1_1_PROGRAMFLOW_H

#include <vector>
#include <string>
#include <iostream>
#include "Point.h"
#include "Grid.h"
#include "Driver.h"
#include "StandardCab.h"
#include "LuxuryCab.h"
#include "BfsAlgorithm.h"
#include "TaxiCenter.h"
#include "SerializationClass.h"
#include "CabFactory.h"
#include <pthread.h>

/*
 * This class manage the flow of the program.
 * It has methods for creating the grid (map), taxi center, drivers and cabs.
 * The run() method is responsible to get data from the user about the grid and
 * the wanted operations and to perform them.
 *
 */
class ProgramFlow {
public:
    /*
     * run the program menu in order to get the user input and do the suitable operations:
     * The first thing which the program takes as input is the size of the grid.
     * The next thing, is the number of obstacles inside the map.
     * Then, if there are any obstacles, the program will expect their locations.
     * After all the relevant input regarding the grid is given,
     * the program should expect a number which represents a command.
     * Here are all the possible inputs it can get:
     * 1 - expect to create communication with the drivers. receive their id and send them
     *     data about their vehicles.
     * 2 - insert a new ride: (id,x_start,y_start,x_end,y_end,num_passengers,tariff,time_of_start)
     *                                              - (int,int,int,int,int,int,double,int)
     * 3 - insert a vehicle: (id,taxi_typ e,manufacturer,color)
     *                  - (int,{1: NormalCab,2:LuxuryCab},char:{H,S,T,F},char:{R,B,G,P,W})
     * 4 - request for a driver location: (driver_id)
     * output: driver location in the format: '(x,y)'.
     * 7 - exit (cleaning up the program and exiting).
     * 9 - advance the time and do the required operations (assigning trips to the drivers
     *     when the starting time of these trips is arriving, advancing of the relevant drivers)
    */
    static void run(Socket *socket);

    /*
     * create taxi center. the taxi center has to contain "bfs" (that know the grid and the
     * obstacles) in order to be able to navigate.
     */
    static TaxiCenter createTaxiCenter(BfsAlgorithm<Point> bfs);

    //create rectangular matrix with list of impassable points
    static Graph<Point> *createGrid(int width, int height, vector<Point> listOfObstacles);
    static void* threadsRun(void *);
    struct threadData{
        Socket *socket;
        int socketDescriptor;
        TaxiCenter *taxiCenter;
    };

    static int acceptConnection(Socket *socket);
};

#endif //EX1_VERSION_1_1_PROGRAMFLOW_H

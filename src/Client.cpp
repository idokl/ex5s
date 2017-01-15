#include <string>
#include "ProgramFlow.h"
#include "Tcp.h"

using namespace std;

int main(int argc, char *argv[]) {
    Socket* socket = NULL;
    if (argc == 3) {
        int port = atoi(argv[2]);
        // 0: isServer = false,
        // port: port of the client
        socket = new Tcp(0, port, argv[1]);
        socket->initialize();

        InputParsing inputParsing = InputParsing();
        string inputString;
        //insert a driver in the following format: (id,age,status,exp erience,vehicle_id)
        //                                              - (int,int,char:{S,M,D,W},int,int)
        getline(cin, inputString);
        InputParsing::parsedDriverData driverData = inputParsing.parseDriverData(inputString);
        try {
            Driver driver = Driver(driverData.id, driverData.age, driverData.status, driverData.yearsOfExperience,
                                   driverData.vehicleId);
            driver.run(socket);
        } catch (const char *msg) {
            cerr << msg << endl;
        }
    }
    delete socket;
    return 0;
}
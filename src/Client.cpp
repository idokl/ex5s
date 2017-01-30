#include <string>
#include "ProgramFlow.h"
#include "Tcp.h"
#include "easylogging++.h"

_INITIALIZE_EASYLOGGINGPP

using namespace std;

int main(int argc, char *argv[]) {
    Socket *socket = NULL;
    if (argc == 3) {
        int port = atoi(argv[2]);
        InputParsing inputParsing = InputParsing();
        string inputString;
        //insert a driver in the following format: (id,age,status,exp erience,vehicle_id)
        //                                              - (int,int,char:{S,M,D,W},int,int)
        InputParsing::parsedDriverData driverData;
        while (true) {
            try {
                getline(cin, inputString);
                driverData = inputParsing.parseDriverData(inputString);
                break;
            } catch (std::exception &e) {
                LINFO << "client: the driver data isn't valid. we will try again to receive it";
                delete socket;
                return 0;
            }
        }
        socket = new Tcp(0, port, argv[1]);
        socket->initialize();
        LINFO << "Client initialized";
        try {
            Driver driver = Driver(driverData.id, driverData.age, driverData.status,
                                   driverData.yearsOfExperience, driverData.vehicleId);
            driver.run(socket);
        } catch (const char *msg) {
            cerr << msg << endl;
        }
    }
    delete socket;
    return 0;
}
#include <iostream>
#include <QtCore>
#include "../test/peripheralTester.hpp"
#include "../test/rtsptester.h"
using namespace std;;

int main(int argc, char ** argv)
{
    QCoreApplication app(argc, argv);

    //sample usage for periphera tester with /dev/ttyUSB0 and baud rate 9600
//    PeripheralTester test("/dev/ttyUSB0", 9600);
   // cout << " Entry point of project" << endl;

    RTSPTester rtspTestr;

    return app.exec();
}

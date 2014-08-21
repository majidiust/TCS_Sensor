#include <iostream>
#include <QtCore>
#include "kigManager.hpp"
using namespace std;;

int main(int argc, char ** argv)
{
    QCoreApplication app(argc, argv);

    //sample usage for periphera tester with /dev/ttyUSB0 and baud rate 9600
    KIGManager kigManager("/dev/ttyUSB0", 9600);

    return app.exec();
}

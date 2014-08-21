#include <QtCore>
#include <iostream>
#include "kigManager.hpp"
using namespace std;
int main(int argc, char ** argv)
{
    QCoreApplication app(argc, argv);
    //sample usage for periphera tester with /dev/ttyUSB0 and baud rate 9600
    KIGManager kigManager;
    kigManager.start();
    return app.exec();
}

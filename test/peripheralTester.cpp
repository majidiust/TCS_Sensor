#include "peripheralTester.hpp"

PeripheralTester::PeripheralTester(std::string port, unsigned int baudRate)
{
    m_peripheral = new Peripheral();
    connectToPeripheral();
    m_peripheral->openPort(port, baudRate);
    m_peripheral->start();
}

void PeripheralTester::connectToPeripheral()
{
    m_connection = m_peripheral->connectExternalEventHandler(boost::bind(&PeripheralTester::newEventHandler, this, _1));
    m_connection = m_peripheral->connectCommandHandler(boost::bind(&PeripheralTester::newCommandHandler, this, _1));
}

void PeripheralTester::disconnectFromPeripheral()
{
    m_peripheral->disconnect(m_connection);
}

void PeripheralTester::newEventHandler(std::string arg)
{
    std::cout << " new event : " << arg << endl;
}

void PeripheralTester::newCommandHandler(int commandType)
{
    switch(commandType)
    {
    case Peripheral::CTBegin:
        std::cout << "Vehicle enter" << endl;
        break;
    case Peripheral::CTCalibration:
        std::cout << "Callibration" << endl;
        break;
    case Peripheral::CTEnd:
        std::cout << "Vehicle exit" << endl;
        break;;
    case Peripheral::CTNode:
        std::cout << " None commande" << endl;
        break;
    case Peripheral::CTPowerOn:
        std::cout << " Power On" << endl;
        break;
    case Peripheral::CTTakePicture:
        std::cout << "Take picture" << endl;
        break;
    }
}

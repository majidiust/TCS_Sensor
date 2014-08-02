#include "peripheralTester.hpp"

PeripheralTester::PeripheralTester(std::string port, unsigned int baudRate)
{
    m_peripheral = new Peripheral();
    m_peripheral->openPort(port, baudRate);
}

void PeripheralTester::connectToPeripheral()
{
    m_connection = m_peripheral->connectExternalEventHandler(boost::bind(&PeripheralTester::newEventHandler, this, _1));
}

void PeripheralTester::disconnectFromPeripheral()
{
    m_peripheral->disconnect(m_connection);
}

void PeripheralTester::newEventHandler(std::string arg)
{
    std::cout << " new event : " << arg << endl;
}
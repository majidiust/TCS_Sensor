#ifndef PERIPHERALTESTER_HPP
#define PERIPHERALTESTER_HPP
#include "../peripheral/peripheral.hpp"

class PeripheralTester
{
public:
    PeripheralTester(std::string port, unsigned int baudRate);
    void	connectToPeripheral();
    void	disconnectFromPeripheral();
    void	newEventHandler(std::string arg);
    void    newCommandHandler(int commandType);
private:
    Peripheral *m_peripheral;
    Peripheral::PeripheralConnection m_connection;
};
#endif

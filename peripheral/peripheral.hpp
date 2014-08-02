#ifndef PERIPHERAL_HPP
#define PERIPHERAL_HPP

#include <iostream>
#include "../base.hpp"

using namespace std;

class Peripheral : public Base
{
    public:
	typedef boost::signal2::signal<void(char)> KeyPressed;	
	typedef boost::signal2::signa<void (std::string)> ExternalEvent;
	typedef boost::signal2::connection PeripheralConnection;
	bool openPort(std::string port, unsigned int baudRate);
	Peripheral();
	~Peripheral();
	void writeString(std::string str);
	void receiver();
	void start();
	void stop();
	char* getHex(char c);
	
	PeripheralConnection connectExternalEventHandler(const ExternalEvent::slot_type &slot);
	void disconnect(PeripheralConnection connection);

	void announcePowerOff();
	void announcePowerOn();
	void announceNetworkConnected();
	void announceNetworkDisconnected();
	void announceError();
	void announceCameraError();
};
#endif



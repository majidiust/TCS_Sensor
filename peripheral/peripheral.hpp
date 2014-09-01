#ifndef PERIPHERAL_HPP
#define PERIPHERAL_HPP

#include <iostream>
#include "../base.hpp"

using namespace std;

class Peripheral : public Base
{
    public:

    enum CommandType{
        CTBegin = 4,
        CTEnd = 5,
        CTTakePicture = 3,
        CTPowerOn = 1,
        CTCalibration = 2,
        CTNode = 6
    };

	typedef boost::signals2::signal<void(char)> KeyPressed;	
	typedef boost::signals2::signal<void (std::string)> ExternalEvent;
    typedef boost::signals2::signal<void (int)> CommandEvent;
	typedef boost::signals2::connection PeripheralConnection;

	bool openPort(std::string port, unsigned int baudRate);
	Peripheral();
	~Peripheral();
	void writeString(std::string str);
	void receiver();
	void start();
	void stop();
	char* getHex(char c);
	
	PeripheralConnection connectExternalEventHandler(const ExternalEvent::slot_type &slot);
    PeripheralConnection connectCommandHandler(const CommandEvent::slot_type &slot);
	void disconnect(PeripheralConnection connection);

	void announcePowerOff();
	void announcePowerOn();
	void announceNetworkConnected();
	void announceNetworkDisconnected();
	void announceError();
	void announceCameraError();
    
    private:
	bool checkCrc(unsigned char * crc, unsigned char * data, unsigned int size);

	boost::asio::io_service *m_io;
	boost::asio::serial_port *m_serial;
	bool m_isRun;
	boost::mutex m_lock;
	boost::shared_ptr<boost::thread> m_receiverThread;
    mutable	ExternalEvent m_externalEvent;
    mutable CommandEvent m_commandEvent;
};
#endif



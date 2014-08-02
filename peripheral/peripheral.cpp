#include "peripheral.hpp"

bool Peripheral::openPort(std::string port, unsigned int baud_rate)
{
    try
    {
	m_io = new boost::asio::io_service();
	m_serial = new boost::asio::serial_port(*m_io, port);
	m_serial->set_option(boost::asio::serial_port_base::baud_rate(baud_rate));
	m_serial->set_option(boost::asio::serial_port_base::character_size(8));
	m_serial->set_option(boost::asio::serial_port::stop_bits(boost::asio::serial_port_base::stop_bits::one));
	m_serial->set_option(boost::asio::serial_port::parity(boost::asio::serial_port_base::parity::none));    
	m_isRun = true;
        return true;
    }
    catch(boost::exception &ex)
    {
	stdLog("Error in openning serial");
	return false;
    }
}

Peripheral::Peripheral()
{
    stdLog("Peripheral constructor");
}

Peripheral::~Peripheral()
{
    stop();
}

void Peripheral::writeString(std::string s)
{
    boost::asio::write(*m_serial, boost::asio::buffer(s.c_str(), s.size()));
}

void Peripheral::receiver()
{
    while(m_isRun)
    {
    
    }
}

void Peripheral::start()
{
    m_receiverThread = boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&Peripheral::receiver, this)));
}

void Peripheral::stop()
{
    boost::mutex::scoped_lock l(m_lock);
    m_isRun = false;
    
}

Peripheral::PeripheralConnection Peripheral::connectExternalEventHandler(const ExternalEvent::slot_type &slot)
{
    return m_externalEvent.connect(slot);
}

void Peripheral::disconnect(PeripheralConnection connection)
{
    connection.disconnect();
}

bool Peripheral::checkCrc(unsigned char * crc, unsigned char *data, unsigned int size)
{
    unsigned int currentCRC = 0 ;
    for(unsigned int i = 0 ; i < size - 1 ; i ++)
    {
	currentCRC = (currentCRC & 0xFF) + data[i];
    }
    currentCRC = currentCRC & 0xFF;
    if(currentCRC == (int)crc[0])
	return true;
    else return false;
} 
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
    using namespace boost;
//    std::cout << m_isRun << endl;
//    std::cout << __FUNCTION__ << endl;
    while(m_isRun)
    {
        std::cout << " In endless loop" << endl;
        unsigned char * sot  = new unsigned char[1];
        unsigned char * id   = new unsigned char[1];
        unsigned char * cmd  = new unsigned char[1];
        unsigned char * s = new unsigned char[1];

        asio::read(*m_serial, asio::buffer(sot, 1));
        asio::read(*m_serial, asio::buffer(id, 1));
        asio::read(*m_serial, asio::buffer(cmd, 1));
        asio::read(*m_serial, asio::buffer(s, 1));

        unsigned int size = (int)s[0];

        unsigned char * data = new unsigned char[size];
        for(int i = 0 ; i < size ; i++)
        {
            unsigned char *tempBuffer = new unsigned char[1];
            asio::read(*m_serial, asio::buffer(tempBuffer, 1));
            data[i] = tempBuffer[0];
            std::cout << getHex(data[i]);
        }

        unsigned char * crc = new unsigned char[1];
        unsigned char * eot = new unsigned char[1];
        asio::read(*m_serial, asio::buffer(crc, 1));
        asio::read(*m_serial, asio::buffer(eot, 1));

        //         cout << getHex(sot[0]) << ":" << getHex(id[0]) << " : " << getHex(cmd[0]) << " : " << getHex(s[0]) << endl;

        m_commandEvent((int)cmd[0]);
    }
}

void Peripheral::start()
{
    m_isRun = true;
    m_receiverThread = boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&Peripheral::receiver, this)));
}

void Peripheral::stop()
{
    boost::mutex::scoped_lock l(m_lock);
    m_isRun = false;
}

char* Peripheral::getHex(char c)
{
    char *f = new char[8];

    sprintf(f, "%x ", (c&0x000000ff));
    return f;
}

Peripheral::PeripheralConnection Peripheral::connectExternalEventHandler(const ExternalEvent::slot_type &slot)
{
    return m_externalEvent.connect(slot);
}

Peripheral::PeripheralConnection Peripheral::connectCommandHandler(const CommandEvent::slot_type &slot)
{
    return m_commandEvent.connect(slot);
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

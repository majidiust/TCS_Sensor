#include "kigManager.hpp"
#include "settings.hpp"

KIGManager::KIGManager(){
    std::cout << "Kaveh industrial group control traffic system" << endl;
}

void KIGManager::start(){
    std::cout << " Started KIG Agent ............ " << endl;
    m_db.connectToDB();
    m_db.loadSettings();
    m_peripheral = new Peripheral();
    m_connection = m_peripheral->connectExternalEventHandler(boost::bind(&KIGManager::newEventHandler, this, _1));
    m_connection = m_peripheral->connectCommandHandler(boost::bind(&KIGManager::newCommandHandler, this, _1));
    m_lastStep = 0;
    tryOpenPort();
}

void KIGManager::disconnectFromPeripheral()
{
    m_peripheral->disconnect(m_connection);
}

void KIGManager::tryOpenPort(){
    do{
        usleep(Settings::ReTryOpenSerialPort);
        std::cout << "Try open serial port ..." << " baudrate : " << Settings::Baudrate << " : port /dev/ttyUSB0" << endl;
    }while(!m_peripheral->openPort("/dev/ttyUSB0", Settings::Baudrate));
    std::cout<< "Openning theport" << std::endl;
    m_peripheral->start();
}

void KIGManager::newEventHandler(std::string arg)
{
    std::cout << " new event : " << arg << endl;
}

void KIGManager::newCommandHandler(int commandType)
{
    switch(commandType)
    {
    case Peripheral::CTBegin:
        std::cout << "Vehicle enter" << endl;
        m_lastStep = 1;
        saveEventBegin();
        break;
    case Peripheral::CTCalibration:
        std::cout << "Callibration" << endl;
        break;
    case Peripheral::CTEnd:
        std::cout << "Vehicle exit" << endl;
        saveEventEnd();
        break;;
    case Peripheral::CTNode:
        std::cout << " None commande" << endl;
        break;
    case Peripheral::CTPowerOn:
        std::cout << " Power On" << endl;
        break;
    case Peripheral::CTTakePicture:
        std::cout << "Take picture" << endl;
        saveEventOnDemand();
        break;
    }
}

void KIGManager::saveEventBegin(){
    QString id = m_db.insertNewTraffic();
    std::cout << id.toStdString() << endl;
    m_client = new RTSPClient(id, QString::fromStdString(Settings::RTSPUrl), QString::fromStdString(Settings::FPS));
    m_client->start();
}

void KIGManager::saveEventEnd(){
    std::cout << "saveEventEnd" << endl;
    m_client->stopProcessDelayed();
}

void KIGManager::saveEventOnDemand(){
    QString id = m_db.insertNewTraffic();
    std::cout << id.toStdString() << endl;
    m_client = new RTSPClient(id, QString::fromStdString(Settings::RTSPUrl), QString::fromStdString(Settings::FPS));
    m_client->start();
    m_client->stopProcessDelayed();
}

#include "kigManager.hpp"

KIGManager::KIGManager(std::string port, int baudRate){
    m_db.connectToDB();
    m_peripheral = new Peripheral();
    m_connection = m_peripheral->connectExternalEventHandler(boost::bind(&KIGManager::newEventHandler, this, _1));
    m_connection = m_peripheral->connectCommandHandler(boost::bind(&KIGManager::newCommandHandler, this, _1));
    m_peripheral->openPort(port, baudRate);
    m_peripheral->start();
    m_lastStep = 0;
}

void KIGManager::disconnectFromPeripheral()
{
    m_peripheral->disconnect(m_connection);
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
    m_client = new RTSPClient(id, "rtsp://admin:admin@192.168.2.1:554/mpeg", "fps=5/10");
    m_client->start();
}

void KIGManager::saveEventEnd(){
    std::cout << "saveEventEnd" << endl;
    m_client->stopProcessDelayed();
}

void KIGManager::saveEventOnDemand(){
    QString id = m_db.insertNewTraffic();
    std::cout << id.toStdString() << endl;
    m_client = new RTSPClient(id, "rtsp://admin:admin@192.168.2.1:554/mpeg", "fps=5/10");
    m_client->start();
    m_client->stopProcessDelayed();
}

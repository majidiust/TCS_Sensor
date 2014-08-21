#ifndef KIGMANAGER_H
#define KIGMANAGER_H

#include <QObject>
#include "db/mongo.hpp"
#include "peripheral/peripheral.hpp"
#include "rtsp/rtspclient.hpp"

class KIGManager: public QObject
{
    Q_OBJECT    

    public:
        KIGManager(std::string port, int baudRate);
        void	newEventHandler(std::string arg);
        void    newCommandHandler(int commandType);
        void    disconnectFromPeripheral();
        void    saveEventBegin();
        void 	saveEventEnd();
        void    saveEventOnDemand();

    private:    
        RTSPClient *m_client;
        MongoDB m_db;
        Peripheral *m_peripheral;
        Peripheral::PeripheralConnection m_connection;
        int m_lastStep;
};
#endif

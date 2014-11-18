#ifndef KIGMANAGER_H
#define KIGMANAGER_H

#include <QObject>
#include <QImage>
#include <QColor>
#include <QRgb>
#include <QVector>
#include "db/mongo.hpp"
#include "peripheral/peripheral.hpp"
#include "rtsp/rtspclient.hpp"
#include <vector>
#include <map>

using namespace std;
class PlateProfile{
public:
    std::string englishPlate;
    std::wstring persianPlate1;
    std::wstring persianPlate2;
    int count;
};


class KIGManager: public QObject
{
    Q_OBJECT    

    public:
        KIGManager();
        void    start();
        void	newEventHandler(std::string arg);
        void    newCommandHandler(int commandType);
        void    disconnectFromPeripheral();
        void    saveEventBegin();
        void 	saveEventEnd();
        void    saveEventOnDemand();
        void	tryOpenPort();

        void    plateDetectorHandler(std::string id);

public slots:
        void    OnStopRTSP(QString id);

    private:    
        vector<RTSPClient*> m_client;
        std::string m_root;
        MongoDB m_db;
        Peripheral *m_peripheral;
        Peripheral::PeripheralConnection m_connection;
        int m_lastStep;
        std::vector<PlateProfile> m_detectedPlates;
        std::vector<PlateProfile> m_lastPlates;
};


#endif

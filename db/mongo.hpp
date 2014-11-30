#ifndef MONGODB_H
#define MONGODB_H

#include <mongo/client/dbclient.h>
#include <QString>
#include <QDebug>
#include <iostream>
#include <cstring>
#include <sstream>
#include <QDate>
#include <boost/thread/thread.hpp>
#include "utility/cdate.hpp"
#include "camera.hpp"
#include <vector>

using namespace std;
using namespace mongo;
using mongo::be;
using mongo::bo;
using mongo::bob;

class MongoDB{
public:
    MongoDB();
    void connectToDB(QString dbUrl = "localhost");
    QString insertNewTraffic();
    void makeRecordValid(QString id);
    void insertPlateForRecord(QString is,QString englishPlate, QString persianPlate1, QString persianPlate2,QString image);
    void closeConnection();
    std::string getCollectionName();
    std::string getSettingCollectionName();
    std::string getCameraCollectionName();
    std::string getProfileCollectionName();
    void    loadSettings();
    vector<Camera*> getAllCamera();
    void insertNewTrafficHandler();
    void tryToReconnect();
    QString getRelatedProfile(QString profile);
private:
    mongo::DBClientConnection m_connection;
    QString m_dbName;
    QString m_traficCollectionName;
    QString m_settingsCollectionName;
    QString m_cameraCollectionName;
    QString m_profileCollectionName;
};

#endif

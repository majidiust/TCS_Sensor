#ifndef MONGODB_H
#define MONGODB_H

#include <mongo/client/dbclient.h>
#include <QString>
#include <QDebug>
#include <iostream>
#include <cstring>
#include <sstream>
#include <QDate>
#include "utility/cdate.hpp"

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
    void    loadSettings();
private:
    mongo::DBClientConnection m_connection;
    QString m_dbName;
    QString m_traficCollectionName;
    QString m_settingsCollectionName;
};

#endif

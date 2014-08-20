#ifndef MONGODB_H
#define MONGODB_H

#include <mongo/client/dbclient.h>
#include <QString>
#include <QDebug>
#include <iostream>
#include <cstring>
#include <QDate>
#include "utility/cdate.hpp"

class MongoDB{
public:
    MongoDB();
    void connectToDB(QString dbUrl = "localhost");
    QString insertNewTraffic();
    void closeConnection();
    std::string getCollectionName();
private:
    mongo::DBClientConnection m_connection;
    QString m_dbName;
    QString m_traficCollectionName;
};

#endif

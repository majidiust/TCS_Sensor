#ifndef MONGODB_H
#define MONGODB_H

#include <mongo/client/dbclient.h>
#include <QString>
#include <QDebug>
#include <iostream>
#include <cstring>

class MongoDB{
public:
    MongoDB();
    void connectToDB(QString dbUrl = "localhost");
    void insertNewTraffic();
    void closeConnection();
    std::string getCollectionName();
private:
    mongo::DBClientConnection m_connection;
    QString m_dbName;
    QString m_traficCollectionName;
};

#endif

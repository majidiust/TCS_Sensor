#include "mongo.hpp"

void MongoDB::connectToDB(QString dbUrl){
    try
    {
        m_connection.connect(dbUrl.toStdString());
        qDebug() << "Connected to database successfully";
    }
    catch(const mongo::DBException &e){
        qDebug() << e.what();
    }
}

void MongoDB::closeConnection()
{
    try
    {

        qDebug() << "disConnected to database successfully";
    }
    catch(const mongo::DBException &e){
        qDebug() << e.what();
    }
}

void MongoDB::insertNewTraffic(){
    mongo::BSONObjBuilder obj;

    m_connection.insert(getCollectionName(), obj.obj());
}

MongoDB::MongoDB(){
    m_dbName = "KIG";
    m_traficCollectionName = "traffic";
}

std::string MongoDB::getCollectionName(){
    return (m_dbName + "." + m_traficCollectionName).toStdString();
}

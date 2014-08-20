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

QString MongoDB::insertNewTraffic(){

    CDate::Now n;
    std::string jdate = CDate::DateConv::ToJalali(n);
    QString date = QTime::currentTime().toString();
    qDebug() << "Jalali date is : " << QString::fromStdString(jdate);
    qDebug() << "time is : " << date;
    mongo::BSONObj obj =  mongo::BSONObjBuilder().genOID().append("date" , jdate).append("time", date.toStdString()).obj();
    m_connection.insert(getCollectionName(), obj);
    qDebug() << "object id is : " << QString::fromStdString(obj["_id"]).split(":")[1] << endl;
    return QString::fromStdString(obj["_id"]).split(":")[1];
}

MongoDB::MongoDB(){
    m_dbName = "KIG";
    m_traficCollectionName = "traffic";
}

std::string MongoDB::getCollectionName(){
    return (m_dbName + "." + m_traficCollectionName).toStdString();
}

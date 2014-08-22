#include "mongo.hpp"
#include "settings.hpp"

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

void MongoDB::loadSettings(){
    bool isThereSettings = false;
    std::auto_ptr<mongo::DBClientCursor> cursor =
            m_connection.query(this->getSettingCollectionName());
    if (cursor->more()) {
        std::cout << "settings readed from database" << std::endl;
        mongo::BSONObj p = cursor->next();
        Settings::Baudrate = atoi(p.getStringField("baudRate"));
        std::cout << "try : " << p.getStringField("baudRate") << std::endl;
        Settings::FPS = p.getStringField("FPS");
        Settings::ReTryOpenSerialPort = atoi(p.getStringField("ReTryOpenSerialPort"));
        Settings::RTSPUrl = p.getStringField("RTSPUrl");
        Settings::StopRTSPThr = atoi(p.getStringField("StopRTSPThr"));
        Settings::log();
        isThereSettings = true;
    }
    if(!isThereSettings){
        mongo::BSONObj obj =  mongo::BSONObjBuilder().
                genOID().
                append("baudRate" , std::to_string(Settings::Baudrate)).
                append("FPS", Settings::FPS).
                append("ReTryOpenSerialPort",std::to_string(Settings::ReTryOpenSerialPort)).
                append("RTSPUrl", Settings::RTSPUrl).
                append("StopRTSPThr", std::to_string(Settings::StopRTSPThr)).
                obj();
        m_connection.insert(getSettingCollectionName(), obj);
        std::cout << "Settings inited in database" << endl;
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
    mongo::BSONObj obj =  mongo::BSONObjBuilder().genOID().append("date" , jdate).append("time", date.toStdString())
            .append("tics", QString::number(QDateTime::currentMSecsSinceEpoch()).toStdString()).obj();
    m_connection.insert(getCollectionName(), obj);
    qDebug() << "object id is : " << QString::fromStdString(obj["_id"]).split(":")[1] << endl;
    return QString::fromStdString(obj["_id"]).split(":")[1];
}

MongoDB::MongoDB(){
    m_dbName = "KIG";
    m_traficCollectionName = "traffic";
    m_settingsCollectionName = "settings";
}

std::string MongoDB::getCollectionName(){
    return (m_dbName + "." + m_traficCollectionName).toStdString();
}

std::string MongoDB::getSettingCollectionName(){
    return (m_dbName + "." + m_settingsCollectionName).toStdString();
}

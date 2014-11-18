#include "mongo.hpp"
#include <mongo/client/dbclient.h>
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

void MongoDB::insertNewTrafficHandler(){
    boost::thread *newThrThread = new boost::thread(boost::bind(&MongoDB::insertNewTrafficHandler, this)); 
}


void MongoDB::tryToReconnect(){
    closeConnection();
    usleep(30000);
    connectToDB();
}

QString MongoDB::insertNewTraffic()
{
    try
    {	
	tryToReconnect();
	CDate::Now n;
	std::string jdate = CDate::DateConv::ToJalali(n);
	QString date = QTime::currentTime().toString();
	qDebug() << "Jalali date is : " << QString::fromStdString(jdate);
	qDebug() << "time is : " << date;
	mongo::BSONObj obj =  mongo::BSONObjBuilder().genOID().append("date" , jdate).append("status", "false").append("time", date.toStdString())
            .append("tics", QString::number(QDateTime::currentMSecsSinceEpoch()).toStdString()).obj();
	m_connection.insert(getCollectionName(), obj);
	qDebug() << "object id is : " << QString::fromStdString(obj["_id"]).split(":")[1] << endl;
	std::string tmpId = QString::fromStdString(obj["_id"]).split(":")[1].toStdString();
	std::string id = "";
	int state = 0 ;
	for(int i = 11 ; i < tmpId.size() - 2 ; i++){
	    char d = tmpId[i];
	    id  += d;
	}
	std::cout <<  "Id is : " << id  << endl;
	return QString::fromStdString(id);
    }
    catch(mongo::UserException &ue){
	tryToReconnect();
//	std::cout << __LINE__ << " : " << __FILE__ << " : " << ue.toString() << endl;
	return insertNewTraffic();
    }
    catch(mongo::MsgAssertionException &ex){
	tryToReconnect();
	return insertNewTraffic();
//	std::cout << __LINE__ << " : " << __FILE__ << " : " << ex.toString() << endl;
    }
    catch(mongo::DBException &ex){
	tryToReconnect();
///	std::cout << __LINE__ << " : " << __FILE__ << " : " << ex.what() << endl;
	return insertNewTraffic();
    }
}


void MongoDB::makeRecordValid(QString id){
    try
    {
	std::cout << " Try to make status of record true" << endl;
	OID oid(id.toStdString());
	m_connection.update(getCollectionName(), QUERY("_id" << oid), BSON("$set" << BSON("status" << "true")));
    }
    catch(mongo::UserException &ue){
//	std::cout << __LINE__ << " : " << __FILE__ << " : " << ue.toString() << endl;
    }
    catch(mongo::MsgAssertionException &ex){
//	std::cout << __LINE__ << " : " << __FILE__ <<  " : " << ex.toString() << endl;
    }
    catch(mongo::DBException &ex){
//	std::cout << __LINE__ << " : " << __FILE__ << " : " << ex.what() << endl;
    }
}

void MongoDB::insertPlateForRecord(QString id, QString englishPlate, QString persianPlate1, QString persianPlate2, QString image){
    try
    {
	OID oid(id.toStdString());
	mongo::BSONObj obj = mongo::BSONObjBuilder().
            append("englishPlate" , englishPlate.toStdString()).
            append("persianPlate1" , persianPlate1.toStdWString().c_str()).
            append("persianPlate2" , persianPlate2.toStdWString().c_str()).
            append("detectedImage" , image.toStdString()).
            obj();
	m_connection.update(getCollectionName(), QUERY("_id"<< oid), BSON("$set" << BSON("englishPlate" << englishPlate.toStdString())));
	m_connection.update(getCollectionName(), QUERY("_id"<< oid), BSON("$set" << BSON("persianPlate1" << persianPlate1.toStdString())));
	m_connection.update(getCollectionName(), QUERY("_id"<< oid), BSON("$set" << BSON("persianPlate2" << persianPlate2.toStdString())));
	m_connection.update(getCollectionName(), QUERY("_id"<< oid), BSON("$set" << BSON("detectedImage" << image.toStdString())));
	QString profileId = getRelatedProfile(persianPlate2);
	std::cout << "***************************** Profile id is : " << profileId.toStdString() << endl;
	OID profileOID(profileId.toStdString());
	m_connection.update(getCollectionName(), QUERY("_id"<< oid), BSON("$set" << BSON("profile" << profileOID)));
    }
    catch(mongo::UserException & ue){
//	std::cout << __LINE__ << " : " << __FILE__ <<  " Mongo Exception : " << ue.toString() << endl;
    }
    catch(mongo::MsgAssertionException& ex){
//	std::cout << __LINE__ << " : " << __FILE__ << " Msg Assert Exception : " << ex.toString() << endl;
    }
    catch(mongo::DBException &ex)
    {
//	std::cout << __LINE__ << " : " << __FILE__ << ex.what() << endl;
    }

}

QString MongoDB::getRelatedProfile(QString plate){
    try
    {
	QString res = "";
    	qDebug() << "------------------------------ Begin of detect reocrd profile -----------" << endl;
	qDebug() << plate ;
	std::string splate = plate.toStdString();
	std::cout << " Plate is : " << splate  << endl;
//	std::auto_ptr<mongo::DBClientCursor> cursor = m_connection.query(getProfileCollectionName(), QUERY("plate" << splate));
	std::cout << "Database is now open " << endl;
	std::auto_ptr<mongo::DBClientCursor> cursor = m_connection.query(this->getProfileCollectionName());
	while (cursor->more()) 
	{
    	    //std::cout << "settings readed from database" << std::endl;
    	    mongo::BSONObj obj = cursor->next();
	    std::string pl = obj.getStringField("plate");
		std::cout << pl << " : " << splate << endl;
	    if(pl == splate){
		qDebug() << "object id is : " << QString::fromStdString(obj["_id"]).split(":")[1] << endl;
		std::string tmpId = QString::fromStdString(obj["_id"]).split(":")[1].toStdString();
		cout << tmpId << endl;
		std::string id = "";
		int state = 0 ;
		for(int i = 11 ; i < tmpId.size() - 2 ; i++){
		    char d = tmpId[i];
		    id  += d;
		}
		std::cout <<  "Id is : " << id  << endl; 	
		res =  QString::fromStdString(id);
	    }
	}
	return res;    
    }
    catch(mongo::UserException & ue){
	std::cout << __LINE__ << " : " << __FILE__ <<  " Mongo Exception : " << ue.toString() << endl;
    }
    catch(mongo::MsgAssertionException& ex){
    	std::cout << __LINE__ << " : " << __FILE__ << " Msg Assert Exception : " << ex.toString() << endl;
    }
    catch(mongo::DBException &ex)
    {
	std::cout << __LINE__ << " : " << __FILE__ << ex.what() << endl;
    }
}

MongoDB::MongoDB(){
    m_dbName = "KIG";
    m_traficCollectionName = "traffic";
    m_settingsCollectionName = "settings";
    m_cameraCollectionName = "cameras";
    m_profileCollectionName = "profile";
}

std::string MongoDB::getCollectionName(){
    return (m_dbName + "." + m_traficCollectionName).toStdString();
}

std::string MongoDB::getSettingCollectionName(){
    return (m_dbName + "." + m_settingsCollectionName).toStdString();
}

std::string MongoDB::getCameraCollectionName(){
    return (m_dbName + "." + m_cameraCollectionName).toStdString();
}

std::string MongoDB::getProfileCollectionName(){
    string  cmd =  (m_dbName + "." + m_profileCollectionName).toStdString();
    std::cout << " CMD is : " << cmd << endl;
    return cmd;
}

vector<Camera*> MongoDB::getAllCamera(){
    vector<Camera*> result;
    std::auto_ptr<mongo::DBClientCursor> cursor =
            m_connection.query(this->getCameraCollectionName());
    while (cursor->more()) {
        std::cout << "settings readed from database" << std::endl;
        mongo::BSONObj p = cursor->next();
	Camera *newCamera = new Camera();
        newCamera->name = p.getStringField("Name");
	newCamera->fps = p.getStringField("FPS");
	newCamera->rtsp = p.getStringField("RTSPUrl");
	newCamera->status = p.getStringField("Status");
	result.push_back(newCamera);
    }
    return result;
}
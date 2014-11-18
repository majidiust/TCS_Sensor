#include "kigManager.hpp"
#include "settings.hpp"
#include "Libs/CarPlateRecognition.h"

KIGManager::KIGManager(){
    std::cout << "Kaveh industrial group control traffic system" << endl;
    m_root = "/home/blurkaveh/TCS_Services/tcs_services/public/www/";
}

void KIGManager::start(){
    std::cout << " Started KIG Agent ............ " << endl;
    m_db.connectToDB();
    m_db.loadSettings();
    m_peripheral = new Peripheral();
    m_connection = m_peripheral->connectExternalEventHandler(boost::bind(&KIGManager::newEventHandler, this, _1));
    m_connection = m_peripheral->connectCommandHandler(boost::bind(&KIGManager::newCommandHandler, this, _1));
    m_lastStep = 0;
    tryOpenPort();
}

void KIGManager::disconnectFromPeripheral()
{
    m_peripheral->disconnect(m_connection);
}

void KIGManager::tryOpenPort(){
    do{
        usleep(Settings::ReTryOpenSerialPort);
        std::cout << "Try open serial port ..." << " baudrate : " << Settings::Baudrate << " : port /dev/ttyUSB0" << endl;
    }while(!m_peripheral->openPort("/dev/ttyUSB0", Settings::Baudrate));
    std::cout<< "Openning theport" << std::endl;
    m_peripheral->start();
}

void KIGManager::newEventHandler(std::string arg)
{
    std::cout << " new event : " << arg << endl;
}

void KIGManager::newCommandHandler(int commandType)
{
    switch(commandType)
    {
    case Peripheral::CTBegin:
        std::cout << "Vehicle enter" << endl;
        m_lastStep = 1;
        saveEventBegin();
        break;
    case Peripheral::CTCalibration:
        std::cout << "Callibration" << endl;
        break;
    case Peripheral::CTEnd:
        std::cout << "Vehicle exit" << endl;
        saveEventEnd();
        break;;
    case Peripheral::CTNode:
        std::cout << " None commande" << endl;
        break;
    case Peripheral::CTPowerOn:
        std::cout << " Power On" << endl;
        break;
    case Peripheral::CTTakePicture:
        std::cout << "Take picture" << endl;
        saveEventOnDemand();
        break;
    }
}

void KIGManager::saveEventBegin(){
    QString id = m_db.insertNewTraffic();
    std::cout << id.toStdString() << endl;
    vector<Camera*> cameras = m_db.getAllCamera();
    m_client.clear();
    for(int i = 0 ; i < cameras.size() ; i++){
	RTSPClient * tmpClient = new RTSPClient(id, QString::fromStdString(cameras[i]->rtsp), QString::fromStdString("fps=" + cameras[i]->fps), QString::fromStdString(cameras[i]->name));
	m_client.push_back(tmpClient);
	tmpClient->start();
	if(i == 0)
	{
	    connect(tmpClient, SIGNAL(processStopped(QString)), this, SLOT(OnStopRTSP(QString)));
	}
    }
}

void KIGManager::saveEventEnd(){
    std::cout << "saveEventEnd" << endl;
    for(int i = 0 ; i < m_client.size() ; i++){
	m_client[i]->stopProcessDelayed();
    }
}

void KIGManager::saveEventOnDemand(){
    QString id = m_db.insertNewTraffic();
    std::cout << id.toStdString() << endl;
    //m_client = new RTSPClient(id, QString::fromStdString(Settings::RTSPUrl), QString::fromStdString(Settings::FPS));
    //m_client->start();
    //m_client->stopProcessDelayed();
}

void KIGManager::OnStopRTSP(QString id){
    std::cout << " On Stop RTS : " << id.toStdString() <<  endl;
    m_db.makeRecordValid(id);
    boost::shared_ptr<boost::thread> detectorPlate = boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&KIGManager::plateDetectorHandler, this, id.toStdString())));
}

void KIGManager::plateDetectorHandler(string id){
    
    usleep(5000000);
    std::cout << " Try to find true plate in images for " << id << endl;
    DIR *dp;
    CarPlateRecognition lpr;
    dp = opendir((m_root + id).c_str());
    struct dirent *dirp;
    while(dirp = readdir(dp)){
        std::string imageName = m_root + id + "/" + dirp->d_name;
	std::cout << "Inspecting the image " << imageName << endl;
        Mat img = imread(imageName);
        if(img.empty())
	{
	    std::cout << "Image is Empty : " << imageName << endl;
            continue;
	}

	// end of detection
        vector<Plate> plates = lpr.PlateDetognitionStandard(img, Mat());
        bool isDetected = false;
        for(int i = 0 ; i < plates.size() ; i++){
	    //std::cout << plates[i].plateStringShow << " : " << plates[i].plateString0 << " : " << plates[i].plateString << endl;
	    QString persian1 = QString::fromStdWString(plates[i].plateStringShow);
            QString english =  QString::fromStdString(plates[i].plateString0);
            QString persian2 = QString::fromStdWString(plates[i].plateString);
            qDebug() << persian1 << " : " << english << " : " << persian2 ;
	    if(i >= 1)
	    {
		    QString duplicateId = m_db.insertNewTraffic();
		    QDir tmpDir(QString::fromStdString((m_root + duplicateId.toStdString())));
		    if(!tmpDir.exists()){
			tmpDir.mkpath(".");
    		    }
		    string cmd = "cp " + m_root + id + "/* " + m_root + duplicateId.toStdString() + " -r";
		    std::cout << "CMD IS : " << cmd << endl;
		    system(cmd.c_str());
		    m_db.insertPlateForRecord(duplicateId, english, persian1, persian2, QString::fromStdString(imageName));
	    }
	    else{
		m_db.insertPlateForRecord(QString::fromStdString(id), english, persian1, persian2, QString::fromStdString(imageName));
	    }
            isDetected = true;
        }
        if(isDetected)
	{
	    std::cout << "Image Detected and saved" << endl;	
            break;
	}
	else
	{
	    std::cout << "Image is not detected" << endl;
	}
    }
}

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
    bool isAsigned = false;
    for(int i = 0 ; i < cameras.size() ; i++){
        RTSPClient * tmpClient = new RTSPClient(id, QString::fromStdString(cameras[i]->rtsp), QString::fromStdString("fps=" + cameras[i]->fps), QString::fromStdString(cameras[i]->name), QString::fromStdString(cameras[i]->role));
        m_client.push_back(tmpClient);
        tmpClient->start();
        if( i == cameras.size() -1)
        {
            isAsigned = true;
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
    if(m_lock.try_lock()){
        std::vector<PlateProfile> m_detectedPlates;
        usleep(5000000);
        std::cout << " Try to find true plate in images for " << id << endl;
        DIR *dp;
        CarPlateRecognition lpr;
        dp = opendir((m_root + id).c_str());
        m_detectedPlates.clear();
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
            qDebug() << "-------------------------Image Role---------" << QString::fromStdString(getImageRole(imageName));
            if(getImageRole(imageName) != "plate "){

                continue;
            }
            // end of detection
            vector<Plate> plates = lpr.PlateDetognitionStandard(img, Mat());
            qDebug() << " ----------------------------------------Plates : " << plates.size() ;
            for(int i = 0 ; i < plates.size() ; i++){
                //std::cout << plates[i].plateStringShow << " : " << plates[i].plateString0 << " : " << plates[i].plateString << endl;
                QString persian1 = QString::fromStdWString(plates[i].plateStringShow);
                QString english =  QString::fromStdString(plates[i].plateString0);
                QString persian2 = QString::fromStdWString(plates[i].plateString);

                bool f = false;
                for(int k = 0 ; k < m_detectedPlates.size(); k++){
                    if(m_detectedPlates[k].persianPlate2 == plates[i].plateString){
                        // m_detectedPlates[i].count ++;
                        qDebug() << " ------------------------------- This plate detected before ";
                        qDebug() << persian1 << " : " << english << " : " << persian2 ;
                        f = true;
                        break;
                    }
                }
                if(!f){
                    qDebug() << " --------------------------------- This plate is new";
                    qDebug() << persian1 << " : " << english << " : " << persian2 ;
                    PlateProfile tmpProfile;
                    tmpProfile.count = 0;
                    tmpProfile.englishPlate = plates[i].plateString0;
                    tmpProfile.persianPlate1 = plates[i].plateStringShow;
                    tmpProfile.persianPlate2 = plates[i].plateString;
                    tmpProfile.imageName = imageName;
                    m_detectedPlates.push_back(tmpProfile);

                }
            }
        }

        if(true)
        {
            qDebug() << " ------------------------------- Number of detected plates in profile " << m_detectedPlates.size() ;
            for(int  i = 0 ; i < m_detectedPlates.size() ; i++){
                if(i == 0){
                    m_db.insertPlateForRecord(QString::fromStdString(id), QString::fromStdString(m_detectedPlates[i].englishPlate), QString::fromStdWString(m_detectedPlates[i].persianPlate1), QString::fromStdWString(m_detectedPlates[i].persianPlate2), QString::fromStdString(m_detectedPlates[i].imageName));
                    qDebug() << " ------------------------------- save for first time";
                }
                else{
                    QString duplicateId = m_db.insertNewTraffic();
                    qDebug() << "1" ;
                    QDir tmpDir(QString::fromStdString((m_root + duplicateId.toStdString())));
                    if(!tmpDir.exists()){
                        tmpDir.mkpath(".");
                    }
                    string cmd = "cp " + m_root + id + "/* " + m_root + duplicateId.toStdString() + " -r";
                    std::cout << "CMD IS : " << cmd << endl;
                    system(cmd.c_str());
                    qDebug() << "2" ;
                    m_db.insertPlateForRecord(duplicateId, QString::fromStdString(m_detectedPlates[i].englishPlate), QString::fromStdWString(m_detectedPlates[i].persianPlate1), QString::fromStdWString(m_detectedPlates[i].persianPlate2), QString::fromStdString(m_detectedPlates[i].imageName));
                    qDebug() << " -------------------------------Saved for second time";
                }
            }
            std::cout << "Image Detected and saved" << endl;
        }
        else
        {
            std::cout << "Image is not detected" << endl;
        }
        m_lock.unlock();
    }
}

string KIGManager::getImageRole(string imageName){
    if(imageName.size() <= 1)
        return "";
    string res = "";
    int bi = 0;
    for(int i = 0 ; i < imageName.length() ; i++){
        if(imageName[i] == '/'){
            bi = i;
        }
    }

    for(int i = bi+1 ; i < imageName.size() ; i++){
        if(imageName[i] != '_'){
            res+= imageName[i];
        }
        else break;
    }
    return res;
}

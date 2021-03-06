#include "rtspclient.hpp"
#include "settings.hpp"

RTSPClient::RTSPClient(QString recordId, QString rtspUrl, QString fps, QString name, QString role,  QObject *parent){
    m_id = recordId;
    m_program = "ffmpeg";
    m_root = "/home/blurkaveh/TCS_Services/tcs_services/public/www/";
    QDir dir(m_root);
    if (!dir.exists()){
      dir.mkpath(".");
    }

    QDir dir2(m_root+ "/" + recordId);
    if (!dir2.exists()){
        dir2.mkpath(".");
    }
    m_argument << "-i" << rtspUrl << "-f" << "image2" << "-vf" << fps << m_root+ "/" + recordId +"/" + role + " _" + name + "_img%03d.jpg";
    qDebug() << m_argument ;
    m_process = new QProcess();
    m_process->moveToThread(this);
}

RTSPClient::~RTSPClient(){
    m_process->close();
    emit processStopped(m_id);
}

void RTSPClient::startProcess(){
    m_process->execute(m_program, m_argument);
    emit processStarted();
}

void RTSPClient::stopProcess(){
    
    //while(m_process->waitForFinished(250))
    {
        m_process->kill();
        this->terminate();
        emit processStopped(m_id);
    }
}

void RTSPClient::stopProcessDelayed(){
    
    usleep(5000000);
    stopProcess();
}

void RTSPClient::readyReadOutput()
{
    qDebug() << m_process->readAllStandardOutput();
}

void RTSPClient::run(){
    startProcess();
//    stopProcessDelayed();
}

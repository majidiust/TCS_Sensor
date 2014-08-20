#include "rtspclient.hpp"

RTSPClient::RTSPClient(QString recordId, QString rtspUrl, QString fps, QObject *parent){

    m_program = "ffmpeg";
    m_defultThr = 4000;
    m_root = "/home/majid/blob";
    QDir dir(m_root);
    if (!dir.exists()){
      dir.mkpath(".");
    }

    QDir dir2(m_root+ "/" + recordId);
    if (!dir2.exists()){
        dir2.mkpath(".");
    }
    m_argument << "-i" << rtspUrl << "-f" << "image2" << "-vf" << fps << m_root+ "/" + recordId +"/" + "img%03d.jpg";
    qDebug() << m_argument ;
    m_process = new QProcess();
    m_process->moveToThread(this);
}

RTSPClient::~RTSPClient(){
    m_process->close();
    emit processStopped();
}

void RTSPClient::startProcess(){
    m_process->execute(m_program, m_argument);
    emit processStarted();
}

void RTSPClient::stopProcess(){
    m_process->close();
    m_process->waitForFinished();
    emit processStopped();
}

void RTSPClient::stopProcessDelayed(){
    QTimer::singleShot(m_defultThr, this, SLOT(stopProcess()));
}

void RTSPClient::readyReadOutput()
{
    qDebug() << m_process->readAllStandardOutput();
}

void RTSPClient::run(){
    startProcess();
}

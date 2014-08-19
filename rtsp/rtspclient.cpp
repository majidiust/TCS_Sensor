#include "rtspclient.hpp"

RTSPClient::RTSPClient(QString recordId, QString rtspUrl, QString fps, QObject *parent){
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
    m_process = new QProcess(this);
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
    emit processStopped();
}

void RTSPClient::stopProcessDelayed(){
    QTimer::singleShot(m_defultThr, this, SLOT(stopProcess()));
}

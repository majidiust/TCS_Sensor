#include "rtsptester.h"

RTSPTester::RTSPTester(QObject *parent) :
    QObject(parent)
{
    m_client = new RTSPClient("1212", "rtsp://192.168.88.1:8090/1", "fps=1/10");
    connect(m_client, SIGNAL(processStarted()), this, SLOT(started()));
    connect(m_client, SIGNAL(processStopped()), this, SLOT(stopped()));
    m_client->start();
}


void RTSPTester::started()
{
    qDebug() << "Streaming started ....";
}

void RTSPTester::stopped()
{
    qDebug() << "Streaming stopped ....";
}

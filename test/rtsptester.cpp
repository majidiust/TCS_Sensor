#include "rtsptester.h"

RTSPTester::RTSPTester(QObject *parent) :
    QObject(parent)
{
    m_client = new RTSPClient("1212", "rtsp://admin:admin@192.168.2.1:554/mpeg", "fps=5/10");
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

#ifndef RTSPCLIENT_HPP
#define RTSPCLIENT_HPP

#include <iostream>
#include "../base.hpp"

#include <QObject>
#include <QDir>
#include <QTimer>
#include <QString>
#include <QProcess>
#include <QDebug>

using namespace std;

class RTSPClient : public QObject
{
    Q_OBJECT
public:
    RTSPClient(QString recordId, QString rtspUrl, QString fps="1/1", QObject *parent = 0);
    ~RTSPClient();
public slots:
    void startProcess();
    void stopProcess();
    void stopProcessDelayed();
signals:
    void processStopped();
    void processStarted();
private:
    QProcess *m_process;
    const QString m_root = "/home/majid/blob";
    const int m_defultThr = 4000; // milisecond
    const QString m_program = "ffmpeg";
    QStringList m_argument;
};

#endif

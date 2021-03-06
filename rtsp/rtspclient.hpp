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
#include <QThread>
using namespace std;

class RTSPClient : public QThread
{
    Q_OBJECT
public:
    RTSPClient(QString recordId, QString rtspUrl, QString fps="1/1", QString name="img", QString role="plate", QObject *parent = 0);
    ~RTSPClient();
public slots:
    void startProcess();
    void stopProcess();
    void stopProcessDelayed();
    void readyReadOutput();
    void run();

signals:
    void processStopped(QString id);
    void processStarted();
private:
    QProcess *m_process;
    QString m_root;
    QString m_program;
    QStringList m_argument;
    QString m_id;
};

#endif

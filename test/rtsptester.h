#ifndef RTSPTESTER_H
#define RTSPTESTER_H

#include <QObject>
#include <QDebug>
#include "rtspclient.hpp"

class RTSPTester : public QObject
{
    Q_OBJECT
public:
    explicit RTSPTester(QObject *parent = 0);

signals:

public slots:
    void started();
    void stopped();
private:
    RTSPClient *m_client;
};

#endif // RTSPTESTER_H

#pragma once

#include <QString>
#include <QDebug>
#include <iostream>

using namespace std;

class Camera {
    public:
    std::string name;
    std::string fps;
    bool status;
    std::string rtsp;
    std::string role;
};

#include "settings.hpp"

int Settings::Baudrate = 9600;
std::string Settings::RTSPUrl = "rtsp://admin:admin@192.168.2.1:554/mpeg";
std::string Settings::FPS = "fps=5/10";
int Settings::StopRTSPThr = 5000; //milisecond
int Settings::ReTryOpenSerialPort = 2000000; //microsecond
void Settings::log(){
    std::cout << " BaudRate : " << Settings::Baudrate
                 << ", RTSP : " << Settings::RTSPUrl
                 << ", FPS  : " << Settings::FPS
                 << ", STOPRTSpthr : " << Settings::StopRTSPThr
                 << ", RetryOpenSeriaPort : " << Settings::ReTryOpenSerialPort << std::endl;
}

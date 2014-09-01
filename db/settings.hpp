#ifndef SETTINGS_H
#define SETTINGS_H

#include <iostream>
#include <string>

class Settings{
    public:
    static int Baudrate;
	static std::string FPS;
	static int StopRTSPThr;
	static int ReTryOpenSerialPort;
	static std::string RTSPUrl;
    static void log();
};
#endif

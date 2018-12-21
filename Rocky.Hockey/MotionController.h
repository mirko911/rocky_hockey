#pragma once
#include "definitions.h"

#include "SerialConnection.h"

#define BYTE_START 0XAA
#define BYTE_END 0xCC
enum CommandIndex {
    COMMAND_MOVE = 0x00,
    COMMAND_VELOCITY = 0x01,
    COMMAND_ACCELLERATION = 0x02,
};
//-128 <=> 127

class MotionController
{
private:
    SerialConnection m_serial;
public:
    MotionController(const std::string tty_name, const int baudRate);
    void moveToPosition(const Vector position);
    void calculateRatio(const cv::Size &size);
    ~MotionController();
};


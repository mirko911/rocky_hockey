#pragma once
#include "definitions.h"

#include "SerialConnection.h"

constexpr uint8_t  BYTE_START = 0XAA;
constexpr uint8_t  BYTE_END = 0xCC;
constexpr uint8_t  COMMAND_MOVE = 0x00;
constexpr uint8_t  COMMAND_VELOCITY = 0x01;
constexpr uint8_t  COMMAND_ACCELERATION = 0x02;

class MotionController
{
private:
    SerialConnection m_serial;
	Eigen::Matrix2f m_rotationMatrix;
    float m_ratioX;
    float m_ratioY;
public:
    MotionController(const std::string tty_name, const int baudRate);
    void moveToPosition(const Vector position);
	void setVelocity(const int x, const int y);
	void setAcceleration(const int x, const int y);
    void calculateRatio(const cv::Size &size);
    ~MotionController();
};


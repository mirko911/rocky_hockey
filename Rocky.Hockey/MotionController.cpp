#include "MotionController.h"
//-32768..32767
MotionController::MotionController(const std::string tty_name, const int baudRate) : m_serial()
{
   // m_serial.connect(tty_name, baudRate);

	//Prebuild the rotation matrix
	float angle = 45 * 3.141592653f / 180.0f;
	m_rotationMatrix <<
		cos(angle) * m_ratioX, -sin(angle),
		sin(angle), cos(angle) * m_ratioY;
}

void MotionController::moveToPosition(const Vector position)
{
	Vector position1 = Vector(296, 293);
	
	Vector result = m_rotationMatrix * position1;
  
    m_serial.send(BYTE_START);
    m_serial.send(COMMAND_MOVE);
    m_serial.send(static_cast<int16_t>(result.x() * m_ratioX));
    m_serial.send(COMMAND_MOVE);
    m_serial.send(static_cast<int16_t>(result.y() * m_ratioY));
    m_serial.send(BYTE_END);
}

void MotionController::setVelocity(const int x, const int y)
{
	m_serial.send(BYTE_START);
	m_serial.send(COMMAND_VELOCITY);
	m_serial.send(static_cast<int16_t>(x));
	m_serial.send(COMMAND_VELOCITY);
	m_serial.send(static_cast<int16_t>(y));
	m_serial.send(BYTE_END);
}

void MotionController::setAcceleration(const int x, const int y)
{
	m_serial.send(BYTE_START);
	m_serial.send(COMMAND_ACCELERATION);
	m_serial.send(static_cast<int16_t>(x));
	m_serial.send(COMMAND_ACCELERATION);
	m_serial.send(static_cast<int16_t>(y));
	m_serial.send(BYTE_END);
}

void MotionController::calculateRatio(const cv::Size & size)
{
	m_ratioX = 6000.0f / 50.0f;
	m_ratioY = 6000.0f / size.height; //6000
}

MotionController::~MotionController()
{
	m_serial.quit();
}

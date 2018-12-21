#include "MotionController.h"
//-32768..32767
MotionController::MotionController(const std::string tty_name, const int baudRate) : m_serial()
{
   // m_serial.connect(tty_name, baudRate);
}

void MotionController::moveToPosition(const Vector position)
{
    float angle = -45;
    float x = cos(angle)*position.x() - sin(angle) * position.y();
    float y = sin(angle)*position.x() + cos(angle) * position.y();
    Vector rotated_position = Vector(x, y);

    Vector position_motion_space = Vector(
        rotated_position.x() * m_ratioX,
        rotated_position.y() * m_ratioY
    );

    float newx = position_motion_space.x();
    float newY = position_motion_space.y();

    m_serial.send(static_cast<uint8_t>(BYTE_START));
    m_serial.send(static_cast<uint8_t>(COMMAND_MOVE));
    m_serial.send(static_cast<int16_t>(position_motion_space.x()));
    m_serial.send(static_cast<uint8_t>(COMMAND_MOVE));
    m_serial.send(static_cast<int16_t>(position_motion_space.y()));
    m_serial.send(static_cast<uint8_t>(BYTE_END));
}

void MotionController::calculateRatio(const cv::Size & size)
{
    m_ratioX = 6000.0f / size.width;
    m_ratioY = 6000.0f / 50.0f; //6000
}

MotionController::~MotionController()
{
}

#include "MotionController.h"

MotionController::MotionController(const std::string tty_name, const int baudRate) : m_serial()
{
    m_serial.connect(tty_name, baudRate);
}

void MotionController::moveToPosition(const Vector position)
{

}

void MotionController::calculateRatio(const cv::Size & size)
{
    m_motion2PixelRatioX = 255.0f / size.width + 128;
}

MotionController::~MotionController()
{
}

#include "Tracker.h"

Tracker::Tracker()
{
    std::cout << "[Tracker] created tracker" << std::endl;
}

bool Tracker::Tick(const cv::Mat & src, cv::Mat &dst, Puck & puck)
{
    if (m_wrongDetections >= config.wrongDetectionThreshold) {
        //std::cerr << "[Tracker] Puck wasn't found for " + std::to_string(m_wrongDetections) + "frames. Reset puck status" << std::endl;
        puck.resetDirection();
        puck.resetPosition();
        puck.resetVelocity();
        m_wrongDetections = 0;
    }

    std::vector < cv::Vec3f > circles;

    for (const auto &circle : circles) {
        cv::circle(dst, cv::Point2f(circle[0], circle[1]), circle[2], cv::Scalar(0, 0, 255), 1, 8, 0);
    }



    //https://dsp.stackexchange.com/questions/22648/in-opecv-function-hough-circles-how-does-parameter-1-and-2-affect-circle-detecti
    /// Apply the Hough Transform to find the circles
    cv::HoughCircles(src, circles, cv::HOUGH_GRADIENT, static_cast<float>(dp/10.0f), src.rows/8, threshold1, threshold2, minArea, maxArea);

    if (circles.size() == 0) {
        //std::cerr << "[Tracker] Nothing found" << std::endl;
        m_wrongDetections++;
        return false;
    }

    if (circles.size() > 1) {
        //std::cerr << "[Tracker] Found more then 1 puck" << std::endl;
        m_wrongDetections++;
     //   return false;
    }

    //Reset the wrong detection status
    m_wrongDetections = 0;

    //We don't need a loop, because we only need the 1st element
    cv::Vec3f circle = circles[0];

    //Extract puck position and radius from array
    Vector position(circle[0], circle[1]);
    int radius = static_cast<int>(circle[2]);

    Vector oldPosition = puck.getPosition();
    if (oldPosition.hasNaN()) {
        oldPosition = position;
        puck.setPosition(position);
    }

    //Calculate mean of all old directions and blend them with a 2:1 ratio into a new direction
    Vector direction = Vector((position - oldPosition));
    Vector oldDirection = puck.getDirection();
    if (oldDirection.hasNaN()) {
        oldDirection = direction;
    }
    Vector direction_blend = (direction + oldDirection) * 0.5f;
    float distance = direction.norm();

    //Pass new vectors to puck class
    if (distance >= 5.0f) {
        puck.setPosition(position);
        puck.setDirection(direction);
    }
    puck.setVelocity(distance);

#ifdef _DEBUG
    //Draw a circle and an arrow to visualize the puck position and the direction
    cv::circle(dst, cv::Point2f(position.x(), position.y()), radius, cv::Scalar(0, 0, 255), 1, 8, 0);
    
    if (puck.getDirection().hasNaN()) {
        //don't draw arrow if the direction vector is NaN
        return true;
    }

    Vector arrowPos = position + (puck.getDirection() * 100);
    cv::arrowedLine(
        dst,
        cv::Point2f(position.x(), position.y()),
        cv::Point2f(arrowPos.x(), arrowPos.y()),
        cv::Scalar(255, 0, 0),
        2,
        8,
        0,
        0.1f
    );
#endif
    return true;
}


Tracker::~Tracker()
{
}

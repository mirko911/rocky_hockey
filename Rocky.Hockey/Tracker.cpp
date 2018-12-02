#include "Tracker.h"

Tracker::Tracker()
{
    std::cout << "[Tracker] created tracker" << std::endl;
}

void Tracker::Tick(const cv::Mat & src, cv::Mat &dst, Puck & puck)
{
    std::vector < cv::Vec3f > circles;

    //https://dsp.stackexchange.com/questions/22648/in-opecv-function-hough-circles-how-does-parameter-1-and-2-affect-circle-detecti
    /// Apply the Hough Transform to find the circles
    cv::HoughCircles(src, circles, cv::HOUGH_GRADIENT, 1.3f, src.rows/8, 186, 13, 7, 9);

    if (circles.size() == 0) {
        //std::cerr << "[Tracker] Nothing found" << std::endl;
        return;
    }

    if (circles.size() > 1) {
        //std::cerr << "[Tracker] Found more than one circle" << std::endl;
        return;
    }

    //We don't need a loop, because we only need the 1st element
    cv::Vec3f circle = circles[0];

    //Extract puck position and radius from array
    Vector position(circle[0], circle[1]);
    int radius = circle[2];

	/*
    //Blend old and new position to smooth the jitter
    Vector oldPosition = puck.getPosition();
    Vector position_blend = (position + oldPosition) * 0.5f;


    //Calculate mean of all old directions and blend them with a 2:1 ratio into a new direction
    Vector oldDirectionMean(0, 0);
    Vector direction = Vector((position_blend - oldPosition));
    std::deque<Vector> directionQueue = puck.getDirectionQueue();
    for (const auto &oldDirection : directionQueue) {
        oldDirectionMean += oldDirection;
    }
    oldDirectionMean /= directionQueue.size();
	*/

    //this call now does the work of the code above
    puck.setPosition(position);


#ifdef _DEBUG
    //Draw a circle and an arrow to visualize the puck position and the direction
    cv::circle(dst, cv::Point(position.x(), position.y()), radius, cv::Scalar(0, 0, 255), 1, 8, 0);
    
    Vector arrowPos = position + (puck.getDirection() * 100);
    cv::arrowedLine(
        dst,
        cv::Point(position.x(), position.y()),
        cv::Point(arrowPos.x(), arrowPos.y()),
        cv::Scalar(255, 0, 0),
        2,
        8,
        0,
        0.1f
    );
#endif
}


Tracker::~Tracker()
{
}

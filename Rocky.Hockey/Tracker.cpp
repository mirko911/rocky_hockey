#include "Tracker.h"

Tracker::Tracker()
{
    cv::SimpleBlobDetector::Params params;
    params.minThreshold = 0;
    params.maxThreshold = 255;
    params.thresholdStep = 255;
    params.filterByColor = false;
    params.blobColor = 0;
    params.filterByArea = true;
    params.filterByConvexity = false;
    params.filterByInertia = false;
    params.filterByCircularity = true;
    params.minCircularity = 0.2f; //12,564
    params.minArea = 140; //73.509 //5329
    params.maxArea = 1000;//87 7569
    params.minRepeatability = 1;
    m_detector = cv::SimpleBlobDetector::create(params);
    std::cout << "[Tracker] created tracker" << std::endl;
}

void Tracker::Tick(const cv::Mat & src, cv::Mat &dst, Puck & puck)
{
    std::vector < cv::Vec3f > circles;

    //https://dsp.stackexchange.com/questions/22648/in-opecv-function-hough-circles-how-does-parameter-1-and-2-affect-circle-detecti
    /// Apply the Hough Transform to find the circles
    cv::HoughCircles(src, circles, cv::HOUGH_GRADIENT, 1.3f, src.rows/8, 186, 13, 7, 9);

    if (circles.size() == 0) {
        //std::cerr << "Nothing found" << std::endl;
    }

    /// Draw the circles detected
    for (size_t i = 0; i < circles.size(); i++)
    {
        cv::Point center(circles[i][0], circles[i][1]);
        int radius = circles[i][2];

       // std::cout << center << " " << radius << std::endl;

        // circle center
        //circle(dst, center, 3, cv::Scalar(0, 255, 0), -1, 8, 0);
        // circle outline
        circle(dst, center, radius, cv::Scalar(0, 0, 255), 1, 8, 0);

        Vector oldPosition = puck.getPosition();
        Vector position(static_cast<float>(center.x), static_cast<float>(center.y));
        Vector position_blend = (position + oldPosition) * 0.5f;
        puck.setPosition(position_blend);

        Vector newDirection(0,0);
        Vector direction = Vector((puck.getPosition() - oldPosition));
        std::deque<Vector> directionQueue = puck.getDirectionQueue();
        directionQueue.push_back(direction);
        for (int i = 0; i < directionQueue.size(); i++) {
            newDirection += directionQueue[i];
        }
        newDirection /= directionQueue.size();

        //std::cout << "[" << newDirection.x() << "," << newDirection.y() << "]" << std::endl;

        //std::cout << "Squared diff " << (puck.getDirection() - directionBlend).squaredNorm() << std::endl;
        
        puck.setDirection(newDirection);


#ifdef _DEBUG
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
    /*


    std::vector<cv::KeyPoint> keypoints;
    m_detector->detect(src, keypoints);

#ifdef _DEBUG
    cv::drawKeypoints(dst, keypoints, dst, cv::Scalar(0, 255, 255));
#endif

    if (keypoints.size() > 0) {
        Vector oldPosition = puck.getPosition();
        Vector position(keypoints[0].pt.x, keypoints[0].pt.y);

        puck.setPosition(position);

        Vector directionBlend = ((position - oldPosition) + puck.getDirection() / 2);
        if ((puck.getDirection() - directionBlend).squaredNorm() > 1) {
            puck.setDirection(directionBlend);
        }

#ifdef _DEBUG
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

    */
}


Tracker::~Tracker()
{
}

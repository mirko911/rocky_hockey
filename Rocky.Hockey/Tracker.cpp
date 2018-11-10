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
    params.minCircularity = 0.6f; //12,564
    params.minArea = 140; //73.509 //5329
    params.maxArea = 1000;//87 7569
    params.minRepeatability = 1;
    m_detector = cv::SimpleBlobDetector::create(params);
    std::cout << "[Tracker] created tracker" << std::endl;
}

void Tracker::Tick(const cv::Mat & src, cv::Mat &dst, Puck & puck)
{
    std::vector<cv::KeyPoint> keypoints;
    m_detector->detect(src, keypoints);

#ifdef _DEBUG
    cv::drawKeypoints(dst, keypoints, dst, cv::Scalar(0, 255, 255));
#endif

    if (keypoints.size() > 0) {
        Eigen::Vector2f oldPosition = puck.getPosition();
        Eigen::Vector2f position(keypoints[0].pt.x, keypoints[0].pt.y);

        puck.setPosition(position);

        Eigen::Vector2f directionBlend = ((position - oldPosition) + puck.getDirection() / 2);
        if ((puck.getDirection() - directionBlend).squaredNorm() > 1) {
            puck.setDirection(directionBlend);
        }

#ifdef _DEBUG
        Eigen::Vector2f arrowPos = position + (puck.getDirection() * 100);
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

}


Tracker::~Tracker()
{
}

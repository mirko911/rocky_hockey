#include "ImageTransformation.h"



std::vector<Vector> ImageTransformation::findCornerPoints()
{
    std::vector<Vector> points = {
    {27,38}, //Top Left
    {282,42},  //Top Right
    {25, 193}, //Bottom Left
    {279, 191} //Bottom right
    };

    std::cout << "[ImageTransform] foud corner points :" ;
    for (const auto &point : points) {
        std::cout << "[" << point.x() << "," << point.y() << "],";
    };
    std::cout << std::endl;

    return points;
}

void ImageTransformation::calculateValues(const std::vector<Vector>& points)
{
    //Point order is TopLeft TopRight BottomLeft, BottomRight
    float widthTop = (points[1] - points[0]).norm();
    float widthBottom = (points[2] - points[3]).norm();
    m_maxWidth = static_cast<int>(std::max(widthTop, widthBottom));

    float heightLeft = (points[2] - points[0]).norm();
    float heightRight = (points[3] - points[1]).norm();
    m_maxHeight = static_cast<int>(std::max(heightLeft, heightRight));

    std::cout << "[ImageTransform] maxwidth: " << m_maxWidth << ", maxheight: " << m_maxHeight << std::endl;

	Config::get()->fieldWidth = m_maxWidth;
	Config::get()->fieldHeight = m_maxHeight;

    cv::Point2f src_vertices[4] = {
        cv::Point2i(static_cast<int>(points[0].x()), static_cast<int>(points[0].y())),
        cv::Point2i(static_cast<int>(points[1].x()), static_cast<int>(points[1].y())),
        cv::Point2i(static_cast<int>(points[2].x()), static_cast<int>(points[2].y())),
        cv::Point2i(static_cast<int>(points[3].x()), static_cast<int>(points[3].y())),
    };

    cv::Point2f dst_vertices[4] = {
        cv::Point2i(0,0),
        cv::Point2i(m_maxWidth - 1,0),
        cv::Point2i(0, m_maxHeight - 1),
        cv::Point2i(m_maxWidth - 1, m_maxHeight - 1),
    };

    m_M = cv::getPerspectiveTransform(src_vertices, dst_vertices);

    //cv::undistort is a combination of cv::initUndistortRectifyMap and cv::remap. initUndistortRectifyMap is very expensive and slows down 
    //the live feed. We can calculate this once and use it later for the live feed
    cv::initUndistortRectifyMap(m_intrinsic, m_distCoeffs, cv::Mat(), m_intrinsic, cv::Size(320, 240), CV_16SC2, m_map1, m_map2);
}

void ImageTransformation::undistort(cv::Mat & src, cv::Mat & dst)
{
    cv::remap(src, dst, m_map1, m_map2, cv::INTER_LINEAR, cv::BORDER_CONSTANT);
}

void ImageTransformation::warpPerspective(cv::Mat & src, cv::Mat & dst)
{
    cv::warpPerspective(src, dst, m_M, cv::Size(m_maxWidth, m_maxHeight), cv::INTER_LINEAR, cv::BORDER_CONSTANT);
}

cv::Size ImageTransformation::getFieldSize()
{
    return cv::Size(m_maxWidth, m_maxHeight);
}

ImageTransformation::ImageTransformation()
{
    std::cout << "[ImageTransform] construct" << std::endl;
    std::vector<Vector> points = findCornerPoints();
    calculateValues(points);
}


ImageTransformation::~ImageTransformation()
{
}

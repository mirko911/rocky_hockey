#pragma once
#include <opencv2/opencv.hpp>
#include <cmath>

#include "definitions.h"
#include "Config.h"

class ImageTransformation
{
private:
    cv::Mat m_intrinsic = (cv::Mat_<float>(3, 3) <<
        5.4840665967621987e+02, 0., 320.,
        0., 5.4840665967621987e+02, 240.,
        0., 0., 1.);

    cv::Mat m_distCoeffs = (cv::Mat_<float>(1, 5) <<
        -1.2745977774417608e-01,
        2.5673480915119734e-01,
        6.2181312531178434e-05,
        2.4478544954494273e-03,
        -1.7442435168323636e-01
        );

    //Transformation Matrix
    cv::Mat m_M;
    int m_maxWidth;
    int m_maxHeight;
    cv::Mat m_map1;
    cv::Mat m_map2;
    std::vector<Vector> findCornerPoints();
    void calculateValues(const std::vector<Vector> & points);
public:
    void undistort(cv::Mat &src, cv::Mat &dst);
    void warpPerspective(cv::Mat &src, cv::Mat &dst);
    cv::Size getFieldSize();
    ImageTransformation();
    ~ImageTransformation();
};


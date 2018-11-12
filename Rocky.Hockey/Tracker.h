#pragma once
#include <memory>
#include <iostream>

#include <opencv2/opencv.hpp>

#include "definitions.h"
#include "Puck.h"

class Tracker
{
private:
    cv::Ptr<cv::SimpleBlobDetector> m_detector;
public:
    Tracker();
    void Tick(const cv::Mat &src, cv::Mat &dst, Puck& puck);
    ~Tracker();
};


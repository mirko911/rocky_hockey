#pragma once
#include <memory>
#include <iostream>
#include <opencv2/opencv.hpp>

#include "definitions.h"
#include "Puck.h"

class Tracker
{
private:
    int m_wrongDetections = 0;
public:
    Tracker();
    bool Tick(const cv::Mat &src, cv::Mat &dst, Puck& puck);
    ~Tracker();
};


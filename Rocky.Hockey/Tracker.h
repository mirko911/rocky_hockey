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

    int dp = 17;
    int threshold1 = 186;
    int threshold2 = 26;
    int minArea = 7;
    int maxArea = 9;
    int areaLimit = 15;
    int thresholdLimit = 200;
    int dpLimit = 60;
};


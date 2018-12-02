#pragma once
#include <memory>
#include <iostream>
#include <opencv2/opencv.hpp>

#include "definitions.h"
#include "Puck.h"

class Tracker
{
public:
    Tracker();
    void Tick(const cv::Mat &src, cv::Mat &dst, Puck& puck);
    ~Tracker();
};


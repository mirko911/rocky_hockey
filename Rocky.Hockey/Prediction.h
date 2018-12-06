#pragma once

#include <array>

#include "definitions.h"
#include "Puck.h"

class Prediction
{
private:
    std::array<Wall, 4> m_walls;
    Vector m_predictedPosition;
    std::deque<Vector> m_predictionQueue;
    Wall m_defendLine;
public:
    Prediction();
    void tick(cv::Mat &dst, Puck &puck);
    void setFieldSize(const cv::Size &size);
    ~Prediction();
};


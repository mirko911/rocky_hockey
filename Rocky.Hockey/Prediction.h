#pragma once

#include <array>

#include "definitions.h"
#include "Puck.h"

struct Reflection {
	Vector position;
	Vector direction;
};

class Prediction
{
private:
    std::array<Wall, 4> m_walls;
    Vector m_predictedPosition;
    std::deque<Vector> m_predictionQueue;
	std::vector<Reflection> m_reflections;
    Wall m_defendLine;
	std::vector<Vector> m_positions;
public:
    Prediction();
    void tick(cv::Mat &dst, Puck &puck);
    void setFieldSize(const cv::Size &size);
	std::vector<Reflection> getReflections() const;
	Vector getPredictedPosition() const;
	std::array<Wall, 4> getWalls() const;
	Wall getDefendLine() const;
    ~Prediction();
};


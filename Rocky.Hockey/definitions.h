#pragma once

#include <string>
#include <Eigen/Dense>
#include <opencv2/opencv.hpp>
#include "Config.h"

typedef Eigen::Vector2f Vector;
typedef Eigen::Hyperplane<float, 2> Line;


static constexpr struct ConfigLegacy {
    int camWidth = 480;
    int camHeight = 240;
    int targetFPS = 10;
    int fieldWidth = 1000;
    int fieldHeight = 1000;
    int wrongDetectionThreshold = 10;
    int puckRadius = 8;
} config;

struct Wall {
    Line line;
    Vector start; //Keep start and end position for visual debugging
    Vector end;
    //Normal Vector
    Vector normal;
};

inline cv::Point Vector2Point(const Vector& vector) {
    return cv::Point2f(vector.x(), vector.y());
}

inline Vector Point2Vector(const cv::Point & point) {
    return Vector(point.x, point.y);
}

inline Vector getMean(const std::deque<Vector>& queue)
{
    Vector vector(0, 0);
    for (const auto &item : queue) {
        vector += item;
    }
    return vector /= static_cast<float>(queue.size());
}

inline std::string printVector(const Vector& vector) {
    return "[" + std::to_string(vector.x()) + "," + std::to_string(vector.y()) + "]";
}

inline bool isInsideWall(const Wall& wall, const Vector& vector) {
    float x = vector.x(); float y = vector.y();

    float leftx = wall.start.x();
    float lefty = wall.start.y();

    float rightx = wall.end.x();
    float righty = wall.end.y();

    return ((x >= leftx - 0.0003f && x <= rightx + 0.0003f) || (x >= rightx - 0.0003f && x <= leftx + 0.0003f))
        && ((y >= lefty - 0.0003f && y <= righty + 0.0003f) || (y >= righty - 0.0003f && y <= lefty + 0.0003f));
}
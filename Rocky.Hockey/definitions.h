#pragma once

#include <Eigen/Dense>

typedef Eigen::Vector2f Vector;

static constexpr struct Config {
    int camWidth = 480;
    int camHeight = 240;
    int targetFPS = 25;
    int fieldWidth = 1000;
    int fieldHeight = 1000;
} config;

struct Wall {
    Vector start;
    Vector end;

    //Normal Vector
    Vector normal;
};
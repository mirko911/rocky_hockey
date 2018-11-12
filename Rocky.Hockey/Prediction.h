#pragma once

#include <array>

#include "definitions.h"

class Prediction
{
private:
    std::array<Wall, 4> m_walls;
public:
    Prediction();
    ~Prediction();
};


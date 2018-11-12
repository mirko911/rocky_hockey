#pragma once

#include <queue>
#include <iostream>

#include <Eigen/Dense>

#include "definitions.h"

class Puck
{
private:
    Vector m_position;
    Vector m_direction;
    float m_radius;
    float m_queueLimit = 10;
    std::queue<Vector> m_positionQueue;
public:
    Puck(const float radius);
    Puck();

    void setRadius(const float radius);
    void setPosition(const Vector &position);
    void setDirection(const Vector &direction);

    float getRadius() const;
    Vector getPosition() const;
    Vector getDirection() const;
    std::queue<Vector>& getPositionQueue();

    ~Puck();
};


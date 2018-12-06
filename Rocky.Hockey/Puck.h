#pragma once

#include <deque>
#include <iostream>

#include <Eigen/Dense>

#include "definitions.h"

class Puck
{
private:
    Vector m_position;
    Vector m_direction;
    float m_velocity;
    float m_radius;
    float m_queueLimit = 10;
    std::deque<Vector> m_positionQueue;
    std::deque<Vector> m_directionQueue;

public:
    Puck(const float radius);
    Puck();

    void setRadius(const float radius);
    void setPosition(const Vector &position);
    void setDirection(const Vector &direction);
    void setVelocity(const float velocity);

    float getRadius() const;
    Vector getPosition() const;
    Vector getDirection() const;
    float getVelocity() const;
    std::deque<Vector> getPositionQueue();
    std::deque<Vector> getDirectionQueue();

    void resetPosition();
    void resetDirection();
    void resetVelocity();
    ~Puck();
};


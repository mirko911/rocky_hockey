#pragma once

#include <deque>
#include <iostream>

#include <Eigen/Dense>

#include "definitions.h"

class Puck
{
private:
	std::deque<Vector> m_positionQueue;
	std::deque<Vector> m_directionQueue;
    Vector m_position;
    Vector m_direction;
    float m_radius;
    float m_queueLimit = 10;
	unsigned int m_smoothingStepSize = 20; //create a trackbar for this?

public:
    Puck(const float radius);
    Puck();

    void setRadius(const float radius);
    void setPosition(const Vector &position);

    float getRadius() const;
    Vector getPosition() const;
    Vector getDirection() const;
    std::deque<Vector> getPositionQueue();
    std::deque<Vector> getDirectionQueue();

    ~Puck();
};


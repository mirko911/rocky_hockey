#pragma once

#include <queue>
#include <iostream>

#include <Eigen/Dense>

class Puck
{
private:
    Eigen::Vector2f m_position;
    Eigen::Vector2f m_direction;
    float m_radius;
    float m_queueLimit = 10;
    std::queue<Eigen::Vector2f> m_positionQueue;
public:
    Puck(const float radius);
    Puck();

    void setRadius(const float radius);
    void setPosition(const Eigen::Vector2f &position);
    void setDirection(const Eigen::Vector2f &direction);

    float getRadius() const;
    Eigen::Vector2f getPosition() const;
    Eigen::Vector2f getDirection() const;
    std::queue<Eigen::Vector2f>& getPositionQueue();

    ~Puck();
};


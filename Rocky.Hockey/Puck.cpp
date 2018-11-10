#include "Puck.h"

Puck::Puck(const float radius) : m_position(-1,-1), m_direction(0,0), m_radius(radius)
{
    std::cout << "[Puck] spawned puck at position (-1,-1) with radius " << m_radius << std::endl;
}

Puck::Puck() :  m_position(-1, -1), m_direction(0, 0), m_radius(10)
{
    std::cout << "[Puck] spawned default puck at position (-1,-1) with radius " << m_radius << std::endl;
}

void Puck::setRadius(const float radius)
{
    m_radius = radius;
}

void Puck::setPosition(const Eigen::Vector2f & position)
{
    m_position = position;
    m_positionQueue.push(position);
    if (m_positionQueue.size() > m_queueLimit)
    {
        std::cout << "[Puck] positionQueue is full. Start to pop entries" << std::endl;
        m_positionQueue.pop();
    }
}

void Puck::setDirection(const Eigen::Vector2f & direction)
{
    m_direction = direction.normalized();
}

float Puck::getRadius() const
{
    return m_radius;
}

Eigen::Vector2f Puck::getPosition() const
{
    return m_position;
}

Eigen::Vector2f Puck::getDirection() const
{
    return m_direction;
}

std::queue<Eigen::Vector2f>& Puck::getPositionQueue()
{
    return m_positionQueue;
}

Puck::~Puck()
{
}

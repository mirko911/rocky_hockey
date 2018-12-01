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

void Puck::setPosition(const Vector & position)
{
    m_position = position;
    m_positionQueue.push_back(position);
    if (m_positionQueue.size() > m_queueLimit)
    {
        m_positionQueue.pop_front();
    }
}

void Puck::setDirection(const Vector & direction)
{
    m_direction = direction.normalized();
    m_directionQueue.push_back(m_direction);
    if (m_directionQueue.size() > m_queueLimit)
    {
        m_directionQueue.pop_front();
    }
}

float Puck::getRadius() const
{
    return m_radius;
}

Vector Puck::getPosition() const
{
    return m_position;
}

Vector Puck::getDirection() const
{
    return m_direction;
}

std::deque<Vector> Puck::getPositionQueue()
{
    return m_positionQueue;
}

std::deque<Vector> Puck::getDirectionQueue()
{
    return m_directionQueue;
}

Puck::~Puck()
{
}

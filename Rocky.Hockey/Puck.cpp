#include "Puck.h"

Puck::Puck(const float radius) : m_position(NAN, NAN), m_direction(NAN,NAN), m_radius(radius)
{
    std::cout << "[Puck] spawned puck at position (-1,-1) with radius " << m_radius << std::endl;
    //m_directionQueue.push_back(m_direction);
}

Puck::Puck() :  m_position(NAN, NAN), m_direction(NAN, NAN), m_radius(10)
{
    std::cout << "[Puck] spawned default puck at position (-1,-1) with radius " << m_radius << std::endl;
    m_positionQueue.push_back(m_position);
   // m_directionQueue.push_back(m_direction);
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

void Puck::setVelocity(const float velocity)
{
    m_velocity = velocity;
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

float Puck::getVelocity() const
{
    return m_velocity;
}

std::deque<Vector> Puck::getPositionQueue()
{
    return m_positionQueue;
}

std::deque<Vector> Puck::getDirectionQueue()
{
    return m_directionQueue;
}

void Puck::resetPosition()
{
    m_position = Vector(NAN, NAN);
    m_positionQueue.clear();
}

void Puck::resetDirection()
{
    m_direction = Vector(NAN, NAN);
    m_directionQueue.clear();
    //m_directionQueue.push_back(m_direction);
}

void Puck::resetVelocity()
{
    m_velocity = 0.0f;
}

Puck::~Puck()
{
}

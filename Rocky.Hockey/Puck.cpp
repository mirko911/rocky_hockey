#include "Puck.h"

Puck::Puck(const float radius) : m_position(-1,-1), m_direction(0,0), m_radius(radius)
{
    std::cout << "[Puck] spawned puck at position (-1,-1) with radius " << m_radius << std::endl;
    m_positionQueue.push_back(m_position);
    m_directionQueue.push_back(m_direction);
}

Puck::Puck() :  m_position(-1, -1), m_direction(0, 0), m_radius(10)
{
    std::cout << "[Puck] spawned default puck at position (-1,-1) with radius " << m_radius << std::endl;
    m_positionQueue.push_back(m_position);
    m_directionQueue.push_back(m_direction);
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

	//TODO: check if direction changed to the other side wall, this would make the current direction calculation wrong
	//the position buffer and direction buffer would have to be wiped in that case
	if (m_positionQueue.size() > m_smoothingStepSize)
	{
		m_direction = (position - m_positionQueue[m_smoothingStepSize]).normalized();

		//add to direction buffer
		m_directionQueue.push_back(m_direction);
		if (m_directionQueue.size() > m_queueLimit)
		{
			m_directionQueue.pop_front();
		}

		//this does not seem to make to much of a difference, the blend might have to be weighted differently 
		/*
		//blend current direction with previous directions, all of these are normalized, current direction is counted doubly
		for (const auto &oldDirection : m_directionQueue) {
			m_direction += oldDirection;
		}
		m_direction /= m_directionQueue.size();
		*/
	}
	else
	{
		m_direction = (position - m_positionQueue[1]).normalized();

		m_directionQueue.push_back(m_direction);
		if (m_directionQueue.size() > m_queueLimit)
		{
			m_directionQueue.pop_front();
		}
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

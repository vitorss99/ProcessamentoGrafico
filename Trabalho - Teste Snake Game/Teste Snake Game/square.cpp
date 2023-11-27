#include "square.h"

#include <cstdlib>

Square::Square(float x, float y, float length)
{
    // constructor
    m_Position = glm::vec3(x, y, 0.0f);
    m_Color.r = (std::rand() % 25) / 25.0;
    m_Color.g = (std::rand() % 25) / 25.0;
    m_Length = length;
    m_Direction = DIR_RIGHT; // default direction
}

glm::vec3 Square::getPosition()
{
    return m_Position;
}

glm::vec4 Square::getColor()
{
    return m_Color;
}

Square::DIRECTION Square::getDirection()
{
    return m_Direction;
}

void Square::setDirection(DIRECTION direction)
{
    m_Direction = direction;
}

void Square::move()
{
    // updates the square block's position
    switch (m_Direction)
    {
    case DIR_LEFT:
        m_Position -= glm::vec3(m_Length, 0.0f, 0.0f);
        break;
    case DIR_RIGHT:
        m_Position += glm::vec3(m_Length, 0.0f, 0.0f);
        break;
    case DIR_UP:
        m_Position += glm::vec3(0.0f, m_Length, 0.0f);
        break;
    case DIR_DOWN:
        m_Position -= glm::vec3(0.0f, m_Length, 0.0f);
        break;

    default:
        break;
    }
}

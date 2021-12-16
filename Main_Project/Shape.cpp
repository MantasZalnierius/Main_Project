#include "Shape.h"

Shape::Shape(float t_posX, float t_posY)
{
	m_posX = t_posX;
	m_posY = t_posY;
	m_velX = 0.0f;
	m_velY = 0.0f;
}

void Shape::setPosition(float t_posX, float t_posY)
{
	m_posX = t_posX;
	m_posY = t_posY;
}

void Shape::setVelocity(float t_velX, float t_velY)
{
	m_velX = t_velX;
	m_velY = t_velY;
}

#pragma once
#include <SFML/Graphics.hpp>
class Shape
{
public:
	Shape() = default;
	Shape(float t_posX, float t_posY);
	virtual ~Shape() {};
	virtual void render(sf::RenderWindow& t_window) = 0;
	virtual void update(sf::Time t_deltaTime) = 0;
	void setPosition(float t_posX, float t_posY);
	void setVelocity(float t_velX, float t_velY);
	inline float getPositionX() { return m_posX; }
	inline float getPositionY() { return m_posY; }
	inline float getVelocityX() { return m_velX; }
	inline float getVelocityY() { return m_velY; }
private:
	float m_posX, m_posY;
	float m_velX, m_velY;
};

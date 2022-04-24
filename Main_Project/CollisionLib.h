#pragma once
#include "GlobalHeaders.h"
#include "Object.h"
class CollisionHandler
{
public:
	static inline void Box2Box(std::weak_ptr<Object> t_first, std::weak_ptr<Object> t_second, int& t_collisionChecks)
	{
		t_collisionChecks++;
		if (t_first.lock()->getGlobalBounds().intersects(t_second.lock()->getGlobalBounds()))
		{
			t_first.lock()->setColor(sf::Color::Green);
			t_second.lock()->setColor(sf::Color::Green);
		}
	}
	static float sqauaredDistance(const sf::Vector2f& t_first, const sf::Vector2f& t_second)
	{
		float x = t_first.x - t_second.x;
		float y = t_first.y - t_second.y;
		return ((x * x) + (y * y));
	}
};

#pragma once
#include "GlobalHeaders.h"
#include "Object.h"
class CollisionHandler
{
public:
	static inline void Circle2Circle(std::weak_ptr<Object> t_first, std::weak_ptr<Object> t_second)
	{
		if (t_first.lock()->getGlobalBounds().intersects(t_second.lock()->getGlobalBounds()))
		{
			t_first.lock()->setColor(sf::Color::Cyan);
			t_second.lock()->setColor(sf::Color::Cyan);
		}
	}
};

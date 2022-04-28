#pragma once
#include "GlobalHeaders.h"
#include "Object.h"
class CollisionHandler
{
public:
	/// <summary>
	/// This functions handles the collision between two rectangles in the game.
	/// </summary>
	/// <param name="t_first">Bounds of the first object</param>
	/// <param name="t_second">Bounds of the second object</param>
	/// <param name="t_collisionChecks">Number of collisions occuring per frame</param>
	static inline void Box2Box(std::weak_ptr<Object> t_first, std::weak_ptr<Object> t_second, int& t_collisionChecks)
	{
		t_collisionChecks++;
		if (t_first.lock()->getGlobalBounds().intersects(t_second.lock()->getGlobalBounds()))
		{
			// Change color if colliding.
			t_first.lock()->setColor(m_GREEN);
			t_second.lock()->setColor(m_GREEN);
		}
	}
	/// <summary>
	/// Gets the squared distance between two vectors.
	/// </summary>
	/// <param name="t_first">First Vector</param>
	/// <param name="t_second">Second Vector</param>
	/// <returns>The sqaured distance as a float</returns>
	static float sqauaredDistance(sf::Vector2f t_first, sf::Vector2f t_second)
	{
		float x = t_first.x - t_second.x;
		float y = t_first.y - t_second.y;
		return ((x * x) + (y * y));
	}

	/// <summary>
	/// Checks if two float rects are colliding with each other.
	/// </summary>
	/// <param name="t_firstObject">First rect</param>
	/// <param name="t_secondObject">Second rect</param>
	/// <returns>True if colliding else not</returns>
	static bool aabbCollision(sf::FloatRect t_firstObject, sf::FloatRect t_secondObject)
	{
		return (t_firstObject.left <= t_secondObject.left && t_firstObject.top <= t_secondObject.top
			&& t_firstObject.left + t_firstObject.width >= t_secondObject.left + t_secondObject.width
			&& t_firstObject.top + t_firstObject.height >= t_secondObject.top + t_secondObject.height);
	}

};

#ifndef GAME_HPP
#define GAME_HPP
#include <SFML/Graphics.hpp>
#include "Object.h"
#include "QuadTree.h"
#include "Window.h"
#include "CollisionLib.h"

class Game
{
public:
	Game();
	~Game();
	/// <summary>
	/// main method for game
	/// </summary>
	void run();
private:

	void processEvents();
	void processMouseEvents(sf::Event t_event);
	void processKeyEvents(sf::Event t_event);
	void update(sf::Time t_deltaTime);
	void render();
	void handleCollisions();
	bool m_exitGame; // control exiting game
	std::vector<std::shared_ptr<Object>> m_objects;
	QuadTree m_spaitalMap{ sf::FloatRect{ 0U, 0U, 800U, 600U }, 15U, 1U };
	bool m_showSpatialMap = false;
};

#endif // !GAME_HPP

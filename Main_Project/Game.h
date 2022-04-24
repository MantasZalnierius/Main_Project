#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include "Object.h"
#include "QuadTree.h"
#include "Window.h"
#include "CollisionLib.h"
#include "SpitialHasing.h"

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
	void updateWithNoGrid(sf::Time t_deltaTime);
	void updateWithSpitalGrid(sf::Time t_deltaTime, std::shared_ptr<Object> t_object);
	void updateWithQuadTree(sf::Time t_deltaTime);
	void render();
	void handleCollisions();
	void setUpFontAndText();

	bool m_useSpaitalHasing = false;
	bool m_NoGrid = true;
	bool m_exitGame; // control exiting game
	std::vector<std::shared_ptr<Object>> m_objects;
	bool m_showSpatialMap = false;
	QuadTree m_quadTreeMap;
	SpatialHash m_spaitalHashMap;
	int framesPerSecond;
	int m_collisionChecks;
	// Text to display data in the scene.
	sf::Font m_font;
	sf::Text m_collisionNumberText;
	sf::Text m_numberOfObjectsText;
	sf::Text m_framesText;
	sf::Text m_typeOfGridText;
	std::string m_typeOfGrid;
};

#endif // !GAME_HPP

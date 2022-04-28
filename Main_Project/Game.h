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
	/// runs the game
	/// </summary>
	void run();
private:
	/// <summary>
	/// Handles events for the game.
	/// </summary>
	void processEvents();

	/// <summary>
	/// Handles mouse events for the game.
	/// </summary>
	/// <param name="t_event">Mouse event</param>
	void processMouseEvents(sf::Event t_event);

	/// <summary>
	/// Handles Keyboard events for the game.
	/// </summary>
	/// <param name="t_event">Keyboard event.</param>
	void processKeyEvents(sf::Event t_event);

	/// <summary>
	/// Updates the game every 60th of a second.
	/// </summary>
	/// <param name="t_deltaTime">time since last update.</param>
	void update(sf::Time t_deltaTime);

	/// <summary>
	/// Updates the game with no grid.
	/// </summary>
	/// <param name="t_deltaTime">time since last update.</param>
	void updateWithNoGrid(sf::Time t_deltaTime);

	/// <summary>
	/// Updates the game with the spatial grid active.
	/// </summary>
	/// <param name="t_deltaTime">time since last update.</param>
	/// <param name="t_object">A rectangle within the game.</param>
	void updateWithSpitalGrid(sf::Time t_deltaTime, std::shared_ptr<Object> t_object);

	/// <summary>
	/// Updates the game with the quad-tree grid active. 
	/// </summary>
	/// <param name="t_deltaTime">time since last update.</param>
	void updateWithQuadTree(sf::Time t_deltaTime);

	/// <summary>
	/// Renders the game.
	/// </summary>
	void render();

	/// <summary>
	/// Handles collisions with the quad-grid active.
	/// </summary>
	void handleQuadTreeCollisions();

	/// <summary>
	/// Sets up the font and text for the game.
	/// </summary>
	void setUpFontAndText();

	bool m_useSpaitalHasing = false; // If false uses quad-tree grid otherwise if true use the spatial grid.
	bool m_NoGrid = true; // If you don't want to use a grid.
	bool m_exitGame; // control exiting game
	std::vector<std::shared_ptr<Object>> m_objects;
	bool m_showGrids = false; // show the grids if active otherwise don't.
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

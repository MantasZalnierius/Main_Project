#include "Game.h"
Game::Game() :
	m_exitGame{ false },
	m_quadTreeMap(sf::FloatRect{ 0U, 0U, static_cast<float>(Window::getWindowWidth()), static_cast<float>(Window::getWindowHeight()) }, 30U, 3U),
	m_spaitalHashMap()
{
	setUpFontAndText();
}

void Game::setUpFontAndText()
{
	m_collisionChecks = 0;
	if (!m_font.loadFromFile("ASSETS\\FONTS\\ariblk.ttf"))
	{
		std::cout << "problem loading arial black font" << std::endl;
	}

	m_typeOfGrid = " NONE";

	m_collisionNumberText.setFont(m_font);
	m_collisionNumberText.setString("COLLISION CHECKS: " + std::to_string(m_collisionChecks));
	m_collisionNumberText.setCharacterSize(20);
	m_collisionNumberText.setFillColor(sf::Color::Black);
	m_collisionNumberText.setPosition(1205, 50);

	m_numberOfObjectsText.setFont(m_font);
	m_numberOfObjectsText.setString("OBJECTS: " + std::to_string(m_objects.size()));
	m_numberOfObjectsText.setCharacterSize(20);
	m_numberOfObjectsText.setFillColor(sf::Color::Black);
	m_numberOfObjectsText.setPosition(1205, 100);

	m_framesText.setFont(m_font);
	m_framesText.setString("FRAME RATE: " + std::to_string(framesPerSecond));
	m_framesText.setCharacterSize(20);
	m_framesText.setFillColor(sf::Color::Black);
	m_framesText.setPosition(1205, 150);

	m_typeOfGridText.setFont(m_font);
	m_typeOfGridText.setString("GRID TYPE: " + m_typeOfGrid);
	m_typeOfGridText.setCharacterSize(20);
	m_typeOfGridText.setFillColor(sf::Color::Black);
	m_typeOfGridText.setPosition(1205, 200);
}

Game::~Game()
{
}

void Game::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	const float fps{ 60.0f };
	sf::Time timePerFrame = sf::seconds(1.0f / fps); // 60 fps
	while (Window::getWindow().isOpen())
	{
		processEvents(); // as many as possible
		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate > timePerFrame)
		{
			framesPerSecond = static_cast<int>(1.0f / timeSinceLastUpdate.asSeconds());
			m_framesText.setString("FRAME RATE: " + std::to_string(framesPerSecond));
			timeSinceLastUpdate -= timePerFrame;
			processEvents(); // at least 60 fps
			update(timePerFrame); //60 fps
		}
		render(); // as many as possible
	}
}

void Game::processEvents()
{
	sf::Event newEvent;
	while (Window::getWindow().pollEvent(newEvent))
	{
		if (sf::Event::Closed == newEvent.type) // window message
		{
			m_exitGame = true;
		}
		if (sf::Event::MouseButtonPressed == newEvent.type)
		{
			processMouseEvents(newEvent);
		}
		if (sf::Event::KeyReleased == newEvent.type)
		{
			processKeyEvents(newEvent);
		}
	}
}

void Game::processMouseEvents(sf::Event t_event)
{
	if (t_event.key.code == sf::Mouse::Left)
	{
		sf::Vector2f mousePos = Window::getWindow().mapPixelToCoords(sf::Mouse::getPosition(Window::getWindow()));
		if (mousePos.x >= 0 && mousePos.x <= Window::getWindowWidth() - 20)
		{
			float velX = static_cast<float>((rand() % 100) + 1);
			float velY = static_cast<float>((rand() % 100) + 1);
			std::shared_ptr<Object> obj = std::make_shared<Object>(mousePos);
			obj->setColor(sf::Color::Red);
			m_objects.emplace_back(obj);
			m_objects.back()->setVelocity({ velX, velY });
			if (m_useSpaitalHasing)
			{
				m_spaitalHashMap.addObject(obj);
			}
			else
			{
				m_quadTreeMap.insert(m_objects);
			}
			m_numberOfObjectsText.setString("OBJECTS: " + std::to_string(m_objects.size()));
		}
	}
}

void Game::processKeyEvents(sf::Event t_event)
{
	if (t_event.key.code == sf::Keyboard::P)
	{
		m_showSpatialMap = (m_showSpatialMap == true) ? false : true;
	}
	if (t_event.key.code == sf::Keyboard::Space)
	{
		m_useSpaitalHasing = (m_useSpaitalHasing == true) ? false : true;
	}
	if (t_event.key.code == sf::Keyboard::Enter)
	{
		m_NoGrid = (m_NoGrid == true) ? false : true;
	}

	if (m_NoGrid)
	{
		m_typeOfGrid = " NO GRID";
	}
	else if (m_useSpaitalHasing && !m_NoGrid)
	{
		m_typeOfGrid = " SPAITAL HASING GRID";
	}
	else
	{
		m_typeOfGrid = " QUAD-TREE GRID";
	}
	m_typeOfGridText.setString("GRID TYPE: " + m_typeOfGrid);
}

void Game::update(sf::Time t_deltaTime)
{
	m_collisionChecks = 0;
	for (auto& object : m_objects) {
		object->setColor(sf::Color::Red);
		object->update(t_deltaTime);
		updateWithNoGrid(t_deltaTime);
		updateWithSpitalGrid(t_deltaTime, object);
	}
	updateWithQuadTree(t_deltaTime);
	m_collisionNumberText.setString("COLLISION CHECKS: " + std::to_string(m_collisionChecks));
}

void Game::updateWithNoGrid(sf::Time t_deltaTime)
{
	if (m_NoGrid) // NO GRID
	{
		for (int i = 0; i < m_objects.size(); i++)
		{
			for (int j = 0; j < m_objects.size(); j++)
			{
				if (i == j) continue;
				CollisionHandler::Box2Box(m_objects.at(i), m_objects.at(j), m_collisionChecks);
			}
		}
	}
}

void Game::updateWithSpitalGrid(sf::Time t_deltaTime, std::shared_ptr<Object> t_object)
{
	std::vector<std::shared_ptr<Object>> colliders;
	if (m_objects.size() > 1 && m_useSpaitalHasing && !m_NoGrid) // SPITAL HASHING.
	{
		colliders = m_spaitalHashMap.spaitalSearch(t_object, 90);
		for (auto& collider : colliders)
		{
			CollisionHandler::Box2Box(collider, t_object, m_collisionChecks);
		}
		m_spaitalHashMap.updateObject(t_object);
	}
}

void Game::updateWithQuadTree(sf::Time t_deltaTime)
{
	if (!m_useSpaitalHasing && !m_NoGrid) // QUAD-TREE.
	{
		m_quadTreeMap.clear();
		m_quadTreeMap.insert(m_objects);
		handleCollisions();
	}
}

void Game::render()
{
	Window::getWindow().clear(sf::Color::White);
	for (auto const& object : m_objects)
		Window::getWindow().draw(*object);
	if (m_showSpatialMap && !m_useSpaitalHasing && !m_NoGrid)
	{
		m_quadTreeMap.render(Window::getWindow());
	}
	else if (m_showSpatialMap && !m_NoGrid)
	{
		m_spaitalHashMap.render();
	}
	Window::getWindow().draw(m_collisionNumberText);
	Window::getWindow().draw(m_numberOfObjectsText);
	Window::getWindow().draw(m_framesText);
	Window::getWindow().draw(m_typeOfGridText);
	Window::getWindow().display();
}

void Game::handleCollisions()
{
	if (m_objects.size() <= 0)
	{
		return;
	}
	else
	{
		for (int i = 0; i < m_objects.size(); ++i)
		{
			std::vector<std::weak_ptr<Object>> nearbyColliders = m_quadTreeMap.getColliders(m_objects[i]->getGlobalBounds());
			if (nearbyColliders.size() <= 1)
			{
				continue;
			}
			else
			{
				for (int j = 0; j < nearbyColliders.size(); ++j)
				{

					if (nearbyColliders.at(j).lock().get() == m_objects.at(i).get())
					{
						continue;
					}
					else
					{
						CollisionHandler::Box2Box(nearbyColliders.at(j), m_objects.at(i), m_collisionChecks);
					}
				}
			}
		}
	}
}
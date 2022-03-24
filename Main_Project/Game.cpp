#include "Game.h"

Game::Game() : m_exitGame{ false }
{
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
		float velX = static_cast<float>((rand() % 100) + 1);
		float velY = static_cast<float>((rand() % 100) + 1);
		sf::Vector2f mousePos = Window::getWindow().mapPixelToCoords(sf::Mouse::getPosition(Window::getWindow()));
		std::shared_ptr<Object> obj = std::make_shared<Object>(mousePos);
		obj->setColor(sf::Color::Red);
		m_objects.emplace_back(obj);
		m_objects.back()->setVelocity({ velX, velY });
		m_spaitalMap.insert(m_objects);
	}
}

void Game::processKeyEvents(sf::Event t_event)
{
	if (t_event.key.code == sf::Keyboard::P)
	{
		m_showSpatialMap = (m_showSpatialMap == true) ? false : true;
	}
}

void Game::update(sf::Time t_deltaTime)
{
	for (auto& object : m_objects) {
		object->update(t_deltaTime);
		if (!Node::contains({ 0U, 0U, 800U, 600U }, object->getGlobalBounds()))
			object->setVelocity({ object->getVelocity().y, -object->getVelocity().x });
	}

	m_spaitalMap.clear();
	m_spaitalMap.insert(m_objects);
	handleCollisions();
}

void Game::render()
{
	// Clear the window
	Window::getWindow().clear(sf::Color::Black);

	// Draw the balls
	for (auto const& object : m_objects)
		Window::getWindow().draw(*object);

	if (m_showSpatialMap)
	{
		m_spaitalMap.drawBorders(Window::getWindow());
	}

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
		for (int i = 0; i < m_objects.size() - 1; ++i)
		{
			std::vector<std::weak_ptr<Object>> nearbyColliders =
				m_spaitalMap.getCollisionable(m_objects[i]->getGlobalBounds());
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
						CollisionHandler::Circle2Circle(nearbyColliders.at(j), m_objects.at(i));
					}
				}
			}
		}
	}
}
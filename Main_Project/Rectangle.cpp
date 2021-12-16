#include "Rectangle.h"
#include <iostream>
Rectangle::Rectangle(float t_posX, float t_posY, float t_width, float t_height) : Shape(t_posX, t_posY)
{
	m_height = t_height;
	m_width = t_width;
	m_body.setSize(sf::Vector2f(m_width, m_height));
	m_body.setPosition(getPositionX(), getPositionY());
	m_adsrEnevelope = new ADSREnevelope();
}

void Rectangle::updatePosition(sf::Time t_deltaTime)
{
	m_adsrEnevelope->update(t_deltaTime);
	float newX = getPositionX();
	float newY = getPositionY();
	newX += m_adsrEnevelope->getVelX();
	newY += m_adsrEnevelope->getVelY();
	if (!m_adsrEnevelope->getIsGrounded())
	{
		if (newY >= 100.0f)
		{
			setPosition(getPositionX(), 100.0f);
			m_adsrEnevelope->setisGrounded(true);
			m_adsrEnevelope->setVelY(0.0f);
		}
		else
		{
			setPosition(newX, newY);
		}
	}
	else
	{
		setPosition(newX, newY);
	}
	m_body.setPosition(getPositionX(), getPositionY());
}

void Rectangle::update(sf::Time t_deltaTime)
{
	updatePosition(t_deltaTime);
}

void Rectangle::render(sf::RenderWindow& t_window)
{
	t_window.draw(this->getBody());
}

void Rectangle::processKeyUp(sf::Event t_event)
{
	if (sf::Keyboard::Left == t_event.key.code)
	{
		m_adsrEnevelope->setisMovingLeft(false);
	}
	if (sf::Keyboard::Right == t_event.key.code)
	{
		m_adsrEnevelope->setisMovingRight(false);
	}
	if (sf::Keyboard::Space == t_event.key.code)
	{
		m_adsrEnevelope->setisJumping(false);
		m_adsrEnevelope->setIsJumpingHeld(false);
	}
}

void Rectangle::processKeyDown(sf::Event t_event)
{
	if (sf::Keyboard::Left == t_event.key.code)
	{
		m_adsrEnevelope->setisMovingLeft(true);
	}
	if (sf::Keyboard::Right == t_event.key.code)
	{
		m_adsrEnevelope->setisMovingRight(true);
	}
	if (sf::Keyboard::Space == t_event.key.code && m_adsrEnevelope->getIsGrounded())
	{
		m_adsrEnevelope->setisJumping(true);
	}
}

#include "Object.h"

Object::Object(sf::Vector2f t_position)
{
    // sets up the shape of the object.
    m_shape.setSize(sf::Vector2f(m_OBJECT_SIZE, m_OBJECT_SIZE));
    m_shape.setFillColor(m_RED);
    m_shape.setPosition(t_position);
}

void Object::update(sf::Time t_deltaTime)
{
    // updates the new position of the object with the velocity and acclearation over time.
    sf::Vector2f newPos = m_shape.getPosition() + ((m_velocity * m_acclearation) * t_deltaTime.asSeconds());
    m_shape.setPosition(newPos);
    handleBorderCollisions();
}

void Object::handleBorderCollisions()
{
    // handles border collisions.
    if (m_shape.getPosition().x + getGlobalBounds().width > Window::getWindowWidth())
    {
        setVelocity({ getVelocity().y, -getVelocity().x });
    }
    if (m_shape.getPosition().x < 0)
    {
        setVelocity({ getVelocity().y, -getVelocity().x });
    }
    if (m_shape.getPosition().y + getGlobalBounds().height > Window::getWindowHeight())
    {
        setVelocity({ -getVelocity().y, getVelocity().x });
    }
    if (m_shape.getPosition().y < 0)
    {
        setVelocity({ -getVelocity().y, getVelocity().x });
    }
    // handles border collisions.
}

void Object::render()
{
    Window::getWindow().draw(m_shape);
}
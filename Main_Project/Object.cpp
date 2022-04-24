#include "Object.h"

Object::Object(sf::Vector2f t_position)
{
    m_shape.setSize(sf::Vector2f(20, 20));
    m_shape.setFillColor(sf::Color::Red);
    setPosition(t_position);
}

void Object::update(sf::Time const& dt)
{
    sf::Vector2f initalPos = getPosition();
    setPosition(initalPos + (m_velocity * dt.asSeconds() * 3.0f));
    handleWindowBorderCollisions();
}

void Object::setVelocity(sf::Vector2f v)
{
    m_velocity = v;
}

sf::Vector2f const& Object::getVelocity() const
{
    return m_velocity;
}

void Object::setColor(sf::Color color)
{
    m_shape.setFillColor(color);
}

void Object::handleWindowBorderCollisions()
{
    if (!Node::contains({ 0U, 0U, static_cast<float>(Window::getWindowWidth()), static_cast<float>(Window::getWindowHeight()) }, getGlobalBounds()))
    {
        setVelocity({ getVelocity().y, -getVelocity().x });
    }
}

sf::FloatRect Object::getGlobalBounds() const noexcept
{
    return getTransform().transformRect(m_shape.getLocalBounds());
}

void Object::draw(sf::RenderTarget& target, sf::RenderStates states) const noexcept
{
    states.transform.combine(getTransform());
    target.draw(m_shape, states);
}
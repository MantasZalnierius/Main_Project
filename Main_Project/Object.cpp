#include "Object.h"
Object::Object(sf::Vector2f position)
{
    m_sprite.setSize(sf::Vector2f(20, 20));
    m_sprite.setFillColor(sf::Color::Red);
    setPosition(position);
}

void Object::update(sf::Time const& dt)
{
    sf::Vector2f initalPos = getPosition();
    setPosition(initalPos + (m_vel * dt.asSeconds()));
}

void Object::setVelocity(sf::Vector2f v)
{
    m_vel = std::move(v);
}

sf::Vector2f const& Object::getVelocity() const
{
    return m_vel;
}

void Object::setColor(sf::Color color)
{
    m_sprite.setFillColor(color);
}

sf::FloatRect Object::getGlobalBounds() const noexcept
{
    return getTransform().transformRect(m_sprite.getLocalBounds());
}

void Object::draw(sf::RenderTarget& target, sf::RenderStates states) const noexcept
{
    states.transform.combine(getTransform());

    target.draw(m_sprite, states);
}
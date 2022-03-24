#pragma once
#include "GlobalHeaders.h"

class Node;

/// An entity is a circle that moves around the screen
class Object : public sf::Transformable, public sf::Drawable
{
public:
    /// Construct a new entity at a given position
    explicit Object(sf::Vector2f position);

    /// Get the bounds of this entity, relative to its transform
    sf::FloatRect getGlobalBounds() const noexcept;

    /// Integrate over some amount of time
    void update(sf::Time const& dt);

    /// Set/get the current velocity
    void setVelocity(sf::Vector2f velocity);
    sf::Vector2f const& getVelocity() const;

    /// Set the circle's color
    void setColor(sf::Color color);

    void handleWindowBorderCollisions();

protected:
    /// Draws to screen
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const noexcept override;

    /// The circle
    sf::RectangleShape m_sprite;

    /// The current velocity
    sf::Vector2f m_vel;
};

#include "Node.h"
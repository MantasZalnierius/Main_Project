#pragma once
#include "GlobalHeaders.h"
class Object
{
public:
    Object() = default;
    /// <summary>
    /// Creates an object with a set posiition.
    /// </summary>
    /// <param name="t_position">position of the object to placed in.</param>
    Object(sf::Vector2f t_position);

    /// <summary>
    /// Updates an object every 60th of a second.
    /// </summary>
    /// <param name="t_deltaTime">Time since last update.</param>
    void update(sf::Time t_deltaTime);

    /// <summary>
    /// Renders the Object.
    /// </summary>
    void render();

    /// <summary>
    /// Gets the hash key from this object.
    /// </summary>
    /// <returns>returns a hash key.</returns>
    inline int getHashKey() { return m_hashKey; }

    /// <summary>
    /// Sets the hash key for this object.
    /// </summary>
    /// <param name="t_hashKey">a new hash key.</param>
    inline void setHashKey(int t_hashKey) { m_hashKey = t_hashKey; }

    /// <summary>
    /// Gets the velocity of this object.
    /// </summary>
    /// <returns>returns the current velocity of this object.</returns>
    inline sf::Vector2f getVelocity() { return m_velocity; }

    /// <summary>
    /// Gets the current position of this object.
    /// </summary>
    /// <returns>returns the current position of this object.</returns>
    inline sf::Vector2f getPosition() { return m_shape.getPosition(); }

    /// <summary>
    /// Sets the color of this object.
    /// </summary>
    /// <param name="t_color">a new color</param>
    inline void setColor(sf::Color t_color) { m_shape.setFillColor(t_color); }

    /// <summary>
    /// Sets the velocity of this object to a new velocity.
    /// </summary>
    /// <param name="t_velocity">a new velocity for this object.</param>
    inline void setVelocity(sf::Vector2f t_velocity) { m_velocity = t_velocity; }

    /// <summary>
    /// Gets the global bounds of this object.
    /// </summary>
    /// <returns>returns the objects global bounds.</returns>
    inline sf::FloatRect getGlobalBounds() { return m_shape.getGlobalBounds(); }
private:
    int m_hashKey; // Hash key for spaital hasing.

    /// <summary>
    /// This function handles boundary collisions.
    /// </summary>
    void handleBorderCollisions();

    sf::RectangleShape m_shape;
    sf::Vector2f m_velocity;
    float m_acclearation = 3.0f;
};
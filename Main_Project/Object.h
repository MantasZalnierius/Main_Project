#pragma once
#include "GlobalHeaders.h"

class Node;
class Object : public sf::Transformable,
    public sf::Drawable
{
private:
    int m_hashKey; // Hash key for spaital hasing.
public:
    explicit Object(sf::Vector2f t_position);
    sf::FloatRect getGlobalBounds() const noexcept;
    void update(sf::Time const& dt);
    void setVelocity(sf::Vector2f velocity);
    sf::Vector2f const& getVelocity() const;
    void setColor(sf::Color color);
    void handleWindowBorderCollisions();
    inline int getHashKey() { return m_hashKey; }
    inline void setHashKey(int t_hashKey) { m_hashKey = t_hashKey; }
protected:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const noexcept override;
    sf::RectangleShape m_shape;
    sf::Vector2f m_velocity;
};
#include "Node.h"
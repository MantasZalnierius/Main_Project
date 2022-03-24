#pragma once
#include "GlobalHeaders.h"
#include "Object.h"

class QuadTree;

enum Position 
{ 
    NW = 0,
    NE = 1, 
    SW = 2, 
    SE = 3 
};


class Node
{
    friend class QuadTree;
public:
    Node(sf::FloatRect bounds, size_t level,
        size_t const& maxLevel, size_t const& maxObjects);

    size_t countObjects() const;

    static bool contains(sf::FloatRect const& first, sf::FloatRect const& second);

private:
    /// These member functions should be called by this or the QuadTree class!

    void insert(std::weak_ptr<Object> object);

    void drawBorders(sf::RenderTarget& t) const;

    void subdivide();

    void clear();

    size_t getIndex(sf::FloatRect objBounds) const;

    std::vector<std::weak_ptr<Object>> getCollisionable(sf::FloatRect const& bounds) const;

    std::vector<std::weak_ptr<Object>> getAllObjects() const;

    /// Member variable declarations

    sf::FloatRect m_border;
    size_t const m_depth;
    size_t const& m_maxDepth, m_maxObjects;

    // Pointers
    Node* m_head{ nullptr };
    std::vector<std::weak_ptr<Object>> m_objects{ };
    std::vector<std::weak_ptr<Object>> m_collidedObjects{ };

    std::array<std::unique_ptr<Node>, 4> m_childNodes{ { nullptr, nullptr, nullptr, nullptr } };

    bool m_canSubdivide{ false };
};
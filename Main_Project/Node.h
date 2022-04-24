#pragma once
#include "GlobalHeaders.h"
#include "Object.h"

enum Cardinal
{
    North = 0,
    EAST = 1,
    WEST = 2,
    SOUTH = 3
};

class QuadTree;

class Node
{
    friend class QuadTree;
public:
    Node(sf::FloatRect t_shapeBounds, size_t t_level, size_t const& t_maxLevel, size_t const& t_maxObjects);
    size_t countObjects() const;
    static bool contains(sf::FloatRect const& t_firstObject, sf::FloatRect const& t_secondObject);
    void render(sf::RenderTarget& t_target) const;
private:
    void insert(std::weak_ptr<Object> object);
    void subdivide();
    void clear();
    size_t getIndex(sf::FloatRect t_objectBounds) const;
    std::vector<std::weak_ptr<Object>> getColliders(sf::FloatRect const& t_objectBounds) const;
    std::vector<std::weak_ptr<Object>> getObjects() const;

    bool m_canSubdivide{ false };
    sf::FloatRect m_shapeBounds;
    size_t const m_depth;
    size_t const& m_maxDepth;
    size_t const& m_maxObjects;
    Node* m_head{ nullptr };
    std::vector<std::weak_ptr<Object>> m_objects{ };
    std::vector<std::weak_ptr<Object>> m_colliders{ };
    std::array<std::unique_ptr<Node>, 4> m_childNodes{ { nullptr, nullptr, nullptr, nullptr } };
};
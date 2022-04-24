#pragma once
#include "GlobalHeaders.h"
#include "Node.h"

class QuadTree
{
public:
    explicit QuadTree(sf::FloatRect t_shapeBounds, size_t t_maxLevel, size_t t_maxObjects);
    size_t countObjects() const;
    void insert(std::vector<std::shared_ptr<Object>> const& t_objects);
    void clear();
    void render(sf::RenderTarget& t_target) const;
    std::vector<std::weak_ptr<Object>> getColliders(sf::FloatRect const& t_shapeBounds) const;
private:
    sf::FloatRect const m_shapeBounds;
    size_t const m_maxLevel;
    size_t const m_maxObjects;
    Node m_head;
};
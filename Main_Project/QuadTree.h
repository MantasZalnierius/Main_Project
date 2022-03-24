#pragma once
#include "GlobalHeaders.h"
#include "Node.h"

class QuadTree
{
public:
    explicit QuadTree(sf::FloatRect bounds,
        size_t maxLevel, size_t maxObjects);

    size_t countObjects() const;

    void insert(std::vector<std::shared_ptr<Object>> const& ptrArray);

    void clear();

    void drawBorders(sf::RenderTarget& t) const;

    std::vector<std::weak_ptr<Object>> getCollisionable(sf::FloatRect const& bounds) const;
private:
    sf::FloatRect const mBounds;
    size_t const mMaxLevel, mMaxObjects;

    Node mRoot;
};
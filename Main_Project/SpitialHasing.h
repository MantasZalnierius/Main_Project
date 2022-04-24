#pragma once
#include <unordered_set>
#include <SFML/Graphics.hpp>
#include "Object.h"
#include "CollisionLib.h"
#include "Window.h"

class SpatialHash
{

public:
    SpatialHash();
    ~SpatialHash();

    SpatialHash(SpatialHash&& t_other) noexcept;
    SpatialHash& operator=(SpatialHash&& t_other) noexcept;

    void addObject(std::shared_ptr<Object> t_object);
    void removeObject(std::shared_ptr<Object> t_object);
    void updateObject(std::shared_ptr<Object> t_object);
    void clear();
    std::vector<std::shared_ptr<Object>> spaitalSearch(std::shared_ptr<Object> t_object, int t_searchArea);
    std::vector<sf::RectangleShape> m_nodes;
    void render();
private:
    sf::Vector2u m_gridSize;
    std::map<int, std::unordered_set<std::shared_ptr<Object>>> m_grid;
    int* m_offsetBetweenCells;
    int* m_offset;

    int createHaskKey(sf::Vector2u t_position);
    sf::Vector2i getKey(sf::Vector2f t_position);
};
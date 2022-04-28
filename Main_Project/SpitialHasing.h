#pragma once
#include <unordered_set>
#include <SFML/Graphics.hpp>
#include "Object.h"
#include "CollisionLib.h"
#include "Window.h"

class SpatialHash
{
public:
    /// <summary>
    /// sets up the spatial hashing grid.
    /// </summary>
    SpatialHash();
    ~SpatialHash() = default;

    /// <summary>
    /// Adds an object to the grid.
    /// </summary>
    /// <param name="t_object">an object to be added.</param>
    void insert(std::shared_ptr<Object> t_object);

    /// <summary>
    /// removes an object from the grid
    /// </summary>
    /// <param name="t_object">Object to be removed.</param>
    void remove(std::shared_ptr<Object> t_object);

    /// <summary>
    /// Update the grid and the hash key values of every object.
    /// </summary>
    /// <param name="t_object">Object to be updated.</param>
    void update(std::shared_ptr<Object> t_object);
    
    /// <summary>
    /// Searches a objects surroundings, to see if there any objects close by to check collisions with.
    /// </summary>
    /// <param name="t_object">object to check around.</param>
    /// <param name="t_searchArea">the search area</param>
    /// <returns>returns a vector of collidable objects.</returns>
    std::vector<std::shared_ptr<Object>> spaitalSearch(std::shared_ptr<Object> t_object, int t_searchArea);

    /// <summary>
    /// Renders every cell within the grid.
    /// </summary>
    void render();
private:
    sf::Vector2u m_gridSize;
    std::map<int, std::unordered_set<std::shared_ptr<Object>>> m_hashTable; // A map with hash key values and a set of objects that corllate to the hash key.
    int* m_offsetBetweenCells;
    int* m_offset;
    std::vector<sf::RectangleShape> m_nodes; // nodes to be drawn in the render function

    /// <summary>
    /// Creates a hash key from the position passed in.
    /// </summary>
    /// <param name="t_position"></param>
    /// <returns>returns a new hash key.</returns>
    int createHaskKey(sf::Vector2u t_position);

    /// <summary>
    /// gets the position from the hash key value.
    /// </summary>
    /// <param name="t_position"></param>
    /// <returns>returns the position that collrates with the hash key.</returns>
    sf::Vector2i getPositionFromHashKey(sf::Vector2f t_position);

    /// <summary>
    /// Sets up the grid's hash key's
    /// </summary>
    void setUpGrid();

    /// <summary>
    /// Sets up all the cells for the grid that are needed to be drawn.
    /// </summary>
    void setupCellsWithinGrid();
};
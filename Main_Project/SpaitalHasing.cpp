#include "SpitialHasing.h"
#include <cstdlib>
#include <algorithm>
#include "SpitialHasing.h"

SpatialHash::SpatialHash()
{
    // Sets up all the grid's hash key's and sets up all of the cells within the grid.
    setUpGrid();
    setupCellsWithinGrid();
    // Sets up all the grid's hash key's and sets up all of the cells within the grid.
}

void SpatialHash::setUpGrid()
{
    // Sets the grid size to be size of the window provided.
    // For showcasing data in the game, I have set window length to be shorter here in the x axis.
    // Then the actual window.
    m_gridSize.x = ceil(Window::getWindowWidth() / m_CELL_SIZE);
    m_gridSize.y = ceil(Window::getWindowHeight() / m_CELL_SIZE);

    sf::Vector2u center = m_gridSize / 2u;
    int startingKey = createHaskKey(center); // Gets the start hash key via the center position of the grid.

    std::map<int, std::unordered_set<int>> offsetsFromCenter{};
    int maxDistSquared = 0;

    // sets up the grids hash values.
    for (int row = 0; row < m_gridSize.y; row++)
    {
        for (int col = 0; col < m_gridSize.x; col++)
        {
            if (row == center.y && col == center.x)
            {
                offsetsFromCenter[0].insert(0);
                continue;
            }
            else
            {
                int key = createHaskKey(sf::Vector2u(col, row));
                int x = std::max((abs(static_cast<int>((col - center.x))) - 1), 0);
                int y = std::max((abs(static_cast<int>((row - center.y))) - 1), 0);
                int distSquared = x * x + y * y;
                offsetsFromCenter[distSquared].insert(key - startingKey);
                if (distSquared > maxDistSquared)
                {
                    maxDistSquared = distSquared;
                }
            }
        }
    }

    m_offsetBetweenCells = new int[maxDistSquared + 1];
    int numCells = m_gridSize.x * m_gridSize.y;
    m_offset = new int[numCells];
    int j = 0;
    for (int i = 0; i <= maxDistSquared; i++)
    {
        if (offsetsFromCenter[i].empty())
        {
            m_offsetBetweenCells[i] = m_offsetBetweenCells[i - 1];
        }
        else
        {
            for (int offset : offsetsFromCenter[i])
            {
                m_offset[i] = offset;
                j++;
            }
            m_offsetBetweenCells[i] = j;
        }
    }
    // sets up the grids hash values.
}

void SpatialHash::setupCellsWithinGrid()
{
    // Sets up all the cells for the hash grid to be drawn.
    sf::RectangleShape shape(sf::Vector2f(m_CELL_SIZE, m_CELL_SIZE));
    for (int i = 0; i < m_gridSize.x; i++)
    {
        for (int j = 0; j < m_gridSize.y; j++)
        {
            shape.setOutlineColor(sf::Color::Black);
            shape.setFillColor(sf::Color::Transparent);
            shape.setOutlineThickness(2);
            int x = i * m_CELL_SIZE;
            int y = j * m_CELL_SIZE;
            shape.setPosition(x, y);
            m_nodes.push_back(shape);
        }
    }
    // Sets up all the cells for the hash grid to be drawn.
}

void SpatialHash::insert(std::shared_ptr<Object> t_object)
{
    // creates a new hashid for the hash table and also sets the objects hash id to this new hash id.
    int x = getPositionFromHashKey(t_object->getPosition()).x;
    int y = getPositionFromHashKey(t_object->getPosition()).y;
    int hashID = createHaskKey(sf::Vector2u(x, y));
    m_hashTable[hashID].insert(t_object);
    t_object->setHashKey(hashID);
    // creates a new hashid for the hash table and also sets the objects hash id to this new hash id.
}

int SpatialHash::createHaskKey(sf::Vector2u t_position)
{
    return t_position.x + t_position.y * m_gridSize.x;
}

sf::Vector2i SpatialHash::getPositionFromHashKey(sf::Vector2f t_position)
{
    float x = t_position.x / Window::getWindowWidth();
    float y = t_position.y / Window::getWindowWidth();
    int xIndex = floor(x * (m_gridSize.x - 1));
    int yIndex = floor(y * (m_gridSize.y - 1));
    return sf::Vector2i{ xIndex, yIndex };
}

std::vector<std::shared_ptr<Object>> SpatialHash::spaitalSearch(std::shared_ptr<Object> t_object, int t_searchArea)
{
    std::vector<std::shared_ptr<Object>> objectsWithinSearch;

    double distance = t_searchArea / static_cast<double>(m_CELL_SIZE);
    int cells = std::floor(distance * distance);

    int index = t_object->getHashKey();
    // gets all the objects are within the search area. 
    for (int i = 0; i < m_offsetBetweenCells[cells]; i++)
    {
        int offset = m_offset[i];
        int maxPossibleOffset = m_gridSize.x * m_gridSize.y - 1;
        int offsetWithinBounds = std::min<int>(std::max(0, index + offset), maxPossibleOffset);
        for (auto object : m_hashTable[offsetWithinBounds])
        {
            if (object == t_object) continue; // don't include itself.
            else
            {
                if (CollisionHandler::sqauaredDistance(object->getPosition(), object->getPosition()) <= t_searchArea * t_searchArea)
                {
                    objectsWithinSearch.push_back(object);
                }
            }
        }
    }
    // gets all the objects are within the search area. 
    return objectsWithinSearch;
}

void SpatialHash::render()
{
    for (auto& node : m_nodes)
    {
        Window::getWindow().draw(node);
    }
}

void SpatialHash::remove(std::shared_ptr<Object> t_object)
{
    // remove an object from the hash table using that objects hash key.
    m_hashTable[t_object->getHashKey()].erase(t_object);
}

void SpatialHash::update(std::shared_ptr<Object> t_object)
{
    // Updates all the hash keys for every object.
    int x = getPositionFromHashKey(t_object->getPosition()).x;
    int y = getPositionFromHashKey(t_object->getPosition()).y;
    int hashID = createHaskKey(sf::Vector2u(x, y));
    if (hashID == t_object->getHashKey())
    {
        return;
    }
    else
    {
        remove(t_object);
        insert(t_object);
    }
    // Updates all the hash keys for every object.
}
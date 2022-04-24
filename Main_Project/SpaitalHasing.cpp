#include "SpitialHasing.h"
#include <cstdlib>
#include <algorithm>
#include "SpitialHasing.h"

SpatialHash::SpatialHash()
{
    m_gridSize.x = ceil(Window::getWindowWidth() / m_CELL_SIZE);
    m_gridSize.y = ceil(Window::getWindowHeight() / m_CELL_SIZE);

    sf::Vector2u center = m_gridSize / 2u;
    int startingKey = createHaskKey(center);

    std::map<int, std::unordered_set<int>> offsetsFromCenter{};
    int maxDistSquared = 0;

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
    int i = 0;

    for (int d2 = 0; d2 <= maxDistSquared; d2++)
    {
        if (offsetsFromCenter[d2].empty())
        {
            m_offsetBetweenCells[d2] = m_offsetBetweenCells[d2 - 1];
        }
        else
        {
            for (int offset : offsetsFromCenter[d2])
            {
                m_offset[i] = offset;
                i++;
            }
            m_offsetBetweenCells[d2] = i;
        }
    }
    sf::RectangleShape shape(sf::Vector2f(50, 50));
    for (int i = 0; i < m_gridSize.x; i++)
    {
        for (int j = 0; j < m_gridSize.y; j++)
        {
            shape.setOutlineColor(sf::Color::Black);
            shape.setFillColor(sf::Color::Transparent);
            shape.setOutlineThickness(2);
            int x = i * 50;
            int y = j * 50;
            shape.setPosition(x, y);
            m_nodes.push_back(shape);
        }
    }
}

SpatialHash::~SpatialHash()
{
    delete[] m_offsetBetweenCells;
    delete[] m_offset;
}

void SpatialHash::addObject(std::shared_ptr<Object> t_object)
{
    int x = getKey(t_object->getPosition()).x;
    int y = getKey(t_object->getPosition()).y;
    int hashID = createHaskKey(sf::Vector2u(x, y));
    m_hashTable[hashID].insert(t_object);
    t_object->setHashKey(hashID);
}

int SpatialHash::createHaskKey(sf::Vector2u t_position)
{
    return t_position.x + t_position.y * m_gridSize.x;
}

sf::Vector2i SpatialHash::getKey(sf::Vector2f t_position)
{
    float x = t_position.x / Window::getWindowWidth();
    float y = t_position.y / Window::getWindowWidth();
    int xIndex = floor(x * (m_gridSize.x - 1));
    int yIndex = floor(y * (m_gridSize.y - 1));
    return { xIndex, yIndex };
}

void SpatialHash::clear()
{
    m_hashTable.clear();
}

std::vector<std::shared_ptr<Object>> SpatialHash::spaitalSearch(std::shared_ptr<Object> t_object, int t_searchArea)
{
    std::vector<std::shared_ptr<Object>> objectsWithinSearch;

    double distance = t_searchArea / static_cast<double>(m_CELL_SIZE);
    int cells = std::floor(distance * distance);

    int index = t_object->getHashKey();

    for (int i = 0; i < m_offsetBetweenCells[cells]; i++)
    {
        int offset = m_offset[i];
        int maxPossibleOffset = m_gridSize.x * m_gridSize.y - 1;
        int offsetWithinBounds = std::min<int>(std::max(0, index + offset), maxPossibleOffset);
        for (auto object : m_hashTable[offsetWithinBounds])
        {
            if (object == t_object) continue;
            if (CollisionHandler::sqauaredDistance(object->getPosition(), object->getPosition()) <= t_searchArea * t_searchArea)
            {
                objectsWithinSearch.push_back(object);
            }
        }
    }
    return objectsWithinSearch;
}

void SpatialHash::render()
{
    for (auto& node : m_nodes)
    {
        Window::getWindow().draw(node);
    }
}

void SpatialHash::removeObject(std::shared_ptr<Object> t_object)
{
    m_hashTable[t_object->getHashKey()].erase(t_object);
}

void SpatialHash::updateObject(std::shared_ptr<Object> t_object)
{
    int x = getKey(t_object->getPosition()).x;
    int y = getKey(t_object->getPosition()).y;
    int hashID = createHaskKey(sf::Vector2u(x, y));
    if (hashID == t_object->getHashKey())
    {
        return;
    }
    else
    {
        removeObject(t_object);
        addObject(t_object);
    }
}

SpatialHash::SpatialHash(SpatialHash&& other) noexcept
{
    m_gridSize = other.m_gridSize;
    m_hashTable = other.m_hashTable;
    m_offsetBetweenCells = other.m_offsetBetweenCells;
    m_offset = other.m_offset;
    other.m_offsetBetweenCells = nullptr;
    other.m_offset = nullptr;
}

SpatialHash& SpatialHash::operator=(SpatialHash&& other) noexcept
{
    if (this != &other)
    {
        delete[] m_offsetBetweenCells;
        delete[] m_offset;
        m_gridSize = other.m_gridSize;
        m_hashTable = other.m_hashTable;
        m_offsetBetweenCells = other.m_offsetBetweenCells;
        m_offset = other.m_offset;
        other.m_offsetBetweenCells = nullptr;
        other.m_offset = nullptr;
    }
    return *this;
}
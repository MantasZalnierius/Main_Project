#include "QuadTree.h"

QuadTree::QuadTree(sf::FloatRect t_shapeBounds, int t_maxDepth, int t_maxObjects) :
    m_shapeBounds{ t_shapeBounds },
    m_maxDepth{ t_maxDepth },
    m_maxObjects{ t_maxObjects },
    m_head{ m_shapeBounds, 0U, m_maxDepth, m_maxObjects }
{
}

void QuadTree::insert(std::vector<std::shared_ptr<Object>> t_objects)
{
    // inserts all objects into the head node.
    for (auto& object : t_objects)
    {
        m_head.insert(static_cast<std::weak_ptr<Object>>(object));
    }
}

void QuadTree::clear()
{
    // clears all nodes within the head node.
    m_head.clear();
}

void QuadTree::render()
{
    m_head.render();
}
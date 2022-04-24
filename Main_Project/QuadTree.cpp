#include "QuadTree.h"

QuadTree::QuadTree(sf::FloatRect t_shapeBounds, size_t t_maxLevel, size_t t_maxObjects) :
    m_shapeBounds{ t_shapeBounds },
    m_maxLevel{ t_maxLevel },
    m_maxObjects{ t_maxObjects },
    m_head{ m_shapeBounds, 0U, m_maxLevel, m_maxObjects }
{

}

size_t QuadTree::countObjects() const
{
    return std::move(m_head.countObjects());
}

void QuadTree::insert(std::vector<std::shared_ptr<Object>> const& t_objects)
{
    for (auto const& object : t_objects)
    {
        m_head.insert(static_cast<std::weak_ptr<Object>>(object));
    }
}

void QuadTree::clear()
{
    m_head.clear();
}

void QuadTree::render(sf::RenderTarget& t_target) const
{
    m_head.render(t_target);
}

std::vector<std::weak_ptr<Object>> QuadTree::getColliders(sf::FloatRect const& t_shapeBounds) const
{
    return m_head.getColliders(t_shapeBounds);
}
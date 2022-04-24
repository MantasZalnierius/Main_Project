#include "Node.h"

Node::Node(sf::FloatRect t_shapeBounds, size_t t_level, size_t const& t_maxLevel, size_t const& t_maxObjects) :
    m_shapeBounds{ t_shapeBounds },
    m_depth{ t_level },
    m_maxDepth{ t_maxLevel },
    m_maxObjects{ t_maxObjects }
{}

size_t Node::countObjects() const
{
    if (!m_canSubdivide)
    {
        return std::count_if(m_objects.begin(), m_objects.end(), [](auto& obj)->bool { return !obj.expired(); });
    }
    size_t objCount = std::count_if(m_objects.begin(), m_objects.end(), [](auto& obj)->bool { return !obj.expired(); });
    for (auto& child : m_childNodes)
    {
        objCount += child->countObjects();
    }
    return objCount;
}

void Node::subdivide()
{
    if (m_canSubdivide)
    {
        return;
    }
    else
    {
        float midWidth{ m_shapeBounds.width / 2.0f }, midHeight{ m_shapeBounds.height / 2.0f };
        float left{ m_shapeBounds.left }, top{ m_shapeBounds.top };
        m_childNodes[0] = std::make_unique<Node>(sf::FloatRect{ left, top, midWidth, midHeight }, m_depth + 1, m_maxDepth, m_maxObjects);
        m_childNodes[1] = std::make_unique<Node>(sf::FloatRect{ left + midWidth, top,midWidth, midHeight }, m_depth + 1, m_maxDepth, m_maxObjects);
        m_childNodes[2] = std::make_unique<Node>(sf::FloatRect{ left, top + midHeight, midWidth, midHeight }, m_depth + 1, m_maxDepth, m_maxObjects);
        m_childNodes[3] = std::make_unique<Node>(sf::FloatRect{ left + midWidth, top + midHeight, midWidth, midHeight }, m_depth + 1, m_maxDepth, m_maxObjects);
        for (auto& child : m_childNodes)
        {
            child->m_head = this;
        }
        m_canSubdivide = true;
    }
}

void Node::insert(std::weak_ptr<Object> ptr)
{
    if (m_canSubdivide)
    {
        size_t index = getIndex(ptr.lock()->getGlobalBounds());
        if (contains(m_childNodes[index]->m_shapeBounds, ptr.lock()->getGlobalBounds()))
        {
            m_childNodes[index]->insert(ptr);
            return;
        }
        m_colliders.push_back(ptr);
        return;
    }
    m_objects.push_back(ptr);
    if (m_depth < m_maxDepth && m_objects.size() > m_maxObjects)
    {
        subdivide();
        for (auto& object : m_objects)
        {
            insert(object);
        }
        m_objects.clear();
    }
}

void Node::render(sf::RenderTarget& t_target) const
{
    sf::RectangleShape shape(sf::Vector2f(m_shapeBounds.width, m_shapeBounds.height));
    shape.setPosition(m_shapeBounds.left, m_shapeBounds.top);
    shape.setOutlineThickness(2.0f);
    shape.setOutlineColor(sf::Color::Black);
    shape.setFillColor(sf::Color::Transparent);
    t_target.draw(shape);
    if (m_canSubdivide)
    {
        for (auto& child : m_childNodes)
        {
            child->render(t_target);
        }
    }
}

void Node::clear()
{
    m_colliders.clear();
    m_objects.clear();
    if (!m_canSubdivide)
    {
        return;
    }
    else
    {
        for (auto& child : m_childNodes)
        {
            child->clear();
            child.reset(nullptr);
        }
        m_canSubdivide = false;
    }
}

size_t Node::getIndex(sf::FloatRect t_objectBounds) const
{
    float midWidth = m_shapeBounds.left + (m_shapeBounds.width / 2.0f);
    float midHeight = m_shapeBounds.top + (m_shapeBounds.height / 2.0f);
    bool left = t_objectBounds.left <= midWidth;
    bool top = t_objectBounds.top <= midHeight;
    size_t index = North;
    if (left)
    {
        if (!top)
        {
            index = WEST;
        }
    }
    else
    {
        if (top)
        {
            index = EAST;
        }
        else
        {
            index = SOUTH;
        }
    }
    return index;
}

std::vector<std::weak_ptr<Object>> Node::getColliders(sf::FloatRect const& t_objectBounds) const
{
    if (!contains(m_shapeBounds, t_objectBounds))
    {
        return {};
    }
    else
    {
        std::vector<std::weak_ptr<Object>> ret;
        if (m_canSubdivide)
        {
            size_t possibleIndex = getIndex(t_objectBounds);
            if (contains(m_childNodes[possibleIndex]->m_shapeBounds, t_objectBounds))
            {
                auto temp = m_childNodes[possibleIndex]->getColliders(t_objectBounds);
                ret.insert(ret.end(), std::make_move_iterator(temp.begin()), std::make_move_iterator(temp.end()));
            }
            else
            {
                if (t_objectBounds.left <= m_childNodes[EAST]->m_shapeBounds.left)
                {
                    if (t_objectBounds.top < m_childNodes[WEST]->m_shapeBounds.top)
                    {
                        auto temp = m_childNodes[North]->getObjects();
                        ret.insert(ret.end(), std::make_move_iterator(temp.begin()), std::make_move_iterator(temp.end()));
                    }
                    if (t_objectBounds.top + t_objectBounds.height > m_childNodes[WEST]->m_shapeBounds.top)
                    {
                        auto temp = m_childNodes[WEST]->getObjects();
                        ret.insert(ret.end(), std::make_move_iterator(temp.begin()), std::make_move_iterator(temp.end()));
                    }
                }

                if (t_objectBounds.left + t_objectBounds.width > m_childNodes[EAST]->m_shapeBounds.left)
                {
                    if (t_objectBounds.top <= m_childNodes[SOUTH]->m_shapeBounds.top)
                    {
                        auto temp = m_childNodes[EAST]->getObjects();
                        ret.insert(ret.end(), std::make_move_iterator(temp.begin()), std::make_move_iterator(temp.end()));
                    }
                    if (t_objectBounds.top + t_objectBounds.height > m_childNodes[SOUTH]->m_shapeBounds.top)
                    {
                        auto tmp{ m_childNodes[SOUTH]->getObjects() };
                        ret.insert(ret.end(), std::make_move_iterator(tmp.begin()), std::make_move_iterator(tmp.end()));
                    }
                }
            }
        }
        ret.insert(ret.end(), m_colliders.begin(), m_colliders.end());
        ret.insert(ret.end(), m_objects.begin(), m_objects.end());
        return ret;
    }
}

std::vector<std::weak_ptr<Object>> Node::getObjects() const
{
    std::vector<std::weak_ptr<Object>> ret;
    if (m_canSubdivide)
    {
        for (auto const& child : m_childNodes)
        {
            auto temp = child->getObjects();
            ret.insert(ret.end(), temp.begin(), temp.end());
        }
    }
    ret.insert(ret.end(), m_objects.begin(), m_objects.end());
    ret.insert(ret.end(), m_colliders.begin(), m_colliders.end());
    return ret;
}

bool Node::contains(sf::FloatRect const& t_firstObject, sf::FloatRect const& t_secondObject)
{
    return (t_firstObject.left <= t_secondObject.left && t_firstObject.top <= t_secondObject.top
        && t_firstObject.left + t_firstObject.width >= t_secondObject.left + t_secondObject.width
        && t_firstObject.top + t_firstObject.height >= t_secondObject.top + t_secondObject.height);
}
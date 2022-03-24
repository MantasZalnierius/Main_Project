#include "Node.h"

Node::Node(sf::FloatRect bounds, size_t level,
    size_t const& maxLevel, size_t const& maxObjects)
    : m_border{ bounds }, m_depth{ level }, m_maxDepth{ maxLevel }, m_maxObjects{ maxObjects }
{}

size_t Node::countObjects() const
{
    if (!m_canSubdivide)
        return std::count_if(m_objects.begin(), m_objects.end(),
            [](auto& obj)->bool { return !obj.expired(); });


    size_t objCount = std::count_if(m_objects.begin(), m_objects.end(),
        [](auto& obj)->bool { return !obj.expired(); });

    for (auto const& child : m_childNodes)
        objCount += child->countObjects();

    return objCount;
}

void Node::subdivide()
{
    if (m_canSubdivide)
        return;

    float midWidth{ m_border.width / 2.f }, midHeight{ m_border.height / 2.f };
    float left{ m_border.left }, top{ m_border.top };

    m_childNodes[0] = std::make_unique<Node>(sf::FloatRect{ left, top, midWidth, midHeight }, m_depth+ 1, m_maxDepth, m_maxObjects);
    m_childNodes[1] = std::make_unique<Node>(sf::FloatRect{ left + midWidth, top,midWidth, midHeight }, m_depth + 1, m_maxDepth, m_maxObjects);
    m_childNodes[2] = std::make_unique<Node>(sf::FloatRect{ left, top + midHeight, midWidth, midHeight }, m_depth + 1, m_maxDepth, m_maxObjects);
    m_childNodes[3] = std::make_unique<Node>(sf::FloatRect{ left + midWidth, top + midHeight, midWidth, midHeight }, m_depth + 1, m_maxDepth, m_maxObjects);

    for (auto& child : m_childNodes)
        child->m_head = this;

    m_canSubdivide = true;
}

void Node::insert(std::weak_ptr<Object> ptr)
{
    if (m_canSubdivide) {
        // Try to insert it in a subnode
        auto i{ getIndex(ptr.lock()->getGlobalBounds()) };

        if (contains(m_childNodes[i]->m_border, ptr.lock()->getGlobalBounds())) {
            m_childNodes[i]->insert(std::move(ptr));
            return;
        }

        // If it reaches this point, then it's between subnodes
        m_collidedObjects.push_back(std::move(ptr));
        return;
    }

    m_objects.push_back(std::move(ptr));

    if (m_depth < m_maxDepth && m_objects.size() > m_maxObjects)
    {
        subdivide();

        for (auto& obj : m_objects)
            insert(std::move(obj));

        m_objects.clear();
    }
}

void Node::clear()
{
    m_collidedObjects.clear();
    m_objects.clear();

    if (!m_canSubdivide)
        return;

    for (auto& child : m_childNodes) {
        child->clear();
        child.reset(nullptr);
    }

    m_canSubdivide = false;
}

void Node::drawBorders(sf::RenderTarget& t) const
{
    sf::RectangleShape border{ sf::Vector2f{m_border.width, m_border.height} };
    border.setPosition(m_border.left, m_border.top);
    border.setOutlineThickness(2.0f);
    border.setOutlineColor(sf::Color::White);
    border.setFillColor(sf::Color::Transparent);
    t.draw(border);

    if (m_canSubdivide)
        for (auto& child : m_childNodes)
            child->drawBorders(t);
}

size_t Node::getIndex(sf::FloatRect objBounds) const
{
    float midWidth{ m_border.left + (m_border.width / 2.f) },
        midHeight{ m_border.top + (m_border.height / 2.f) };

    bool left{ objBounds.left <= midWidth },
        top{ objBounds.top <= midHeight };


    size_t index = NW;
    if (left) {
        if (!top)
            index = SW;
    }
    else {
        if (top) {
            index = NE;
        }
        else {
            index = SE;
        }
    }

    return index;
}

std::vector<std::weak_ptr<Object>> Node::getCollisionable(sf::FloatRect const& bounds) const
{
    if (!contains(m_border, bounds))
        return {};

    std::vector<std::weak_ptr<Object>> ret;
    if (m_canSubdivide) {
        size_t possibleIndex{ getIndex(bounds) };
        if (contains(m_childNodes[possibleIndex]->m_border, bounds)) {
            auto tmp{ m_childNodes[possibleIndex]->getCollisionable(bounds) };
            ret.insert(ret.end(), std::make_move_iterator(tmp.begin()), std::make_move_iterator(tmp.end()));
        }
        else {
            if (bounds.left <= m_childNodes[NE]->m_border.left) {
                if (bounds.top < m_childNodes[SW]->m_border.top) {
                    auto tmp{ m_childNodes[NW]->getAllObjects() };
                    ret.insert(ret.end(), std::make_move_iterator(tmp.begin()), std::make_move_iterator(tmp.end()));
                }

                if (bounds.top + bounds.height > m_childNodes[SW]->m_border.top) {
                    auto tmp{ m_childNodes[SW]->getAllObjects() };
                    ret.insert(ret.end(), std::make_move_iterator(tmp.begin()), std::make_move_iterator(tmp.end()));
                }
            }

            if (bounds.left + bounds.width > m_childNodes[NE]->m_border.left) {
                if (bounds.top <= m_childNodes[SE]->m_border.top) {
                    auto tmp{ m_childNodes[NE]->getAllObjects() };
                    ret.insert(ret.end(), std::make_move_iterator(tmp.begin()), std::make_move_iterator(tmp.end()));
                }

                if (bounds.top + bounds.height > m_childNodes[SE]->m_border.top) {
                    auto tmp{ m_childNodes[SE]->getAllObjects() };
                    ret.insert(ret.end(), std::make_move_iterator(tmp.begin()), std::make_move_iterator(tmp.end()));
                }
            }
        }
    }

    ret.insert(ret.end(), m_collidedObjects.begin(), m_collidedObjects.end());
    ret.insert(ret.end(), m_objects.begin(), m_objects.end());

    return std::move(ret);
}

std::vector<std::weak_ptr<Object>> Node::getAllObjects() const
{
    std::vector<std::weak_ptr<Object>> ret;

    if (m_canSubdivide)
        for (auto const& child : m_childNodes) {
            auto tmp{ child->getAllObjects() };
            ret.insert(ret.end(), tmp.begin(), tmp.end());
        }

    ret.insert(ret.end(), m_objects.begin(), m_objects.end());
    ret.insert(ret.end(), m_collidedObjects.begin(), m_collidedObjects.end());

    return ret;
}

bool Node::contains(sf::FloatRect const& first, sf::FloatRect const& second)
{
    return (first.left <= second.left && first.top <= second.top
        && first.left + first.width >= second.left + second.width
        && first.top + first.height >= second.top + second.height);
}
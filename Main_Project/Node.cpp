#include "Node.h"

Node::Node(sf::FloatRect t_shapeBounds, int t_depth, int t_maxDepth, int t_maxObjects) :
    m_shapeBounds{ t_shapeBounds },
    m_depth{ t_depth },
    m_maxDepth{ t_maxDepth },
    m_maxObjects{ t_maxObjects }
{
    // set up the shape for this node.
    m_shape.setOutlineThickness(2.0f);
    m_shape.setOutlineColor(sf::Color::Black);
    m_shape.setFillColor(sf::Color::Transparent);
}

int Node::getObjectCount()
{
    // gets the object count of this node and of all of it's children.
    if (!m_isSubdived)
    {
        return std::count_if(m_objects.begin(), m_objects.end(), [](auto& obj)->bool { return !obj.expired(); });
    }
    else
    {
        int numberOfOjects = std::count_if(m_objects.begin(), m_objects.end(), [](auto& obj)->bool { return !obj.expired(); });
        for (auto& child : m_childNodes)
        {
            numberOfOjects += child->getObjectCount();
        }
        return numberOfOjects;
    }
    // gets the object count of this node and of all of it's children nodes.
}

void Node::subdivide()
{
    if (m_isSubdived) // if it's already subdivded, theres no need to subdivde further.
    {
        return;
    }
    else
    {
        // else creates for nodes of this node and sets all of the child nodes head node to this node.
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
        m_isSubdived = true;
        // else creates for nodes of this node and sets all of the child nodes head node to this node.
    }
}

void Node::insert(std::weak_ptr<Object> t_object)
{
    // Inserts an object into the node.
    if (m_isSubdived)
    {
        int objectIndex = getObjectIndex(t_object.lock()->getGlobalBounds());
        if (CollisionHandler::aabbCollision(m_childNodes[objectIndex]->m_shapeBounds, t_object.lock()->getGlobalBounds()))
        {
            m_childNodes[objectIndex]->insert(t_object);
            return;
        }
        else
        {
            m_colliders.push_back(t_object);
            return;
        }
    }
    else
    {
        m_objects.push_back(t_object);
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
    // Inserts an object into the node.
}

void Node::render()
{
    // renders all bounds of this node and all of it's children nodes.
    m_shape.setSize(sf::Vector2f(m_shapeBounds.width, m_shapeBounds.height));
    m_shape.setPosition(m_shapeBounds.left, m_shapeBounds.top);
    Window::getWindow().draw(m_shape);
    if (m_isSubdived)
    {
        for (auto& child : m_childNodes)
        {
            child->render();
        }
    }
    // renders all bounds of this node and all of it's children nodes.
}

void Node::clear()
{
    // clears everything from this node.
    m_colliders.clear();
    m_objects.clear();
    if (!m_isSubdived)
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
        m_isSubdived = false;
    }
    // clears everything from this node.
}

int Node::getObjectIndex(sf::FloatRect t_objectBounds)
{
    // Gets the object index in respect to where it is in the node.
    float midWidth = m_shapeBounds.left + (m_shapeBounds.width / 2.0f);
    float midHeight = m_shapeBounds.top + (m_shapeBounds.height / 2.0f);
    bool left = t_objectBounds.left <= midWidth;
    bool top = t_objectBounds.top <= midHeight;
    int index = North;
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
    // Gets the object index in respect to where it is in the node.
}

std::vector<std::weak_ptr<Object>> Node::getColliders(sf::FloatRect t_objectBounds)
{
    // Gets all the collidable objects within a node.
    if (!CollisionHandler::aabbCollision(m_shapeBounds, t_objectBounds)) // if an object is not colliding with a node's bounds
        // don't do anything.
    {
        return {};
    }
    else
    {
        // else it is colliding and get all the objects close to the colliding object.
        std::vector<std::weak_ptr<Object>> objects;
        if (m_isSubdived)
        {
            int possibleIndex = getObjectIndex(t_objectBounds);
            if (CollisionHandler::aabbCollision(m_childNodes[possibleIndex]->m_shapeBounds, t_objectBounds))
            {
                auto temp = m_childNodes[possibleIndex]->getColliders(t_objectBounds);
                objects.insert(objects.end(), std::make_move_iterator(temp.begin()), std::make_move_iterator(temp.end()));
            }
            else
            {
                if (t_objectBounds.left <= m_childNodes[EAST]->m_shapeBounds.left)
                {
                    if (t_objectBounds.top < m_childNodes[WEST]->m_shapeBounds.top)
                    {
                        auto temp = m_childNodes[North]->getObjects();
                        objects.insert(objects.end(), std::make_move_iterator(temp.begin()), std::make_move_iterator(temp.end()));
                    }
                    if (t_objectBounds.top + t_objectBounds.height > m_childNodes[WEST]->m_shapeBounds.top)
                    {
                        auto temp = m_childNodes[WEST]->getObjects();
                        objects.insert(objects.end(), std::make_move_iterator(temp.begin()), std::make_move_iterator(temp.end()));
                    }
                }

                if (t_objectBounds.left + t_objectBounds.width > m_childNodes[EAST]->m_shapeBounds.left)
                {
                    if (t_objectBounds.top <= m_childNodes[SOUTH]->m_shapeBounds.top)
                    {
                        auto temp = m_childNodes[EAST]->getObjects();
                        objects.insert(objects.end(), std::make_move_iterator(temp.begin()), std::make_move_iterator(temp.end()));
                    }
                    if (t_objectBounds.top + t_objectBounds.height > m_childNodes[SOUTH]->m_shapeBounds.top)
                    {
                        auto tmp{ m_childNodes[SOUTH]->getObjects() };
                        objects.insert(objects.end(), std::make_move_iterator(tmp.begin()), std::make_move_iterator(tmp.end()));
                    }
                }
            }
        }
        // else it is colliding and get all the objects close to the colliding object.
        objects.insert(objects.end(), m_colliders.begin(), m_colliders.end());
        objects.insert(objects.end(), m_objects.begin(), m_objects.end());
        return objects;
    }
    // Gets all the collidable objects within a node.
}

std::vector<std::weak_ptr<Object>> Node::getObjects()
{
    // gets all of the objects of the within this node and it's children. 
    std::vector<std::weak_ptr<Object>> objects;
    if (m_isSubdived)
    {
        for (auto const& child : m_childNodes)
        {
            auto temp = child->getObjects();
            objects.insert(objects.end(), temp.begin(), temp.end());
        }
    }
    // gets all of the objects of the within this node and it's children. 
    objects.insert(objects.end(), m_objects.begin(), m_objects.end());
    objects.insert(objects.end(), m_colliders.begin(), m_colliders.end());
    return objects;
}
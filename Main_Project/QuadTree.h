#pragma once
#include "GlobalHeaders.h"
#include "Node.h"

class QuadTree
{
public:
    QuadTree() = default;

    /// <summary>
    /// sets up the quad-tree using head bounds, the max depth any node can reach and the max objects any node can contain.
    /// </summary>
    /// <param name="t_shapeBounds">the bounds of the head node.</param>
    /// <param name="t_maxDepth">the max depth the node can reach.</param>
    /// <param name="t_maxObjects">The max number of objects the head node can contain.</param>
    QuadTree(sf::FloatRect t_shapeBounds, int t_maxDepth, int t_maxObjects);

    /// <summary>
    /// Inserts a vector of objects into the head node. 
    /// </summary>
    /// <param name="t_objects">The vector of objects to inserted.</param>
    void insert(std::vector<std::shared_ptr<Object>> t_objects);

    /// <summary>
    /// Clears everything from the Quad-Tree.
    /// </summary>
    void clear();

    /// <summary>
    /// Renders all the nodes within the quad-tree.
    /// </summary>
    void render();

    /// <summary>
    /// Gets all the colliders that need to have collisions checked.
    /// </summary>
    /// <param name="t_shapeBounds">the shape bounds to be checked.</param>
    /// <returns>returns a vector of collidable objects.</returns>
    inline std::vector<std::weak_ptr<Object>> getColliders(sf::FloatRect t_shapeBounds) { return m_head.getColliders(t_shapeBounds); }

    /// <summary>
    /// Gets the object contained within every single node.
    /// </summary>
    /// <returns></returns>
    inline int getObjectCount() { return m_head.getObjectCount(); }
private:
    sf::FloatRect m_shapeBounds; // The shape of the head node.
    int const m_maxDepth; // max depth any node can reach.
    int const m_maxObjects; // max objects any node can cotain.
    Node m_head;
};
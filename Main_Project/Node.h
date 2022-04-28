#pragma once
#include "GlobalHeaders.h"
#include "Object.h"
#include "CollisionLib.h"

class QuadTree;

class Node
{
    friend class QuadTree;
public:
    Node() = default;
    /// <summary>
    /// Constructs a node with it's bounds, the depth it's on, the current max depth it can reach and the number
    /// of objects that node can contain.
    /// </summary>
    /// <param name="t_shapeBounds">the bounds of the node.</param>
    /// <param name="t_depth">the depth the node is on.</param>
    /// <param name="t_maxDepth">the max depth the node it can reach.</param>
    /// <param name="t_maxObjects">The objects the node can contain.</param>
    Node(sf::FloatRect t_shapeBounds, int t_depth, int t_maxDepth, int t_maxObjects);

    /// <summary>
    /// returns the number of objects this node contains.
    /// </summary>
    /// <returns>the number objects this node contains.</returns>
    int getObjectCount();

    /// <summary>
    /// Renders all the nodes and child nodes of a node.
    /// </summary>
    void render();
private:
    /// <summary>
    /// Inserts an object into a node.
    /// </summary>
    /// <param name="object">an Object to be inserted.</param>
    void insert(std::weak_ptr<Object> object);

    /// <summary>
    /// This functions checks if the node is already subdivded or can subdivde.
    /// and if there can be a subdivison, children of this node will be created.
    /// </summary>
    void subdivide();

    /// <summary>
    /// Clears everything from this node.
    /// </summary>
    void clear();

    /// <summary>
    /// Gets the object index within the node.
    /// </summary>
    /// <param name="t_objectBounds">the object bounds.</param>
    /// <returns>the index of an object within a node.</returns>
    int getObjectIndex(sf::FloatRect t_objectBounds);

    /// <summary>
    /// Gets all colliders from a node.
    /// </summary>
    /// <param name="t_objectBounds">the object bounds.</param>
    /// <returns>returns a vector of collidable objects.</returns>
    std::vector<std::weak_ptr<Object>> getColliders(sf::FloatRect t_objectBounds);

    /// <summary>
    /// gets all the objects within a node.
    /// </summary>
    /// <returns>a vector of objects.</returns>
    std::vector<std::weak_ptr<Object>> getObjects();

    bool m_isSubdived = false; // if this node is subdivded or not.
    sf::FloatRect m_shapeBounds; // the bounds of the node.
    int m_depth; // the current depth of the node.
    int m_maxDepth; // the max depth of the node.
    int m_maxObjects; // max objects a node can contain.
    Node* m_head{ nullptr }; // The head node.
    sf::RectangleShape m_shape; // shape of node.
    std::vector<std::weak_ptr<Object>> m_objects; // The objects this node contains.
    std::vector<std::weak_ptr<Object>> m_colliders; // the collidable objects this node contains.
    std::array<std::unique_ptr<Node>, 4> m_childNodes = { nullptr, nullptr, nullptr, nullptr }; // child nodes of this node.
};
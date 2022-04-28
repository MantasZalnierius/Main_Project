#pragma once
#include <memory>
#include <iostream>
#include <chrono>
#include <vector>
#include <array>
#include <random>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "Window.h"

enum Cardinal
{
    North = 0,
    EAST = 1,
    WEST = 2,
    SOUTH = 3
};

static const unsigned int m_CELL_SIZE = 50; // a cell size for each cell in the spatial grid.
static const unsigned int m_OBJECT_SIZE = 20; // The rectangle size.
static sf::Color m_RED = sf::Color::Red; // Color if not colliding.
static sf::Color m_GREEN = sf::Color::Green; // Color if colliding.
#include "Window.h"

sf::RenderWindow Window::m_window{ sf::VideoMode{ Window::getWindowWidth(), Window::getWindowHeight(), 32U }, "MAIN PROJECT" }; // Static assingement of the window.
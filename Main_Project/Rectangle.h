#pragma once
#include "Shape.h"
#include "ADSREnvelope.h"
class Rectangle : public Shape
{
public:
	Rectangle() = default;
	Rectangle(float t_posX, float t_posY, float t_width, float t_height);
	virtual ~Rectangle() {};
	inline sf::RectangleShape getBody() { return m_body; }
	void updatePosition(sf::Time t_deltaTime);
	void update(sf::Time t_deltaTime) override;
	void render(sf::RenderWindow& t_window) override;
	void processKeyUp(sf::Event t_event);
	void processKeyDown(sf::Event t_event);
private:
	sf::RectangleShape m_body;
	float m_height;
	float m_width;
	ADSREnevelope* m_adsrEnevelope;
};

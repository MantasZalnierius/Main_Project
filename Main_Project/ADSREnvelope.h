#pragma once
#include "SFML/Graphics.hpp"
#include <algorithm> 

class ADSREnevelope
{
public:
	ADSREnevelope();
	~ADSREnevelope() = default;
	void update(sf::Time t_deltaTime);

	inline void setisMovingRight(bool t_isMovingRight) { m_isMovingRight = t_isMovingRight; }
	inline void setisMovingLeft(bool t_isMovingLeft) { m_isMovingLeft = t_isMovingLeft; }
	inline void setisJumping(bool t_isJumping) { m_isJumping = t_isJumping; }
	inline void setIsJumpingHeld(bool t_isJumpingHeld) { m_isJumpHeld = t_isJumpingHeld; }
	inline void setisGrounded(bool t_isGrounded) { m_isGrounded = t_isGrounded; }

	inline bool getIsMovingRight() { return m_isMovingRight; }
	inline bool getIsMovingLeft() { return m_isMovingLeft; }
	inline bool getIsJumping() { return m_isJumping; }
	inline bool getIsGrounded() { return m_isGrounded; }

	inline float getVelX() { return m_velX; }
	inline float getVelY() { return m_velY; }

	inline void setVelX(float t_velX) { m_velX = t_velX; }
	inline void setVelY(float t_velY) { m_velY = t_velY; }
private:
	float m_gravity;
	float m_acclearation;
	float m_jumpTimeCounter;
	float m_timeLeft;
	float m_declaration;
	float m_elaspedTimeSinceButtonPress;
	float m_impluseJumpVel;
	float m_timeToReachMaxHeight;
	float m_movementTime;
	float m_maxMovementSpeed;
	float m_lowestMovementSpeed;
	bool m_isMovingRight;
	bool m_isMovingLeft;
	bool m_isJumping;
	bool m_isJumpHeld;
	bool m_isGrounded;
	float m_velX;
	float m_velY;

	void move(sf::Time t_deltaTime);
	void jump(sf::Time t_deltaTime);
	void moveLeft(sf::Time t_deltaTime);
	void moveRight(sf::Time t_deltaTime);
	void stopLeftAndRightMovement(sf::Time t_deltaTime);
	float getNewVelocityX(float t_time);
};

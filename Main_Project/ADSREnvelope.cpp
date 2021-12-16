#include "ADSREnvelope.h"

ADSREnevelope::ADSREnevelope()
{
	m_acclearation = 2.0f;
	m_declaration = 0.0f;
	m_elaspedTimeSinceButtonPress = 0.0f;
	m_gravity = -9.8f;
	m_impluseJumpVel = 5.0f;
	m_jumpTimeCounter = 0.0f;
	m_lowestMovementSpeed = 0.3f;
	m_maxMovementSpeed = 4.0f;
	m_movementTime = 0.100f;
	m_timeToReachMaxHeight = 0.5f;
	m_isGrounded = true;
	m_isMovingLeft = false;
	m_isMovingRight = false;
	m_isJumping = false;
	m_isJumpHeld = false;
	m_velX = 0.0f;
	m_velY = 0.0f;
}

void ADSREnevelope::update(sf::Time t_deltaTime)
{
	move(t_deltaTime);
	jump(t_deltaTime);
	if (!m_isJumpHeld && !m_isGrounded)
	{
		m_velY -= m_gravity * (1.55f - 1) * t_deltaTime.asSeconds() * 2;
	}
}

void ADSREnevelope::moveLeft(sf::Time t_deltaTime)
{
	if (m_elaspedTimeSinceButtonPress < m_timeLeft) // If the elaspedtime on when the button is pressed is less than 
	{
		m_velX = getNewVelocityX(m_elaspedTimeSinceButtonPress);
		m_velX = std::clamp(m_velX, -m_maxMovementSpeed, m_maxMovementSpeed);
	}
	else
	{
		m_velX = -m_maxMovementSpeed;
	}
}

void ADSREnevelope::moveRight(sf::Time t_deltaTime)
{
	if (m_elaspedTimeSinceButtonPress < m_timeLeft)
	{
		m_velX = getNewVelocityX(m_elaspedTimeSinceButtonPress);
		m_velX = std::clamp(m_velX, -m_maxMovementSpeed, m_maxMovementSpeed);
	}
	else
	{
		m_velX = m_maxMovementSpeed;
	}
}

void ADSREnevelope::stopLeftAndRightMovement(sf::Time t_deltaTime)
{
	if (m_velX != 0.0f)
	{
		m_declaration = m_acclearation / m_movementTime * t_deltaTime.asSeconds(); // Declaration = v/t.
		if (m_velX < 0.0f)
		{
			m_velX += m_declaration; // reduce vel by declaration.
			if (m_velX >= -m_lowestMovementSpeed)
			{
				m_velX = 0.0f;
			}
		}
		else if (m_velX > 0.0f)
		{
			m_velX -= m_declaration; // reduce vel by declaration.
			if (m_velX <= m_lowestMovementSpeed)
			{
				m_velX = 0.0f;
			}
		}
	}
}

void ADSREnevelope::move(sf::Time t_deltaTime)
{
	if (m_isMovingRight || m_isMovingLeft)
	{
		if (m_isMovingRight)
		{
			moveRight(t_deltaTime);
		}
		else if (m_isMovingLeft)
		{
			moveLeft(t_deltaTime);
		}
		m_elaspedTimeSinceButtonPress += t_deltaTime.asSeconds();
	}
	else
	{
		stopLeftAndRightMovement(t_deltaTime);
	}
}

void ADSREnevelope::jump(sf::Time t_deltaTime)
{
	if (m_isJumping && getIsGrounded())
	{
		m_velY = -m_impluseJumpVel;
		m_isGrounded = false;
	}
}

float ADSREnevelope::getNewVelocityX(float t_time)
{
	return m_acclearation * t_time;
}
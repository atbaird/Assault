#include "Game/Tank.hpp"

//Constructors and Destructor
Tank::Tank() : LivingEntity(ENTITY_PLAYER)
{
	//constructor
	m_color = Rgba(0.f, 1.f, 0.f, 1.f);
	m_position = Vector2(1.5f, 1.5f);
	m_collisionRadius = 0.5f;
	m_maxMovementSpeed = 2.5f;
	m_additiveSpeed = 0.f;
	m_angularDisplacement = 0.f;
	m_alliance = 1;
	tankTexture = Texture::CreateOrGetTexture("Data/Images/Tank.png");
	SpriteSheet tanktredsSpriteSheet = SpriteSheet("Data/Images/TankTreds animation.png", 8, 1);
	leftTred = SpriteAnim(tanktredsSpriteSheet, 1.f, SPRITE_ANIM_MODE_LOOPING, 0, 7);
	leftTred.Reverse();
	rightTred = SpriteAnim(tanktredsSpriteSheet, 1.f, SPRITE_ANIM_MODE_LOOPING, 0, 7);
	rightTred.Reverse();
	m_controller = nullptr;
	m_id = 0;
	max_health = 10;
	current_health = max_health;
	m_deltaSecondsForRightTreds = 0.f;
	m_deltaSecondsForLeftTreds = 0.f;
}
Tank::~Tank()
{
	//destructor
	m_controller = nullptr;
}

//Updates and Render
void Tank::PlayerUpdate(float deltaSeconds, bool W, bool A, bool D,bool S)
{
	//player update method, is special to the player controlled units.
	//basically allows for the tank to handle things based on player input.
	if (m_dead == false && m_dieing == false)
	{
		bool alreadyMoved = false;
		m_deltaSecondsForRightTreds = 0.f;
		m_deltaSecondsForLeftTreds = 0.f;
		if (m_id == 0)
		{
			alreadyMoved = keyboardControls(deltaSeconds, W, A, D, S);
			if (alreadyMoved == true)
			{
				m_deltaSecondsForRightTreds = deltaSeconds * 3.f;
				m_deltaSecondsForLeftTreds = deltaSeconds * 3.f;
			}
		}
		UpdateVelocityFromKeyboard();

		if (alreadyMoved == false && m_controller->amIPluggedIn() == 0)
		{
			xboxControls(deltaSeconds);
		}
		leftTred.Update(m_deltaSecondsForLeftTreds);
		rightTred.Update(m_deltaSecondsForRightTreds);
	}
	Update(deltaSeconds);
}
//controls

bool Tank::keyboardControls(float& deltaSeconds, bool& W, bool& A, bool& D, bool& S)
{
	//Checks for Keyboard Controls used
	bool alreadyMoved = false;
	if (W == true && S == false)
	{
		m_additiveSpeed = m_maxMovementSpeed;
		alreadyMoved = true;
		if (leftTred.IsReversed() == false)
		{
			leftTred.Reverse();
		}
		if (rightTred.IsReversed() == false)
		{
			rightTred.Reverse();
		}

	}
	else if (W == false && S == false)
	{
		m_velocity = Vector2();
		m_additiveSpeed = 0.f;
	}
	else if (W == false && S == true)
	{
		m_additiveSpeed = -m_maxMovementSpeed;
		alreadyMoved = true;
		if (leftTred.IsReversed() == true)
		{
			leftTred.Reverse();
		}
		if (rightTred.IsReversed() == true)
		{
			rightTred.Reverse();
		}
	}
	if (A == true && D == false)
	{
		//left turn
		m_angularDisplacement += (30.f * deltaSeconds);
		alreadyMoved = true;
		if (W == false && S == false &&leftTred.IsReversed() == true)
		{
			leftTred.Reverse();
		}
		if (W == false && S == false  && rightTred.IsReversed() == false)
		{
			rightTred.Reverse();
		}
	}
	else if (A == false && D == true)
	{
		//right turn
		m_angularDisplacement -= (30.f * deltaSeconds);
		alreadyMoved = true;
		if (W == false && S == false && leftTred.IsReversed() == false)
		{
			leftTred.Reverse();
		}
		if (W == false && S == false && rightTred.IsReversed() == true)
		{
			rightTred.Reverse();
		}
	}
	return alreadyMoved;
}

bool Tank::xboxControls(float deltaSeconds)
{
	//checks for xbox controller used
	bool xboxControlsUsed = false;
	Vector2 leftAnalogPosition = m_controller->getLeftAnalogPosition();
	Vector2 rightAnalogPosition = m_controller->getRightAnalogPosition();

	//tred animation control
	if (rightTred.IsReversed() == false && rightAnalogPosition.y > 0.f)
	{
		rightTred.Reverse();
	}
	else if (rightTred.IsReversed() == true && rightAnalogPosition.y < 0.f)
	{
		rightTred.Reverse();
	}
	if (leftTred.IsReversed() == false && leftAnalogPosition.y > 0.f)
	{
		leftTred.Reverse();
	}
	else if (leftTred.IsReversed() == true && leftAnalogPosition.y < 0.f)
	{
		leftTred.Reverse();
	}
	m_deltaSecondsForRightTreds = 4 * deltaSeconds;
	m_deltaSecondsForLeftTreds = 4 * deltaSeconds;
	if (abs(rightAnalogPosition.y) > 0.f)
	{
		m_deltaSecondsForRightTreds = m_deltaSecondsForRightTreds * abs(rightAnalogPosition.y);
		m_deltaSecondsForLeftTreds = m_deltaSecondsForLeftTreds * abs(rightAnalogPosition.y);
	}
	if (abs(leftAnalogPosition.y) > 0.f)
	{
		m_deltaSecondsForRightTreds = m_deltaSecondsForRightTreds * abs(leftAnalogPosition.y);
		m_deltaSecondsForLeftTreds = m_deltaSecondsForLeftTreds * abs(leftAnalogPosition.y);
	}
	if (abs(rightAnalogPosition.y) < 0.1f)
	{
		m_deltaSecondsForRightTreds = 0.f;
	}
	if (abs(leftAnalogPosition.y) < 0.1f)
	{
		m_deltaSecondsForLeftTreds = 0.f;
	}
	if (abs(leftAnalogPosition.x) < 1.f && abs(rightAnalogPosition.x) < 1.f
		&& ((leftAnalogPosition.y > 0.f) && (rightAnalogPosition.y > 0.f) || (leftAnalogPosition.y < 0.f) && (rightAnalogPosition.y < 0.f)))
	{
		//two tred moves (forward/backward)
		float negative = (leftAnalogPosition.y + rightAnalogPosition.y) / abs(leftAnalogPosition.y + rightAnalogPosition.y);
		m_velocity.x = m_maxMovementSpeed * CosDegrees(m_angularDisplacement) * negative;
		m_velocity.y = m_maxMovementSpeed * SinDegrees(m_angularDisplacement) * negative;
		if (leftAnalogPosition.y != 0.f)
		{
			m_velocity.x *= abs(leftAnalogPosition.y);
			m_velocity.y *= abs(leftAnalogPosition.y);
		}
		if (rightAnalogPosition.y != 0.f)
		{
			m_velocity.x *= abs(rightAnalogPosition.y);
			m_velocity.y *= abs(rightAnalogPosition.y);
		}
		xboxControlsUsed = true;
	}
	else if (abs(leftAnalogPosition.x) < 1.f && abs(rightAnalogPosition.x) < 1.f
		&& ((leftAnalogPosition.y > 0.f) && (rightAnalogPosition.y < 0.f) || (leftAnalogPosition.y < 0.f && rightAnalogPosition.y > 0.f)))
	{
		m_angularDisplacement += (rightAnalogPosition.y - leftAnalogPosition.y);
		xboxControlsUsed = true;
	}
	else if (((leftAnalogPosition.x < 0.f && rightAnalogPosition.x < 0.f) || (leftAnalogPosition.x > 0.f && rightAnalogPosition.x > 0.f))
		&& abs(leftAnalogPosition.y) < 0.9f && abs(rightAnalogPosition.y) < 0.9f)
	{
		//straifing
		float negative = -1 * ((leftAnalogPosition.x + rightAnalogPosition.x) / abs(leftAnalogPosition.x + rightAnalogPosition.x));
		m_velocity.x = (m_maxMovementSpeed / 5.f) * CosDegrees(m_angularDisplacement + 90) * negative;
		m_velocity.y = (m_maxMovementSpeed / 5.f) * SinDegrees(m_angularDisplacement + 90) * negative;
		if (leftAnalogPosition.x != 0.f)
		{
			m_velocity.x *= abs(leftAnalogPosition.x);
			m_velocity.y *= abs(leftAnalogPosition.x);
		}
		if (rightAnalogPosition.x != 0.f)
		{
			m_velocity.x *= abs(rightAnalogPosition.x);
			m_velocity.y *= abs(rightAnalogPosition.x);
		}
		xboxControlsUsed = true;
	}
	else if ((rightAnalogPosition.y < 0.1f && abs(leftAnalogPosition.y) > 0.f) 
		|| (abs(rightAnalogPosition.y) > 0.f && leftAnalogPosition.y < 0.1f))
	{
		//one tred moves (forward/backward)
		float negative = (leftAnalogPosition.y + rightAnalogPosition.y) / abs(leftAnalogPosition.y + rightAnalogPosition.y);
		m_velocity.x = (m_maxMovementSpeed / 2.f) * CosDegrees(m_angularDisplacement) * negative;
		m_velocity.y = (m_maxMovementSpeed / 2.f) * SinDegrees(m_angularDisplacement) * negative;
		if (leftAnalogPosition.y != 0.f)
		{
			m_velocity.x *= abs(leftAnalogPosition.y);
			m_velocity.y *= abs(leftAnalogPosition.y);
		}
		if (rightAnalogPosition.y != 0.f)
		{
			m_velocity.x *= abs(rightAnalogPosition.y);
			m_velocity.y *= abs(rightAnalogPosition.y);
		}
		m_angularDisplacement -= ((leftAnalogPosition.y - rightAnalogPosition.y) / 2.f);
		xboxControlsUsed = true;
	}
	
	
	
	return xboxControlsUsed;
}

//Updates and Render continued
void Tank::setPlayerID(int id)
{
	//assigns a player id
	m_id = id;
}
void Tank::assignController(XboxController* controller)
{
	//assigns an xbox controller to the tank
	m_controller = controller;
}
void Tank::Update(float deltaSeconds)
{
	//update tank method.
	LivingEntity::Update(deltaSeconds);
}
void Tank::UpdateVelocityFromKeyboard()
{
	//updates the x and y speeds based on whether or not those speeds are presently less than the maximum possible speed.
	m_velocity.x = CosDegrees(m_angularDisplacement) * (m_additiveSpeed);
	m_velocity.y = SinDegrees(m_angularDisplacement) * (m_additiveSpeed);
}
const void Tank::Render(Vector2& scale)
{
	//Renders the tank.

	g_Renderer->Push();
	g_Renderer->Translate(Vector2(m_position.x*scale.x, m_position.y * scale.y));
	
	g_Renderer->Rotate(m_angularDisplacement);

	AABB2 tankBounds = AABB2(Vector2(-m_collisionRadius * scale.x, -m_collisionRadius * scale.y), Vector2(m_collisionRadius * scale.x, m_collisionRadius * scale.y));

	if (invulnerable == false || (invulnerable == true && invuln_show_model == true))
	{
		g_Renderer->DrawTexturedAABB(tankBounds, tankTexture, Vector2(0.f, 0.f), Vector2(1.f, 1.f), m_color);
		leftTred.Render(AABB2(Vector2((m_collisionRadius / 2.f) * scale.x, (m_collisionRadius) * scale.y), 
			Vector2(-(m_collisionRadius *(2.19f / 3.f)) * scale.x, (m_collisionRadius* (0.9f / 2.f)) * scale.y)), 0.f, m_color);
		rightTred.Render(AABB2(Vector2(-(m_collisionRadius / 2.f) * scale.x, (m_collisionRadius) * scale.y), 
			Vector2(m_collisionRadius* (2.19f / 3.f) * scale.x, (m_collisionRadius * (0.9f / 2.f)) * scale.y)), 180.f, m_color);
	}
	//g_Renderer->RenderPolygon(Vector2(0,0), m_collisionRadius, 20, m_angularDisplacement, m_color);
	g_Renderer->Pop();

}
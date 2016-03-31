#include "AppClass.h"
void AppClass::ProcessKeyboard(void)
{
	bool bModifier = false;
	float fSpeed = 0.01f;

#pragma region ON PRESS/RELEASE DEFINITION
	static bool	bLastF1 = false, bLastF2 = false, bLastF3 = false, bLastF4 = false, bLastF5 = false,
				bLastF6 = false, bLastF7 = false, bLastF8 = false, bLastF9 = false, bLastF10 = false,
				bLastEscape = false;
#define ON_KEY_PRESS_RELEASE(key, pressed_action, released_action){  \
			bool pressed = sf::Keyboard::isKeyPressed(sf::Keyboard::key);			\
			if(pressed){											\
				if(!bLast##key) pressed_action;}/*Just pressed? */\
			else if(bLast##key) released_action;/*Just released?*/\
			bLast##key = pressed; } //remember the state
#pragma endregion

#pragma region Modifiers
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
		bModifier = true;
#pragma endregion

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
	{
		m_v3Orientation = vector3(0.0f);
		quat_mToWorld = quaternion(m_v3Orientation);
		rotX.x = 0;
		rotY.y = 0;
		rotZ.z = 0;
		
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::X))
	{
		if (!bModifier) m_v3Orientation.x += 1.0f;
		else m_v3Orientation.x -= 1.0f;
	
	 // When x is pressed. It creates a quat that rotates around the x axis
	 rotX = quaternion(glm::rotate(IDENTITY_M4, m_v3Orientation.x, REAXISX));

	 // Multiply x by the original quat and set it back to itself.
	quat_mToWorld = quat_mToWorld * rotX ;
	

	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Y))
	{
		if (!bModifier) m_v3Orientation.y += 1.0f;
		else m_v3Orientation.y -= 1.0f;

		// When y is pressed. It creates a quat that rotates around the y axis
		rotY = quaternion(glm::rotate(IDENTITY_M4, m_v3Orientation.y, REAXISY));
		
		// Multiply y by the original quat and set it back to itself.
		quat_mToWorld = quat_mToWorld * rotY;

	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
	{
		if (!bModifier) m_v3Orientation.z += 1.0f;
		else m_v3Orientation.z -= 1.0f;


		// When z is pressed. It creates a quat that rotates around the z axis
		rotZ = quaternion(glm::rotate(IDENTITY_M4, m_v3Orientation.z, REAXISZ));

		// Multiply z by the original quat and set it back to itself.
		quat_mToWorld = quat_mToWorld * rotZ ;
	
	}

#pragma region Camera Positioning
	if(bModifier)
		fSpeed *= 10.0f;
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		m_pCameraMngr->MoveForward(fSpeed);

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		m_pCameraMngr->MoveForward(-fSpeed);
	
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		m_pCameraMngr->MoveSideways(-fSpeed);

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		m_pCameraMngr->MoveSideways(fSpeed);
	m_pCameraMngr->CalculateView();
#pragma endregion

#pragma region Other Actions
	ON_KEY_PRESS_RELEASE(Escape, NULL, PostMessage(m_pWindow->GetHandler(), WM_QUIT, NULL, NULL))
#pragma endregion
}
void AppClass::ProcessMouse(void)
{
	m_bArcBall = false;
	m_bFPC = false;
	
	if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Middle))
		m_bArcBall = true;
	
	if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
		m_bFPC = true;
}

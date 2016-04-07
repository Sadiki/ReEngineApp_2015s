#include "CameraSingleton.h"


// Constructor
CameraSingleton::CameraSingleton()
{
	// Set the defaults
	v3_Position = glm::vec3(0.0f, 0.0f, 10.0f);
	v3_Target = glm::vec3(0.0f, 0.0f, -1.0f);
	v3_Orientation = glm::vec3(0.0f, 1.0f, 0.0f);
	m4_CameraView = glm::lookAt(
		v3_Position, // Position
		v3_Target, // What i'm looking at
		v3_Orientation); // rotation orientation - currently up
}

matrix4 CameraSingleton::GetView()
{
	// Return the camera's projection.
	return m4_CameraView;
}
matrix4 CameraSingleton::GetProjection(bool bOrtho)
{
	// If true
	if (bOrtho) {
		// Return Ortho.
		return glm::ortho(
			-20.0f,//left 
			20.0f,//right 
			-10.0f,//bottom
			10.0f,///top
			0.01f,//near, 
			1000.0f);//far);
		
	}
	else
	{
		// Return Perspective.
		return glm::perspective(
			45.0f, // Camera viewing angle
			1080.0f / 768.0f, // Camera viewing ratio
			0.01f,
			1000.0f);
	}
}

void CameraSingleton::SetPosition(vector3 v3Position)
{
	// Set position to argument.
	v3_Position = v3Position;
	m4_CameraView = glm::lookAt(
		v3_Position, // Position
		v3_Target, // What i'm looking at
		v3_Orientation); // rotation orientation - currently up
}
void CameraSingleton::SetTarget(vector3 v3Target)
{
	// Set Target to argument.
	v3_Target = v3Target;
	m4_CameraView = glm::lookAt(
		v3_Position, // Position
		v3_Target, // What i'm looking at
		v3_Orientation); // rotation orientation - currently up
}
void CameraSingleton::SetUp(vector3 v3Up)
{
	// Set Up to argument.
	v3_Orientation = v3Up;
	m4_CameraView = glm::lookAt(
		v3_Position, // Position
		v3_Target, // What i'm looking at
		v3_Orientation); // rotation orientation - currently up
}

// Function to move forward
void CameraSingleton::MoveForward(float fIncrement)
{
		// Set position to argument.
		v3_Position += vector3(0.0f, 0.0f, fIncrement);

		m4_CameraView = glm::lookAt(
			v3_Position, // Position
			v3_Position + v3_Target, // What i'm looking at
			v3_Orientation); // rotation orientation - currently up

}

// Function to move left or right
void CameraSingleton::MoveSideways(float fIncrement)
{
	// Set position to argument.
	v3_Position += vector3(fIncrement, 0.0f, 0.0f);
	m4_CameraView = glm::lookAt(
		v3_Position, // Position
		v3_Position + v3_Target, // What i'm looking at
		v3_Orientation); // rotation orientation - currently up
}

// Function to move up or down.
void CameraSingleton::MoveVertical(float fIncrement)
{
	// Set position to argument.
	v3_Position += vector3(0.0f, fIncrement, 0.0f);
	m4_CameraView = glm::lookAt(
		v3_Position, // Position
		v3_Position + v3_Target, // What i'm looking at
		v3_Orientation); // rotation orientation - currently up
}

void CameraSingleton::ChangePitch(float fIncrement)
{
	m_quatOrientation = m_quatOrientation * quaternion(vector3(fIncrement, 0.0f, 0.0f));
	//v3_Position += vector3(0.0f, fIncrement, 0.0f);
	//v3_Orientation += vector3(m_quatOrientation.x, m_quatOrientation.y, m_quatOrientation.z);
	//v3_Position += vector3(0.0f, fIncrement, fIncrement);
	//vector3 dir = glm::normalize(v3_Position - v3_Target);
	
	vector3 tempPos;
	tempPos.x = m_quatOrientation.x;
	tempPos.y = m_quatOrientation.y;
	tempPos.z = m_quatOrientation.z;
	//v3_Target += v3_Position * v3_Orientation;
	//v3_Target = glm::normalize(v3_Target);
	m4_CameraView = glm::lookAt(
		v3_Position, // Position
		v3_Target + tempPos, // What i'm looking at
		v3_Orientation); // rotation orientation - currently up
}
void CameraSingleton::ChangeRoll(float fIncrement)
{

}
void CameraSingleton::ChangeYaw(float fIncrement)
{

}

// Sets the values back to default
void CameraSingleton::ResetValues() {
	v3_Position = glm::vec3(0.0f, 0.0f, 10.0f);
	//v3_Front = glm::vec3(0.0f, 0.0f, 3.0f);
	v3_Target = glm::vec3(0.0f, 0.0f, -1.0f);
	v3_Orientation = glm::vec3(0.0f, 1.0f, 0.0f);
	m4_CameraView = glm::lookAt(
		v3_Position, // Position
		v3_Target, // What i'm looking at
		v3_Orientation); // rotation orientation - currently up
}
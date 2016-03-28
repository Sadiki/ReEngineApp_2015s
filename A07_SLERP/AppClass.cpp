#include "AppClass.h"
void AppClass::InitWindow(String a_sWindowName)
{
	super::InitWindow("SLERP - sxs6590"); // Window Name

	//Setting the color to black
	m_v4ClearColor = vector4(0.0f);
}

void AppClass::InitVariables(void)
{
	//Setting the position in which the camera is looking and its interest point
	m_pCameraMngr->SetPositionTargetAndView(vector3(12.12f, 28.52f, 11.34f), ZERO_V3, REAXISY);

	//Setting the color to black
	m_v4ClearColor = vector4(0.0f);

	//Loading the models
	m_pMeshMngr->LoadModel("Planets\\00_Sun.obj", "Sun");
	m_pMeshMngr->LoadModel("Planets\\03_Earth.obj", "Earth");
	m_pMeshMngr->LoadModel("Planets\\03A_Moon.obj", "Moon");

	//Setting the days duration
	m_fDay = 1.0f;
	
}

void AppClass::Update(void)
{

	// Create size for Sun, Earth, and Moon
	vector3 v3_sunSize(5.936, 5.936, 5.936);
	matrix4 m4_earthSize = glm::scale(0.524f, 0.524f, 0.524f);
	vector3 v3_moonSize(0.27, 0.27, 0.27);

	// Create distance from origin for Earth, and Moon
	matrix4 m4_earthDist = glm::translate(11.0f, 0.0f, 0.0f);
	vector3 v3_moonDist(2.0f, 0.0f, 0.0f);
	
	//Update the system's time
	m_pSystem->UpdateTime();

	//Update the mesh manager's time without updating for collision detection
	m_pMeshMngr->Update();

	//First person camera movement
	if (m_bFPC == true)
		CameraRotation();

	//Getting the time between calls
	double fCallTime = m_pSystem->LapClock();
	//Counting the cumulative time
	static double fRunTime = 0.0f;
	fRunTime += fCallTime;

	static double fRotationTime = 0.0f;
	fRotationTime += fCallTime;

	//Earth Orbit
	double fEarthHalfOrbTime = 182.5f * m_fDay; //Earths orbit around the sun lasts 365 days / half the time for 2 stops
	float fEarthHalfRevTime = 0.5f * m_fDay; // Move for Half a day
	float fMoonHalfOrbTime = 14.0f * m_fDay; //Moon's orbit is 28 earth days, so half the time for half a route

	float fPercent = MapValue(static_cast<float>(fRunTime), 0.0f, static_cast<float>(fEarthHalfOrbTime), 0.0f, 1.0f);

	if (fRunTime >= fEarthHalfOrbTime) {
		fEarthHalfOrbTime *= 2;
	}



	// Convert earth rotation to a quat
	//glm::quat earthQuat = glm::quat_cast(earthRot);

	// do slerp between the earth's roation and the last point
	double earthFinalQuat = glm::mix(glm::quat(vector3(0.0f, 0.0f, 0.0f)), glm::quat(vector3(0.0f, 720.0f, 0.0f)), fPercent).y;

	float testVar = glm::degrees(static_cast<float>(earthFinalQuat));
	//matrix4  earthMat4 = glm::mat4_cast(earthFinalQuat);
	matrix4 earthRot = glm::rotate(IDENTITY_M4, glm::degrees(static_cast<float>(earthFinalQuat)), vector3(0.0f, 1.0f, 0.0f));

	m4_Earth = earthRot * m4_earthSize * m4_earthDist;
	

	//Setting the matrices
	m_pMeshMngr->SetModelMatrix(m4_Sun, "Sun");
	m_pMeshMngr->SetModelMatrix(m4_Earth, "Earth");
	m_pMeshMngr->SetModelMatrix(m4_Moon, "Moon");

	//Adds all loaded instance to the render list
	m_pMeshMngr->AddInstanceToRenderList("ALL");

	static int nEarthOrbits = 0;
	static int nEarthRevolutions = 0;
	static int nMoonOrbits = 0;

	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();

	//Print info on the screen
	m_pMeshMngr->PrintLine(m_pSystem->GetAppName(), REYELLOW);
	
	m_pMeshMngr->Print("Time:");
	m_pMeshMngr->PrintLine(std::to_string(earthFinalQuat));

	m_pMeshMngr->Print("Day:");
	m_pMeshMngr->PrintLine(std::to_string(fPercent));

	m_pMeshMngr->Print("E_Orbits:");
	m_pMeshMngr->PrintLine(std::to_string(testVar));

	m_pMeshMngr->Print("E_Revolutions:");
	m_pMeshMngr->PrintLine(std::to_string(nEarthRevolutions));

	m_pMeshMngr->Print("M_Orbits:");
	m_pMeshMngr->PrintLine(std::to_string(nMoonOrbits));

	m_pMeshMngr->Print("FPS:");
	m_pMeshMngr->Print(std::to_string(nFPS), RERED);
}

void AppClass::Display(void)
{
	//clear the screen
	ClearScreen();

	//Render the grid based on the camera's mode:
	switch (m_pCameraMngr->GetCameraMode())
	{
	default:
		break;
	case CAMERAMODE::CAMROTHOX:
		m_pMeshMngr->AddGridToQueue(1.0f, REAXIS::YZ, RERED * 0.75f); //renders the YZ grid with a 100% scale
		break;
	case CAMERAMODE::CAMROTHOY:
		m_pMeshMngr->AddGridToQueue(1.0f, REAXIS::XZ, REGREEN * 0.75f); //renders the XZ grid with a 100% scale
		break;
	case CAMERAMODE::CAMROTHOZ:
		m_pMeshMngr->AddGridToQueue(1.0f, REAXIS::XY, REBLUE * 0.75f); //renders the XY grid with a 100% scale
		break;
	}
	
	m_pMeshMngr->Render(); //renders the render list

	m_pGLSystem->GLSwapBuffers(); //Swaps the OpenGL buffers
}

void AppClass::Release(void)
{
	super::Release(); //release the memory of the inherited fields
}
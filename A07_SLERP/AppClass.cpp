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

	static int nEarthOrbits = 0;
	static int nEarthRevolutions = 0;
	static int nMoonOrbits = 0;

	// Create size for Sun, Earth, and Moon
	matrix4 m4_sunSize = glm::scale(5.936f, 5.936f, 5.936f);
	matrix4 m4_earthSize = glm::scale(0.524f, 0.524f, 0.524f);
	matrix4 m4_moonSize = glm::scale(0.27f, 0.27f, 0.27f);

	// Create distance from origin for Earth, and Moon
	matrix4 m4_earthDist = glm::translate(11.0f, 0.0f, 0.0f);
	matrix4 m4_moonDist = glm::translate(2.0f, 0.0f, 0.0f);
	
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

	static double fEarthRotationTime = 0.0f;
	fEarthRotationTime += fCallTime;

	static double fMoonRotationTime = 0.0f;
	fMoonRotationTime += fCallTime;

	//Earth Orbit
	double fEarthHalfOrbTime = 182.5f * m_fDay; //Earths orbit around the sun lasts 365 days / half the time for 2 stops
	float fEarthHalfRevTime = 0.5f * m_fDay; // Move for Half a day
	float fMoonHalfOrbTime = 14.0f * m_fDay; //Moon's orbit is 28 earth days, so half the time for half a route

	float fEarthPercent = MapValue(static_cast<float>(fRunTime), 0.0f, 365.0f, 0.0f, 1.0f);
	float fMoonPercent = MapValue(static_cast<float>(fRunTime), 0.0f, 28.0f, 0.0f, 1.0f);

	if (fEarthRotationTime >= 365.0f ) {
		fEarthRotationTime = 0.0f;
		nEarthOrbits++;
	}

	if (fMoonRotationTime >= 28.0f) {
		fMoonRotationTime = 0.0f;
		nMoonOrbits++;
	}



	// Convert earth rotation to a quat
	//glm::quat earthQuat = glm::quat_cast(earthRot);

	// do slerp between the earth's roation and the last point
	double earthFinalQuat = glm::mix(glm::quat(vector3(0.0f, 0.0f, 0.0f)), glm::quat(vector3(0.0f, 720.0f, 0.0f)), fEarthPercent).y;

	double moonFinalQuat = glm::mix(glm::quat(vector3(0.0f, 0.0f, 0.0f)), glm::quat(vector3(0.0f, 720.0f, 0.0f)), fEarthPercent).y;

	//matrix4  earthMat4 = glm::mat4_cast(earthFinalQuat);

    m4_Sun = m4_sunSize;

	matrix4 earthRot = glm::rotate(IDENTITY_M4, glm::degrees(static_cast<float>(earthFinalQuat)), vector3(0.0f, 1.0f, 0.0f));

	m4_Earth = earthRot * m4_earthDist * m4_earthSize ;

	matrix4 moonRot = glm::rotate(m4_Earth, glm::degrees(static_cast<float>(moonFinalQuat)), vector3(0.0f, 1.0f, 0.0f));
	m4_Moon = moonRot  * m4_moonDist * m4_moonSize;

	//Setting the matrices
	m_pMeshMngr->SetModelMatrix(m4_Sun, "Sun");
	m_pMeshMngr->SetModelMatrix(m4_Earth, "Earth");
	m_pMeshMngr->SetModelMatrix(m4_Moon, "Moon");

	//Adds all loaded instance to the render list
	m_pMeshMngr->AddInstanceToRenderList("ALL");



	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();

	//Print info on the screen
	m_pMeshMngr->PrintLine(m_pSystem->GetAppName(), REYELLOW);
	
	m_pMeshMngr->Print("Time:");
	m_pMeshMngr->PrintLine(std::to_string(fRunTime));

	m_pMeshMngr->Print("Day:");
	m_pMeshMngr->PrintLine(std::to_string(m_fDay));

	m_pMeshMngr->Print("E_Orbits:");
	m_pMeshMngr->PrintLine(std::to_string(nEarthOrbits));

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
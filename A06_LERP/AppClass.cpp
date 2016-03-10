#include "AppClass.h"

void AppClass::InitWindow(String a_sWindowName)
{
	super::InitWindow("Assignment  06 - LERP"); // Window Name
}

void AppClass::InitVariables(void)
{
	m_pCameraMngr->SetPositionTargetAndView(vector3(0.0f, 0.0f, 15.0f), ZERO_V3, REAXISY);

	// Color of the screen
	m_v4ClearColor = vector4(REBLACK, 1); // Set the clear color to black

	m_pMeshMngr->LoadModel("Sorted\\WallEye.bto", "WallEye");

	fDuration = 1.0f;

	// Store locations
	locationList.push_back(vector3(-4.0f, -2.0f, 5.0f));
	locationList.push_back(vector3(1.0f, -2.0f, 5.0f));
	locationList.push_back(vector3(-3.0f, -1.0f, 3.0f));
	locationList.push_back(vector3(2.0f, -1.0f, 3.0f));
	locationList.push_back(vector3(-2.0f, 0.0f, 0.0f));
	locationList.push_back(vector3(3.0f, 0.0f, 0.0f));
	locationList.push_back(vector3(-1.0f, 1.0f, -3.0f));
	locationList.push_back(vector3(4.0f, 1.0f, -3.0f));
	locationList.push_back(vector3(0.0f, 2.0f, -5.0f));
	locationList.push_back(vector3(5.0f, 2.0f, -5.0f));
	locationList.push_back(vector3(1.0f, 3.0f, -5.0f));

	// Initialize a sphere and sphere's matrix
	m_pSphere = new PrimitiveClass();
	m_m4Sphere = IDENTITY_M4;

	// Intialize the model matrix
	m_m4Model = IDENTITY_M4;

	// Create the sphere
	m_pSphere->GenerateSphere(0.1f, 5, RERED);
}

void AppClass::Update(void)
{
#pragma region Does not change anything here
	//Update the system's time
	m_pSystem->UpdateTime();

	//Update the mesh manager's time without updating for collision detection
	m_pMeshMngr->Update();
#pragma region

#pragma region Does not need changes but feel free to change anything here
	//Lets us know how much time has passed since the last call
	double fTimeSpan = m_pSystem->LapClock(); //Delta time (between frame calls)

	//cumulative time
	static double fRunTime = 0.0f; //How much time has passed since the program started
	fRunTime += fTimeSpan;
#pragma endregion

#pragma region Your Code goes here

	// Create a static counter. Only instantiates once.
	static int counter = 0;
	// Check if counter is greater than the size of the location
		if(counter + 1 < locationList.size()){
			// Map the system run time to the duration.
			float fPercent = MapValue(static_cast<float>(fRunTime), 0.0f, fDuration, 0.0f, 1.0f);
			// Use lerp to get the vector between the set points.
			vector3 fPostion = glm::lerp(locationList[counter], locationList[counter + 1], fPercent);
			// Move the model based on that vector.
			m_m4Model = glm::translate(fPostion);
			// Set the model's matrix.
			m_pMeshMngr->SetModelMatrix(m_m4Model, "WallEye");
			// If the runtime equals the duration then the runtime is set back to 0 and the next set of points is moved onto.
			if (fRunTime >= fDuration) {
				fRunTime = 0.0f;
				counter++;
				}
			}
			else {	
				// Map the system run time to the duration.
					float fPercent = MapValue(static_cast<float>(fRunTime), 0.0f, fDuration, 0.0f, 1.0f);
					// Use lerp to get the vector between the last point and the first.
					vector3 fPostion = glm::lerp(locationList[counter], locationList[0], fPercent);
					// Move the model based on that vector.
					m_m4Model = glm::translate(fPostion);
					// Set the model's matrix.
					m_pMeshMngr->SetModelMatrix(m_m4Model, "WallEye");
					// If the runtime equals the duration then the runtime is set back to 0 and the next set of points is put back to start.
					if (fRunTime >= fDuration) {
						counter = 0;
						fRunTime = 0;
					}
				}
#pragma endregion

#pragma region Does not need changes but feel free to change anything here
	//Adds all loaded instance to the render list
	m_pMeshMngr->AddInstanceToRenderList("ALL");

	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();

	//Print info on the screen
	m_pMeshMngr->PrintLine(m_pSystem->GetAppName(), REYELLOW);
	m_pMeshMngr->Print("FPS:");
	m_pMeshMngr->PrintLine(std::to_string(nFPS), RERED);
	m_pMeshMngr->Print("fTimeSpan:");
	m_pMeshMngr->PrintLine(std::to_string(fTimeSpan), REYELLOW);
	m_pMeshMngr->Print("fRunTime:");
	m_pMeshMngr->PrintLine(std::to_string(fRunTime), REYELLOW);
	m_pMeshMngr->Print("fDuration:");
	m_pMeshMngr->PrintLine(std::to_string(fDuration), REYELLOW);
#pragma endregion
}

void AppClass::Display(void)
{
	//clear the screen
	ClearScreen();

	//Render the grid based on the camera's mode:
	switch (m_pCameraMngr->GetCameraMode())
	{
	default: //Perspective
		m_pMeshMngr->AddGridToQueue(1.0f, REAXIS::XY); //renders the XY grid with a 100% scale
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

	// Loop through the locations
	for (unsigned int i = 0; i < locationList.size(); i++) {
		// Move and render spheres based on location.
		m_m4Sphere = glm::translate(locationList[i]);
		m_pSphere->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m_m4Sphere);
	}

	m_pGLSystem->GLSwapBuffers(); //Swaps the OpenGL buffers
}

void AppClass::Release(void)
{
	super::Release(); //release the memory of the inherited fields
}
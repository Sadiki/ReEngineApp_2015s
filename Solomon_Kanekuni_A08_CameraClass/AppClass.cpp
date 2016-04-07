#include "AppClass.h"
void AppClass::InitWindow(String a_sWindowName)
{
	super::InitWindow("Kanekuni_Solomon_E10_Projections"); // Window Name
}
void AppClass::InitVariables(void)
{
	//Camera singleton
	camera = new CameraSingleton();

	//Generate the Cone
	m_pCone = new PrimitiveClass();
	m_pCone->GenerateCone(1.0f, 1.0f, 10, RERED);

	//Generate the Cylinder
	m_pCylinder = new PrimitiveClass();
	m_pCylinder->GenerateCylinder(1.0f, 1.0f, 10, REGREEN);

	//Calculate the first projections
	m_m4Projection = glm::perspective(
		45.0f, // Camera viewing angle
		1080.0f / 768.0f, // Camera viewing ratio
		0.01f,
		1000.0f);

	m_m4View = glm::lookAt(
		glm::vec3(0.0f, 0.0f, 15.0f), // Position
		glm::vec3(0.0f, 0.0f, 14.0f), // What i'm looking at
		glm::vec3(0.0f, 1.0f, 0.0f)); // rotation orientation - currently up
	
}

void AppClass::Update(void)
{
	//Update the system's time
	m_pSystem->UpdateTime();

	//Update the mesh manager's time without updating for collision detection
	m_pMeshMngr->Update();

	//Adds all loaded instance to the render list
	m_pMeshMngr->AddInstanceToRenderList("ALL");

	int nFPS = m_pSystem->GetFPS();
	m_pMeshMngr->PrintLine(m_pSystem->GetAppName(), REYELLOW);
	m_pMeshMngr->Print("P-X:", REYELLOW);
	m_pMeshMngr->PrintLine(std::to_string(camera->v3_Position.x), RERED);
	m_pMeshMngr->Print("P-Y:", REYELLOW);
	m_pMeshMngr->PrintLine(std::to_string(camera->v3_Position.y), RERED);
	m_pMeshMngr->Print("P-Z:", REYELLOW);
	m_pMeshMngr->PrintLine(std::to_string(camera->v3_Position.z), RERED);

	m_pMeshMngr->Print("FPS:");
	m_pMeshMngr->Print(std::to_string(nFPS), RERED);
	
}

void AppClass::Display(void)
{
	//Clear the window
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Render the grid
	m_pMeshMngr->AddGridToQueue(1.0f, REAXIS::XY);

	//Render the cone
	//m_pCone->Render(camera->GetProjection(false),camera->GetView(), IDENTITY_M4);

	//Render the cylinder
	m_pCylinder->Render(camera->GetProjection(false), camera->GetView() , IDENTITY_M4 * glm::mat4(3.0f));

	//Render the rest of the meshes
	m_pMeshMngr->Render();

	//Swaps the OpenGL buffers
	m_pGLSystem->GLSwapBuffers();
}

void AppClass::Release(void)
{
	//Release the memory of the member fields
	SafeDelete(m_pCone);
	SafeDelete(m_pCylinder);
	SafeDelete(camera);

	//Release the memory of the inherited fields
	super::Release(); 
}





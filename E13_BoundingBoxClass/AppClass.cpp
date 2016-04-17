#include "AppClass.h"
void AppClass::InitWindow(String a_sWindowName)
{
	super::InitWindow("MyBoundingSphereClass Elsberry, Bouffard, Kanekuni, Solomon"); // Window Name

	// Set the clear color based on Microsoft's CornflowerBlue (default in XNA)
	//if this line is in Init Application it will depend on the .cfg file, if it
	//is on the InitVariables it will always force it regardless of the .cfg
	m_v4ClearColor = vector4(0.4f, 0.6f, 0.9f, 0.0f);
}

void AppClass::InitVariables(void)
{
	//Initialize positions
	m_v3O1 = vector3(-2.5f, 0.0f, 0.0f);
	m_v3O2 = vector3(2.5f, 0.0f, 0.0f);
	

	//Load Models
	m_pMeshMngr->LoadModel("Minecraft\\Steve.obj", "Steve");
	m_pMeshMngr->LoadModel("Minecraft\\Creeper.obj", "Creeper");


	//steveBO = new MyBoundingSphereClass(m_pMeshMngr->GetVertexList("Steve"));
	//creeperBO = new MyBoundingSphereClass(m_pMeshMngr->GetVertexList("Creeper"));


	m_pBox1 = new MyBoundingCubeClass(m_pMeshMngr->GetVertexList("Steve"));
	m_pBox2 = new MyBoundingCubeClass(m_pMeshMngr->GetVertexList("Creeper"));

}

void AppClass::Update(void)
{
	//Update the system's time
	m_pSystem->UpdateTime();

	//Update the mesh manager's time without updating for collision detection
	m_pMeshMngr->Update();

	//First person camera movement
	if (m_bFPC == true)
		CameraRotation();

	ArcBall();
	

	//steveBO->UpdatePosition(m_v3O1);
	//steveBO->SetModelMatrix(glm::translate(m_v3O1)* glm::translate(0.0f, 1.0f, 0.0f) * ToMatrix4(m_qArcBall));
	//creeperBO->SetModelMatrix(glm::translate(m_v3O2)* glm::translate(0.0f, .8f, 0.0f));
	
	
	//m_m4Steve = steveBO->GetModelMatrix() * glm::translate(m_v3Center1);
	if (m_pBox1->IsColliding(m_pBox2)) {
		m_pMeshMngr->AddCubeToQueue(m_pBox1->GetModelMatrix() * glm::scale(vector3(m_pBox1->GetSize() * 2.0f)), RERED, WIRE);
		m_pMeshMngr->AddCubeToQueue(m_pBox2->GetModelMatrix() * glm::scale(vector3(m_pBox2->GetSize() * 2.0f)), RERED, WIRE);
		//m_pMeshMngr->AddSphereToQueue(steveBO->GetModelMatrix() * glm::scale(vector3(m_fRadius1 * 2.0f)), RERED, WIRE);
		//m_pMeshMngr->AddSphereToQueue(creeperBO->GetModelMatrix() * glm::scale(vector3(m_fRadius1 * 2.0f)), RERED, WIRE);
		//m_pMeshMngr->AddSphereToQueue(m_m4Steve * glm::scale(vector3(m_fRadius1 * 2.0f)), RERED, WIRE);
		m_pMeshMngr->PrintLine("They are colliding! >_<", RERED);
	}
	else
	{
		m_pMeshMngr->AddCubeToQueue(
			//glm::translate(vector3(m_pBox1->GetCenterG())) *
			glm::translate(vector3(m_pBox1->GetCenterG())) *
			ToMatrix4(m_qArcBall) *
			glm::scale(vector3(m_pBox1->GetSize()))
			, REGREEN, WIRE);
			
			
		m_pMeshMngr->AddCubeToQueue(m_pBox2->GetModelMatrix() * glm::scale(vector3(m_pBox2->GetRadius() * 2.0f)), REWHITE, WIRE);
		//m_pMeshMngr->AddSphereToQueue(steveBO->GetModelMatrix() * glm::scale(vector3(m_fRadius1 * 2.0f)), REWHITE, WIRE);
		//m_pMeshMngr->AddSphereToQueue(creeperBO->GetModelMatrix() * glm::scale(vector3(m_fRadius1 * 2.0f)), REWHITE, WIRE);
		//m_pMeshMngr->AddSphereToQueue(m_m4Steve * glm::scale(vector3(m_fRadius1 * 2.0f)), REWHITE, WIRE);
		m_pMeshMngr->PrintLine("They are not colliding! =)", REGREEN);
	}

	matrix4 m4Projection = m_pCameraMngr->GetProjectionMatrix();
	matrix4 m4View = m_pCameraMngr->GetViewMatrix();

	//m_pSphere1->Render(m4Projection, m4View, m4Model);
	
	

	
	//Set the model matrices for both objects and Bounding Spheres
	m_pMeshMngr->SetModelMatrix(glm::translate(m_v3O1) * ToMatrix4(m_qArcBall), "Steve");
	m_pMeshMngr->SetModelMatrix(glm::translate(m_v3O2), "Creeper");


	m_pBox1->SetModelMatrix(m_pMeshMngr->GetModelMatrix("Steve"));
	//m_pBox1->SetModelMatrix(glm::translate(m_v3O1));

	m_pBox2->SetModelMatrix(m_pMeshMngr->GetModelMatrix("Creeper"));

	bool isColliding = m_pBox1->IsColliding(m_pBox2);

	
	m_pMeshMngr->AddCubeToQueue(glm::translate(m_pBox1->GetCenterG()) * glm::scale(vector3(m_pBox1->GetSize())), REBLACK, WIRE);
	//m_pMeshMngr->AddCubeToQueue(m_pBox1->GetModelMatrix() * glm::scale(vector3(m_pBox1->GetRadius() * 2.0f)), REWHITE, WIRE); //Big box around steve


	m_pMeshMngr->AddCubeToQueue(glm::translate(vector3(m_pBox2->GetCenterG()))  *
		glm::scale(vector3(m_pBox2->GetSize())), REGREEN, WIRE);	//Creeper Small Box
	
	

	//Adds all loaded instance to the render list
	m_pMeshMngr->AddInstanceToRenderList("ALL");

	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();


	//Print info on the screen
	//m_pMeshMngr->Print("x:" + std::to_string(m_pBox1->GetPosition().x) + " ", RERED);
	//m_pMeshMngr->Print("y:" + std::to_string(steveBO->GetPosition().y) + " ", RERED);
	//m_pMeshMngr->Print("z:" + std::to_string(steveBO->GetPosition().z) + " ", RERED);
	//m_pMeshMngr->PrintLine("");

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

	m_pGLSystem->GLSwapBuffers(); //Swaps the OpenGL buffers
}

void AppClass::Release(void)
{
	if (m_pBox1 != nullptr)
	{
		delete m_pBox1;
		m_pBox1 = nullptr;

	}
	if (m_pBox2 != nullptr)
	{
		delete m_pBox2;
		m_pBox2 = nullptr;

	}
	super::Release(); //release the memory of the inherited fields
}
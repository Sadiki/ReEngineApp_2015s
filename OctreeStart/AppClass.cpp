#include "AppClass.h"
void AppClass::InitWindow(String a_sWindowName)
{
	super::InitWindow("A12_Spatial Optimization"); // Window Name

	// Set the clear color based on Microsoft's CornflowerBlue (default in XNA)
	//if this line is in Init Application it will depend on the .cfg file, if it
	//is on the InitVariables it will always force it regardless of the .cfg
	m_v4ClearColor = vector4(0.4f, 0.6f, 0.9f, 0.0f);
	//m_pSystem->SetWindowResolution(RESOLUTIONS::HD_1280X720);
	//m_pSystem->SetWindowFullscreen(); //Sets the window to be fullscreen
	//m_pSystem->SetWindowBorderless(true); //Sets the window to not have borders
}

void AppClass::InitVariables(void)
{
	//Reset the selection to -1, -1
	m_selection = std::pair<int, int>(-1, -1);
	//Set the camera position
	m_pCameraMngr->SetPositionTargetAndView(
		vector3(0.0f, 0.0f, 35.0f),//Camera position
		vector3(0.0f, 0.0f, 0.0f),//What Im looking at
		REAXISY);//What is up
	//Load a model onto the Mesh manager
	m_pBOMngr = MyBOManager::GetInstance();
	
	creeps = 50;
	for (uint n = 0; n < creeps; n++)	//Set the number of creepers to spawn in the screen
	{
		String sName = "Creeper" + std::to_string(n);
		vector3 v3Position = glm::sphericalRand(10.0f);
		m_pMeshMngr->LoadModel("Minecraft\\Creeper.obj", sName, false, 
			glm::translate(v3Position + vector3(5,0,0)));
		m_pBOMngr->AddObject(sName);
		objectList.push_back(sName);
	}

	m_pOctreeHead = new MyOctant();

	//Check to see if subdivisions are neccessary
	if (creeps > 1) {	
		m_pOctreeHead->MakeChildrenPrime(8);
	}

	displayGeometry = true;
	
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

	//Call the arcball method
	ArcBall();

	//Set the model matrix for the first model to be the arcball
	//m_pMeshMngr->SetModelMatrix(ToMatrix4(m_qArcBall), 0);

	//Adds all loaded instance to the render list
	//m_pMeshMngr->AddSkyboxToRenderList("Skybox_01.png");


	if (m_pBOMngr->GetSphereCheck()) {
		m_pBOMngr->Update();

		for (int i = 0; i < creeps; i++) {
		if (m_pBOMngr->GetBoundingObject(i)->GetIsSphereDisplayed()) {
			String sName = "Creeper" + std::to_string(i);
			m_pBOMngr->DisplaySphere(sName, RERED);
		}
	}
	}

	if (displayGeometry) {
		m_pMeshMngr->AddInstanceToRenderList("ALL");
	}
	else
	{
		m_pMeshMngr->ResetRenderList();
	}


	//Check to see if subdivisions are neccessary
	if (creeps > 1) {
		m_pOctreeHead->MakeChildrenPrime(8);

	}

	if (m_pOctreeHead->GetIsDisplayOctrees()) {
		m_pOctreeHead->Display();
	}

	


	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();
	//print info into the console
	//printf("FPS: %d            \r", nFPS);//print the Frames per Second
	//Print info on the screen
	m_pMeshMngr->PrintLine(m_pSystem->GetAppName(), REYELLOW);

	m_pMeshMngr->Print("Selection: ");
	m_pMeshMngr->PrintLine(m_pMeshMngr->GetInstanceGroupName(m_selection.first, m_selection.second), REYELLOW);

	m_pMeshMngr->Print("FPS:");
	m_pMeshMngr->PrintLine(std::to_string(nFPS), RERED);

	// If true then enable SAT
	m_pMeshMngr->Print("<K> Check Collisions: ");
	if (m_pBOMngr->GetCollisionSwitch()) {
		m_pMeshMngr->PrintLine("Spatial Optimization", REGREEN);
	}
	else {
		m_pMeshMngr->PrintLine("Brute Force", RERED);
	}

	// If true then octrees are drawn
	m_pMeshMngr->Print("<H> Display Octree: ");
	if (m_pOctreeHead->GetIsDisplayOctrees()) {
		m_pMeshMngr->PrintLine("ON", REGREEN);
	}
	else {
		m_pMeshMngr->PrintLine("OFF", RERED);
	}

	
	//// If true then show collisions
	//m_pMeshMngr->Print("<J> Display Collisions: ");
	//for (int i = 0; i < creeps; i++) {
	//	if (m_pBOMngr->GetBoundingObject(i)->GetIsSphereDisplayed()) {
	//		showCollisions = true;
	//	}
	//	else {
	//		showCollisions = false;
	//	}
	//}

	m_pMeshMngr->Print("<J> Display Collisions: ");
	if (m_pBOMngr->GetSphereCheck()) {
		m_pMeshMngr->PrintLine("ON", REGREEN);
	}
	else
	{
		m_pMeshMngr->PrintLine("OFF", RERED);
	}

	m_pMeshMngr->Print("<G> Display Geometry: ");
	if (displayGeometry) {
		m_pMeshMngr->PrintLine("ON", REGREEN);
	}
	else {
		m_pMeshMngr->PrintLine("OFF", RERED);
	}

	m_pMeshMngr->Print("Number of Blocks: ");
	m_pMeshMngr->PrintLine(std::to_string(creeps), REBLUE);
}

void AppClass::Display(void)
{
	//clear the screen
	ClearScreen();
	//Render the grid based on the camera's mode:
	m_pMeshMngr->AddGridToRenderListBasedOnCamera(m_pCameraMngr->GetCameraMode());
		m_pMeshMngr->Render(); //renders the render list
	
	m_pMeshMngr->ResetRenderList(); //Reset the Render list after render
	m_pGLSystem->GLSwapBuffers(); //Swaps the OpenGL buffers
	
}

void AppClass::Release(void)
{
	if (m_pOctreeHead != nullptr)
	{
		delete m_pOctreeHead;
		m_pOctreeHead = nullptr;
	}
	super::Release(); //release the memory of the inherited fields
}
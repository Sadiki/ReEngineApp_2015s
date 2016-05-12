#include "MyOctant.h"
using namespace ReEng;
//  MyOctant
bool MyOctant::m_bHead = true;
void MyOctant::Init(void)
{
	m_v3Position = vector3(0.0f);
	m_pMeshMngr = MeshManagerSingleton::GetInstance();
	m_fSize = 0.0f;
	m_nChildCount = 0;
	m_pBOMngr = MyBOManager::GetInstance();
	once = true;

	if (m_bHead)
	{
		int nObjectCont = m_pBOMngr->GetObjectCount();
		vector3 v3MinG = m_pBOMngr->GetBoundingObject(0)->GetMinG();
		vector3 v3MaxG = m_pBOMngr->GetBoundingObject(0)->GetMaxG();
		for (uint i = 1; i < nObjectCont; i++)
		{
			vector3 v3Min = m_pBOMngr->GetBoundingObject(i)->GetMinG();
			vector3 v3Max = m_pBOMngr->GetBoundingObject(i)->GetMaxG();
			//for x
			if (v3MinG.x > v3Min.x)
				v3MinG.x = v3Min.x;
			else if (v3MaxG.x < v3Max.x)
				v3MaxG.x = v3Max.x;

			//for y
			if (v3MinG.y > v3Min.y)
				v3MinG.y = v3Min.y;
			else if (v3MaxG.y < v3Max.y)
				v3MaxG.y = v3Max.y;

			//for z
			if (v3MinG.z > v3Min.z)
				v3MinG.z = v3Min.z;
			else if (v3MaxG.z < v3Max.z)
				v3MaxG.z = v3Max.z;
		}
		m_v3Position = (v3MaxG + v3MinG) / 2.0f;
		float fSizeX = glm::distance(vector3(v3MinG.x, 0, 0), vector3(v3MaxG.x, 0, 0));
		float fSizeY = glm::distance(vector3(0, v3MinG.y, 0), vector3(0, v3MaxG.y, 0));
		float fSizeZ = glm::distance(vector3(0, 0, v3MinG.z), vector3(0, 0, v3MaxG.z));

		m_fSize = fSizeX;
		if (m_fSize < fSizeY)
			m_fSize = fSizeY;
		if (m_fSize < fSizeZ)
			m_fSize = fSizeZ;

	}
}
void MyOctant::Swap(MyOctant& other)
{

}
void MyOctant::Release(void)
{
}
//The big 3
MyOctant::MyOctant(){Init();}
MyOctant::MyOctant(float a_fSize) { Init(); m_fSize = a_fSize; }
MyOctant::MyOctant(MyOctant const& other)
{

}
MyOctant& MyOctant::operator=(MyOctant const& other)
{
	if(this != &other)
	{
		Release();
		Init();
		MyOctant temp(other);
		Swap(temp);
	}
	return *this;
}
MyOctant::~MyOctant(){Release();};

void MyOctant::Display(void)
{
		for (uint n = 0; n < m_nChildCount; n++)
		{
			m_pChildren[n].Display();
		}
		m_pMeshMngr->AddCubeToRenderList(glm::translate(m_v3Position) * glm::scale(vector3(m_fSize)), REWHITE, WIRE);
		//used to check min and max of boxes
		/*m_pMeshMngr->AddCubeToRenderList(glm::translate(vector3(m_v3Position.x - m_fSize/2, m_v3Position.y - m_fSize/2, m_v3Position.z - m_fSize/2)) * glm::scale(vector3(.5)), RERED, SOLID);
		m_pMeshMngr->AddCubeToRenderList(glm::translate(vector3(m_v3Position.x + m_fSize / 2, m_v3Position.y + m_fSize / 2, m_v3Position.z + m_fSize / 2)) * glm::scale(vector3(.5)), REGREEN, SOLID);*/
}


void MyOctant::MakeChildrenPrime(int numChild) //Make the prime children, the first real 
											   //subdivsion of the cube around the creepers
{
	m_bHead = false;
	m_pChildren = new MyOctant[8];
	m_nChildCount = 8;
	float fNewSize = this->m_fSize / 2;

	for (uint index = 0; index < 8; index++)
	{
		m_pChildren[index].m_fSize = fNewSize;
		m_pChildren[index].m_v3Position = m_v3Position;
	}

	fNewSize /= 2.0f;

	//for the index 0
	m_pChildren[0].m_v3Position.x += fNewSize;
	m_pChildren[0].m_v3Position.y += fNewSize;
	m_pChildren[0].m_v3Position.z += fNewSize;

	//for the index 1
	m_pChildren[1].m_v3Position.x -= fNewSize;
	m_pChildren[1].m_v3Position.y += fNewSize;
	m_pChildren[1].m_v3Position.z += fNewSize;

	//for the index 2
	m_pChildren[2].m_v3Position.x -= fNewSize;
	m_pChildren[2].m_v3Position.y -= fNewSize;
	m_pChildren[2].m_v3Position.z += fNewSize;

	//for the index 3
	m_pChildren[3].m_v3Position.x += fNewSize;
	m_pChildren[3].m_v3Position.y -= fNewSize;
	m_pChildren[3].m_v3Position.z += fNewSize;

	//for the index 4
	m_pChildren[4].m_v3Position.x += fNewSize;
	m_pChildren[4].m_v3Position.y += fNewSize;
	m_pChildren[4].m_v3Position.z -= fNewSize;

	//for the index 5
	m_pChildren[5].m_v3Position.x -= fNewSize;
	m_pChildren[5].m_v3Position.y += fNewSize;
	m_pChildren[5].m_v3Position.z -= fNewSize;

	//for the index 6
	m_pChildren[6].m_v3Position.x -= fNewSize;
	m_pChildren[6].m_v3Position.y -= fNewSize;
	m_pChildren[6].m_v3Position.z -= fNewSize;

	//for the index 7
	m_pChildren[7].m_v3Position.x += fNewSize;
	m_pChildren[7].m_v3Position.y -= fNewSize;
	m_pChildren[7].m_v3Position.z -= fNewSize;

	//positions at xyz are the center of the cube
	//this->CheckSub(m_pChildren[1]);


	for (uint q = 0; q < m_nChildCount; q++) 
	{
		m_pChildren[q].isSubdivide = false;
		int nObjectCount = m_pBOMngr->GetObjectCount();
		for (uint i = 0; i < nObjectCount; i++)
		{
			vector3 v3Min = m_pBOMngr->GetBoundingObject(i)->GetMinG();
			vector3 v3Max = m_pBOMngr->GetBoundingObject(i)->GetMaxG();
			vector3 v3Cent = m_pBOMngr->GetBoundingObject(i)->GetCenterGlobal();
			//for x
			if (v3Min.x > m_pChildren[q].m_v3Position.x - (m_pChildren[q].m_fSize / 2) && v3Max.x < m_pChildren[q].m_v3Position.x + (m_pChildren[q].m_fSize / 2) && //all parts are in the child
				v3Min.y > m_pChildren[q].m_v3Position.y - (m_pChildren[q].m_fSize / 2) && v3Max.y < m_pChildren[q].m_v3Position.y + (m_pChildren[q].m_fSize / 2) &&
				v3Min.z > m_pChildren[q].m_v3Position.z - (m_pChildren[q].m_fSize / 2) && v3Max.z < m_pChildren[q].m_v3Position.z + (m_pChildren[q].m_fSize / 2) 
				)
			{
				m_pChildren[q].tooManyObjects++;
			}
			else if (v3Min.x > m_pChildren[q].m_v3Position.x - (m_pChildren[q].m_fSize / 2) && v3Min.x < m_pChildren[q].m_v3Position.x + (m_pChildren[q].m_fSize / 2) &&//min point in child
					v3Min.y > m_pChildren[q].m_v3Position.y - (m_pChildren[q].m_fSize / 2) && v3Min.y < m_pChildren[q].m_v3Position.y + (m_pChildren[q].m_fSize / 2) &&
					v3Min.z > m_pChildren[q].m_v3Position.z - (m_pChildren[q].m_fSize / 2) && v3Min.z < m_pChildren[q].m_v3Position.z + (m_pChildren[q].m_fSize / 2)
					)
			{
				m_pChildren[q].tooManyObjects++;
			}
			
			else if(v3Max.x < m_pChildren[q].m_v3Position.x + (m_pChildren[q].m_fSize / 2) && v3Max.x > m_pChildren[q].m_v3Position.x - (m_pChildren[q].m_fSize / 2) &&//max point in child
					v3Max.y < m_pChildren[q].m_v3Position.y + (m_pChildren[q].m_fSize / 2) && v3Max.y > m_pChildren[q].m_v3Position.y - (m_pChildren[q].m_fSize / 2) &&
					v3Max.z < m_pChildren[q].m_v3Position.z + (m_pChildren[q].m_fSize / 2) && v3Max.z > m_pChildren[q].m_v3Position.z - (m_pChildren[q].m_fSize / 2)
					)
			{
				m_pChildren[q].tooManyObjects++;
			}
			else if(v3Cent.x > m_pChildren[q].m_v3Position.x - (m_pChildren[q].m_fSize / 2) && v3Cent.x < m_pChildren[q].m_v3Position.x + (m_pChildren[q].m_fSize / 2) && //center point in child
					v3Cent.y > m_pChildren[q].m_v3Position.y - (m_pChildren[q].m_fSize / 2) && v3Cent.y < m_pChildren[q].m_v3Position.y + (m_pChildren[q].m_fSize / 2) &&
					v3Cent.z > m_pChildren[q].m_v3Position.z - (m_pChildren[q].m_fSize / 2) && v3Cent.z < m_pChildren[q].m_v3Position.z + (m_pChildren[q].m_fSize / 2) 
					)
			{
				m_pChildren[q].tooManyObjects++;
			}

			if (m_pChildren[q].tooManyObjects > 1) {//set the number of things that can inhabit a child before subdivide
				m_pChildren[q].tooManyObjects = 0;
				m_pChildren[q].isSubdivide = true;
				
				
			}
			
		}

		if (m_pChildren[q].isSubdivide)//recursion and limits the number of subdivisons 
		{
			// If SubCount is less than 5
			if (subCount < 5) {
				//Increment Subcount by 1;
				subCount++;
				for (int i = 0; i < numChild; i++) {
					m_pChildren[q].MakeChildrenPrime(numChild - 1);

				}

				if (subCount > 5) {
					subCount = 0;
				}
			}
			else
			{
				m_pChildren[q].MakeChildrenPrime(0);
			}


		}
	}
}

bool MyOctant::CheckSub(MyOctant octChild) {

	int nObjectCount = m_pBOMngr->GetObjectCount();

		for (uint i = 1; i < nObjectCount; i++)
		{
			vector3 v3Min = m_pBOMngr->GetBoundingObject(i)->GetMinG();
			vector3 v3Max = m_pBOMngr->GetBoundingObject(i)->GetMaxG();
			
			if (v3Min.x > octChild.m_v3Position.x - (octChild.m_fSize / 2) && v3Max.x < octChild.m_v3Position.x + (octChild.m_fSize / 2) &&
				v3Min.y > octChild.m_v3Position.y - (octChild.m_fSize / 2) && v3Max.y < octChild.m_v3Position.y + (octChild.m_fSize / 2) &&
				v3Min.z > octChild.m_v3Position.z - (octChild.m_fSize / 2) && v3Max.z < octChild.m_v3Position.z + (octChild.m_fSize / 2))
			{
				octChild.tooManyObjects++;
			}

			/*////for x
			//if (v3Min.x > octChild.m_v3Position.x - (octChild.m_fSize / 2) || v3Max.x < octChild.m_v3Position.x + (octChild.m_fSize / 2)) {
			//	octChild.tooManyObjects++; 
			//}

			//else if (v3Min.y > octChild.m_v3Position.y - (octChild.m_fSize / 2) || v3Max.y < octChild.m_v3Position.y + (octChild.m_fSize / 2)) {
			//	octChild.tooManyObjects++;
			//}

			//else if (v3Min.z > octChild.m_v3Position.z - (octChild.m_fSize / 2) || v3Max.z < octChild.m_v3Position.z + (octChild.m_fSize / 2)) {
			//	octChild.tooManyObjects++;
			//}*///OLD AND BROKEN

			if (octChild.tooManyObjects > 0) {
				octChild.tooManyObjects = 0;
				octChild.isSubdivide = true;
			}

		}
		return octChild.isSubdivide;
	
}

void MyOctant::SetTooManyObjects(int index, int objNum) {
	m_pChildren[index].tooManyObjects = objNum;
}

int MyOctant::GetTooManyObjects(int index) {
	return m_pChildren[index].tooManyObjects;
}

void MyOctant::ReleaseChildren(void)
{
	if (m_pChildren != nullptr)
	{
		delete[] m_pChildren;
		m_pChildren = nullptr;
	}
}

int MyOctant::GetSubCount() {
	return subCount;
}
// Accessors
bool MyOctant::GetIsDisplayOctrees() {
	return isDisplayOctrees;
}

// Mutators
void MyOctant::SetIsDisplayOctrees(bool displayed ) {
	isDisplayOctrees = displayed;
}

//--- Non Standard Singleton Methods




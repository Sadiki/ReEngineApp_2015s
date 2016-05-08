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

		subCount = 4;
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
}

void MyOctant::Subdivide(void)
{


	m_bHead = false;
	m_pChildren = new MyOctant[8];
	m_nChildCount = 8;
	float fNewSize = this->m_fSize / 2;
	int newSubCount = subCount - 1;
	for (uint index = 0; index < 8; index++)
	{
		m_pChildren[index].m_fSize = fNewSize;
		m_pChildren[index].m_v3Position = m_v3Position;
		m_pChildren[index].subCount = newSubCount;
	}
	fNewSize /= 2.0f;


	int nObjectCount = m_pBOMngr->GetObjectCount();
	vector3 v3MinG = m_pBOMngr->GetBoundingObject(0)->GetMinG();
	vector3 v3MaxG = m_pBOMngr->GetBoundingObject(0)->GetMaxG();


	if (m_pChildren != nullptr)
	{
		for (uint index = 0; index < 8; index++)
		{

			m_pChildren[index].m_v3Position = m_v3Position;

			for (uint i = 1; i < nObjectCount; i++)
			{
				vector3 v3Min = m_pBOMngr->GetBoundingObject(i)->GetMinG();
				vector3 v3Max = m_pBOMngr->GetBoundingObject(i)->GetMaxG();
				//for x
				if (v3Min.x > m_pChildren[index].m_v3Position.x - (m_pChildren[index].m_fSize / 2) && v3Max.x < m_pChildren[index].m_v3Position.x + (m_pChildren[index].m_fSize / 2)) {
					m_pChildren[index].tooManyObjects++;
				}

				else if (v3Min.y > m_pChildren[index].m_v3Position.y - (m_pChildren[index].m_fSize / 2) && v3Max.y < m_pChildren[index].m_v3Position.y + (m_pChildren[index].m_fSize / 2)) {
					m_pChildren[index].tooManyObjects++;
				}

				else if (v3Min.z > m_pChildren[index].m_v3Position.z - (m_pChildren[index].m_fSize / 2) && v3Max.z < m_pChildren[index].m_v3Position.z + (m_pChildren[index].m_fSize / 2)) {
					m_pChildren[index].tooManyObjects++;
				}

				

			}
		}
	}

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
}

void MyOctant::CheckSub(void) {

	//int nObjectCount = m_pBOMngr->GetObjectCount();
	//vector3 v3MinG = m_pBOMngr->GetBoundingObject(0)->GetMinG();
	//vector3 v3MaxG = m_pBOMngr->GetBoundingObject(0)->GetMaxG();


	//if (m_pChildren != nullptr)
	//{
	//	for (uint index = 0; index < 8; index++)
	//	{

	//		m_pChildren[index].m_v3Position = m_v3Position;

	//		for (uint i = 1; i < nObjectCount; i++)
	//		{
	//			vector3 v3Min = m_pBOMngr->GetBoundingObject(i)->GetMinG();
	//			vector3 v3Max = m_pBOMngr->GetBoundingObject(i)->GetMaxG();
	//			//for x
	//			if (v3Min.x > m_pChildren[index].m_v3Position.x - (m_pChildren[index].m_fSize / 2) && v3Max.x < m_pChildren[index].m_v3Position.x + (m_pChildren[index].m_fSize / 2)) {
	//				m_pChildren[index].tooManyObjects++;
	//			}

	//			else if (v3Min.y > m_pChildren[index].m_v3Position.y - (m_pChildren[index].m_fSize / 2) && v3Max.y < m_pChildren[index].m_v3Position.y + (m_pChildren[index].m_fSize / 2)) {
	//				m_pChildren[index].tooManyObjects++;
	//			}

	//			else if (v3Min.z > m_pChildren[index].m_v3Position.z - (m_pChildren[index].m_fSize / 2) && v3Max.z < m_pChildren[index].m_v3Position.z + (m_pChildren[index].m_fSize / 2)) {
	//				m_pChildren[index].tooManyObjects++;
	//			}

	//			if (m_pChildren[index].tooManyObjects > 3) {
	//				m_pChildren[index].Subdivide();
	//				m_pChildren[index].tooManyObjects = 0;
	//			}

	//		}
	//	}
	//}
}

void MyOctant::MakeNewChildren(float fNewSize) {

	m_pChildren = new MyOctant[8];

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
//Accessors

//--- Non Standard Singleton Methods


#include "MyBOClass.h"
//  MyBOClass
void MyBOClass::Init(void)
{
	m_m4ToWorld = IDENTITY_M4;

	m_v3Center = vector3(0.0f);
	m_v3Min = vector3(0.0f);
	m_v3Max = vector3(0.0f);

	m_v3CenterG = vector3(0.0f);
	m_v3MinG = vector3(0.0f);
	m_v3MaxG = vector3(0.0f);

	m_v3HalfWidth = vector3(0.0f);
	m_v3HalfWidthG = vector3(0.0f);

	m_fRadius = 0.0f;

	m_pMeshMngr = MeshManagerSingleton::GetInstance();
}
void MyBOClass::Swap(MyBOClass& other)
{
	std::swap(m_m4ToWorld, other.m_m4ToWorld);

	std::swap(m_v3Center, other.m_v3Center);
	std::swap(m_v3Min, other.m_v3Min);
	std::swap(m_v3Max, other.m_v3Max);

	std::swap(m_v3CenterG, other.m_v3CenterG);
	std::swap(m_v3MinG, other.m_v3MinG);
	std::swap(m_v3MaxG, other.m_v3MaxG);

	std::swap(m_v3HalfWidth, other.m_v3HalfWidth);
	std::swap(m_v3HalfWidthG, other.m_v3HalfWidthG);

	std::swap(m_fRadius, other.m_fRadius);

	std::swap(m_pMeshMngr, other.m_pMeshMngr);
}
void MyBOClass::Release(void) {/* No pointers allocated yet */ }
//The big 3
MyBOClass::MyBOClass(std::vector<vector3> a_lVectorList)
{
	//Init the default values
	Init();

	//Count the points of the incoming list
	uint nVertexCount = a_lVectorList.size();

	//If there are none just return, we have no information to create the BS from
	if (nVertexCount == 0)
		return;

	//Max and min as the first vector of the list
	m_v3Max = m_v3Min = a_lVectorList[0];

	//Get the max and min out of the list
	for (uint nVertex = 1; nVertex < nVertexCount; nVertex++)
	{
		if (m_v3Min.x > a_lVectorList[nVertex].x) //If min is larger than current
			m_v3Min.x = a_lVectorList[nVertex].x;
		else if (m_v3Max.x < a_lVectorList[nVertex].x)//if max is smaller than current
			m_v3Max.x = a_lVectorList[nVertex].x;

		if (m_v3Min.y > a_lVectorList[nVertex].y) //If min is larger than current
			m_v3Min.y = a_lVectorList[nVertex].y;
		else if (m_v3Max.y < a_lVectorList[nVertex].y)//if max is smaller than current
			m_v3Max.y = a_lVectorList[nVertex].y;

		if (m_v3Min.z > a_lVectorList[nVertex].z) //If min is larger than current
			m_v3Min.z = a_lVectorList[nVertex].z;
		else if (m_v3Max.z < a_lVectorList[nVertex].z)//if max is smaller than current
			m_v3Max.z = a_lVectorList[nVertex].z;
	}

	//with the max and the min we calculate the center
	m_v3Center = (m_v3Min + m_v3Max) / 2.0f;

	m_fRadius = glm::distance(m_v3Center, m_v3Max);
	//another way of doing this, except that with it the sphere just surrounds the object making the
	//pretest kind of useless
	//m_fRadius = 0;
	//for (uint nVertex = 0; nVertex < nVertexCount; nVertex++)
	//{
	//	float fDistance = glm::distance(m_v3Center, a_lVectorList[nVertex]);
	//	if (m_fRadius < fDistance)
	//		m_fRadius = fDistance;
	//}

	//we calculate the distance between all the values of min and max vectors
	m_v3HalfWidth = (m_v3Max - m_v3Min) / 2.0f;

	m_v3MaxG = m_v3Max;
	m_v3MinG = m_v3Min;
	m_v3CenterG = m_v3Center;
	m_v3HalfWidthG = m_v3HalfWidth;
}
MyBOClass::MyBOClass(MyBOClass const& other)
{
	m_m4ToWorld = other.m_m4ToWorld;

	m_v3Center = other.m_v3Center;
	m_v3Min = other.m_v3Min;
	m_v3Max = other.m_v3Max;

	m_v3CenterG = other.m_v3CenterG;
	m_v3MinG = other.m_v3MinG;
	m_v3MaxG = other.m_v3MaxG;

	m_v3HalfWidth = other.m_v3HalfWidth;
	m_v3HalfWidthG = other.m_v3HalfWidthG;

	m_fRadius = other.m_fRadius;

	m_pMeshMngr = other.m_pMeshMngr;
}
MyBOClass& MyBOClass::operator=(MyBOClass const& other)
{
	if (this != &other)
	{
		Release();
		Init();
		MyBOClass temp(other);
		Swap(temp);
	}
	return *this;
}
MyBOClass::~MyBOClass() { Release(); };
//Accessors
void MyBOClass::SetModelMatrix(matrix4 a_m4ToWorld)
{
	//If there are no changes in the Model Matrix there is no need
	//of doing further calculations
	if (m_m4ToWorld == a_m4ToWorld)
		return;

	m_m4ToWorld = a_m4ToWorld;
	//Calculate the vertex that makes the Object
	vector3 v3Corner[8];
	v3Corner[0] = vector3(m_v3Min.x, m_v3Min.y, m_v3Min.z);
	v3Corner[1] = vector3(m_v3Max.x, m_v3Min.y, m_v3Min.z);
	v3Corner[2] = vector3(m_v3Min.x, m_v3Max.y, m_v3Min.z);
	v3Corner[3] = vector3(m_v3Max.x, m_v3Max.y, m_v3Min.z);

	v3Corner[4] = vector3(m_v3Min.x, m_v3Min.y, m_v3Max.z);
	v3Corner[5] = vector3(m_v3Max.x, m_v3Min.y, m_v3Max.z);
	v3Corner[6] = vector3(m_v3Min.x, m_v3Max.y, m_v3Max.z);
	v3Corner[7] = vector3(m_v3Max.x, m_v3Max.y, m_v3Max.z);
	//Get vectors in global space
	for (uint nVertex = 0; nVertex < 8; nVertex++)
	{
		v3Corner[nVertex] = vector3(m_m4ToWorld * vector4(v3Corner[nVertex], 1.0f));
	}

	//Get the max and min out of the list
	m_v3MaxG = m_v3MinG = v3Corner[0];
	for (uint nVertex = 1; nVertex < 8; nVertex++)
	{
		if (m_v3MinG.x > v3Corner[nVertex].x) //If min is larger than current
			m_v3MinG.x = v3Corner[nVertex].x;
		else if (m_v3MaxG.x < v3Corner[nVertex].x)//if max is smaller than current
			m_v3MaxG.x = v3Corner[nVertex].x;

		if (m_v3MinG.y > v3Corner[nVertex].y) //If min is larger than current
			m_v3MinG.y = v3Corner[nVertex].y;
		else if (m_v3MaxG.y < v3Corner[nVertex].y)//if max is smaller than current
			m_v3MaxG.y = v3Corner[nVertex].y;

		if (m_v3MinG.z > v3Corner[nVertex].z) //If min is larger than current
			m_v3MinG.z = v3Corner[nVertex].z;
		else if (m_v3MaxG.z < v3Corner[nVertex].z)//if max is smaller than current
			m_v3MaxG.z = v3Corner[nVertex].z;
	}
	m_v3CenterG = (m_v3MinG + m_v3MaxG) / 2.0f;

	//we calculate the distance between all the values of min and max vectors
	m_v3HalfWidthG = (m_v3MaxG - m_v3MinG) / 2.0f;

	m_fRadius = glm::distance(m_v3CenterG, m_v3MaxG);
}
float MyBOClass::GetRadius(void) { return m_fRadius; }
matrix4 MyBOClass::GetModelMatrix(void) { return m_m4ToWorld; }
vector3 MyBOClass::GetCenterLocal(void) { return m_v3Center; }
vector3 MyBOClass::GetCenterGlobal(void) { return m_v3CenterG; }
vector3 MyBOClass::GetHalfWidth(void) { return m_v3HalfWidth; }
vector3 MyBOClass::GetHalfWidthG(void) { return m_v3HalfWidthG; }
vector3 MyBOClass::GetMin(void) { return m_v3Min; }
vector3 MyBOClass::GetMax(void) { return m_v3Max; }
vector3 MyBOClass::GetMinG(void) { return m_v3MinG; }
vector3 MyBOClass::GetMaxG(void) { return m_v3MaxG; }

//--- Non Standard Singleton Methods
void MyBOClass::DisplaySphere(vector3 a_v3Color)
{
		m_pMeshMngr->AddSphereToRenderList(glm::translate(m_m4ToWorld, m_v3Center) *
		glm::scale(vector3(m_fRadius * 2.0f)), a_v3Color, WIRE);
}

void MyBOClass::DisplayOriented(vector3 a_v3Color)
{
	m_pMeshMngr->AddCubeToRenderList(glm::translate(m_m4ToWorld, m_v3Center) *
		glm::scale(m_v3HalfWidth * 2.0f), a_v3Color, WIRE);
}

void MyBOClass::DisplayReAlligned(vector3 a_v3Color)
{
	m_pMeshMngr->AddCubeToRenderList(glm::translate(IDENTITY_M4, m_v3CenterG) *
		glm::scale(m_v3HalfWidthG * 2.0f), a_v3Color, WIRE);
}

bool MyBOClass::SAT(MyBOClass* const a, MyBOClass* const b)
{
	GLfloat ra, rb;
	GLfloat R[3][3], AbsR[3][3];
	//Compute rotation matrix expressing b in a's coordinate frame
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			R[i][j] = glm::dot(glm::vec3(a->GetModelMatrix()[i].x, a->GetModelMatrix()[i].y, a->GetModelMatrix()[i].z),
				glm::vec3(b->GetModelMatrix()[j].x, b->GetModelMatrix()[j].y, b->GetModelMatrix()[j].z));

	//Compute translation vector t
	glm::vec3 t = b->GetCenterGlobal() - a->GetCenterGlobal();
	//Bring translation into a's coordinate frame
	t = glm::vec3(glm::dot(t, glm::vec3(a->GetModelMatrix()[0].x, a->GetModelMatrix()[0].y, a->GetModelMatrix()[0].z)),
		glm::dot(t, glm::vec3(a->GetModelMatrix()[1].x, a->GetModelMatrix()[1].y, a->GetModelMatrix()[1].z)),
		glm::dot(t, glm::vec3(a->GetModelMatrix()[2].x, a->GetModelMatrix()[2].y, a->GetModelMatrix()[2].z)));

	//Compute common subexpressions.Add in an epsilon term to
	//counteract arithmetic errors when two edges are parallel and
	//their cross product is (near) null
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			AbsR[i][j] = glm::abs(R[i][j]) + .00001;

	//Test axes L = A0, L = A1, L = A2
	for (int i = 0; i < 3; i++) {
		ra = a->GetHalfWidthG()[i];
		rb = b->GetHalfWidthG()[0] * AbsR[i][0] + b->GetHalfWidthG()[1] * AbsR[i][1] + b->GetHalfWidthG()[2] * AbsR[i][2];
		if (glm::abs(t[i]) > ra + rb)return false;
	}

	//Test axes L = B1, L = B1, L = B2
	for (int i = 0; i < 3; i++) {
		ra = a->GetHalfWidthG()[0] * AbsR[0][i] + a->GetHalfWidthG()[1] * AbsR[1][i] + a->GetHalfWidthG()[2] * AbsR[2][i];
		rb = b->GetHalfWidthG()[i];
		if (glm::abs(t[0] * R[0][i] + t[1] * R[1][i] + t[2] * R[2][i]) > ra + rb)return false;
	}

	//Test axis L = A0 X B0
	ra = a->GetHalfWidthG()[1] * AbsR[2][0] + a->GetHalfWidthG()[2] * AbsR[1][0];
	rb = b->GetHalfWidthG()[1] * AbsR[0][2] + b->GetHalfWidthG()[2] * AbsR[0][1];
	if (glm::abs(t[2] * R[1][0] - t[1] * R[2][0]) > ra + rb)return false;

	//Test axis L = A0 X B1
	ra = a->GetHalfWidthG()[1] * AbsR[2][1] + a->GetHalfWidthG()[2] * AbsR[1][1];
	rb = b->GetHalfWidthG()[0] * AbsR[0][2] + b->GetHalfWidthG()[2] * AbsR[0][0];
	if (glm::abs(t[2] * R[1][1] - t[1] * R[2][1]) > ra + rb)return false;

	//Test axis L = A0 x B2
	ra = a->GetHalfWidthG()[1] * AbsR[2][2] + a->GetHalfWidthG()[2] * AbsR[1][2];
	rb = b->GetHalfWidthG()[0] * AbsR[0][1] + b->GetHalfWidthG()[1] * AbsR[0][0];
	if (glm::abs(t[2] * R[1][2] - t[1] * R[2][2]) > ra + rb)return false;

	//Test axis L = A1 x B0
	ra = a->GetHalfWidthG()[0] * AbsR[2][0] + a->GetHalfWidthG()[2] * AbsR[0][0];
	rb = b->GetHalfWidthG()[1] * AbsR[1][2] + b->GetHalfWidthG()[2] * AbsR[1][1];
	if (glm::abs(t[0] * R[2][0] - t[2] * R[0][0]) > ra + rb)return false;

	//Test axis L = A1 X B1
	ra = a->GetHalfWidthG()[0] * AbsR[2][1] + a->GetHalfWidthG()[2] * AbsR[0][1];
	rb = b->GetHalfWidthG()[0] * AbsR[1][2] + b->GetHalfWidthG()[2] * AbsR[1][0];
	if (glm::abs(t[0] * R[2][1] - t[2] * R[0][1]) > ra + rb) return false;

	//Test axis L = A1 X B2
	ra = a->GetHalfWidthG()[0] * AbsR[2][2] + a->GetHalfWidthG()[2] * AbsR[0][2];
	rb = b->GetHalfWidthG()[0] * AbsR[1][1] + b->GetHalfWidthG()[1] * AbsR[1][0];
	if (glm::abs(t[0] * R[2][2] - t[2] * R[0][2]) > ra + rb)return false;

	//Test axis L = A2 X B0
	ra = a->GetHalfWidthG()[0] * AbsR[1][0] + a->GetHalfWidthG()[1] * AbsR[0][0];
	rb = b->GetHalfWidthG()[1] * AbsR[2][2] + b->GetHalfWidthG()[2] * AbsR[2][1];
	if (glm::abs(t[1] * R[0][0] - t[0] * R[1][0]) > ra + rb)return false;

	//Test axis L = A2 X B1
	ra = a->GetHalfWidthG()[0] * AbsR[1][1] + a->GetHalfWidthG()[1] * AbsR[0][1];
	rb = b->GetHalfWidthG()[0] * AbsR[2][2] + b->GetHalfWidthG()[2] * AbsR[2][0];
	if (glm::abs(t[1] * R[0][1] - t[0] * R[1][1]) > ra + rb)return false;

	//Test axis L = A2 X B2
	ra = a->GetHalfWidthG()[0] * AbsR[1][2] + a->GetHalfWidthG()[1] * AbsR[0][2];
	rb = b->GetHalfWidthG()[0] * AbsR[2][1] + b->GetHalfWidthG()[1] * AbsR[2][0];
	if (glm::abs(t[1] * R[0][2] - t[0] * R[1][2]) > ra + rb) return false;

	return true;
}


bool MyBOClass::IsColliding(MyBOClass* const a_pOther)
{
	//Get all vectors in global space
	vector3 v3Min = vector3(m_m4ToWorld * vector4(m_v3Min, 1.0f));
	vector3 v3Max = vector3(m_m4ToWorld * vector4(m_v3Max, 1.0f));

	vector3 v3MinO = vector3(a_pOther->m_m4ToWorld * vector4(a_pOther->m_v3Min, 1.0f));
	vector3 v3MaxO = vector3(a_pOther->m_m4ToWorld * vector4(a_pOther->m_v3Max, 1.0f));

	//Are they colliding?
	//For Objects we will assume they are colliding, unless at least one of the following conditions is not met
	//first check the bounding sphere, if that is not colliding we can guarantee that there are no collision
	if ((m_fRadius + a_pOther->m_fRadius) < glm::distance(m_v3CenterG, a_pOther->m_v3CenterG))
		return false;

	//If the distance was smaller it might be colliding
	bool bColliding = true;

	//Check for X
	if (m_v3MaxG.x < a_pOther->m_v3MinG.x)
		bColliding = false;
	if (m_v3MinG.x > a_pOther->m_v3MaxG.x)
		bColliding = false;

	//Check for Y
	if (m_v3MaxG.y < a_pOther->m_v3MinG.y)
		bColliding = false;
	if (m_v3MinG.y > a_pOther->m_v3MaxG.y)
		bColliding = false;

	//Check for Z
	if (m_v3MaxG.z < a_pOther->m_v3MinG.z)
		bColliding = false;
	if (m_v3MinG.z > a_pOther->m_v3MaxG.z)
		bColliding = false;

	if (bColliding == false)
		return false;

	return SAT(this , a_pOther);
}

void MyBOClass::SetIsSphereDisplayed(bool displayed) {
	isSphereDisplayed = displayed;
}

bool MyBOClass::GetIsSphereDisplayed() {
	return isSphereDisplayed;
}
/*----------------------------------------------
Programmer: Alberto Bobadilla (labigm@gmail.com)
Date: 2015/06
----------------------------------------------*/
#ifndef __MyOctant_H_
#define __MyOctant_H_

#include "RE\ReEng.h"
#include "MyBOManager.h"

//System Class
class MyOctant
{
	int m_nChildCount = 0;
	vector3 m_v3Position;
	int tooManyObjects = 0;
	float m_fSize = 0.0f;
	int subCount = 0;
	MeshManagerSingleton* m_pMeshMngr = nullptr;
	MyBOManager* m_pBOMngr = nullptr;

	bool isSubdivide = false;

	bool isDisplayOctrees = false;


	
	
public:
	static bool m_bHead;
	MyOctant* m_pChildren = nullptr;
	/*
	Method: MyOctant
	Usage: Constructor
	Arguments: ---
	Output: class object
	*/
	MyOctant(void);
	MyOctant(float a_fSize);
	/*
	Method: MyOctant
	Usage: Copy Constructor
	Arguments: class object to copy
	Output: class object instance
	*/
	MyOctant(MyOctant const& other);
	/*
	Method: operator=
	Usage: Copy Assignment Operator
	Arguments: class object to copy
	Output: ---
	*/
	MyOctant& operator=(MyOctant const& other);
	/*
	Method: ~MyOctant
	Usage: Destructor
	Arguments: ---
	Output: ---
	*/
	~MyOctant(void);

	/*
	Method: Swap
	Usage: Changes object contents for other object's
	Arguments:
	other -> object to swap content from
	Output: ---
	*/
	void Swap(MyOctant& other);

	void Display(void);

	void Subdivide(int);
	void ReleaseChildren(void);
	bool CheckSub(MyOctant);

	int GetSubCount();

	void MakeChildrenPrime(int); // Copulate
	bool once;
	void SetTooManyObjects(int, int);
	int GetTooManyObjects(int);

	void SetIsDisplayOctrees(bool);
	bool GetIsDisplayOctrees();

private:
	/*
	Method: Release
	Usage: Deallocates member fields
	Arguments: ---
	Output: ---
	*/
	void Release(void);
	/*
	Method: Init
	Usage: Allocates member fields
	Arguments: ---
	Output: ---
	*/
	void Init(void);
};


#endif //__MyOctant_H__
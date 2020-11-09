#include "MyOctant.h"
using namespace Simplex;

// Default Static Variables
uint MyOctant::m_uOctantCount = 0;
uint MyOctant::m_uMaxLevel = 3;


MyOctant::MyOctant(uint a_nMaxLevel, uint a_nIdealEntityCount)
{
	// Allocate Fields
	Init();

	// Create a Cumulative Body of all Objects
	std::vector<vector3> cumulativePos;
	for (uint i = 0; i < m_pEntityMngr->GetEntityCount(); i++)
	{
		MyRigidBody* entityBody = m_pEntityMngr->GetEntity(i)->GetRigidBody();
		cumulativePos.push_back(entityBody->GetMaxGlobal());
		cumulativePos.push_back(entityBody->GetMinGlobal());
	}

	// Calculate Cumulative Variables
	MyRigidBody* cumulativeBody = new MyRigidBody(cumulativePos);
	vector3 cumulativeHalf = cumulativeBody->GetHalfWidth();
	vector3 cumulativeCenter = cumulativeBody->GetCenterLocal();

	// Define Variables
	m_uOctantCount = 0;
	m_uMaxLevel = a_nMaxLevel;
	m_uID = m_uOctantCount;
	m_fSize = cumulativeHalf.x * 2.0f;	// x, y, and z are equal
	m_v3Max = cumulativeCenter + (vector3(cumulativeHalf.x));
	m_v3Min = cumulativeCenter - (vector3(cumulativeHalf.x));

	// Set Connection  
	m_pRoot = this;

	// Interval # of Octants
	m_uOctantCount++;

	// Clean
	cumulativePos.clear();
	SafeDelete(cumulativeBody);

	// Create children
	ConstructTree();
}


MyOctant::MyOctant(vector3 a_v3Center, float a_fSize)
{
	// Allocate Fields
	Init();

	// Define Variables
	m_fSize = a_fSize;
	m_v3Center = a_v3Center;
	m_v3Max = m_v3Center + (vector3(m_fSize) / 2.0f);
	m_v3Min = m_v3Center - (vector3(m_fSize) / 2.0f);

	// Interval # of Octants
	m_uOctantCount++;
}


MyOctant::~MyOctant() 
{ 
	// Clean
	KillBranches();
	m_EntityList.clear();
}


void MyOctant::Init(void)
{
	// Define Variables
	m_uID = m_uOctantCount;
	m_uLevel = 0;
	m_uChildren = 0;
	m_fSize = 0.0f;
	m_v3Center = vector3(0.0f);
	m_v3Max = vector3(0.0f);
	m_v3Min = vector3(0.0f);

	m_pMeshMngr = MeshManager::GetInstance();
	m_pEntityMngr = MyEntityManager::GetInstance();

	// Set Connections
	m_pRoot = nullptr;
	m_pParent = nullptr;
	for (int i = 0; i < 8; i++)
	{
		m_pChild[i] = nullptr;
	}
}


vector3 MyOctant::GetMinGlobal(void)
{ 
	return m_v3Min;
}


vector3 MyOctant::GetMaxGlobal(void)
{ 
	return m_v3Max;
}


uint MyOctant::GetOctantCount(void)
{
	return m_uOctantCount;
}


bool MyOctant::IsLeaf(void)
{
	if (m_uChildren == 0)
		return true;
	else
		return false;
}


void MyOctant::Display(vector3 a_v3Color)
{
	// Call for all Children
	for (int i = 0; i < m_uChildren; i++)
		m_pChild[i]->Display(a_v3Color);
	

	// Display
	matrix4 current = glm::translate(IDENTITY_M4, m_v3Center) * glm::scale(vector3(m_fSize));
	m_pMeshMngr->AddWireCubeToRenderList(current, a_v3Color, RENDER_WIRE);
}


void MyOctant::Display(uint a_nIndex, vector3 a_v3Color)
{
	// Call for all Children
	for (int i = 0; i < m_uChildren; i++)
		m_pChild[i]->Display(a_nIndex, a_v3Color);

	// Display if Index Matches ID
	if (m_uID == a_nIndex)
	{
		// Display
		matrix4 current = glm::translate(IDENTITY_M4, m_v3Center) * glm::scale(vector3(m_fSize));
		m_pMeshMngr->AddWireCubeToRenderList(current, a_v3Color, RENDER_WIRE);
	}
}


void MyOctant::ConstructTree(void)
{
	// Divide into 8 Octants
	Subdivide();

	// Assign Objects to Octants
	AssignIDtoEntity();
}


void MyOctant::Subdivide(void)
{
	// If Leaf and not at Max Level
	if (IsLeaf() && m_uLevel < m_uMaxLevel)
	{
		// Add 8 Children
		m_uChildren = 8;

		// Define Variables
		float halfSize = m_fSize / 2.0f;
		float quarterMove = m_fSize / 4.0f;

		// From center of octant move a quarter of the octant's size in the x, y, z axis to find the centers of the 8 new octants and divide its overall size by 2
		// Far Bottom Left (-, -, -)
		m_pChild[0] = new MyOctant(vector3(m_v3Center.x - quarterMove, m_v3Center.y - quarterMove, m_v3Center.z - quarterMove), halfSize);

		// Far Bottom Right (+, -, -)
		m_pChild[1] = new MyOctant(vector3(m_v3Center.x + quarterMove, m_v3Center.y - quarterMove, m_v3Center.z - quarterMove), halfSize);

		// Far Top Right (+, +, -)
		m_pChild[2] = new MyOctant(vector3(m_v3Center.x + quarterMove, m_v3Center.y + quarterMove, m_v3Center.z - quarterMove), halfSize);

		// Far Top Left (-, +, -)
		m_pChild[3] = new MyOctant(vector3(m_v3Center.x - quarterMove, m_v3Center.y + quarterMove, m_v3Center.z - quarterMove), halfSize);

		// Close Top Left (-, +, +)
		m_pChild[4] = new MyOctant(vector3(m_v3Center.x - quarterMove, m_v3Center.y + quarterMove, m_v3Center.z + quarterMove), halfSize);

		// Close Top Right (+, +, +)
		m_pChild[5] = new MyOctant(vector3(m_v3Center.x + quarterMove, m_v3Center.y + quarterMove, m_v3Center.z + quarterMove), halfSize);

		// Close Bottom Right (+, -, +)
		m_pChild[6] = new MyOctant(vector3(m_v3Center.x + quarterMove, m_v3Center.y - quarterMove, m_v3Center.z + quarterMove), halfSize);

		// Close Bottom Left (-, -, +)
		m_pChild[7] = new MyOctant(vector3(m_v3Center.x - quarterMove, m_v3Center.y - quarterMove, m_v3Center.z + quarterMove), halfSize);


		// Loop through all Children and Connect
		for (int i = 0; i < m_uChildren; i++)
		{
			// Increase Level
			m_pChild[i]->m_uLevel = m_uLevel + 1;

			// Define Connections
			m_pChild[i]->m_pRoot = m_pRoot;
			m_pChild[i]->m_pParent = this;

			// Continue to Divide until Max Level is Reached
			m_pChild[i]->Subdivide();
		}
	}
}


void MyOctant::AssignIDtoEntity(void)
{
	// Call for all Children
	for (int i = 0; i < m_uChildren; i++)
		m_pChild[i]->AssignIDtoEntity();

	// If Leaf
	if (IsLeaf())
	{
		// Loop through all Objects
		for (uint i = 0; i < m_pEntityMngr->GetEntityCount(); i++)
		{
			// If Object is Colliding with Leaf Octant
			if (IsColliding(i))
			{
				// Add to Leaf Octant
				m_EntityList.push_back(i);
				m_pEntityMngr->AddDimension(i, m_uID);
			}
		}
	}
}


bool MyOctant::IsColliding(uint a_uRBIndex)
{
	// Define Index Object Variables
	MyRigidBody* indexBody = m_pEntityMngr->GetEntity(a_uRBIndex)->GetRigidBody();
	vector3 indexMax = indexBody->GetMaxGlobal();
	vector3 indexMin = indexBody->GetMinGlobal();

	// Check and see if indexBody is within the Octant that calls this method
	// Check for X Non Colision
	if (m_v3Max.x < indexMin.x)
		return false;
	if (m_v3Min.x > indexMax.x)
		return false;

	// Check for Y Non Colision
	if (m_v3Max.y < indexMin.y)
		return false;
	if (m_v3Min.y > indexMax.y)
		return false;

	// Check for Z Non Colision
	if (m_v3Max.z < indexMin.z)
		return false;
	if (m_v3Min.z > indexMax.z)
		return false;

	// If passed all checks there is collision
	return true;
}


void MyOctant::KillBranches(void)
{
	// Call for all Children
	for (int i = 0; i < m_uChildren; i++)
	{
		// Clean
		m_pChild[i]->KillBranches();
		delete m_pChild[i];
		m_pChild[i] = nullptr;
	}
	// Remove Children
	m_uChildren = 0;
}
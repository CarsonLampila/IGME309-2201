#include "MyOctant.h"
using namespace Simplex;

// Defaults
uint MyOctant::m_uOctantCount = 0;
uint MyOctant::m_uMaxLevel = 3;


MyOctant::MyOctant(uint a_nMaxLevel, uint a_nIdealEntityCount)
{
	Init();

	// Create a cumulative body of positions
	std::vector<vector3> cumulativePos;
	for (uint i = 0; i < m_pEntityMngr->GetEntityCount(); i++)
	{
		MyRigidBody* entityBody = m_pEntityMngr->GetEntity(i)->GetRigidBody();
		cumulativePos.push_back(entityBody->GetMaxGlobal());
		cumulativePos.push_back(entityBody->GetMinGlobal());
	}

	// Calculate cumulative variables
	MyRigidBody* cumulativeBody = new MyRigidBody(cumulativePos);
	vector3 cumulativeHalf = cumulativeBody->GetHalfWidth();
	vector3 cumulativeCenter = cumulativeBody->GetCenterLocal();

	// Define variables
	m_uOctantCount = 0;
	m_uMaxLevel = a_nMaxLevel;
	m_uID = m_uOctantCount;
	m_fSize = cumulativeHalf.x * 2.0f;	// x, y, and z are equal
	m_v3Max = cumulativeCenter + (vector3(cumulativeHalf.x));
	m_v3Min = cumulativeCenter - (vector3(cumulativeHalf.x));

	// Set Connections
	m_pRoot = this;
	m_lChild.clear();

	// Interval # of Octants
	m_uOctantCount++;

	// Clean
	cumulativePos.clear();
	SafeDelete(cumulativeBody);

	// Create children
	ConstructTree(m_uMaxLevel);
}


MyOctant::MyOctant(vector3 a_v3Center, float a_fSize)
{
	Init();

	// Define variables
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
	m_uChildren = 0;
	m_fSize = 0.0f;
	m_EntityList.clear();
	m_lChild.clear();
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
	// Call for all children
	for (uint i = 0; i < m_uChildren; i++)
	{
		m_pChild[i]->Display(a_v3Color);
	}

	// Display
	matrix4 current = glm::translate(IDENTITY_M4, m_v3Center) * glm::scale(vector3(m_fSize));
	m_pMeshMngr->AddWireCubeToRenderList(current, a_v3Color, RENDER_WIRE);
}


void MyOctant::Display(uint a_nIndex, vector3 a_v3Color)
{
	// Call for all children
	for (uint i = 0; i < m_uChildren; i++)
	{
		m_pChild[i]->Display(a_nIndex, a_v3Color);
	}

	// Display if Index matches ID
	if (m_uID == a_nIndex)
	{
		// Display
		matrix4 current = glm::translate(IDENTITY_M4, m_v3Center) * glm::scale(vector3(m_fSize));
		m_pMeshMngr->AddWireCubeToRenderList(current, a_v3Color, RENDER_WIRE);
	}
}


void MyOctant::ConstructTree(uint a_nMaxLevel)
{
	// Define variables
	m_uMaxLevel = a_nMaxLevel;

	// Divide
	Subdivide();

	// Add IDs to Entities
	AssignIDtoEntity();

	// Construct the corresponding list
	ConstructList();
}


void MyOctant::Subdivide(void)
{
	// If leaf and not at max level
	if (IsLeaf() && m_uLevel < m_uMaxLevel)
	{
		// Add 8 Children
		m_uChildren = 8;

		// Define variables
		float halfSize = m_fSize / 2.0f;
		float quarterMove = m_fSize / 4.0f;

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


		// Loop through all children and connect
		for (uint i = 0; i < m_uChildren; i++)
		{
			// Define Variables
			m_pChild[i]->m_uLevel = m_uLevel + 1;

			// Define Connections
			m_pChild[i]->m_pParent = this;
			m_pChild[i]->m_pRoot = m_pRoot;

			// Divide
			m_pChild[i]->Subdivide();
		}
	}
}


bool MyOctant::IsColliding(uint a_uRBIndex)
{
	// Define index body variables
	MyRigidBody* indexBody = m_pEntityMngr->GetEntity(a_uRBIndex)->GetRigidBody();
	vector3 indexMax = indexBody->GetMaxGlobal();
	vector3 indexMin = indexBody->GetMinGlobal();

	// Check for X
	if (m_v3Max.x < indexMin.x)
		return false;
	if (m_v3Min.x > indexMax.x)
		return false;

	// Check for Y
	if (m_v3Max.y < indexMin.y)
		return false;
	if (m_v3Min.y > indexMax.y)
		return false;

	// Check for Z
	if (m_v3Max.z < indexMin.z)
		return false;
	if (m_v3Min.z > indexMax.z)
		return false;

	return true;
}


void MyOctant::AssignIDtoEntity(void)
{
	// Call for all children
	for (uint i = 0; i < m_uChildren; i++)
	{
		m_pChild[i]->AssignIDtoEntity();
	}

	// If leaf
	if (IsLeaf())
	{
		// Loop through all objects
		for (uint i = 0; i < m_pEntityMngr->GetEntityCount(); i++)
		{
			// If colliding
			if (IsColliding(i))
			{
				m_EntityList.push_back(i);
				m_pEntityMngr->AddDimension(i, m_uID);
			}
		}
	}
}


void MyOctant::KillBranches(void)
{
	// Call for all children
	for (uint i = 0; i < m_uChildren; i++)
	{
		// Clean
		m_pChild[i]->KillBranches();
		delete m_pChild[i];
		m_pChild[i] = nullptr;
	}
	// Remove children
	m_uChildren = 0;
}


void MyOctant::Init(void)
{
	// Define variables
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
	m_pParent = nullptr;
	for (uint i = 0; i < 8; i++)
	{
		m_pChild[i] = nullptr;
	}
	m_pRoot = nullptr;
}


void MyOctant::ConstructList(void)
{
	// Call for all children
	for (uint i = 0; i < m_uChildren; i++)
	{
		m_pChild[i]->ConstructList();
	}

	// Add all children to root
	m_pRoot->m_lChild.push_back(this);
}
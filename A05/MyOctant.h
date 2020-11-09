#ifndef __MYOCTATNTCLASS_H_
#define __MYOCTATNTCLASS_H_

#include "MyEntityManager.h"

namespace Simplex
{
	class MyOctant
	{
		static uint m_uOctantCount;					// Number of octants
		static uint m_uMaxLevel;					// Max level of octants

		uint m_uID = 0;								// ID
		uint m_uLevel = 0;							// Level
		uint m_uChildren = 0;						// Number of children

		float m_fSize = 0.0f;						// Size

		MeshManager* m_pMeshMngr = nullptr;			// Mesh Manager Singleton
		MyEntityManager* m_pEntityMngr = nullptr;	// Entity Manager Singleton

		vector3 m_v3Center = vector3(0.0f);			// Center
		vector3 m_v3Max = vector3(0.0f);			// Max
		vector3 m_v3Min = vector3(0.0f);			// Min

		MyOctant* m_pRoot = nullptr;				// Root
		MyOctant* m_pParent = nullptr;				// Parent
		MyOctant* m_pChild[8];						// Children

		std::vector<uint> m_EntityList;				// List of entities under this octant


	public:
		// Root Constructor
		MyOctant(uint a_nMaxLevel = 2, uint a_nIdealEntityCount = 5);

		// Children Constructor
		MyOctant(vector3 a_v3Center, float a_fSize);

		// Destructor
		~MyOctant(void);

		// Get Octant Min Corner in Global
		vector3 GetMinGlobal(void);

		// Get Octant Max Corner in Global
		vector3 GetMaxGlobal(void);

		// Get Total Num of Octants
		uint GetOctantCount(void);

		// Is Leaf
		bool IsLeaf(void);

		// Displays All Octants in designated color
		void Display(vector3 a_v3Color = C_YELLOW);

		// Displays MyOctant specified by index in designated color
		void Display(uint a_nIndex, vector3 a_v3Color = C_YELLOW);

		// Creates a tree by subdividing the octant and reassigning objects
		void ConstructTree(void);

		// Divide into 8 subdivisions
		void Subdivide(void);

		// Divides objects into each of the leafs
		void AssignIDtoEntity(void);

		// Check for Collision between Octant and Specified Object
		bool IsColliding(uint a_uRBIndex);

		// Deletes all Children at all levels
		void KillBranches(void);


	private:
		// Allocate member fields
		void Init(void);
	};
}
#endif
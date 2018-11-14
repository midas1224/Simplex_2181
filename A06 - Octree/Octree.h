#include "Definitions.h";

#include "MyEntityManager.h"

namespace Simplex {
	class MyOctree {
		static uint m_uOctreeCount;
		static uint m_uMaxLevel;
		static uint m_uIdealEntityCount;

		uint m_uID = 0;
		uint m_uLevel = 0;
		uint m_uChildren = 0;

		float m_fSize = 0.0f;

		MeshManager* m_pMeshMngr = nullptr;
		MyEntityManager* m_pEntityMnger = nullptr;

		vector3 m_v3Center = vector3(0.0f);
		vector3 m_v3Min = vector3(0.0f);
		vector3 m_v3Max = vector3(0.0f);

		MyOctree* m_pParent = nullptr;
		MyOctree* m_pChild[8];

		std::vector<uint> m_EntityList;

		MyOctree* m_pRoot = nullptr;
		std::vector<MyOctree*> m_lChild;

		uint m_uCurrentDepth = 0;

		vector3 leftTopFrontCenter;
		vector3 rightTopFrontCenter;
		vector3 rightBottomFrontCenter;
		vector3 leftBottomFrontCenter;

		vector3 leftTopBackCenter;
		vector3 rightTopBackCenter;
		vector3 rightBottomBackCenter;
		vector3 leftBottomBackCenter;

	public:
		void Init();
		void Swap();
		MyOctree(uint a_nMaxLevel, uint a_nIdealEntityCount);
		MyOctree(vector3 a_v3Center, float a_fSize, MyOctree* root);
		uint GetOctreeCount();
		void ConstructChildren();
	};


}

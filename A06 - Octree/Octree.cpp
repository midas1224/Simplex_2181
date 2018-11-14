#include "Octree.h"
using namespace Simplex;

uint MyOctree::m_uOctreeCount = 0;
uint MyOctree::m_uMaxLevel = 3;
uint MyOctree::m_uIdealEntityCount = 5;
uint MyOctree::GetOctreeCount() { return m_uOctreeCount; }
void MyOctree::Init() {
	m_uChildren = 0;
	m_fSize = 0.0f;

	m_uID = m_uOctreeCount;
	m_uLevel = 0;

	m_uCurrentDepth = 0;

	m_v3Center = vector3(0.0f);
	m_v3Min = vector3(0.0f);
	m_v3Max = vector3(0.0f);

	vector3 leftTopFrontCenter = vector3(0.0f);
	vector3 rightTopFrontCenter = vector3(0.0f);
	vector3 rightBottomFrontCenter = vector3(0.0f);
	vector3 leftBottomFrontCenter = vector3(0.0f);

	vector3 leftTopBackCenter = vector3(0.0f);
	vector3 rightTopBackCenter = vector3(0.0f);
	vector3 rightBottomBackCenter = vector3(0.0f);
	vector3 leftBottomBackCenter = vector3(0.0f);

	m_pMeshMngr = MeshManager::GetInstance();
	m_pEntityMnger = MyEntityManager::GetInstance();

	m_pRoot = nullptr;
	m_pParent = nullptr;
	for (uint i = 0; i < 8; i++) {
		m_pChild[i] = nullptr;
	}
}
void MyOctree::ConstructChildren() {

	m_pChild[0] = new MyOctree(leftTopFrontCenter, m_fSize/2.0f, this);
	m_pChild[1] = new MyOctree(rightTopFrontCenter, m_fSize/2.0f, this);
	m_pChild[2] = new MyOctree(rightBottomFrontCenter, m_fSize / 2.0f, this);
	m_pChild[3] = new MyOctree(leftBottomFrontCenter, m_fSize/2.0f, this);
	m_pChild[4] = new MyOctree(leftTopBackCenter, m_fSize/2.0f, this);
	m_pChild[5] = new MyOctree(rightTopBackCenter, m_fSize/2.0f, this);
	m_pChild[6] = new MyOctree(rightBottomBackCenter, m_fSize/2.0f, this);
	m_pChild[7] = new MyOctree(leftBottomBackCenter, m_fSize/2.0f, this);

	if (m_uCurrentDepth <= 2) {
		return;
	}



	ConstructChildren();
}
MyOctree::MyOctree(uint a_nMaxLevel, uint a_nIdealEntityCount) {
	Init();

	m_uOctreeCount = 0;
	m_uMaxLevel = a_nMaxLevel;
	m_uIdealEntityCount = a_nIdealEntityCount;
	m_uID = m_uOctreeCount;

	m_pRoot = this;
	m_lChild.clear();

	std::vector<vector3> lMinMax;

	uint nObjects = m_pEntityMnger->GetEntityCount();
	for (uint i = 0; i < nObjects; i++) {
		MyEntity* pEntity = m_pEntityMnger->GetEntity(i);
		MyRigidBody* pRigidBody = pEntity->GetRigidBody();
		lMinMax.push_back(pRigidBody->GetMinGlobal());
		lMinMax.push_back(pRigidBody->GetMaxGlobal());
	}
	MyRigidBody* pRigidBody = new MyRigidBody(lMinMax);

	vector3 vHalfWidth = pRigidBody->GetHalfWidth();
	float fMax = vHalfWidth.x;
	for (int i = 1; i < 3; i++) {
		if (fMax < vHalfWidth[i])
			fMax = vHalfWidth[i];
	}
	vector3 v3Center = pRigidBody->GetCenterLocal();
	lMinMax.clear();
	SafeDelete(pRigidBody);

	m_fSize = fMax * 2.0f;
	m_v3Center = v3Center;
	m_v3Min = m_v3Center - vector3(fMax);
	m_v3Max = m_v3Center + vector3(fMax);

	m_uOctreeCount++;

	ConstructChildren();
}
MyOctree::MyOctree(vector3 a_v3Center, uint a_uSize, MyOctree* parent) {

	Init();

	m_v3Center = a_v3Center;
	m_v3Max = a_v3Center + vector3(a_uSize / 2.0f);
	m_v3Min = a_v3Center - vector3(a_uSize / 2.0f);
	m_fSize = a_uSize;

	float left = -a_uSize / 4.0f;
	float right = a_uSize / 4.0f;
	float top = a_uSize / 4.0f;
	float bottom = -a_uSize / 4.0f;
	float back = a_uSize / 4.0f;
	float front = -a_uSize / 4.0f;


	vector3 leftTopFrontCenter = m_v3Center + vector3(left, top, front);
	vector3 rightTopFrontCenter = m_v3Center + vector3(right, top, front);
	vector3 rightBottomFrontCenter = m_v3Center + vector3(right, bottom, front);
	vector3 leftBottomFrontCenter = m_v3Center + vector3(left, bottom, front);

	vector3 leftTopBackCenter = m_v3Center + vector3(left, top, back);
	vector3 rightTopBackCenter = m_v3Center + vector3(right, top, back);
	vector3 rightBottomBackCenter = m_v3Center + vector3(right, bottom, back);
	vector3 leftBottomBackCenter = m_v3Center + vector3(left, bottom, back);

	ConstructChildren();
}

#ifndef CollisionMgr_h__
#define CollisionMgr_h__

#include "Engine_Include.h"
#include "Base.h"

#include "GameObject.h"

#include "CubeCollider.h"
#include "SphereCollider.h"

BEGIN(Engine)

class ENGINE_DLL CCollisionMgr : public CBase
{
	DECLARE_SINGLETON(CCollisionMgr)

private:
	typedef struct tagOBB
	{
		_vec3		vPoint[8];
		_vec3		vCenter;
		_vec3		vProjAxis[3];   // 중점에서 콜라이더의 세 평면으로 뻗어나가는 벡터
		_vec3		vAxis[3];		// 임의의 축 벡터
	}OBB;

private:
	/*  Structor  */
	explicit CCollisionMgr();
	~CCollisionMgr(void);
	
public:
	/*  General  */
	HRESULT						AddObject_CollisionMgr(DWORD _dwFlag, CGameObject* _pObject);
	void						LateUpdate_CollisionMgr(const _float& fTimeDelta);

public:
	_bool						Collision_AABB(const _vec3* pDestMin, const _vec3* pDestMax, const _matrix* pDestWorld,
												const _vec3* pSourMin, const _vec3* pSourMax, const _matrix* pSourWorld);
	_bool						Collision_OBB(const _vec3* pDestMin, const _vec3* pDestMax, const _matrix* pDestWorld,
												const _vec3* pSourMin, const _vec3* pSourMax, const _matrix* pSourWorld);
	_bool						Collision_Sphere(const _float fDestRadius, const _matrix* pDestWorld,
												const _float fSourRadius, const _matrix* pSourWorld);

	void						Set_Point(OBB* pObb, const _vec3* pMin, const _vec3* pMax);
	void						Set_Axis(OBB* pObb);

public:
	void						Reset_ObjectList(void);

private:
	map<COLLIDERFLAG, list<CGameObject*>>		m_mapObjectList;
	_float										m_fColRadius;


	/*  Creation and destruction  */
public:
	virtual void				Free(void);
};

END

#endif // CollisionMgr_h__

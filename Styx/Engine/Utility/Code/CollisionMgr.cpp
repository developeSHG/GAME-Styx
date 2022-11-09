#include "CollisionMgr.h"

IMPLEMENT_SINGLETON(Engine::CCollisionMgr)

USING(Engine)
#include <iostream>

Engine::CCollisionMgr::CCollisionMgr()
{

}

Engine::CCollisionMgr::~CCollisionMgr(void)
{
	Free();
}

HRESULT Engine::CCollisionMgr::AddObject_CollisionMgr(DWORD _dwFlag, CGameObject* _pObject)
{
	NULL_CHECK_RETURN(_pObject, E_FAIL);

	if (0 == m_mapObjectList.size())
	{
		for (_ulong i = 0; i < COLLIDERFLAG::COLLIDER_FLAG_END; ++i)
		{
			m_mapObjectList.emplace(COLLIDERFLAG(i), list<Engine::CGameObject*>());
		}
	}

	if (_dwFlag & COL_FLAG_AABB_DEST)
		m_mapObjectList[COLLIDERFLAG::COL_AABB_DEST].push_back(_pObject);
	if (_dwFlag & COL_FLAG_AABB_SOUR)
		m_mapObjectList[COLLIDERFLAG::COL_AABB_SOUR].push_back(_pObject);
	if (_dwFlag & COL_FLAG_OBB_DEST)
		m_mapObjectList[COLLIDERFLAG::COL_OBB_DEST].push_back(_pObject);
	if (_dwFlag & COL_FLAG_OBB_SOUR)
		m_mapObjectList[COLLIDERFLAG::COL_OBB_SOUR].push_back(_pObject);
	if (_dwFlag & COL_FLAG_SPHERE_DEST)
		m_mapObjectList[COLLIDERFLAG::COL_SPHERE_DEST].push_back(_pObject);
	if (_dwFlag & COL_FLAG_SPHERE_SOUR)
		m_mapObjectList[COLLIDERFLAG::COL_SPHERE_SOUR].push_back(_pObject);

	return S_OK;
}

void Engine::CCollisionMgr::LateUpdate_CollisionMgr(const _float& fTimeDelta)
{
	/*  AABB Test  */
	for (auto& pDestObject : m_mapObjectList[COLLIDERFLAG::COL_AABB_DEST])
	{
		for (auto& pSourObject : m_mapObjectList[COLLIDERFLAG::COL_AABB_SOUR])
		{
			if(pSourObject == pDestObject
				|| pSourObject->Get_OwnerObjectID() == pDestObject->Get_OwnerObjectID())
				continue;

			CCubeCollider* pDestCollider = SCAST_CUBECOLLIDER(pDestObject->Get_Component(COM_AABB_COLLIDER));
			CCubeCollider* pSourCollider = SCAST_CUBECOLLIDER(pSourObject->Get_Component(COM_AABB_COLLIDER));

			//if (COLLIDERTYPE::COL_CUBE != pDestCollider->Get_ColliderType()
			//	|| COLLIDERTYPE::COL_CUBE != pSourCollider->Get_ColliderType())
			//	continue;

			/*  Is Player  */
			if (COL_OBJECTID::COL_OBJECT_PLAYER == pDestObject->Get_ColObjectID())
			{
				if (Collision_AABB(pDestCollider->Get_Min(), pDestCollider->Get_Max(), pDestCollider->Get_ColliderWorld(),
									pSourCollider->Get_Min(), pSourCollider->Get_Max(), pSourCollider->Get_ColliderWorld()))
				{
					pDestObject->CollisionUpdate_Object(fTimeDelta, COL_FLAG_AABB_DEST, pSourObject->Get_ColObjectID(), 0.f, pSourObject);
					pSourObject->CollisionUpdate_Object(fTimeDelta, COL_FLAG_AABB_SOUR, pDestObject->Get_ColObjectID(), 0.f, pDestObject);
				}
				else
				{
					//pDestCollider->Set_IsCollide(COLLTYPE::COL_FALSE);
					//pSourCollider->Set_IsCollide(COLLTYPE::COL_FALSE);
				}
			}
			else
			{
				//if (Collision_AABB(pDestCollider->Get_Min(), pDestCollider->Get_Max(), pDestCollider->Get_ColliderWorld(),
				//	pSourCollider->Get_Min(), pSourCollider->Get_Max(), pSourCollider->Get_ColliderWorld()))
				//{
				//	pDestObject->CollisionUpdate_Object(fTimeDelta, COL_FLAG_AABB_DEST, pSourObject->Get_ColObjectID(), 0.f, pSourObject);
				//	pSourObject->CollisionUpdate_Object(fTimeDelta, COL_FLAG_AABB_SOUR, pDestObject->Get_ColObjectID(), 0.f, pDestObject);
				//}
				//else
				//{
				//	//pDestCollider->Set_IsCollide(COLLTYPE::COL_FALSE);
				//	//pSourCollider->Set_IsCollide(COLLTYPE::COL_FALSE);
				//}
			}
		}
	}

	/*  OBB Test  */
	for (auto& pDestObject : m_mapObjectList[COLLIDERFLAG::COL_OBB_DEST])
	{
		for (auto& pSourObject : m_mapObjectList[COLLIDERFLAG::COL_OBB_SOUR])
		{
			if (pSourObject == pDestObject
				|| pSourObject->Get_OwnerObjectID() == pDestObject->Get_OwnerObjectID())
				continue;

			CCubeCollider* pDestCollider = SCAST_CUBECOLLIDER(pDestObject->Get_Component(COM_OBB_COLLIDER));
			CCubeCollider* pSourCollider = SCAST_CUBECOLLIDER(pSourObject->Get_Component(COM_OBB_COLLIDER));

			//if (COLLIDERTYPE::COL_CUBE != pDestCollider->Get_ColliderType()
			//	|| COLLIDERTYPE::COL_CUBE != pSourCollider->Get_ColliderType())
			//	continue;

			if (Collision_OBB(pDestCollider->Get_Min(), pDestCollider->Get_Max(), pDestCollider->Get_ColliderWorld(),
								pSourCollider->Get_Min(), pSourCollider->Get_Max(), pSourCollider->Get_ColliderWorld()))
			{
				pDestObject->CollisionUpdate_Object(fTimeDelta, COL_FLAG_OBB_DEST, pSourObject->Get_ColObjectID(), 0.f, pSourObject);
				pSourObject->CollisionUpdate_Object(fTimeDelta, COL_FLAG_OBB_SOUR, pDestObject->Get_ColObjectID(), 0.f, pDestObject);
			}
			else
			{
				//pDestCollider->Set_IsCollide(COLLTYPE::COL_FALSE);
				//pSourCollider->Set_IsCollide(COLLTYPE::COL_FALSE);
			}
		}
	}
	
	/*  Sphere Test  */
	for (auto& pDestObject : m_mapObjectList[COLLIDERFLAG::COL_SPHERE_DEST])
	{
		for (auto& pSourObject : m_mapObjectList[COLLIDERFLAG::COL_SPHERE_SOUR])
		{
			if (pSourObject == pDestObject
				|| pSourObject->Get_OwnerObjectID() == pDestObject->Get_OwnerObjectID())
				continue;

			//if (Engine::COL_OBJECTID::COL_OBJECT_PLAYER == pDestObject->Get_ColObjectID())
			//{
			//	/*  충돌처리 안해도 되는 것일때 return  */
			//	return;
			//}

			CSphereCollider* pDestCollider = SCAST_SPHERECOLLIDER(pDestObject->Get_Component(COM_SPHERE_COLLIDER));
			CSphereCollider* pSourCollider = SCAST_SPHERECOLLIDER(pSourObject->Get_Component(COM_SPHERE_COLLIDER));

			//if (COLLIDERTYPE::COL_SPHERE != pDestCollider->Get_ColliderType()
			//	|| COLLIDERTYPE::COL_SPHERE != pSourCollider->Get_ColliderType())
			//	continue;

			if (Collision_Sphere(pDestCollider->Get_Radius(), pDestCollider->Get_ColliderWorld(),
									pSourCollider->Get_Radius(), pSourCollider->Get_ColliderWorld()))
			{
				pDestObject->CollisionUpdate_Object(fTimeDelta, COL_FLAG_SPHERE_DEST, pSourObject->Get_ColObjectID(), m_fColRadius, pSourObject);
				pSourObject->CollisionUpdate_Object(fTimeDelta, COL_FLAG_SPHERE_SOUR, pDestObject->Get_ColObjectID(), m_fColRadius, pDestObject);
			}
			else
			{
				//pDestCollider->Set_IsCollide(COLLTYPE::COL_FALSE);
				//pSourCollider->Set_IsCollide(COLLTYPE::COL_FALSE);
			}
		}
	}

	Reset_ObjectList();
}

Engine::_bool Engine::CCollisionMgr::Collision_AABB(const _vec3 * pDestMin, const _vec3 * pDestMax, const _matrix * pDestWorld,
													const _vec3 * pSourMin, const _vec3 * pSourMax, const _matrix * pSourWorld)
{
	_vec3		vDestMin, vDestMax, vSourMin, vSourMax;
	_float		fMin, fMax;

	D3DXVec3TransformCoord(&vDestMin, pDestMin, pDestWorld);
	D3DXVec3TransformCoord(&vDestMax, pDestMax, pDestWorld);

	D3DXVec3TransformCoord(&vSourMin, pSourMin, pSourWorld);
	D3DXVec3TransformCoord(&vSourMax, pSourMax, pSourWorld);

	/*  x축에서 바라봤을 때  */
	fMin = max(vDestMin.x, vSourMin.x);
	fMax = min(vDestMax.x, vSourMax.x);

	if (fMax < fMin)
		return FALSE;

	/*  y축에서 바라봤을 때  */
	fMin = max(vDestMin.y, vSourMin.y);
	fMax = min(vDestMax.y, vSourMax.y);

	if (fMax < fMin)
		return FALSE;

	/*  z축에서 바라봤을 때  */
	fMin = max(vDestMin.z, vSourMin.z);
	fMax = min(vDestMax.z, vSourMax.z);

	if (fMax < fMin)
		return FALSE;

	return TRUE;
}

Engine::_bool Engine::CCollisionMgr::Collision_OBB(const _vec3 * pDestMin, const _vec3 * pDestMax, const _matrix * pDestWorld, 
													const _vec3 * pSourMin, const _vec3 * pSourMax, const _matrix * pSourWorld)
{
	OBB		tObb[2];
	ZeroMemory(&tObb, sizeof(OBB) * 2);

	Set_Point(&tObb[0], pDestMin, pDestMax);
	Set_Point(&tObb[1], pSourMin, pSourMax);

	for (_uint i = 0; i < 8; ++i)
	{
		D3DXVec3TransformCoord(&tObb[0].vPoint[i], &tObb[0].vPoint[i], pDestWorld);
		D3DXVec3TransformCoord(&tObb[1].vPoint[i], &tObb[1].vPoint[i], pSourWorld);
	}

	D3DXVec3TransformCoord(&tObb[0].vCenter, &tObb[0].vCenter, pDestWorld);
	D3DXVec3TransformCoord(&tObb[1].vCenter, &tObb[1].vCenter, pSourWorld);

	for (_uint i = 0; i < 2; ++i)
		Set_Axis(&tObb[i]);

	_float	fDistance[3] = {};

	for (_uint i = 0; i < 2; ++i)
	{
		for (_uint j = 0; j < 3; ++j)
		{
			/*  첫 번째 obb가 임의의 축과 각 면을 향하는 벡터들의 투영한 길이의 합  */
			fDistance[0] = fabs(D3DXVec3Dot(&tObb[0].vProjAxis[0], &tObb[i].vAxis[j])) +
							fabs(D3DXVec3Dot(&tObb[0].vProjAxis[1], &tObb[i].vAxis[j])) +
							fabs(D3DXVec3Dot(&tObb[0].vProjAxis[2], &tObb[i].vAxis[j]));

			/*  두 번째 obb가 임의의 축과 각 면을 향하는 벡터들의 투영한 길이의 합  */
			fDistance[1] = fabs(D3DXVec3Dot(&tObb[1].vProjAxis[0], &tObb[i].vAxis[j])) +
							fabs(D3DXVec3Dot(&tObb[1].vProjAxis[1], &tObb[i].vAxis[j])) +
							fabs(D3DXVec3Dot(&tObb[1].vProjAxis[2], &tObb[i].vAxis[j]));

			/*  실제거리  */
			fDistance[2] = fabs(D3DXVec3Dot(&(tObb[1].vCenter - tObb[0].vCenter), &tObb[i].vAxis[j]));

			/*  실제거리가 최단거리보다 크다면 안된다  */
			if (fDistance[2] > fDistance[0] + fDistance[1])
				return FALSE;
		}
	}

	return TRUE;
}

_bool Engine::CCollisionMgr::Collision_Sphere(const _float fDestRadius, const _matrix* pDestWorld,
												const _float fSourRadius, const _matrix* pSourWorld)
{

	_vec3 vDist = (_vec3)(&pDestWorld->m[3][0]) - (_vec3)(&pSourWorld->m[3][0]);

	_float fRadiusSum = fDestRadius + fSourRadius;

	if (fRadiusSum > D3DXVec3Length(&vDist))
	{
		m_fColRadius = fRadiusSum - D3DXVec3Length(&vDist);
		return TRUE;
	}

	return FALSE;
}

void Engine::CCollisionMgr::Set_Point(OBB* pObb, const _vec3* pMin, const _vec3* pMax)
{
	pObb->vPoint[1] = _vec3(pMax->x, pMax->y, pMin->z);
	pObb->vPoint[2] = _vec3(pMax->x, pMin->y, pMin->z);
	pObb->vPoint[3] = _vec3(pMin->x, pMin->y, pMin->z);

	pObb->vPoint[4] = _vec3(pMin->x, pMax->y, pMax->z);
	pObb->vPoint[5] = _vec3(pMax->x, pMax->y, pMax->z);
	pObb->vPoint[6] = _vec3(pMax->x, pMin->y, pMax->z);
	pObb->vPoint[7] = _vec3(pMin->x, pMin->y, pMax->z);

	pObb->vCenter = (*pMin + *pMax) * 0.5f;
}

void Engine::CCollisionMgr::Set_Axis(OBB* pObb)
{
	/*  중점에서 플레이어의 세 평면으로 뻗어나는 벡터  */
	pObb->vProjAxis[0] = (pObb->vPoint[2] + pObb->vPoint[5]) * 0.5f - pObb->vCenter;
	pObb->vProjAxis[1] = (pObb->vPoint[0] + pObb->vPoint[5]) * 0.5f - pObb->vCenter;
	pObb->vProjAxis[2] = (pObb->vPoint[7] + pObb->vPoint[5]) * 0.5f - pObb->vCenter;

	/*  임의의 기준 축 벡터 ( 회전을 할 수도 있기 떄문에 자신에 맞는 새로운 좌표계를 만든다.)  */
	pObb->vAxis[0] = pObb->vPoint[2] - pObb->vPoint[3];
	pObb->vAxis[1] = pObb->vPoint[0] - pObb->vPoint[3];
	pObb->vAxis[2] = pObb->vPoint[7] - pObb->vPoint[3];

	for (_uint i = 0; i < 3; ++i)
		D3DXVec3Normalize(&pObb->vAxis[i], &pObb->vAxis[i]);
}

void Engine::CCollisionMgr::Reset_ObjectList(void)
{
	for (auto& iter : m_mapObjectList)
	{
		iter.second.clear();
	}
}

void Engine::CCollisionMgr::Free(void)
{
	Reset_ObjectList();
}
#include "Frustum.h"

USING(Engine)

Engine::CFrustum::CFrustum(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
{
	m_pGraphicDev->AddRef();
}

#define FRUSTUM_PLANE_END 0

Engine::CFrustum::~CFrustum(void)
{

	//D3DXPLANE	tPlane[FRUSTUM_PLANE_END];
	//if (FAILED(Make_Plane(vFrustum_Pos, tPlane)))
	//{
	//	MSG_BOX("CFrustum::Make_Plane()");
	//	return false;
	//}

	//_float fDistance = 0.f;

	//for (size_t iIndex = 0; iIndex < FRUSTUM_PLANE_END; ++iIndex)
	//{
	//	fDistance = D3DXPlaneDotCoord(&tPlane[iIndex], &vObjPos_World);

	//	if (fRadius < fDistance)
	//		return false;
	//}



}

HRESULT Engine::CFrustum::Ready_Frustum(void)
{
	m_vPoint[0] = _vec3(-1.f, 1.f, 0.f);
	m_vPoint[1] = _vec3(1.f, 1.f, 0.f);
	m_vPoint[2] = _vec3(1.f, -1.f, 0.f);
	m_vPoint[3] = _vec3(-1.f, -1.f, 0.f);

	m_vPoint[4] = _vec3(-1.f, 1.f, 1.f);
	m_vPoint[5] = _vec3(1.f, 1.f, 1.f);
	m_vPoint[6] = _vec3(1.f, -1.f, 1.f);
	m_vPoint[7] = _vec3(-1.f, -1.f, 1.f);

	return S_OK;
}

_bool Engine::CFrustum::Is_InFrustum(const _vec3* pPos, const _float fRadius /*= 0.f*/, _uint* uDir /*= nullptr*/)
{
	_float	fDistance = 0.f;

	for (_uint i = 0; i < 6; ++i)
	{
		fDistance = D3DXPlaneDotCoord(&m_Plane[i], pPos);

		if (fDistance > fRadius)
		{
			if(nullptr != uDir)
				*uDir = i;
			return false;
		}
	}

	return true;
}

_bool Engine::CFrustum::Is_InFrustum_ForObject(const _vec3* pPos, const _float fRadius /*= 0.f*/, _uint* uDir /*= nullptr*/)
{
	Ready_Frustum();

	_matrix		matProj;
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, NULL, &matProj);

	_matrix		matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, NULL, &matView);

	for (_uint i = 0; i < 8; ++i)
	{
		D3DXVec3TransformCoord(&m_vPoint[i], &m_vPoint[i], &matProj);
		D3DXVec3TransformCoord(&m_vPoint[i], &m_vPoint[i], &matView);
	}

	// x+
	D3DXPlaneFromPoints(&m_Plane[0], &m_vPoint[1], &m_vPoint[5], &m_vPoint[6]);

	// x-
	D3DXPlaneFromPoints(&m_Plane[1], &m_vPoint[4], &m_vPoint[0], &m_vPoint[3]);

	// y+
	D3DXPlaneFromPoints(&m_Plane[2], &m_vPoint[4], &m_vPoint[5], &m_vPoint[1]);

	// y-
	D3DXPlaneFromPoints(&m_Plane[3], &m_vPoint[3], &m_vPoint[2], &m_vPoint[6]);

	// z+
	D3DXPlaneFromPoints(&m_Plane[4], &m_vPoint[7], &m_vPoint[6], &m_vPoint[5]);

	// z-
	D3DXPlaneFromPoints(&m_Plane[5], &m_vPoint[0], &m_vPoint[1], &m_vPoint[2]);

	return Is_InFrustum(pPos, fRadius, uDir);
}

void Engine::CFrustum::Is_InFrustum_ForTerrain(const _vec3* pVtxPos, const _ulong& dwCntX, const _ulong& dwCntZ, Engine::INDEX32* pIndex, _ulong* pTriCnt, CQuadTree* pQuadTree)
{
	Ready_Frustum();

	_matrix		matProj;
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, NULL, &matProj);

	_matrix		matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, NULL, &matView);

	for (_uint i = 0; i < 8; ++i)
	{
		D3DXVec3TransformCoord(&m_vPoint[i], &m_vPoint[i], &matProj);
		D3DXVec3TransformCoord(&m_vPoint[i], &m_vPoint[i], &matView);
	}

	// x+
	D3DXPlaneFromPoints(&m_Plane[0], &m_vPoint[1], &m_vPoint[5], &m_vPoint[6]);

	// x-
	D3DXPlaneFromPoints(&m_Plane[1], &m_vPoint[4], &m_vPoint[0], &m_vPoint[3]);

	// y+
	D3DXPlaneFromPoints(&m_Plane[2], &m_vPoint[4], &m_vPoint[5], &m_vPoint[1]);

	// y-
	D3DXPlaneFromPoints(&m_Plane[3], &m_vPoint[3], &m_vPoint[2], &m_vPoint[6]);

	// z+
	D3DXPlaneFromPoints(&m_Plane[4], &m_vPoint[7], &m_vPoint[6], &m_vPoint[5]);

	// z-
	D3DXPlaneFromPoints(&m_Plane[5], &m_vPoint[0], &m_vPoint[1], &m_vPoint[2]);

	*pTriCnt = 0;

	if (nullptr != pQuadTree)
	{

	}
	else
	{
		_bool	bIsIn[3] = { false };
		_ulong	dwTriCnt = 0;

		for (_ulong i = 0; i < dwCntZ - 1; ++i)
		{
			for (_ulong j = 0; j < dwCntX - 1; ++j)
			{
				_ulong		dwIndex = i * dwCntX + j;

				/*_vec3 vPos1, vPos2, vPos3, vPos4;
				D3DXVec3TransformCoord(&vPos1, &pVtxPos[dwIndex + dwCntX], &matWorld);
				D3DXVec3TransformCoord(&vPos2, &pVtxPos[dwIndex + dwCntX + 1], &matWorld);
				D3DXVec3TransformCoord(&vPos3, &pVtxPos[dwIndex + 1], &matWorld);
				D3DXVec3TransformCoord(&vPos4, &pVtxPos[dwIndex], &matWorld);*/

			/*	_vec3 vPos1 = _vec3(pVtxPos[dwIndex + dwCntX].x / -2.f, 0.f, pVtxPos[dwIndex + dwCntX].z / -2.9f);
				_vec3 vPos2 = _vec3(pVtxPos[dwIndex + dwCntX + 1].x / -2.f, 0.f, pVtxPos[dwIndex + dwCntX + 1].z / -2.9f);
				_vec3 vPos3 = _vec3(pVtxPos[dwIndex + 1].x / -2.f, 0.f, pVtxPos[dwIndex + 1].z / -2.9f);
				_vec3 vPos4 = _vec3(pVtxPos[dwIndex].x / -2.f, 0.f, pVtxPos[dwIndex].z / -2.9f);*/

				// ?????? ??
				bIsIn[0] = Is_InFrustum(&pVtxPos[dwIndex + dwCntX]);
				bIsIn[1] = Is_InFrustum(&pVtxPos[dwIndex + dwCntX + 1]);
				bIsIn[2] = Is_InFrustum(&pVtxPos[dwIndex + 1]);

				if (true == bIsIn[0] || true == bIsIn[1] || true == bIsIn[2])
				{
					pIndex[dwTriCnt]._0 = dwIndex + dwCntX;
					pIndex[dwTriCnt]._1 = dwIndex + dwCntX + 1;
					pIndex[dwTriCnt]._2 = dwIndex + 1;
					++dwTriCnt;
				}

				// ???? ????
				bIsIn[0] = Is_InFrustum(&pVtxPos[dwIndex + dwCntX]);
				bIsIn[1] = Is_InFrustum(&pVtxPos[dwIndex + 1]);
				bIsIn[2] = Is_InFrustum(&pVtxPos[dwIndex]);

				if (true == bIsIn[0] || true == bIsIn[1] || true == bIsIn[2])
				{
					pIndex[dwTriCnt]._0 = dwIndex + dwCntX;
					pIndex[dwTriCnt]._1 = dwIndex + 1;
					pIndex[dwTriCnt]._2 = dwIndex;
					++dwTriCnt;
				}

				// ???? ????
		/*		bIsIn[0] = Is_InFrustum(&vPos1);
				bIsIn[1] = Is_InFrustum(&vPos3);
				bIsIn[2] = Is_InFrustum(&vPos4);

				if (true == bIsIn[0] && true == bIsIn[1] && true == bIsIn[2])
				{
					pIndex[dwTriCnt]._0 = dwIndex + dwCntX;
					pIndex[dwTriCnt]._1 = dwIndex + 1;
					pIndex[dwTriCnt]._2 = dwIndex;
					++dwTriCnt;
				}*/
			}
		}

		*pTriCnt = dwTriCnt;
	}
}

CFrustum* Engine::CFrustum::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CFrustum*	pInstance = new CFrustum(pGraphicDev);

	if (FAILED(pInstance->Ready_Frustum()))
		Safe_Release(pInstance);

	return pInstance;
}

void Engine::CFrustum::Free(void)
{
	Safe_Release(m_pGraphicDev);
}

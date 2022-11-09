#include "NaviMesh.h"

USING(Engine)

#include <iostream>
using namespace std;

Engine::CNaviMesh::CNaviMesh(LPDIRECT3DDEVICE9 pGraphicDev)
	: CMesh(pGraphicDev)
	, m_dwIndex(0)
{

}

Engine::CNaviMesh::CNaviMesh(const CNaviMesh& rhs)
	: CMesh(rhs)
	, m_dwIndex(rhs.m_dwIndex)
	, m_vecCell(rhs.m_vecCell)
	, m_vecInsideCell(rhs.m_vecInsideCell)
	, m_vecCliffCell(rhs.m_vecCliffCell)
{
	for (_ulong i = 0; i < m_vecCell.size(); ++i)
		m_vecCell[i]->AddRef();
}

Engine::CNaviMesh::~CNaviMesh(void)
{

}

HRESULT Engine::CNaviMesh::Ready_NaviMesh(void)
{
	//m_vecCell.reserve(4);

	//CCell*		pCell = nullptr;

	// 
	//pCell = CCell::Create(m_pGraphicDev,
	//	m_vecCell.size(),
	//	&_vec3(0.f, 0.f, 2.f),
	//	&_vec3(2.f, 0.f, 0.f),
	//	&_vec3(0.f, 0.f, 0.f));
	//NULL_CHECK_RETURN(pCell, E_FAIL);
	//m_vecCell.push_back(pCell);

	//// 1
	//pCell = CCell::Create(m_pGraphicDev,
	//	m_vecCell.size(),
	//	&_vec3(0.f, 0.f, 2.f),
	//	&_vec3(2.f, 0.f, 2.f),
	//	&_vec3(2.f, 0.f, 0.f));
	//NULL_CHECK_RETURN(pCell, E_FAIL);
	//m_vecCell.push_back(pCell);

	//// 2
	//pCell = CCell::Create(m_pGraphicDev,
	//	m_vecCell.size(),
	//	&_vec3(0.f, 0.f, 4.f),
	//	&_vec3(2.f, 0.f, 2.f),
	//	&_vec3(0.f, 0.f, 2.f));
	//NULL_CHECK_RETURN(pCell, E_FAIL);
	//m_vecCell.push_back(pCell);

	//// 3
	//pCell = CCell::Create(m_pGraphicDev,
	//	m_vecCell.size(),
	//	&_vec3(2.f, 0.f, 2.f),
	//	&_vec3(4.f, 0.f, 0.f),
	//	&_vec3(2.f, 0.f, 0.f));
	//NULL_CHECK_RETURN(pCell, E_FAIL);
	//m_vecCell.push_back(pCell);

	//FAILED_CHECK_RETURN(Link_Cell(), E_FAIL);

	HANDLE hFile = CreateFile(L"../../Data/NaviMesh.dat", GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(NULL, TEXT("Load Failed"), L"System Message", MB_OK);
		return E_FAIL;
	}

	DWORD dwByte = 0;

	NAVICELL tNaviCell;

	CCell*		pCell = nullptr;

	while (TRUE)
	{
		ReadFile(hFile, &tNaviCell, sizeof(NAVICELL), &dwByte, nullptr);

		if (0 == dwByte)
			break;

		pCell = CCell::Create(m_pGraphicDev,
			m_vecCell.size(),
			&tNaviCell.vPointA,
			&tNaviCell.vPointB,
			&tNaviCell.vPointC);

		NULL_CHECK_RETURN(pCell, E_FAIL);

		pCell->m_eNaviType = tNaviCell.eNaviType;
		if (Engine::NAVI_TYPE::NAVI_INSIDE == tNaviCell.eNaviType)
			m_vecInsideCell.push_back(pCell);
		else
			m_vecCell.push_back(pCell);

		if (Engine::NAVI_TYPE::NAVI_CLIFF == tNaviCell.eNaviType)
		{
			m_vecCliffCell.push_back(pCell);
		}
	}

	FAILED_CHECK_RETURN(Link_Cell(), E_FAIL);

	CloseHandle(hFile);



	return S_OK;
}

void CNaviMesh::Render_NaviMesh(void)
{
	for (auto& iter : m_vecCell)
		iter->Render_Cell();

	for (auto& iter : m_vecInsideCell)
		iter->Render_Cell();
}

_bool Engine::CNaviMesh::Update_CurIndex(const _vec3& _vEndPos,
											const _vec3& _vPlusPos /*= _vec3(0.f, 0.f, 0.f)*/)
{
	_vec3 vPos = _vEndPos + _vPlusPos;
	vPos.y += 5.f;
	_vec3 vRayDir = _vec3(0.f, -1.f, 0.f);

	for (auto& iter : m_vecCell)
	{
		_float fU, fV, fDist;
		if (TRUE == D3DXIntersectTri(iter->Get_Point(CCell::POINT_A),
									iter->Get_Point(CCell::POINT_B),
									iter->Get_Point(CCell::POINT_C),
									&vPos,
									&vRayDir,
									&fU, &fV, &fDist))
		{
			m_dwIndex = *iter->Get_Index();
			return TRUE;
		}
	}
	return FALSE;
}

Engine::CLine::COMPARE Engine::CNaviMesh::Compare_Line(_vec3 vecStartPos, _vec3 vecEndPos, _vec3 vecPlayerPos)
{
	_vec2 m_vDirection = vecEndPos - vecStartPos;
	_vec2 m_vNormal = _vec2(m_vDirection.y * -1.f, m_vDirection.x);
	D3DXVec2Normalize(&m_vNormal, &m_vNormal);

	_vec2		vDest = vecPlayerPos - vecStartPos;

	_float		fResult = D3DXVec2Dot(D3DXVec2Normalize(&vDest, &vDest), &m_vNormal);

	if (0.f <= fResult)
		return CLine::COMPARE_LEFT;
	else
		return CLine::COMPARE_RIGHT;
}

//_vec3 CNaviMesh::MoveOn_NaviMesh(const _vec3 * pTargetPos, const _vec3 * pTargetDir)
//{
//	_vec3		vEndPos = *pTargetPos + *pTargetDir;
//
//	if (CCell::COMPARE_MOVE == m_vecCell[m_dwIndex]->Compare(&vEndPos, &m_dwIndex))
//		return vEndPos;
//
//	else if (CCell::COMPARE_STOP == m_vecCell[m_dwIndex]->Compare(&vEndPos, &m_dwIndex))
//		return *pTargetPos;
//
//	return _vec3(0.f, 0.f, 0.f);
//}

_bool Engine::CNaviMesh::MoveOn_NaviMeshPlayer(_vec3 * pTargetPos,
												_vec3* pNewPos,
												_bool& _bCompare,
												const DWORD& _dwJumpDownFlag,
												_bool& _bRoadCliff)
{
	_vec3		vEndPos = *pNewPos;

	D3DXPLANE tPlane = *D3DXPlaneFromPoints(&tPlane,
											&m_vecCell[m_dwIndex]->m_vPoint[CCell::POINT_A],
											&m_vecCell[m_dwIndex]->m_vPoint[CCell::POINT_B],
											&m_vecCell[m_dwIndex]->m_vPoint[CCell::POINT_C]);

	if (CCell::COMPARE_MOVE == m_vecCell[m_dwIndex]->Compare(&vEndPos, &m_dwIndex))
	{
		_float fHeight;
		fHeight = ((tPlane.a * vEndPos.x) + (tPlane.c * vEndPos.z) + tPlane.d) / -tPlane.b;

		if (fHeight >= vEndPos.y)
		{
			if (Engine::NAVI_TYPE::NAVI_ROAD == m_vecCell[m_dwIndex]->Get_NaviType()
				|| Engine::NAVI_TYPE::NAVI_CLIFF == m_vecCell[m_dwIndex]->Get_NaviType() && (0x00000001 & _dwJumpDownFlag)
				|| Engine::NAVI_TYPE::NAVI_CLIFF == m_vecCell[m_dwIndex]->Get_NaviType() && (0x00000004 & _dwJumpDownFlag))
			{
				vEndPos.y = fHeight;
				*pNewPos = vEndPos;

				/*  Compare Move  */
				_bCompare = TRUE;

				/*  IsOnLand True  */
				return TRUE;
			}
		}

		/*  Compare Move  */
		_bCompare = TRUE;

		/*  IsOnLand False  */
		return FALSE;
	}
	else if (CCell::COMPARE_STOP == m_vecCell[m_dwIndex]->Compare(&vEndPos, &m_dwIndex))
	{
		_vec3 vUp = { 0.f, 1.f, 0.f };
		_vec3 vSliding;

		for (_ushort i = 0; i < Engine::CCell::POINT_END; ++i)
		{
			if (CLine::COMPARE::COMPARE_LEFT == m_vecCell[m_dwIndex]->Compare_Line((CCell::LINE)i, &vEndPos))
			{
				_vec3 vNormal = m_vecCell[m_dwIndex]->m_vPoint[i] - m_vecCell[m_dwIndex]->m_vPoint[(i + 1) % Engine::CCell::POINT_END];
				D3DXVec3Normalize(&vNormal, &vNormal);
				D3DXVec3Cross(&vNormal, &vNormal, &vUp);

				_vec3 vTargetDir = vEndPos - *pTargetPos;
				vSliding = vTargetDir - D3DXVec3Dot(&vTargetDir, &vNormal) * vNormal;

				/*  CurIndex Check  */
				Update_CurIndex(vEndPos);

				break;
			}
		}
		//*pNewPos = *pTargetPos + vSliding;

		/*  False JumpDownState  */
		if (0x00000001 & _dwJumpDownFlag)
		{
			//*pNewPos = *pTargetPos + vSliding;

			if (Engine::NAVI_TYPE::NAVI_CLIFF == m_vecCell[m_dwIndex]->Get_NaviType())
			{
				_vec3 vTargetDir = vEndPos - *pTargetPos;
				vTargetDir.y = 0.f;
				D3DXVec3Normalize(&vTargetDir, &vTargetDir);

				bool bIsOnRoad = FALSE;

				/*  CurIndex Check  */
				if (TRUE == Update_CurIndex(vEndPos, vTargetDir * 1.f))
					bIsOnRoad = TRUE;

				if (TRUE == bIsOnRoad)
				{
					//_float fHeight;
					//fHeight = ((tPlane.a * vPos.x) + (tPlane.c * vPos.z) + tPlane.d) / -tPlane.b;

					//if (fHeight >= pTargetPos->y)
					//{
					//	//pNewPos->y = fHeight;

					//	//*pNewPos = *pTargetPos + vSliding;
					//	pNewPos->x = pTargetPos->x + vSliding.x;
					//	pNewPos->z = pTargetPos->z + vSliding.z;
					//	pNewPos->y = vEndPos.y;

					_vec3 vPos = vEndPos + vTargetDir * 1.f;

					vPos.y = pNewPos->y;
					*pNewPos = vPos;

					/*  Compare Move  */
					_bCompare = TRUE;

					/*  RoadCliff False  */
					_bRoadCliff = FALSE;

					/*  IsOnLand False  */
					return FALSE;
					//}
				}

			}
		}
		/*  True JumpState  */
		else if (0x00000002 & _dwJumpDownFlag)
		{
			_vec3 vTargetDir = vEndPos - *pTargetPos;
			vTargetDir.y = 0.f;
			D3DXVec3Normalize(&vTargetDir, &vTargetDir);

			bool bIsOnCliff = FALSE;

			/*  CurIndex Check  */
			if (TRUE == Update_CurIndex(vEndPos, vTargetDir * 1.f))
				bIsOnCliff = TRUE;

			if (TRUE == bIsOnCliff)
			{
				_vec3 vPos = vEndPos + vTargetDir * 1.f;

				_float fHeight;
				fHeight = ((tPlane.a * vPos.x) + (tPlane.c * vPos.z) + tPlane.d) / -tPlane.b;

				if (fHeight >= pTargetPos->y)
				{
					//*pNewPos = *pTargetPos + vSliding;
					pNewPos->x = pTargetPos->x + vSliding.x;
					pNewPos->z = pTargetPos->z + vSliding.z;
					pNewPos->y = vEndPos.y;
				}

				/*  Compare Stop  */
				_bCompare = FALSE;

				/*  IsOnLand True  */
				return FALSE;
			}
			/*else
				*pNewPos = *pTargetPos + vSliding;*/
		}
		/*  True DownState  */
		else if (0x00000004 & _dwJumpDownFlag)
		{
			_vec3 vTargetDir = vEndPos - *pTargetPos;
			vTargetDir.y = 0.f;
			D3DXVec3Normalize(&vTargetDir, &vTargetDir);

			bool bIsOnCliff = FALSE;

			/*  CurIndex Check  */
			if (TRUE == Update_CurIndex(vEndPos, vTargetDir * 1.f))
				bIsOnCliff = TRUE;

			/*if(FALSE == bIsOnCliff)
				*pNewPos = *pTargetPos + vSliding;*/
		}

		*pNewPos = *pTargetPos + vSliding;

		_float fHeight;
		fHeight = ((tPlane.a * vEndPos.x) + (tPlane.c * vEndPos.z) + tPlane.d) / -tPlane.b;

		if (fHeight >= vEndPos.y)
		{
			pNewPos->y = fHeight;

			/*  Compare Stop  */
			_bCompare = FALSE;

			/*  IsOnLand True  */
			return TRUE;
		}

		/*  Compare Stop  */
		_bCompare = FALSE;

		/*  IsOnLand False  */
		return FALSE;
	}
}

_bool Engine::CNaviMesh::MoveOn_NaviMeshElite(_vec3 * pTargetPos,
											_vec3* pNewPos,
											_bool& _bCompare)
{
	_vec3		vEndPos = *pNewPos;

	/*Update_CurIndex(vEndPos);*/

	D3DXPLANE tPlane = *D3DXPlaneFromPoints(&tPlane,
											&m_vecCell[m_dwIndex]->m_vPoint[CCell::POINT_A],
											&m_vecCell[m_dwIndex]->m_vPoint[CCell::POINT_B],
											&m_vecCell[m_dwIndex]->m_vPoint[CCell::POINT_C]);

	if (CCell::COMPARE_MOVE == m_vecCell[m_dwIndex]->Compare(&vEndPos, &m_dwIndex))
	{
		_float fHeight;
		fHeight = ((tPlane.a * vEndPos.x) + (tPlane.c * vEndPos.z) + tPlane.d) / -tPlane.b;

		vEndPos.y = fHeight;
		*pNewPos = vEndPos;

		_bCompare = TRUE;
		return TRUE;
	}
	else if (CCell::COMPARE_STOP == m_vecCell[m_dwIndex]->Compare(&vEndPos, &m_dwIndex))
	{
		_vec3 vUp = { 0.f, 1.f, 0.f };
		_vec3 vSliding;
		for (_ushort i = 0; i < Engine::CCell::POINT_END; ++i)
		{
			if (CLine::COMPARE::COMPARE_LEFT == m_vecCell[m_dwIndex]->Compare_Line((CCell::LINE)i, &vEndPos))
			{
				_vec3 vNormal = m_vecCell[m_dwIndex]->m_vPoint[i] - m_vecCell[m_dwIndex]->m_vPoint[(i + 1) % Engine::CCell::POINT_END];
				D3DXVec3Normalize(&vNormal, &vNormal);
				D3DXVec3Cross(&vNormal, &vNormal, &vUp);

				_vec3 vTargetDir = vEndPos - *pTargetPos;
				vSliding = vTargetDir - D3DXVec3Dot(&vTargetDir, &vNormal) * vNormal;
				break;
			}
		}

		_float fHeight;
		fHeight = ((tPlane.a * vEndPos.x) + (tPlane.c * vEndPos.z) + tPlane.d) / -tPlane.b;

		*pNewPos = *pTargetPos + vSliding;
		pNewPos->y = fHeight;

		/*  CurIndex Check  */
		Update_CurIndex(vEndPos);

		_bCompare = FALSE;
		return FALSE;
	}
}

_bool Engine::CNaviMesh::MoveOn_NaviMeshCam(const _vec3 * pTargetPos, _vec3* pDir)
{
	_vec3 vRayPos, vRayDir, vPickPos;
	_float fU, fV, fDist, fBeforeDist = 1000.f;
	_bool bPick = FALSE;

	vRayPos = *pTargetPos;
	vRayDir = *pDir;
	
	for (auto& iter : m_vecInsideCell)
	{
		if (D3DXIntersectTri(&iter->m_vPoint[0],
			&iter->m_vPoint[1],
			&iter->m_vPoint[2],
			&vRayPos, &vRayDir, &fU, &fV, &fDist))
		{
			if (fBeforeDist > fDist)
			{
				vPickPos = iter->m_vPoint[0]
							+ fU * (iter->m_vPoint[1] - iter->m_vPoint[0])
							+ fV * (iter->m_vPoint[2] - iter->m_vPoint[0]);
				bPick = TRUE;
				fBeforeDist = fDist;
			}
		}
	}

	if (TRUE == bPick)
	{
		_vec3 vDist = vPickPos - vRayPos;
		if (1.f > D3DXVec3Length(&vDist))
			return TRUE;
	}

	return FALSE;

	//_vec3		vEndPos = *pNewPos;

	//D3DXPLANE tPlane = *D3DXPlaneFromPoints(&tPlane,
	//										&m_vecCell[m_dwIndex]->m_vPoint[CCell::POINT_A],
	//										&m_vecCell[m_dwIndex]->m_vPoint[CCell::POINT_B],
	//										&m_vecCell[m_dwIndex]->m_vPoint[CCell::POINT_C]);

	//for (auto& iter : m_vecCell)
	//{
	//	D3DXPLANE tPlane = *D3DXPlaneFromPoints(&tPlane,
	//											&iter->m_vPoint[CCell::POINT_A],
	//											&iter->m_vPoint[CCell::POINT_B],
	//											&iter->m_vPoint[CCell::POINT_C]);

	//	float fHeight;
	//	fHeight = ((tPlane.a * vEndPos.x) + (tPlane.c * vEndPos.z) + tPlane.d) / -tPlane.b;

	//	if (fHeight > vEndPos.y)
	//	{
	//		return FALSE;
	//	}
	//	else
	//	{

	//		return TRUE;
	//	}
	//}

	//if (CCell::COMPARE_MOVE == m_vecCell[m_dwIndex]->Compare(&vEndPos, &m_dwIndex))
	//{
	//	float fHeight;
	//	fHeight = ((tPlane.a * vEndPos.x) + (tPlane.c * vEndPos.z) + tPlane.d) / -tPlane.b;

	//	if (fHeight > vEndPos.y)
	//	{
	//		
	//	}
	//	return TRUE;
	//}
	//else if (CCell::COMPARE_STOP == m_vecCell[m_dwIndex]->Compare(&vEndPos, &m_dwIndex))
	//{

	//	return FALSE;
	//}
}

_bool Engine::CNaviMesh::MoveOn_NaviMeshCam2(_vec3 * pTargetPos, _vec3* pNewPos)
{
	_vec3		vEndPos = *pNewPos;

	D3DXPLANE tPlane = *D3DXPlaneFromPoints(&tPlane,
		&m_vecCell[m_dwIndex]->m_vPoint[CCell::POINT_A],
		&m_vecCell[m_dwIndex]->m_vPoint[CCell::POINT_B],
		&m_vecCell[m_dwIndex]->m_vPoint[CCell::POINT_C]);

	if (CCell::COMPARE_MOVE == m_vecCell[m_dwIndex]->Compare(&vEndPos, &m_dwIndex))
	{
		float fHeight;
		fHeight = ((tPlane.a * vEndPos.x) + (tPlane.c * vEndPos.z) + tPlane.d) / -tPlane.b;

		if (fHeight >= vEndPos.y)
		{
			return FALSE;
		}
		return TRUE;
	}
	else if (CCell::COMPARE_STOP == m_vecCell[m_dwIndex]->Compare(&vEndPos, &m_dwIndex))
	{
		float fHeight;
		fHeight = ((tPlane.a * vEndPos.x) + (tPlane.c * vEndPos.z) + tPlane.d) / -tPlane.b;

		pTargetPos->y = vEndPos.y;

		if (fHeight >= vEndPos.y)
		{
			return FALSE;
		}

		return FALSE;
	}

	return FALSE;
}

HRESULT Engine::CNaviMesh::Create_Cell(_vec3* _vPos)
{
	CCell*		pCell = nullptr;

	pCell = CCell::Create(m_pGraphicDev,
							m_vecCell.size(),
							&_vPos[0],
							&_vPos[1],
							&_vPos[2]);

	NULL_CHECK_RETURN(pCell, E_FAIL);
	m_vecCell.push_back(pCell);

	FAILED_CHECK_RETURN(Link_Cell(), E_FAIL);

	return S_OK;
}

HRESULT Engine::CNaviMesh::Link_Cell(void)
{
	for (_ulong i = 0; i < m_vecCell.size(); ++i)
	{
		for (_ulong j = 0; j < m_vecCell.size(); ++j)
		{
			if (i == j)
				continue;

			if (nullptr == m_vecCell[i]->Get_Neighbor(CCell::NEIGHBOR_AB) &&
				true == m_vecCell[j]->Compare_Point(m_vecCell[i]->Get_Point(CCell::POINT_A),
													m_vecCell[i]->Get_Point(CCell::POINT_B),
													m_vecCell[i]))
			{
				m_vecCell[i]->Set_Neighbor(CCell::NEIGHBOR_AB, m_vecCell[j]);
				continue;
			}

			if (nullptr == m_vecCell[i]->Get_Neighbor(CCell::NEIGHBOR_BC) &&
				true == m_vecCell[j]->Compare_Point(m_vecCell[i]->Get_Point(CCell::POINT_B),
													m_vecCell[i]->Get_Point(CCell::POINT_C),
													m_vecCell[i]))
			{
				m_vecCell[i]->Set_Neighbor(CCell::NEIGHBOR_BC, m_vecCell[j]);
				continue;
			}

			if (nullptr == m_vecCell[i]->Get_Neighbor(CCell::NEIGHBOR_CA) &&
				true == m_vecCell[j]->Compare_Point(m_vecCell[i]->Get_Point(CCell::POINT_C),
													m_vecCell[i]->Get_Point(CCell::POINT_A),
													m_vecCell[i]))
			{
				m_vecCell[i]->Set_Neighbor(CCell::NEIGHBOR_CA, m_vecCell[j]);
				continue;
			}
		}
	}

	return S_OK;
}

CNaviMesh* Engine::CNaviMesh::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CNaviMesh*	pInstance = new CNaviMesh(pGraphicDev);

	if (FAILED(pInstance->Ready_NaviMesh()))
		Safe_Release(pInstance);

	return pInstance;
}

CResources* Engine::CNaviMesh::Clone(void)
{
	return new CNaviMesh(*this);
}

void Engine::CNaviMesh::Free(void)
{
	CMesh::Free();

	for_each(m_vecCell.begin(), m_vecCell.end(), CDeleteObj());
	m_vecCell.clear();

	for_each(m_vecInsideCell.begin(), m_vecInsideCell.end(), CDeleteObj());
	m_vecInsideCell.clear();
}


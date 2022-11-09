#include "Cell.h"

USING(Engine)

#include <iostream>
using namespace std;

Engine::CCell::CCell(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
{
	ZeroMemory(m_vPoint, sizeof(_vec3) * POINT_END);
	ZeroMemory(m_pNeighbor, sizeof(CCell*) * NEIGHBOR_END);
	ZeroMemory(m_pLine, sizeof(CLine*) * LINE_END);

	m_pGraphicDev->AddRef();
}

Engine::CCell::~CCell(void)
{

}

HRESULT Engine::CCell::Ready_Cell(const _ulong& dwIndex, const _vec3* pPointA, const _vec3* pPointB, const _vec3* pPointC)
{
	m_dwIndex = dwIndex;

	m_vPoint[POINT_A] = *pPointA;
	m_vPoint[POINT_B] = *pPointB;
	m_vPoint[POINT_C] = *pPointC;

	m_pLine[LINE_AB] = CLine::Create(&_vec2(m_vPoint[POINT_A].x, m_vPoint[POINT_A].z), &_vec2(m_vPoint[POINT_B].x, m_vPoint[POINT_B].z));

	m_pLine[LINE_BC] = CLine::Create(&_vec2(m_vPoint[POINT_B].x, m_vPoint[POINT_B].z), &_vec2(m_vPoint[POINT_C].x, m_vPoint[POINT_C].z));

	m_pLine[LINE_CA] = CLine::Create(&_vec2(m_vPoint[POINT_C].x, m_vPoint[POINT_C].z), &_vec2(m_vPoint[POINT_A].x, m_vPoint[POINT_A].z));

	FAILED_CHECK_RETURN(D3DXCreateLine(m_pGraphicDev, &m_pD3DXLine), E_FAIL);

	return S_OK;
}

_bool Engine::CCell::Compare_Point(const _vec3* pFirstPoint, const _vec3* pSecondPoint, CCell* pCell)
{
	_vec3 vDistA = m_vPoint[POINT_A] - *pFirstPoint;
	_vec3 vDistB = m_vPoint[POINT_B] - *pSecondPoint;
	_vec3 vDistC = m_vPoint[POINT_C] - *pSecondPoint;
	
	float fDist = 0.5f;

	if (m_vPoint[POINT_A] == *pFirstPoint)
	//if(fDist > D3DXVec3Length(&vDistA))
	{
		if (m_vPoint[POINT_B] == *pSecondPoint)
		//if (fDist > D3DXVec3Length(&vDistB))
		{
			m_pNeighbor[NEIGHBOR_AB] = pCell;
			//cout << "AB1 TRUE" << endl;
			//cout << D3DXVec3Length(&vDistB) << endl;
			return true;
		}
		if (m_vPoint[POINT_C] == *pSecondPoint)
		//if (fDist > D3DXVec3Length(&vDistC))
		{
			m_pNeighbor[NEIGHBOR_CA] = pCell;
			//cout << "AC1 TRUE" << endl;
			//cout << D3DXVec3Length(&vDistC) << endl;
			return true;
		}
	}

	vDistA = m_vPoint[POINT_A] - *pSecondPoint;
	vDistB = m_vPoint[POINT_B] - *pFirstPoint;
	vDistC = m_vPoint[POINT_C] - *pSecondPoint;

	if (m_vPoint[POINT_B] == *pFirstPoint)
	//if (fDist > D3DXVec3Length(&vDistB))
	{
		if (m_vPoint[POINT_A] == *pSecondPoint)
		//if (fDist > D3DXVec3Length(&vDistA))
		{
			m_pNeighbor[NEIGHBOR_AB] = pCell;
			//cout << "AB2 TRUE" << endl;
			//cout << D3DXVec3Length(&vDistA) << endl;
			return true;
		}
		if (m_vPoint[POINT_C] == *pSecondPoint)
		//if (fDist > D3DXVec3Length(&vDistC))
		{
			m_pNeighbor[NEIGHBOR_BC] = pCell;
			//cout << "BC2 TRUE" << endl;
			//cout << D3DXVec3Length(&vDistC) << endl;
			return true;
		}
	}

	vDistA = m_vPoint[POINT_A] - *pSecondPoint;
	vDistB = m_vPoint[POINT_B] - *pSecondPoint;
	vDistC = m_vPoint[POINT_C] - *pFirstPoint;

	if (m_vPoint[POINT_C] == *pFirstPoint)
	//if (fDist > D3DXVec3Length(&vDistC))
	{
		if (m_vPoint[POINT_A] == *pSecondPoint)
		//if (fDist > D3DXVec3Length(&vDistA))
		{
			m_pNeighbor[NEIGHBOR_CA] = pCell;
			//cout << "CA3 TRUE" << endl;
			//cout << D3DXVec3Length(&vDistA) << endl;
			return true;
		}
		if (m_vPoint[POINT_B] == *pSecondPoint)
		//if (fDist > D3DXVec3Length(&vDistB))
		{
			m_pNeighbor[NEIGHBOR_BC] = pCell;
			//cout << "BC3 TRUE" << endl;
			//cout << D3DXVec3Length(&vDistB) << endl;
			return true;
		}
	}

	return false;
}

Engine::CCell::COMPARE Engine::CCell::Compare(const _vec3* pEndPos, _ulong* pCellIndex)
{
	for (_ulong i = 0; i < LINE_END; ++i)
	{
		if (CLine::COMPARE_LEFT == m_pLine[i]->Compare(&_vec2(pEndPos->x, pEndPos->z)))
		{
			if (nullptr == m_pNeighbor[i])
				return CCell::COMPARE_STOP;
			else
			{
				*pCellIndex = *m_pNeighbor[i]->Get_Index();
				return CCell::COMPARE_MOVE;
			}
		}
	}

	return CCell::COMPARE_MOVE;
}

void Engine::CCell::Render_Cell(void)
{
	// m_pD3DXLine->Draw(); // 2d라인을 그리는 함수

	// m_pD3DXLine->DrawTransform(); // 3D라인을 그리는 함수

	_vec3	vPoint[4];

	vPoint[0] = m_vPoint[POINT_A];
	vPoint[1] = m_vPoint[POINT_B];
	vPoint[2] = m_vPoint[POINT_C];
	vPoint[3] = m_vPoint[POINT_A];

	_matrix		matView, matProj;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	for (_ulong i = 0; i < 4; ++i)
	{
		D3DXVec3TransformCoord(&vPoint[i], &vPoint[i], &matView);

		if (vPoint[i].z <= 0.1f)
			vPoint[i].z = 0.1f;

		D3DXVec3TransformCoord(&vPoint[i], &vPoint[i], &matProj);
	}

	m_pD3DXLine->SetWidth(3.f); // 라인의 굵기를 결정하는 함수

	m_pGraphicDev->EndScene();
	m_pGraphicDev->BeginScene();

	m_pD3DXLine->Begin();

	_matrix matTemp;

	if (Engine::NAVI_TYPE::NAVI_ROAD == m_eNaviType)
		m_pD3DXLine->DrawTransform(vPoint, 4, D3DXMatrixIdentity(&matTemp), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
	else if (Engine::NAVI_TYPE::NAVI_INSIDE == m_eNaviType)
		m_pD3DXLine->DrawTransform(vPoint, 4, D3DXMatrixIdentity(&matTemp), D3DXCOLOR(1.f, 0.f, 0.f, 1.f));
	else if (Engine::NAVI_TYPE::NAVI_CLIFF == m_eNaviType)
		m_pD3DXLine->DrawTransform(vPoint, 4, D3DXMatrixIdentity(&matTemp), D3DXCOLOR(0.f, 0.f, 1.f, 1.f));

	m_pD3DXLine->End();
}

CCell* Engine::CCell::Create(LPDIRECT3DDEVICE9 pGraphicDev,
								const _ulong& dwIndex,
								const _vec3* pPointA,
								const _vec3* pPointB,
								const _vec3* pPointC)
{
	pGraphicDev->AddRef();

	CCell*	pInstance = new CCell(pGraphicDev);

	if (FAILED(pInstance->Ready_Cell(dwIndex, pPointA, pPointB, pPointC)))
		Safe_Release(pInstance);

	Safe_Release(pGraphicDev);

	return pInstance;
}

void Engine::CCell::Free(void)
{
	Safe_Release(m_pD3DXLine);

	for (_ulong i = 0; i < LINE_END; ++i)
		Safe_Release(m_pLine[i]);

	Safe_Release(m_pGraphicDev);
}

Engine::CLine::COMPARE Engine::CCell::Compare_Line(LINE eLine, const _vec3* pEndPos)
{
	return m_pLine[eLine]->Compare(&_vec2(pEndPos->x, pEndPos->z));
}


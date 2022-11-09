#include "Calculator.h"

USING(Engine)
#include <iostream>

Engine::CCalculator::CCalculator(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
{
	m_pGraphicDev->AddRef();
}

Engine::CCalculator::CCalculator(const CCalculator& rhs)
	: m_pGraphicDev(rhs.m_pGraphicDev)
{
	Ready_Calculator();
}

Engine::CCalculator::~CCalculator(void)
{

}

HRESULT Engine::CCalculator::Ready_Calculator(void)
{
	return S_OK;
}

_float Engine::CCalculator::Compute_HeightTerrain(const _vec3* pPos,
													const _vec3* pTerrainVtxPos,
													const _ulong& dwCntX,
													const _ulong& dwCntZ)
{
	_ulong	dwIndex = _ulong(pPos->z / 1.f) * _ulong(dwCntX) + _ulong(pPos->x / 1.f);

	_float	fRatioX = (pPos->x - pTerrainVtxPos[dwIndex + dwCntX].x) / 1.f;

	_float  fRatioZ = (pTerrainVtxPos[dwIndex + dwCntX].z - pPos->z) / 1.f;

	_float		fHeight[4] = {
		pTerrainVtxPos[dwIndex + dwCntX].y,
		pTerrainVtxPos[dwIndex + dwCntX + 1].y,
		pTerrainVtxPos[dwIndex + 1].y,
		pTerrainVtxPos[dwIndex].y
	};

	// ������ ��
	if (fRatioX > fRatioZ)
	{
		return fHeight[0] + (fHeight[1] - fHeight[0]) * fRatioX + (fHeight[2] - fHeight[1]) * fRatioZ;
	}
	// ���� �Ʒ�
	else
	{
		return fHeight[0] + (fHeight[2] - fHeight[3]) * fRatioX + (fHeight[3] - fHeight[0]) * fRatioZ;
	}
}

_bool Engine::CCalculator::Pick_OnTerrain(HWND hWnd, _vec3* pOutPos, const CTerrainTex* pTerrainBufferCom, const CTransform* pTerrainTransform)
{
	POINT		ptMouse{};
	GetCursorPos(&ptMouse);
	ScreenToClient(hWnd, &ptMouse);

	//D3DVIEWPORT9		ViewPort;
	//ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));
	//m_pGraphicDev->GetViewport(&ViewPort);
	
	RECT rc;
	GetWindowRect(hWnd, &rc);
	float fWindowX = float(rc.right - rc.left);
	float fWindowY = float(rc.bottom - rc.top);

	// ���� ������ ���콺�� ��ȯ
	_vec3		vMousePos;
	vMousePos.x = (ptMouse.x + 0) / (fWindowX * 0.5f) - 1.f;
	vMousePos.y = (ptMouse.y + 0) / (fWindowY * -0.5f) + 1.f;
	vMousePos.z = 0.f;

	// �佺���̽��� ���콺�� ��ȯ
	_matrix		matProj;
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, NULL, &matProj);
	D3DXVec3TransformCoord(&vMousePos, &vMousePos, &matProj);

	_vec3		vRayDir, vRayPos;		// �� �����̽� ���� ��ǥ��� ���� �Ͽ� ���

	vRayPos = _vec3(0.f, 0.f, 0.f);
	vRayDir = vMousePos - vRayPos;

	// ���� �����̽��� vRayDir, vRayPos�� ����

	_matrix		matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, NULL, &matView);

	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matView);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matView);

	// ���� �����̽��� vRayDir, vRayPos�� ����

	_matrix		matWorld;
	pTerrainTransform->Get_WorldMatrix(&matWorld);

	D3DXMatrixInverse(&matWorld, NULL, &matWorld);

	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matWorld);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matWorld);


	_ulong	dwVtxCntX = pTerrainBufferCom->Get_VtxCntX();
	_ulong	dwVtxCntZ = pTerrainBufferCom->Get_VtxCntZ();

	const _vec3*	pTerrainVtx = pTerrainBufferCom->Get_VtxPos();
	//const _vec3*	pTerrainVtx = pTerrainBufferCom->Get_VtxPosWorld();

	_ulong	dwVtxIdx[3];

	_float	fU, fV, fDist;

	for (_ulong z = 0; z < dwVtxCntZ - 1; ++z)
	{
		for (_ulong x = 0; x < dwVtxCntX - 1; ++x)
		{
			_ulong	dwIndex = z * dwVtxCntX + x;

			// ������ ��

			dwVtxIdx[0] = dwIndex + dwVtxCntX;
			dwVtxIdx[1] = dwIndex + dwVtxCntX + 1;
			dwVtxIdx[2] = dwIndex + 1;

			if (D3DXIntersectTri(&pTerrainVtx[dwVtxIdx[1]],
				&pTerrainVtx[dwVtxIdx[2]],
				&pTerrainVtx[dwVtxIdx[0]],
				&vRayPos, &vRayDir, &fU, &fV, &fDist))
			{
				*pOutPos = _vec3(pTerrainVtx[dwVtxIdx[1]].x + (pTerrainVtx[dwVtxIdx[0]].x - pTerrainVtx[dwVtxIdx[1]].x)* fU,
					0.f,
					pTerrainVtx[dwVtxIdx[1]].z + (pTerrainVtx[dwVtxIdx[2]].z - pTerrainVtx[dwVtxIdx[1]].z)* fV);
				return TRUE;
			}

			// ���� �Ʒ�

			dwVtxIdx[0] = dwIndex + dwVtxCntX;
			dwVtxIdx[1] = dwIndex + 1;
			dwVtxIdx[2] = dwIndex;

			if (D3DXIntersectTri(&pTerrainVtx[dwVtxIdx[2]],
				&pTerrainVtx[dwVtxIdx[0]],
				&pTerrainVtx[dwVtxIdx[1]],
				&vRayPos, &vRayDir, &fU, &fV, &fDist))
			{
				*pOutPos = _vec3(pTerrainVtx[dwVtxIdx[2]].x + (pTerrainVtx[dwVtxIdx[1]].x - pTerrainVtx[dwVtxIdx[2]].x)* fU,
					0.f,
					pTerrainVtx[dwVtxIdx[2]].z + (pTerrainVtx[dwVtxIdx[0]].z - pTerrainVtx[dwVtxIdx[2]].z)* fV);
				return TRUE;
			}
		}
	}

	return FALSE;
}

_bool Engine::CCalculator::Pick_OnMesh(HWND hWnd, _vec3* pOutPos, CStaticMesh* pMeshBufferCom, CTransform* pTransformCom)
{
	POINT		ptMouse{};

	GetCursorPos(&ptMouse);
	ScreenToClient(hWnd, &ptMouse);
			
	D3DVIEWPORT9      ViewPort;
	ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));
	m_pGraphicDev->GetViewport(&ViewPort);

	RECT rc;
	GetWindowRect(hWnd, &rc);
	float fWindowX = float(rc.right - rc.left);
	float fWindowY = float(rc.bottom - rc.top);

	// ���� ������ ���콺�� ��ȯ
	_vec3		vMousePos;
	vMousePos.x = (ptMouse.x + 0) / (fWindowX * 0.5f) - 1.f;
	vMousePos.y = (ptMouse.y + 0) / (fWindowY * -0.5f) + 1.f;
	vMousePos.z = 0.f;

	// �佺���̽��� ���콺�� ��ȯ
	_matrix      matProj;
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, NULL, &matProj);
	D3DXVec3TransformCoord(&vMousePos, &vMousePos, &matProj);

	// �� �����̽� ���� ��ǥ��� ���� �Ͽ� ���
	_vec3      vRayDir, vRayPos;
	vRayPos = _vec3(0.f, 0.f, 0.f);
	vRayDir = vMousePos - vRayPos;

	// ���� �����̽��� vRayDir, vRayPos�� ����
	_matrix      matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, NULL, &matView);
	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matView);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matView);

	// ���� �����̽��� vRayDir, vRayPos�� ����
	_matrix      matWorld;
	pTransformCom->Get_WorldMatrix(&matWorld);
	D3DXMatrixInverse(&matWorld, NULL, &matWorld);
	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matWorld);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matWorld);

	_float	fU, fV, fDist;
	BOOL	bPickCheck = false;
	DWORD	dwFaceIndex;

	D3DXIntersect(pMeshBufferCom->Get_Mesh(), &vRayPos, &vRayDir, &bPickCheck, &dwFaceIndex, &fU, &fV, &fDist, NULL, NULL);

	if (TRUE == bPickCheck)
	{
		*pOutPos = vRayPos + (vRayDir * fDist);

		_matrix      matWorld;
		pTransformCom->Get_WorldMatrix(&matWorld);
		D3DXVec3TransformCoord(pOutPos, pOutPos, &matWorld);
		return true;
	}
	else
	{
		*pOutPos = _vec3(0.0f, 0.0f, 0.0f);
		return false;
	}
}

CCalculator* Engine::CCalculator::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCalculator*	pInstance = new CCalculator(pGraphicDev);

	if (FAILED(pInstance->Ready_Calculator()))
		Safe_Release(pInstance);

	return pInstance;
}

Engine::CComponent* Engine::CCalculator::Clone_Component(void)
{
	return new CCalculator(*this);
}

void Engine::CCalculator::Free(void)
{
	Safe_Release(m_pGraphicDev);
}
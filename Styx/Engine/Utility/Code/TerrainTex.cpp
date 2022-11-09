#include "TerrainTex.h"

USING(Engine)

Engine::CTerrainTex::CTerrainTex(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
	, m_pPos(nullptr)
	, m_pPosWorld(nullptr)
	, m_bClone(false)
	, m_dwCntX(0)
	, m_dwCntZ(0)
{

}

Engine::CTerrainTex::CTerrainTex(const CTerrainTex& rhs)
	: CVIBuffer(rhs)
	, m_pPos(rhs.m_pPos)
	, m_pPosWorld(rhs.m_pPosWorld)
	, m_bClone(true)
	, m_dwCntX(rhs.m_dwCntX)
	, m_dwCntZ(rhs.m_dwCntZ)
	, m_fH(rhs.m_fH)
	, m_iH(rhs.m_iH)
{

}

Engine::CTerrainTex::~CTerrainTex(void)
{

}

HRESULT Engine::CTerrainTex::Ready_Buffer(void)
{
	return E_FAIL;
}

HRESULT Engine::CTerrainTex::Ready_Buffer(const _ulong& wCntX, const _ulong& wCntZ, const _ulong& wItv)
{
	m_dwVtxSize = sizeof(VTXTERRAINTEX);
	m_dwVtxCnt = wCntX * wCntZ;
	m_dwTriCnt = (wCntX - 1) * (wCntZ - 1) * 2;
	m_dwVtxFVF = FVF_TERRAINTEX;

	m_dwCntX = wCntX;
	m_dwCntZ = wCntZ;

	m_IdxFmt = D3DFMT_INDEX32;
	m_dwIdxSize = sizeof(INDEX32);

	m_pPos = new _vec3[m_dwVtxCnt];
	m_pPosWorld = new _vec3[m_dwVtxCnt];

	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), E_FAIL);

	VTXTERRAINTEX*		pVtxTex = nullptr;
	//DWORD*				pdwPixel = LoadImages();

	int iIndex = 0;

	m_pVB->Lock(0, 0, (void**)&pVtxTex, 0);
	
	for (_ulong z = 0; z < wCntZ; ++z)
	{
		for (_ulong x = 0; x < wCntX; ++x)
		{
			iIndex = z * wCntX + x;

			pVtxTex[iIndex].vPos = D3DXVECTOR3(float(x) * wItv,
												0.f /*(pdwPixel[iIndex] & 0x000000ff) / 20.f*/,
												float(z) * wItv);

			m_pPos[iIndex] = pVtxTex[iIndex].vPos;

			//pVtxTex[iIndex].vNormal = _vec3(0.f, 0.f, 0.f);

			pVtxTex[iIndex].vTexUV = D3DXVECTOR2(x / (wCntX - 1.f),
												 z / (wCntZ - 1.f));
		}
	}


	//Safe_Delete_Array(pdwPixel);

	INDEX32*		pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	int				iTriCnt = 0;

	for (_ulong z = 0; z < wCntZ - 1; ++z)
	{
		for (_ulong x = 0; x < wCntX - 1; ++x)
		{
			iIndex = z * wCntX + x;

			// ¿À¸¥ÂÊ À§ »ï°¢Çü
			pIndex[iTriCnt]._0 = _ushort(iIndex + wCntX);
			pIndex[iTriCnt]._1 = _ushort(iIndex + wCntX + 1);
			pIndex[iTriCnt]._2 = _ushort(iIndex + 1);

			_vec3	vDest, vSour, vNormal;

			/*vDest = pVtxTex[pIndex[iTriCnt]._1].vPos - pVtxTex[pIndex[iTriCnt]._0].vPos;
			vSour = pVtxTex[pIndex[iTriCnt]._2].vPos - pVtxTex[pIndex[iTriCnt]._1].vPos;
			D3DXVec3Cross(&vNormal, &vDest, &vSour);

			pVtxTex[pIndex[iTriCnt]._0].vNormal += vNormal;
			pVtxTex[pIndex[iTriCnt]._1].vNormal += vNormal;
			pVtxTex[pIndex[iTriCnt]._2].vNormal += vNormal;*/
			iTriCnt++;

			// ¿ÞÂÊ ¾Æ·¡ »ï°¢Çü
			pIndex[iTriCnt]._0 = _ushort(iIndex + wCntX);
			pIndex[iTriCnt]._1 = _ushort(iIndex + 1);
			pIndex[iTriCnt]._2 = _ushort(iIndex);

			/*vDest = pVtxTex[pIndex[iTriCnt]._2].vPos - pVtxTex[pIndex[iTriCnt]._1].vPos;
			vSour = pVtxTex[pIndex[iTriCnt]._0].vPos - pVtxTex[pIndex[iTriCnt]._2].vPos;
			D3DXVec3Cross(&vNormal, &vDest, &vSour);

			pVtxTex[pIndex[iTriCnt]._0].vNormal += vNormal;
			pVtxTex[pIndex[iTriCnt]._1].vNormal += vNormal;
			pVtxTex[pIndex[iTriCnt]._2].vNormal += vNormal;*/
			iTriCnt++;
		}
	}

	/*for (_ulong i = 0; i < m_dwVtxCnt; ++i)
		D3DXVec3Normalize(&pVtxTex[i].vNormal, &pVtxTex[i].vNormal);*/

	m_pIB->Unlock();
	m_pVB->Unlock();


	return S_OK;
}

DWORD* Engine::CTerrainTex::LoadImages(void)
{
	_ulong		dwByte = 0;

	m_hFile = CreateFile(L"../Bin/Resource/StudyTexture/Terrain/Height.bmp",
							GENERIC_READ,
							0, 
							NULL, 
							OPEN_EXISTING, 
							FILE_ATTRIBUTE_NORMAL, 
							0);

	ReadFile(m_hFile, &m_fH, sizeof(BITMAPFILEHEADER), &dwByte, NULL);
	ReadFile(m_hFile, &m_iH, sizeof(BITMAPINFOHEADER), &dwByte, NULL);

	_ulong*		pPixel = new _ulong[m_iH.biWidth * m_iH.biHeight];

	ReadFile(m_hFile, pPixel, sizeof(_ulong) * m_iH.biHeight * m_iH.biWidth, &dwByte, NULL);

	CloseHandle(m_hFile);

	return pPixel;
}

void Engine::CTerrainTex::Copy_Index(Engine::INDEX32* pIndex, const _ulong& dwTriCnt)
{
	Engine::INDEX32*			pOriIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pOriIndex, 0);

	memcpy(pOriIndex, pIndex, sizeof(Engine::INDEX32) * dwTriCnt);

	m_pIB->Unlock();

	m_dwTriCnt = dwTriCnt;
}

void Engine::CTerrainTex::Set_VtxPosWorld(const _matrix* _matWorld)
{
	_ulong		dwIndex;

	for (_ulong z = 0; z <  m_dwCntZ - 1; ++z)
	{
		for (_ulong x = 0; x <  m_dwCntX - 1; ++x)
		{
			dwIndex = z * m_dwCntX + x;

			D3DXVec3TransformCoord(&m_pPosWorld[dwIndex + m_dwCntX], &m_pPos[dwIndex + m_dwCntX], _matWorld);
			D3DXVec3TransformCoord(&m_pPosWorld[dwIndex + m_dwCntX + 1], &m_pPos[dwIndex + m_dwCntX + 1], _matWorld);
			D3DXVec3TransformCoord(&m_pPosWorld[dwIndex + 1], &m_pPos[dwIndex + 1], _matWorld);
			D3DXVec3TransformCoord(&m_pPosWorld[dwIndex], &m_pPos[dwIndex], _matWorld);
		}
	}
}

CTerrainTex* Engine::CTerrainTex::Create(LPDIRECT3DDEVICE9 pGraphicDev,
										const _ulong& wCntX, 
										const _ulong& wCntZ,
										const _ulong& wItv)	
{
	CTerrainTex*		pInstance = new CTerrainTex(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer(wCntX, wCntZ, wItv)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

Engine::CResources* Engine::CTerrainTex::Clone(void)
{
	return new CTerrainTex(*this);
}

void Engine::CTerrainTex::Free(void)
{
	CVIBuffer::Free();

	if (false == m_bClone)
	{
		Safe_Delete_Array(m_pPos);
		Safe_Delete_Array(m_pPosWorld);
	}
}
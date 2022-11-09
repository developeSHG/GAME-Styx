#include "TerrainRcTex.h"

Engine::CTerrainRcTex::CTerrainRcTex(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
{

}

Engine::CTerrainRcTex::CTerrainRcTex(const CTerrainRcTex& rhs)
	: CVIBuffer(rhs)
{

}

Engine::CTerrainRcTex::~CTerrainRcTex(void)
{

}

HRESULT Engine::CTerrainRcTex::Ready_Buffer(void)
{
	m_dwVtxCnt = 4;
	m_dwVtxFVF = FVF_TEX;
	m_dwTriCnt = 2;
	m_dwVtxSize = sizeof(VTXTEX);

	m_IdxFmt = D3DFMT_INDEX32;
	m_dwIdxSize = sizeof(INDEX32);

	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), E_FAIL);


	VTXTEX*		pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPos = _vec3(0.f, 0.f, 1.f);
	pVertex[0].vNormal = _vec3(0.f, 1.f, 0.f);
	pVertex[0].vTexUV = _vec2(0.f, 0.f);

	pVertex[1].vPos = _vec3(1.f, 0.f, 1.f);
	pVertex[1].vNormal = _vec3(0.f, 1.f, 0.f);
	pVertex[1].vTexUV = _vec2(1.f, 0.f);

	pVertex[2].vPos = _vec3(1.f, 0.f, 0.f);
	pVertex[2].vNormal = _vec3(0.f, 1.f, 0.f);
	pVertex[2].vTexUV = _vec2(1.f, 1.f);

	pVertex[3].vPos = _vec3(0.f, 0.f, 0.f);
	pVertex[3].vNormal = _vec3(0.f, 1.f, 0.f);
	pVertex[3].vTexUV = _vec2(0.f, 1.f);


	INDEX32*		pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	pIndex[0]._0 = 0;
	pVertex[pIndex[0]._0].vNormal = _vec3(0.f, 1.f, 0.f);
	pIndex[0]._1 = 1;
	pVertex[pIndex[0]._1].vNormal = _vec3(0.f, 1.f, 0.f);
	pIndex[0]._2 = 2;
	pVertex[pIndex[0]._2].vNormal = _vec3(0.f, 1.f, 0.f);

	pIndex[1]._0 = 0;
	pVertex[pIndex[1]._0].vNormal = _vec3(0.f, 1.f, 0.f);
	pIndex[1]._1 = 2;
	pVertex[pIndex[1]._1].vNormal = _vec3(0.f, 1.f, 0.f);
	pIndex[1]._2 = 3;
	pVertex[pIndex[1]._2].vNormal = _vec3(0.f, 1.f, 0.f);

	m_pIB->Unlock();
	m_pVB->Unlock();

	return S_OK;
}

Engine::CTerrainRcTex* Engine::CTerrainRcTex::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTerrainRcTex*	pInstance = new CTerrainRcTex(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

Engine::CResources* Engine::CTerrainRcTex::Clone(void)
{
	return new CTerrainRcTex(*this);
}

void Engine::CTerrainRcTex::Free(void)
{
	CVIBuffer::Free();
}

#include "Optimization.h"

USING(Engine)

Engine::COptimization::COptimization(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
	, m_bClone(false)
	, m_pFrustum(nullptr)
{
	m_pGraphicDev->AddRef();
}

Engine::COptimization::COptimization(const COptimization& rhs)
	: m_pGraphicDev(rhs.m_pGraphicDev)
	, m_bClone(true)
	, m_pFrustum(rhs.m_pFrustum)
{
	m_pGraphicDev->AddRef();
}

Engine::COptimization::~COptimization(void)
{

}

HRESULT Engine::COptimization::Ready_Optimization(void)
{
	m_pFrustum = CFrustum::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(m_pFrustum, E_FAIL);


	return S_OK;
}

_bool COptimization::Is_InFrustum_ForObject(const _vec3 * pPos, const _float fRadius /*= 0.f*/, _uint* uDir /*= nullptr*/)
{
	return m_pFrustum->Is_InFrustum_ForObject(pPos, fRadius, uDir);
}

void COptimization::Is_InFrustum_ForTerrain(const _vec3 * pVtxPos, const _ulong & dwCntX, const _ulong & dwCntZ, Engine::INDEX32 * pIndex, _ulong * pTriCnt, CQuadTree * pQuadTree)
{
	m_pFrustum->Is_InFrustum_ForTerrain(pVtxPos, dwCntX, dwCntZ, pIndex, pTriCnt, nullptr);
}

COptimization* Engine::COptimization::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	COptimization*		pInstance = new COptimization(pGraphicDev);

	if (FAILED(pInstance->Ready_Optimization()))
		Safe_Release(pInstance);

	return pInstance;
}

CComponent* Engine::COptimization::Clone_Component(void)
{
	return new COptimization(*this);
}

void Engine::COptimization::Free(void)
{
	Safe_Release(m_pGraphicDev);

	if (false == m_bClone)
	{
		Safe_Release(m_pFrustum);
	}
}


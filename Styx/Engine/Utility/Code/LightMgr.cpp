#include "LightMgr.h"

USING(Engine)
IMPLEMENT_SINGLETON(CLightMgr)

Engine::CLightMgr::CLightMgr(void)
{

}

Engine::CLightMgr::~CLightMgr(void)
{
	Free();
}

HRESULT Engine::CLightMgr::Ready_Light(LPDIRECT3DDEVICE9 pGraphicDev, 
										const D3DLIGHT9* pLightInfo,
										const _uint& iIndex)
{

	CLight*	pLight = CLight::Create(pGraphicDev, pLightInfo, iIndex);
	NULL_CHECK_RETURN(pLight, E_FAIL);

	m_listLight.push_back(pLight);
	
	return S_OK;
}

HRESULT Engine::CLightMgr::Render_Light(CShader* pShader)
{
	for (auto& pLight : m_listLight)
		pLight->Render_Light(pShader);

	return NOERROR;
}

void Engine::CLightMgr::Set_LightActive(const _uint iIndex /*= 0*/, bool _bActive /*= TRUE*/)
{
	auto	iter = m_listLight.begin();

	for (size_t i = 0; i < iIndex; ++i)
		++iter;

	(*iter)->Set_Active(_bActive);
}

D3DLIGHT9* Engine::CLightMgr::Get_LightInfo(const _uint iIndex /*= 0*/)
{
	auto	iter = m_listLight.begin();

	for (size_t i = 0; i < iIndex; ++i)
		++iter;	

	return (*iter)->Get_LightInfo();
}

_bool Engine::CLightMgr::Get_LightActive(const _uint iIndex /*= 0*/)
{
	auto	iter = m_listLight.begin();

	for (size_t i = 0; i < iIndex; ++i)
		++iter;

	return (*iter)->Get_Active();
}

void Engine::CLightMgr::Free(void)
{
	for_each(m_listLight.begin(), m_listLight.end(), CDeleteObj());
	m_listLight.clear();
}


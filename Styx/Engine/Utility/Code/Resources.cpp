#include "Resources.h"

Engine::CResources::CResources(LPDIRECT3DDEVICE9 _pGraphicDev)
	: m_pGraphicDev(_pGraphicDev)
{
	m_pGraphicDev->AddRef();
}

Engine::CResources::CResources(const CResources& rhs)
	: m_pGraphicDev(rhs.m_pGraphicDev)
{
	m_pGraphicDev->AddRef();
}

Engine::CResources::~CResources(void)
{
	
}

void Engine::CResources::Free(void)
{
	Engine::Safe_Release(m_pGraphicDev);
}


#include "GameObject.h"

USING(Engine)

Engine::CGameObject::CGameObject(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
{
	m_pGraphicDev->AddRef();
}

Engine::CGameObject::~CGameObject(void)
{
	
}

HRESULT Engine::CGameObject::Ready_Object(void)
{
	return S_OK;
}

_int Engine::CGameObject::Update_Object(const _float& fTimeDelta)
{
	_int	iEnd = UPDATE_OK;

	m_fTimeDelta = fTimeDelta;

	for (auto& iter : m_mapComponent[ID_DYNAMIC])
	{
		if (nullptr == iter.second)
			continue;

		iEnd = iter.second->Update_Component(fTimeDelta);

		if (iEnd & 0x80000000)
			return iEnd;
	}

	return iEnd;
}


_int Engine::CGameObject::LateUpdate_Object(const _float& fTimeDelta)
{
	_int	iEnd = UPDATE_OK;

	m_fTimeDelta = fTimeDelta;

	for (auto& iter : m_mapComponent[ID_DYNAMIC])
	{
		if (nullptr == iter.second)
			continue;

		iEnd = iter.second->LateUpdate_Component(fTimeDelta);

		if (iEnd & 0x80000000)
			return iEnd;
	}

	return iEnd;
}

void Engine::CGameObject::PreRender_Object(void)
{

}


void Engine::CGameObject::Render_Object(void)
{

}

void Engine::CGameObject::PostRender_Object(void)
{

}

void Engine::CGameObject::Free(void)
{
	for (_uint i = 0; i < ID_END; ++i)
	{
		for_each(m_mapComponent[i].begin(), m_mapComponent[i].end(), CDeleteMap());
		m_mapComponent[i].clear();
	}

	Safe_Release(m_pGraphicDev);
}

Engine::CComponent* Engine::CGameObject::Get_Component(COMPONENTTYPE _eType)
{
	for (_uint i = 0; i < COMPONENTID::ID_END; ++i)
	{
		auto& iter = m_mapComponent[i].find(_eType);

		if (iter == m_mapComponent[i].end())
		{
			if (COMPONENTID::ID_END - 1 == i)
				return nullptr;
			else
				continue;
		}

		return iter->second;
	}
	return nullptr;
}

void Engine::CGameObject::Compare_ViewZ(const _vec3* pPos)
{
	_matrix		matView;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, 0, &matView);

	_vec3	vCamPos;
	memcpy(&vCamPos, &matView.m[3][0], sizeof(_vec3));

	m_fViewZ = D3DXVec3Length(&(vCamPos - *pPos));
}

HRESULT Engine::CGameObject::LateInit_Object(void)
{
	if (FALSE == m_bLateInit)
	{
		this->LateInit_Object();
		m_bLateInit = TRUE;
	}

	return S_OK;
}
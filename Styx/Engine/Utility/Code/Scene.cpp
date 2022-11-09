#include "Scene.h"

USING(Engine)

Engine::CScene::CScene(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
{
	m_pGraphicDev->AddRef();
}

Engine::CScene::~CScene(void)
{

}

HRESULT Engine::CScene::Ready_Scene(void)
{
	return S_OK;
}

_int Engine::CScene::Update_Scene(const _float& fTimeDelta)
{
	_int iEnd = UPDATE_OK;

	for (auto& iter : m_mapLayer)
	{
		if (nullptr == iter.second)
			continue;

		iEnd = iter.second->Update_Layer(fTimeDelta);

		if (iEnd & 0x80000000)
			return iEnd;

	}

	return iEnd;
}

_int Engine::CScene::LateUpdate_Scene(const float & fTimeDelta)
{
	_int iEnd = UPDATE_OK;

	for (auto& iter : m_mapLayer)
	{
		if (nullptr == iter.second)

			continue;
		iEnd = iter.second->LateUpdate_Layer(fTimeDelta);

		if (iEnd & 0x80000000)
			return iEnd;

	}

	return iEnd;
}

Engine::CLayer* Engine::CScene::Get_Layer(const _tchar* _pLayerTag)
{
	//auto& iter = find_if(m_ma pLayer.begin(), m_mapLayer.end(), CTag_Finder(_pLayerTag));
	auto& iter = m_mapLayer.find(_pLayerTag);

	if (iter == m_mapLayer.end())
		return nullptr;

	return iter->second;
}

void Engine::CScene::Free(void)
{
	Safe_Release(m_pGraphicDev);

	for_each(m_mapLayer.begin(), m_mapLayer.end(), CDeleteMap());
	m_mapLayer.clear();
}




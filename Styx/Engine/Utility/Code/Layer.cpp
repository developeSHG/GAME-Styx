#include "Layer.h"

USING(Engine)

Engine::CLayer::CLayer(void)
{

}

Engine::CLayer::~CLayer(void)
{

}

HRESULT Engine::CLayer::Ready_Layer(void)
{
	return S_OK;
}

_int Engine::CLayer::Update_Layer(const _float& fTimeDelta)
{
	_int	iEnd = UPDATE_OK;

	for (auto& iter : m_mapObject)
	{
		if (nullptr == iter.second)
			continue;

		iEnd = iter.second->Update_Object(fTimeDelta);

		if (iEnd & 0x80000000)
			return iEnd;
	}

	return iEnd;
}

_int Engine::CLayer::LateUpdate_Layer(const _float& fTimeDelta)
{
	_int	iEnd = UPDATE_OK;

	for (auto& iter : m_mapObject)
	{
		if (nullptr == iter.second)
			continue;
		iEnd = iter.second->LateUpdate_Object(fTimeDelta);

		if (iEnd & 0x80000000)
			return iEnd;
	}

	return iEnd;
}

void Engine::CLayer::Tool_DeleteAll()
{
	m_mapObject.clear();
}

Engine::_bool Engine::CLayer::Tool_DeleteObject(const _tchar* pObjTag, CGameObject* pGameObject)
{
	//auto& iter = find_if(m_mapObject.begin(), m_mapObject.end(), CTag_Finder(pObjTag));
	auto& iter = m_mapObject.find(pObjTag);

	if (iter == m_mapObject.end())
		return nullptr;

	auto& multiIter = m_mapObject.equal_range(iter->first);

	auto& eachIter = multiIter.first;

	for (; eachIter != multiIter.second; eachIter++)
	{
		if (eachIter->second == pGameObject)
		{
			m_mapObject.erase(eachIter);
			return TRUE;
		}
	}
	return FALSE;
}

HRESULT Engine::CLayer::Add_Object(const _tchar* pObjTag, CGameObject* pGameObject)
{
	if (nullptr == pGameObject)
		return E_FAIL;

	m_mapObject.emplace(pObjTag, pGameObject);

	return S_OK;
}

CLayer* Engine::CLayer::Create(void)
{
	CLayer*		pInstance = new CLayer;

	if (FAILED(pInstance->Ready_Layer()))
	{
		MSG_BOX("Layer Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void Engine::CLayer::Free(void)
{
	for_each(m_mapObject.begin(), m_mapObject.end(), CDeleteMap());
	m_mapObject.clear();

}



#include "ObjectMgr.h"

IMPLEMENT_SINGLETON(Engine::CObjectMgr)

Engine::CObjectMgr::CObjectMgr()
{

}

Engine::CObjectMgr::~CObjectMgr(void)
{
	Free();
}

HRESULT Engine::CObjectMgr::Add_ObjectMgr(const OBJECTID _eID, Engine::CGameObject* _pGameObject)
{
	if (nullptr == _pGameObject)
		return E_FAIL;

	auto iter = m_mapObjectList.find(_eID);

	if (iter == m_mapObjectList.end())
	{
		m_mapObjectList.emplace(_eID, list<Engine::CGameObject*>());
	}
	
	m_mapObjectList[_eID].push_back(_pGameObject);

	return S_OK;
}

list<Engine::CGameObject*>*	 Engine::CObjectMgr::Find_ObjectList(const OBJECTID _eID)
{
	auto iter = m_mapObjectList.find(_eID);

	if (iter == m_mapObjectList.end() || 0 == m_mapObjectList[_eID].size())
		return nullptr;

	return &(m_mapObjectList[_eID]);
}

Engine::CGameObject* Engine::CObjectMgr::Find_ObjectListFront(const OBJECTID _eID)
{
	if (0 == m_mapObjectList[_eID].size())
		return nullptr;

	return (m_mapObjectList[_eID].front());
}

Engine::CGameObject* Engine::CObjectMgr::Find_ObjectListBack(const OBJECTID _eID)
{
	if (0 == m_mapObjectList[_eID].size())
		return nullptr;

	return (m_mapObjectList[_eID].back());
}

Engine::CGameObject* Engine::CObjectMgr::Find_PlayerObject()
{
	if (0 == m_mapObjectList[OBJECTID::OBJECT_PLAYER].size())
		return nullptr;

	return (m_mapObjectList[OBJECTID::OBJECT_PLAYER].back());
}

list<Engine::CGameObject*>* Engine::CObjectMgr::Find_MonsterObjectList()
{
	auto iter = m_mapObjectList.find(OBJECTID::OBJECT_MONSTER);

	if (iter == m_mapObjectList.end() || 0 == m_mapObjectList[OBJECTID::OBJECT_MONSTER].size())
		return nullptr;

	return &(m_mapObjectList[OBJECTID::OBJECT_MONSTER]);
}

Engine::CGameObject* Engine::CObjectMgr::Find_StaticCameraObject()
{
	if (0 == m_mapObjectList[OBJECTID::OBJECT_STATICCAMERA].size())
		return nullptr;

	return (m_mapObjectList[OBJECTID::OBJECT_STATICCAMERA].back());
}

Engine::CGameObject* Engine::CObjectMgr::Find_DynamicCameraObject()
{
	if (0 == m_mapObjectList[OBJECTID::OBJECT_DYNAMICCAMERA].size())
		return nullptr;

	return (m_mapObjectList[OBJECTID::OBJECT_DYNAMICCAMERA].back());
}

void Engine::CObjectMgr::Free(void)
{
	for_each(m_mapObjectList.begin(), m_mapObjectList.end(), [&](auto& list){
		list.second.clear();
	});
}

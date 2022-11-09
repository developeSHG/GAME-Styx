#include "stdafx.h"
#include "ClientCloneMgr.h"

IMPLEMENT_SINGLETON(CClientCloneMgr)

#include "PlayerBehavior.h"
#include "EliteBehavior.h"

CClientCloneMgr::CClientCloneMgr()
{

}

CClientCloneMgr::~CClientCloneMgr(void)
{
	Free();
}

HRESULT CClientCloneMgr::Add_CloneGameObject(const Engine::OBJECTID& _eID)
{
	/*Engine::CGameObject*	pGameObject = nullptr;

	switch (_eID)
	{
	case Engine::OBJECTID::OBJECT_PLAYER:
		pGameObject = CAbstractFactoryPattern<CPlayer>::CreateObject(m_pGraphicDev);
		break;

	case Engine::OBJECTID::OBJECT_ELITE:
		pGameObject = CAbstractFactoryPattern<CElite>::CreateObject(m_pGraphicDev);
		break;

	case Engine::OBJECTID::OBJECT_SERVANT_QUEST:
		pGameObject = CAbstractFactoryPattern<CServant_Quest>::CreateObject(m_pGraphicDev);
		break;

	case Engine::OBJECTID::OBJECT_SERVANT_RESCUE:
		pGameObject = CAbstractFactoryPattern<CServant_Rescue>::CreateObject(m_pGraphicDev);
		break;

	case Engine::OBJECTID::OBJECT_DOOR:
		pGameObject = CAbstractFactoryPattern<CDoor>::CreateObject(m_pGraphicDev);
		break;

	case Engine::OBJECTID::OBJECT_TRUNK:
		pGameObject = CAbstractFactoryPattern<CTrunk>::CreateObject(m_pGraphicDev);
		break;

	case Engine::OBJECTID::OBJECT_CUPBOARD:
		pGameObject = CAbstractFactoryPattern<CCupboard>::CreateObject(m_pGraphicDev);
		break;

	case Engine::OBJECTID::OBJECT_TORCH:
		pGameObject = CAbstractFactoryPattern<CTorch>::CreateObject(m_pGraphicDev);
		break;

	case Engine::OBJECTID::OBJECT_LEVER:
		pGameObject = CAbstractFactoryPattern<CLever>::CreateObject(m_pGraphicDev);
		break;

	case Engine::OBJECTID::OBJECT_GRID:
		pGameObject = CAbstractFactoryPattern<CGrid>::CreateObject(m_pGraphicDev);
		break;

	case Engine::OBJECTID::OBJECT_SWORD:
		pGameObject = CAbstractFactoryPattern<CSword>::CreateObject(m_pGraphicDev);
		break;

	}

	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	m_mapGameObject.emplace(_eID, pGameObject);*/

	return S_OK;
}

HRESULT CClientCloneMgr::Add_CloneComponent(const Engine::COMPONENTTYPE _eComponentType)
{
	/*Engine::CComponent*	pComponent = nullptr;

	switch (_eComponentType)
	{
	case Engine::COMPONENTTYPE::COM_PLAYERBEHAVIOR:
		pComponent = CPlayerBehavior::Create();
		break;

	case Engine::COMPONENTTYPE::COM_ELITEBEHAVIOR:
		pComponent = CEliteBehavior::Create();
		break;

	}

	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.emplace(_eComponentType, pComponent);*/

	return S_OK;
}

Engine::CGameObject* CClientCloneMgr::Clone_GameObject(const Engine::OBJECTID _eObjectID)
{
	Engine::CGameObject*		pGameObject = nullptr;

	pGameObject = m_mapGameObject.find(_eObjectID)->second;

	NULL_CHECK_RETURN(pGameObject, nullptr);

	return pGameObject->Clone_GameObject();
}

Engine::CComponent* CClientCloneMgr::Clone_Component(const Engine::COMPONENTTYPE _eComponentType)
{
	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_mapComponent.find(_eComponentType)->second;

	NULL_CHECK_RETURN(pComponent, nullptr);

	return pComponent->Clone_Component();
}

void CClientCloneMgr::Free(void)
{
	for (auto& iter : m_mapGameObject)
		Engine::Safe_Release(iter.second);

	m_mapGameObject.clear();

	for (auto& iter : m_mapComponent)
		Engine::Safe_Release(iter.second);

	m_mapComponent.clear();
}

#ifndef ClientCloneMgr_h__
#define ClientCloneMgr_h__

#include "Engine_Include.h"
#include "Base.h"

#include "GameObject.h"
#include "Component.h"

class CClientCloneMgr : public CBase
{
	DECLARE_SINGLETON(CClientCloneMgr)

private:
	typedef map<const Engine::OBJECTID, Engine::CGameObject*>				MAPGAMEOBJECT;
	typedef map<const Engine::COMPONENTTYPE, Engine::CComponent*>			MAPCOMPONENT;

private:
	/*  Structor  */
	explicit CClientCloneMgr();
	~CClientCloneMgr(void);

public:
	HRESULT									Add_CloneGameObject(const Engine::OBJECTID& _eID);
	HRESULT									Add_CloneComponent(const Engine::COMPONENTTYPE _eComponentType);

public:
	/*  Set  */
	void									Set_ClientCloneMgrGraphicDev(LPDIRECT3DDEVICE9 _pDevice) { m_pGraphicDev = _pDevice; }

public:
	/*  Get  */
	
private:
	LPDIRECT3DDEVICE9						m_pGraphicDev;

private:
	MAPGAMEOBJECT							m_mapGameObject;
	MAPCOMPONENT							m_mapComponent;


	/*  Creation and destruction  */
public:
	Engine::CGameObject*					Clone_GameObject(const Engine::OBJECTID _eObjectID);
	Engine::CComponent*						Clone_Component(const Engine::COMPONENTTYPE _eComponentType);
public:
	virtual void							Free(void);
};


#endif // ClientCloneMgr_h__

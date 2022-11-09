#ifndef ObjectMgr_h__
#define ObjectMgr_h__

#include "Engine_Include.h"
#include "Base.h"

#include "GameObject.h"
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CObjectMgr : public CBase
{
	DECLARE_SINGLETON(CObjectMgr)

private:
	typedef map<const OBJECTID, list<CGameObject*>>				MAPOBJECTLIST;

private:
	/*  Structor  */
	explicit CObjectMgr();
	~CObjectMgr(void);

public:
	/*  Set  */
	HRESULT									Add_ObjectMgr(const OBJECTID _eID,
															CGameObject* _pGameObject);

public:
	/*  Get  */
	list<CGameObject*>*						Find_ObjectList(const OBJECTID _eID);
	CGameObject*							Find_ObjectListFront(const OBJECTID _eID);
	CGameObject*							Find_ObjectListBack(const OBJECTID _eID);
	CGameObject*							Find_PlayerObject();
	list<CGameObject*>*						Find_MonsterObjectList();
	CGameObject*							Find_StaticCameraObject();
	CGameObject*							Find_DynamicCameraObject();


private:
	MAPOBJECTLIST							m_mapObjectList;



	/*  Creation and destruction  */
public:
	virtual void							Free(void);
};

END

#endif // ObjectMgr_h__

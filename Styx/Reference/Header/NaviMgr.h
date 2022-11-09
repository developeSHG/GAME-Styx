#ifndef NaviMgr_h__
#define NaviMgr_h__

#include "Engine_Include.h"
#include "Base.h"

#include "GameObject.h"
#include "Component.h"

BEGIN(Engine)

class CNaviMesh;

class ENGINE_DLL CNaviMgr : public CBase
{
private:
	enum RESOURCEID { RESOURCE_STATIC, RESOURCE_LOGO, RESOURCE_STAGE, RESOURCE_END };

public:
	DECLARE_SINGLETON(CNaviMgr)

private:
	/*  Structor  */
	explicit CNaviMgr(void);
	~CNaviMgr(void);

public:
	/*  Set  */
	

public:
	/*  Get  */
	Engine::CNaviMesh*			Get_NaviMesh(void) { return m_pNaviMesh; }

public:
	HRESULT						Create_NaviMesh(void);

private:        
	CNaviMesh*					m_pNaviMesh;

public:
	_bool						m_bCollRender;


	/*  Creation and destruction  */
public:
	virtual void				Free(void);
};

END

#endif // NaviMgr_h__
